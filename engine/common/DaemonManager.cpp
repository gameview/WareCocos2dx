#include "DaemonManager.h"
#include "platform/platform.h"
#include "platform/CCThread.h"

#ifdef EMSCRIPTEN
// Hack to get ASM.JS validation (no undefined symbols allowed).
#define pthread_cond_signal(_)
#endif // EMSCRIPTEN

NS_CC_ENGINE_BEGIN

CDaemonManager::CDaemonManager(void)
{
	m_bQuit = false;
}


CDaemonManager::~CDaemonManager(void)
{
}
//-------------------------------------------------------------------------
/**
@brief 初始化
@param
*/
void CDaemonManager::init()
{
	pthread_mutex_init(&m_asyncStructQueueMutex, NULL);
	pthread_mutex_init(&m_finishMutex, NULL);
	pthread_mutex_init(&m_SleepMutex, NULL);
	pthread_cond_init(&m_SleepCondition, NULL);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
	pthread_create(&m_loadingThread, NULL, CDaemonManager::threadProcess, this);
#endif
}
/**
@brief 退出
@param
*/
void CDaemonManager::exit()
{
	m_lstTaskQueue.clear();
	m_lstDaemonQueue.clear();
	m_lstFinishQueue.clear();

#ifdef WIN32
	if(m_asyncStructQueueMutex)
	{
		pthread_mutex_destroy(&m_asyncStructQueueMutex);
	}

	if(m_finishMutex)
	{
		pthread_mutex_destroy(&m_finishMutex);
	}

	if(m_SleepMutex)
	{
		pthread_mutex_destroy(&m_SleepMutex);
	}

	if(m_SleepCondition)
	{
		pthread_cond_destroy(&m_SleepCondition);
	}
#else

	pthread_mutex_destroy(&m_asyncStructQueueMutex);
	pthread_mutex_destroy(&m_finishMutex);
	pthread_mutex_destroy(&m_SleepMutex);
	pthread_cond_destroy(&m_SleepCondition);

#endif

	m_bQuit = true;
}
//-------------------------------------------------------------------------
/**
@brief 添加任务
@param
*/
void CDaemonManager::addTask( ITask* pTask )
{
	if( pTask == NULL )
	{
		return;
	}

	m_lstTaskQueue.push_back( pTask );
}
//-------------------------------------------------------------------------
/**
@brief 运行
@param
*/
void CDaemonManager::run()
{
	/// 推送任务到后台
	if( !m_lstTaskQueue.empty() )
	{
		std::list<ITask*>::iterator it = m_lstTaskQueue.begin();
		pthread_mutex_lock(&m_asyncStructQueueMutex);// get async struct from queue
		for( ; it != m_lstTaskQueue.end(); ++it )
		{
			m_lstDaemonQueue.push_back( (*it) );
		}
		m_lstTaskQueue.clear();
		pthread_mutex_unlock(&m_asyncStructQueueMutex);


		pthread_cond_signal(&m_SleepCondition); /// 发送后台任务准备完毕事件
	}

	if( m_lstFinishQueue.empty() )
	{
		return;
	}

	/// 处理任务结束
	std::list<ITask*> lstTask;
	std::list<ITask*>::iterator it = m_lstFinishQueue.begin();
	pthread_mutex_lock(&m_finishMutex);// get async struct from queue
	for( ; it != m_lstFinishQueue.end(); ++it )
	{
		lstTask.push_back( (*it) );
	}
	m_lstFinishQueue.clear();
	pthread_mutex_unlock(&m_finishMutex);

	it = lstTask.begin();
	for( ; it != lstTask.end(); ++it )
	{
		if( (*it) == NULL )
		{
			continue;
		}

		((ITask*)(*it))->onTaskEnd();
	}
}
//-------------------------------------------------------------------------
/**
@brief 线程处理函数 
@param
*/
void* CDaemonManager::threadProcess(void* data)
{
	CDaemonManager* pMgr = (CDaemonManager*)(data);
	if( pMgr == NULL )
	{
		return 0;
	}

	while(true)
	{
		// create autorelease pool for iOS
		CCThread thread;
		thread.createAutoreleasePool();

		std::list<ITask*>& pTaskList = pMgr->m_lstDaemonQueue;
		pthread_mutex_lock(&pMgr->m_asyncStructQueueMutex);// get async struct from queue
		if (pTaskList.empty())
		{
			pthread_mutex_unlock(&pMgr->m_asyncStructQueueMutex);
			if( pMgr->m_bQuit )
			{
				break;
			}
			else
			{
				pthread_cond_wait(&pMgr->m_SleepCondition, &pMgr->m_SleepMutex);
				continue;
			}
		}
		else
		{
			std::list<ITask*> lstTask;
			std::list<ITask*>::iterator it = pTaskList.begin();
			for( ; it != pTaskList.end(); ++it )
			{
				lstTask.push_back( (*it) );
			}
			pMgr->m_lstDaemonQueue.clear();
			pthread_mutex_unlock(&pMgr->m_asyncStructQueueMutex);

			it = lstTask.begin();
			for( ; it != lstTask.end(); ++it )
			{
				if( (*it) == NULL )
				{
					continue;
				}
				else
				{
					((ITask*)(*it))->onDoTask();

					pthread_mutex_lock(&pMgr->m_finishMutex);// get async struct from queue
					pMgr->m_lstFinishQueue.push_back((*it));
					pthread_mutex_unlock(&pMgr->m_finishMutex);
				}
			}
		}        
	}

	return 0;
}

NS_CC_ENGINE_END