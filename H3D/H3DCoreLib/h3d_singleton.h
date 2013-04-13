/********************************************************************
	created:	2012/02/29
	created:	29:2:2012   22:37
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_singleton.h
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/
#ifndef h3d_singleton_h__
#define h3d_singleton_h__
//   类
#include "h3d_debug.h"

namespace Heaven3D
{
	template <typename T> 
	class H3DSingleton : public noncopyable
	{
	protected:
		static T* ms_pkSingleton;
	public:
		H3DSingleton( void )
		{
			debug_assert( !ms_pkSingleton , "");
			ms_pkSingleton = static_cast<T*>(this);
		}

		~H3DSingleton( void )
		{  
			debug_assert( ms_pkSingleton , "" );  
			ms_pkSingleton = 0;  
		}

		static T* GetSingletonPtr( void )
		{  
			if( ms_pkSingleton )
				return ms_pkSingleton;
			else
			{
				ms_pkSingleton = new T;
				return ms_pkSingleton;  
			}
		}

		static void DestroySingleton()
		{
			if( ms_pkSingleton )
			{
				delete ms_pkSingleton;
				ms_pkSingleton = 0;
			}
		}
	};
}

#endif // h3d_singleton_h__