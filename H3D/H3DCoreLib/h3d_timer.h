/********************************************************************
	created:	2012/03/14
	created:	14:3:2012   0:48
	filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_timer.h
	author:		Xiong Xinke
	
	purpose:	高精度计时器
*********************************************************************/
#ifndef h3d_timer_h__
#define h3d_timer_h__

namespace Heaven3D
{
	class H3DTimer
	{
	public:
		H3DTimer(void);
		~H3DTimer(void);

		/** 
		* @brief StartTimer 
		* 
		* Detailed description. 启动计时器
		* @return		void  
		*/
		void StartTimer();

		/** 
		* @brief GetCurrentTime 
		* 
		* Detailed description. 获取到当前时刻
		* @return		double  
		*/
		double GetCurrentTime();

		/** 
		* @brief GetTimeInterval 
		* 
		* Detailed description. 该函数返回本次调用和上次调用此函数的时间间隔
		* @return		double  
		*/
		double GetTimeInterval();
	protected:
		int64_t counter_start_;			// 启动计时器时的滴答值
		int64_t last_time_;				// 上次调用GetTimeInterval成员函数时的时刻
		double  counts_per_second_;		// 每秒钟的滴答数
	};
}

#endif // h3d_timer_h__

