//
//template< typename T >
//T* CFreeList<T>::CreateObject()
//{
//	if( m_lstObjectIdle.empty() )
//	{
//		T* pObj = new T();
//		m_lstObject.push_back( pObj );
//		return pObj;
//	}
//
//	T* pObj = m_lstObjectIdle.front();
//	T* pObjTmp = new (pObj)T();
//	m_lstObjectIdle.pop_front();
//	m_lstObject.push_back( pObj );
//	return pObjTmp;
//}
//
//template < typename T >
//void CFreeList<T>::ReleaseObject( T* pObj )
//{
//	if( !pObj )
//	{
//		return;
//	}
//
//	pObj->~T();
//	m_lstObject.remove( pObj );
//	m_lstObjectIdle.push_back( pObj );
//}
//
//template< typename T >
//void CFreeList<T>::Clear()
//{
//	//assert( m_lstObject.empty() && "警告：程序存在内存泄漏，请检查代码！" );
//	//std::list< T* >::iterator iter = m_lstObjectIdle.begin();
//	for( std::list< T* >::iterator iter = m_lstObjectIdle.begin(); iter != m_lstObjectIdle.end(); ++iter )
//	{
//		if( (*iter) )
//		{
//			delete (*iter);
//		}
//	}
//
//	m_lstObjectIdle.clear();
//}