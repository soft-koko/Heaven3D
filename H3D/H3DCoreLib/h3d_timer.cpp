#include "h3d_pch.h"
#include "h3d_timer.h"
//   类
namespace Heaven3D
{
	H3DTimer::H3DTimer(void)
	{
	}

	H3DTimer::~H3DTimer(void)
	{
	}

	void H3DTimer::StartTimer()
	{
		LARGE_INTEGER frequency_count;
		QueryPerformanceFrequency(&frequency_count);
		counts_per_second_ = double(frequency_count.QuadPart);
		QueryPerformanceCounter(&frequency_count);
		counter_start_ = frequency_count.QuadPart;
	}

	double H3DTimer::GetCurrentTime()
	{
		LARGE_INTEGER cur_time;
		QueryPerformanceCounter(&cur_time);
		return double(cur_time.QuadPart - counter_start_) / counts_per_second_;
	}

	double H3DTimer::GetTimeInterval()
	{
		LARGE_INTEGER cur_time;
		int64_t tick_count;
		QueryPerformanceCounter(&cur_time);

		tick_count = cur_time.QuadPart-last_time_;
		last_time_ = cur_time.QuadPart;

		if(tick_count < 0)
			tick_count = 0;

		return float(tick_count)/counts_per_second_;
	}
}