//#ifndef _FREELIST_H_
//#define _FREELIST_H_
//
//#include "cocos2dx_types.h"
//#include "cocos2dx_macros.h"
//#include <list>
//
//NS_CC_ENGINE_BEGIN
//
///// T只支持type的默认构造
///// FreeList不对new operator做重载，以后可能加一个分配器的参数
//template < typename T >
//class CFreeList
//{
//public:
//	CFreeList() {}
//	~CFreeList() { Clear(); }
//
//	T*				CreateObject();
//	void			ReleaseObject( T* pObj );
//
//	/// 有多少空闲的对象
//	int32			GetIdleObjectCount()	{ return static_cast<int32>( m_lstObjectIdle.size() );}
//
//	/// 总共创建了多少对象
//	int32			GetObjectCount()		{ return static_cast<int32>( m_lstObject.size() ); }
//
//private:
//	void			Clear();
//
//private:
//	std::list< T* >	m_lstObject;
//	std::list< T* >	m_lstObjectIdle;
//};
//
//#include "FreeList.hpp"
//
//NS_CC_ENGINE_END
//
//#endif // _FREELIST_H_