//*************************************************************************
//	创建日期:	2014-3-7   16:59
//	文件名称:	DaemonManager.h
//  创 建 人:   Rare	
//	版权所有:	Cocos2DRes.com
//	说    明:	后台任务管理
//*************************************************************************
#ifndef _DAEMONMANAGER_H__
#define _DAEMONMANAGER_H__

#include <pthread.h>
#include <stack>
#include <string>
#include <queue>
#include <list>

#include "cocos2dx_macros.h"

NS_CC_ENGINE_BEGIN

struct ITask
{
public:

	/**
	@brief 获取任务ID
	@param
	*/
	virtual unsigned int getID() = 0;

	/**
	@brief 任务结束时调用
	@param
	*/
	virtual void	onDoTask() = 0;

	/**
	@brief 任务结束时调用
	@param
	*/
	virtual void	onTaskEnd() = 0;

	/**
	@brief 任务是否结束
	@param
	*/
	virtual bool	isEnd() = 0;
};

/**
@brief 后台任务管理
*/
class CDaemonManager
{
private:
	CDaemonManager(void);
	~CDaemonManager(void);

public:
	static CDaemonManager& Instance()
	{
		static CDaemonManager _inst;
		return _inst;
	}

	/**
	@brief 初始化
	@param
	*/
	void	init();

	/**
	@brief 退出
	@param
	*/
	void	exit();

	/**
	@brief 添加任务
	@param
	*/
	void	addTask( ITask* pTask );

	/**
	@brief 运行
	@param
	*/
	void	run();

	/**
	@brief 线程处理函数 
	@param
	*/
	static	void* threadProcess(void* data);

public:

	bool				m_bQuit;					/// 程序退出

	pthread_t			m_loadingThread;			/// 线程ID

	pthread_mutex_t		m_SleepMutex;				/// 互斥
	pthread_cond_t		m_SleepCondition;			/// 条件变量

	pthread_mutex_t		m_asyncStructQueueMutex;	/// 互斥
	pthread_mutex_t		m_finishMutex;				/// 完成互斥

	std::list<ITask*>	m_lstTaskQueue;				/// 任务列表
	std::list<ITask*>	m_lstDaemonQueue;			/// 任务列表
	std::list<ITask*>	m_lstFinishQueue;			/// 任务列表

};

NS_CC_ENGINE_END

#endif // _DAEMONMANAGER_H__
