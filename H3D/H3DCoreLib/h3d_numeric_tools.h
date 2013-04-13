/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:31
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_numeric_tools.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_numeric_tools_h__
#define h3d_numeric_tools_h__


#include "h3d_debug.h"

namespace Heaven3D
{

#define DEFAULT_EPSILON	((float)0.00001f)

	// 求value的绝对值
	template <class T> T AbsoluteValue(T value);

	// a是正数吗
	template <class T> bool is_positive(T a);

	// a是负数吗
	template <class T> bool is_negative(T a);

	// a，b的符号相同吗
	template <class T> bool same_signs(T a, T b);

	// 如果s是负数，返回一个负值
	template <class T> T copy_sign(T value, T s);

	// value testing
	template <class T> bool delta_range_test(T a, T b, T epsilon);
	template <class T> bool delta_range_test(T a, T b);
	template <class T> const T& minimum(const T& a, const T& b);
	template <class T> const T& maximum(const T& a, const T& b);

	// value clamping
	template <class T> T clamp(T value, T low, T high);
	template <class T> T clamp_positive(T value);
	template <class T> T clamp_negative(T value);
	float clamp_unit_size(float value);

	// bit testing
	template<class T> int highest_bit_set(T input);
	template<class T> int lowest_bit_set (T input);

	// power of two functions
	template<class T> bool is_power_of_two(T input);
	template<class T> T nearest_power_of_two(T input);
	template<class T> T ceiling_power_of_two(T input);
	template<class T> T floor_power_of_two(T input);

	// simple calculations
	template <class T> T raise_to_power(T value, T power);
	template <class T> T modulus(T value, T Y);
	template <class T> T align(const T& value, T alignment);

	// swap two objects
	template <class T> void swap(T& a, T& b);


	// floating-point specific Inverse (1/value)
	float Inverse(float value);

	// round floating point numbers to the desired precision
	float trim_float(float input, uint8_t precision);

	// handy enum set for trim_float precision values
	enum trimFloat_PrecisionValues
	{
		tf_whole_unit = 0,	// 0 bits after binary point
		tf_half_unit,		// 1 bit
		tf_4th_unit,		// 2 bits
		tf_8th_unit,		// 3 bits
		tf_16th_unit,		// 4 bits
		tf_32nd_unit,		// 5 bits
		tf_64th_unit,		// 6 bits
		tf_128th_unit,		// 7 bits
		tf_256th_unit,		// 8 bits
	};

	// 把输入的单精度浮点数转换成一个32位有符号整数
	int32_t realToInt32(float input);

	// 把输入的单精度浮点数砍掉小数位保留整数部分，然后返回一个32位有符号整数
	int32_t realToInt32_chop(float input);

	// 返回小于或者等于输入单精度浮点数的最大整数
	int32_t realToInt32_floor(float input);

	// 返回大于或者等于输入单精度浮点数的最小整数
	int32_t realToInt32_ceiling(float input);

	// 把输入的双精度浮点数转换成一个32位有符号整数
	int32_t realToInt32(double input);

	// 把输入的双精度浮点数砍掉小数位保留整数部分，然后返回一个32位有符号整数
	int32_t realToInt32_chop(double input);

	// 返回小于或者等于输入双精度浮点数的最大整数
	int32_t realToInt32_floor(double input);

	// 返回大于或者等于输入双精度浮点数的最小整数
	int32_t realToInt32_ceiling(double input);

	template<class _OUT, class _IN> 
	inline _OUT fast_cast(_IN input)
	{
		return (static_cast<_OUT>(input)); 
	}

	template<> 
	inline int32_t fast_cast(float input)
	{
		return realToInt32_chop(input); 
	}
	
	template<>
	inline int32_t fast_cast(double input)
	{
		return realToInt32_chop((float)input); 
	}

	// 把一个浮点数重新解释为一个32位整数
#define fpBits(f) (*reinterpret_cast<const int32_t*>(&(f))) 

	// 把一个32位整数重新解释为一个浮点数
#define intBits(i) (*reinterpret_cast<const float*>(&(i))) 

	// 根据输入的浮点数是否有符号返回0或者-1
#define fpSign(f) (fpBits(f)>>31) 

	// extract the 8 bits of exponent as a signed integer
	// by masking out this bits, shifting down by 23,
	// and subtracting the bias value of 127
	// 把浮点数中的8位指数部分分离出来，然后右移23位，接着减去127
	// 即得到该浮点数的指数部分的值是多少，
	// 因为exponent表示浮点数的指数部分，是一个无符号整数。因为指数值
	// 可以是正值，也可以是负值，所以通过一个偏差值对它进行置偏，及指
	// 数的真实值=指数部分的整数— 偏差值。对于32位浮点数，偏差值=127；对于64位浮点数，偏差值=1023
#define fpExponent(f) (((fpBits(f)&0x7fffffff)>>23)-127) 

	// return 0 or -1 based on the sign of the exponent
	// 如果浮点数的指数部分是正数，返回0，是负数，返回-1
#define fpExponentSign(f) (fpExponent(f)>>31) 

	// get the 23 bits of mantissa without the implied bit
	// 获取到23位尾数
#define fpPureMantissa(f) ((fpBits(f)&0x7fffff)) 

	// get the 23 bits of mantissa with the implied bit replaced
#define fpMantissa(f) (fpPureMantissa(f) | (1<<23)) 

	// 浮点数1.0
#define fpOneBits 0x3F800000

	// flipSign is a helper Macro to
	// invert the sign of i if flip equals -1, 
	// if flip equals 0, it does nothing
	// 如果flip等于-1，把i设置为-i,如果flip等于0就什么也不做
#define flipSign(i, flip) ((i^ flip) - flip)

	template <class T> 
	inline T AbsoluteValue(T value)
	{
		return abs(value); // call the standard C abs()
	}

	// 快速取得浮点数value的值
	template <> 
	inline float AbsoluteValue(float value)
	{
#ifdef USE_H3D_NUMERIC_TOOL
		uint32_t abs_value = fpBits(value);
		abs_value &= 0x7fffffff;
		return intBits(abs_value);
#else
		return fabsf(value);
#endif
	}

	// 很显然下面这些都是正数
	template<> 
	inline uint8_t AbsoluteValue(uint8_t a) 
	{
		return a;
	}

	template<> 
	inline uint16_t AbsoluteValue(uint16_t a) 
	{
		return a;
	}

	template<> 
	inline uint32_t AbsoluteValue(uint32_t a) 
	{
		return a;
	}

	template <class T> 
	inline bool is_positive(T a)
	{
		return (a >= 0) ? true : false;
	}

	template<> 
	inline bool is_positive(float a)
	{
		return !fpSign(a);
	}

	template<> 
	inline bool is_positive(uint8_t a) 
	{
		return true;
	}

	template<> 
	inline bool is_positive(uint16_t a) 
	{
		return true;
	}

	template<>
	inline bool is_positive(uint32_t a) 
	{
		return true;
	}

	template<class T> 
	inline bool is_negative(T a)
	{
		return (a < 0) ? true : false;
	}

	template<> 
	inline bool is_negative(float a)
	{
		return fpSign(a) ? true:false;
	}

	template<> 
	inline bool is_negative(uint8_t a) 
	{
		return false;
	}

	template<> 
	inline bool is_negative(uint16_t a) 
	{
		return false;
	}

	template<> 
	inline bool is_negative(uint32_t a) 
	{
		return false;
	}

	template<class T> 
	inline bool same_signs(T a, T b)
	{
		return (is_negative(a) == is_negative(b));
	}

	template<class T> 
	inline T copy_sign(T value, T s)
	{
		return is_negative(s) ? -AbsoluteValue(value) : AbsoluteValue(value);
	}

	// 如果a与b的差值的绝对值小于epsilon就返回true，否则返回false
	template<class T>
	inline bool delta_range_test(T a, T b, T epsilon)
	{
		return (AbsoluteValue(a - b) < epsilon) ? true : false;
	}

	template <class T>
	inline bool delta_range_test(T a, T b)
	{
		return (a==b);
	}

	template<>
	inline bool delta_range_test(float a, float b)
	{
		return (AbsoluteValue(a - b) < DEFAULT_EPSILON) ? true : false;
	}

	template <class T> 
	inline const T& minimum(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template <class T> 
	inline const T& maximum(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	// 如果value大于high值则返回high，小于low则返回low，否则返回自己
	template <class T> 
	inline T clamp(T value, T low, T high)
	{
		if(value < low)
		{
			return low;
		}

		if(value > high)
		{
			return high;
		} 

		return value;
	}

	// 如果value小于0则返回0，否则返回自己
	template <class T> 
	inline T clamp_positive(T value)
	{
		return value < 0 ? 0 : value;
	}

	// 如果input是一个负数，那么返回0
	template<> 
	inline float clamp_positive(float input)
	{
		int value = fpBits(input);
		int sign_mask = ~fpSign(input);
		value &= sign_mask;
		return intBits(value);
	}

	// 如果value大于0则返回0，否则返回自己
	template <class T> 
	inline T clamp_negative(T value)
	{
		return value > 0 ? 0 : value;
	}

	// 如果input是一个正数，那么返回0
	template <> 
	inline float clamp_negative(float input)
	{
		int value = fpBits(input);
		int sign_mask = fpSign(input);
		value &= sign_mask;
		return intBits(value);
	}

	// 如果input大于1，则返回1，如果input小于-1.0，则返回-1
	inline float clamp_unit_size(float input)
	{
		uint32_t value = fpBits(input);
		uint32_t abs_value = value & 0x7fffffff;
		abs_value -= (127<<23);
		abs_value >>= 31;
		uint32_t one = (127<<23) & ~abs_value;
		value = (value & abs_value) + one;
		return intBits(value);
	} 

	/*
	找出输入数input中为1的最高位的索引值
	BSR OPD,OPS
	32位汇编指令功能：
	向后位扫描指令.本指令从最高位(第15位或31位)开始测试OPS中的各位,当遇到有1的位时,ZF=0,且将该位
	的序 号存入OPD中,如OPS的所有位都是0,则ZF=1且OPD中的值无意义.(执行逻辑移位前,决定应移位的次数)
	*/
	template<class T>
	inline int highest_bit_set(T input)
	{
		register int result;
		assert(input); // 不能为0，且操作数必须是32位
		assert(sizeof(T)==4); // 32bit data only!

		_asm 
		{
			bsr eax, input
			mov result, eax
		}

		return result;
	}

	template<>
	inline int highest_bit_set (uint8_t input)
	{
		register int result;
		assert(input);

		_asm 
		{
			mov dl, input 
			and edx, 0xff
			bsr eax, edx
			mov result, eax
		}

		return result;
	}

	template<>
	inline int highest_bit_set (int8_t input)
	{
		register int result;
		assert(input); 

		_asm 
		{
			mov dl, input 
			and edx, 0xff
			bsr eax, edx
			mov result, eax
		}

		return result;
	}

	template<>
	inline int highest_bit_set (uint16_t input)
	{
		register int result;
		assert(input);
		
		_asm 
		{
			mov dx, input 
			and edx, 0xffff 
			bsr eax, edx
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int highest_bit_set (int16_t input)
	{
		register int result;
		assert(input); 
		
		_asm 
		{
			mov dx, input 
			and edx, 0xffff 
			bsr eax, edx
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int highest_bit_set (float f)
	{
		register int result;
		register uint32_t input = fpBits(f);
		assert(input);
		
		_asm 
		{
			bsr eax, input
			mov result, eax
		}
		
		return result;
	}

	template<class T>
	inline int lowest_bit_set(T input)
	{
		register int result;
		assert(input); // zero is invalid input!
		assert(sizeof(T)==4); // 32bit data only!
		
		_asm 
		{
			bsf eax, input
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int lowest_bit_set (uint8_t input)
	{
		register int result;
		assert(input);
		
		_asm 
		{
			mov dl, input
			and edx, 0xff
			bsf eax, edx 
			mov result, eax
		}

		return result;
	}

	template<>
	inline int lowest_bit_set (int8_t input)
	{
		register int result;
		assert(input); // zero is invalid input!
		
		_asm 
		{
			mov dl, input // copy into a 32bit reg
			and edx, 0xff // keep only the bits we want
			bsf eax, edx // perform the scan
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int lowest_bit_set (uint16_t input)
	{
		register int result;
		assert(input);
		
		_asm 
		{
			mov dx, input 
			and edx, 0xffff
			bsf eax, edx
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int lowest_bit_set (int16_t input)
	{
		register int result;
		assert(input);
		_asm
		{
			mov dx, input
			and edx, 0xffff
			bsf eax, edx
			mov result, eax
		}
		
		return result;
	}

	template<>
	inline int lowest_bit_set (float f)
	{
		register int result;
		register uint32_t input = fpBits(f);
		assert(input);
		
		_asm 
		{
			bsf eax, input
			mov result, eax
		}
		
		return result;
	}

	template<class T>
	inline bool is_power_of_two(T input)
	{
		return (input>0 && highest_bit_set(input) == lowest_bit_set(input));
	}

	template<>
	inline bool is_power_of_two(float input)
	{
		return (!fpPureMantissa(input));
	}



	//:	nearest_power_of_two
	//----------------------------------------------------------------------------------------
	//
	//	Rounds the input value to the nearest power-of-two. 
	//  All values below 1 generate a result of 1
	//
	//-------------------------------------------------------------------------------------://
	// 根据输入数input，返回最接近这个输入数的2的整数次方值，如果input小于1，则返回1
	template<class T>
	inline T nearest_power_of_two(T input)
	{

		// the least possible power-of-two value is 1
		if (input <= 1) return 1;

		int highestBit = highest_bit_set(input);
		int roundingTest = input & (1<< (highestBit-1)); 
		if (roundingTest) ++highestBit;
		return static_cast<T>(1<<highestBit);
	}

	template<>
	inline float nearest_power_of_two(float input)
	{
		// convert the value to an int
		int result = fpBits(input);

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// if anything is in the high bit of the mantissa, 
		// use it to add one to the exponent
		result += (result & 0x800000)<<1;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (intBits(result));
	}



	//:	ceiling_power_of_two
	//----------------------------------------------------------------------------------------
	//
	//	Rounds the next-highest power-of-two value. 
	//  All values below 1 generate a result of 1
	//
	//-------------------------------------------------------------------------------------://
	// 根据输入数input，返回最接近这个输入数的2的整数次方值的下一个更大的值，如果input小于1，则返回1
	template<class T>
	inline T ceiling_power_of_two(T input)
	{
		// the least possible power-of-two value is 1
		if (input <= (T)1) return 1;

		int highestBit = highest_bit_set(index);
		int mask = input & ((1<< highestBit)-1); 
		highestBit += mask & 1;
		return static_cast<T>(1<<highestBit);
	}

	template<>
	inline float ceiling_power_of_two(float input)
	{
		// convert the value to an int
		int result = fpBits(input);

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// if anything is in the mantissa, round up
		result += 0x7fffff;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (intBits(result));
	}



	//:	floor_power_of_two
	//----------------------------------------------------------------------------------------
	//
	//	Rounds the next-least power-of-two value. 
	//  All values below 1 generate a result of 1
	//
	//-------------------------------------------------------------------------------------://
	// 根据输入数input，返回最接近这个输入数的2的整数次方值的下一个更小的值，如果input小于1，则返回1
	template<class T>
	inline T floor_power_of_two(T input)
	{
		// the least possible power-of-two value is 1
		if (input <= (T)1) return 1;

		int highestBit = highest_bit_set(input);
		return static_cast<T>(1<<highestBit);
	}

	template<>
	inline float floor_power_of_two(float input)
	{
		// convert the value to an int
		int result = fpBits(input);

		// if the value is negative, or less than 1.0f, return 1.0f
		// this mask test for the sign bit and the exponents sign in one step
		if (result & 0xc0000000) return 1.0f;

		// trim away the mantissa
		result &= ~((1<<23)-1);

		// convert back to floating-point as we return
		return (intBits(result));
	}



	//:	raise_to_power
	//----------------------------------------------------------------------------------------
	//
	//	Calculates the value of a given Number raised to Power.
	//
	//-------------------------------------------------------------------------------------://
	template <class T> 
	inline T raise_to_power(T number, T power)
	{
		return (value^power); 
	}

	template <> 
	inline float raise_to_power(float number, float power)
	{
		return (float)(powf(number, power)); 
	}



	//:	modulus
	//----------------------------------------------------------------------------------------
	//
	//	Rounds the next-least power-of-two value. 
	//  All values below 1 generate a result of 1
	//
	//-------------------------------------------------------------------------------------://
	template <class T> 
	inline T modulus(T a, T b)
	{
		return (a%b); 
	}

	template<> 
	inline float modulus(float a, float b)
	{
		return (float)(fmodf(a, b)); 
	}



	//:	alignUp
	//----------------------------------------------------------------------------------------
	//
	//	returns the input value, aligned to the next higher multiple of alignment
	//
	//-------------------------------------------------------------------------------------://
	template <class T> 
	inline T alignUp(const T& value, T alignment)
	{
		T remainder = modulus(value, alignment);

		if (remainder == 0)
		{
			return value;
		}

		return value + (alignment - remainder);
	}



	//:	alignDown
	//----------------------------------------------------------------------------------------
	//
	//	returns the input value, aligned to the next lower multiple of alignment
	//
	//-------------------------------------------------------------------------------------://
	template <class T> 
	inline T alignDown(const T& value, T alignment)
	{
		T remainder = modulus(value, alignment);

		if (remainder == 0)
		{
			return(value);
		}

		return value - remainder;
	}



	//:	swap
	//----------------------------------------------------------------------------------------
	//
	//	swap the value of two numbers
	//
	//-------------------------------------------------------------------------------------://
	template <class T>
	inline void swap(T& a, T& b)
	{
		T temp(a);
		a = b;
		b = temp;
	}

	//:	Inverse
	//----------------------------------------------------------------------------------------
	//
	//	Calculates the Inverse of a given value. (1/value)
	//
	//-------------------------------------------------------------------------------------://
	inline float Inverse(float value)
	{
		int _i = 2 * fpOneBits - fpBits(value);                                 
		float r = intBits(_i);                                                       
		return fpBits(value) ? (r * (2.0f - (value) * r)) : 0;  
	}


	//:	trim_float
	//----------------------------------------------------------------------------------------
	//
	//	Rounds the input value to the desired precision. Precision is stated as the number
	//	of bits used to store the fractional value. Fractional bits are negative exponents,
	//	1 bit equals a precision of half a unit (0.5f), 2 bits equals one quarter (0.25f), etc.
	//
	//-------------------------------------------------------------------------------------://
	inline float trim_float(float input, uint8_t precision)
	{
		float result = input;
		int32_t exponent      = fpExponent(input);
		int32_t bias          = 23 - (exponent + precision);

		if (bias < 1)
		{
			return result;
		}
		else if (bias > 24)
		{
			return 0.0f;
		}
		else if (bias == 24)
		{
			int32_t value = fpBits(input);
			value &= (1<<31);
			exponent = -precision;
			value += (exponent+127)<<23;
			return intBits(value);
		}

		int32_t value         = fpBits(input);
		_asm
		{
			clc
			mov ecx, bias
			mov eax, value

			shr eax, cl
			adc eax, 0
			shl eax, cl

			mov value, eax
		};

		return intBits(value);
	};

	// 把一个浮点数转换为一个32位有符号整数
	inline int32_t realToInt32(float input)
	{
		int32_t result;

		__asm 
		{
			fld   input
			fistp DWORD PTR result
		}
		
		return result;
	}

	// 把一个浮点数的小数部分去掉，然后把整数部分转换为一个32位有符号整数
	inline int32_t realToInt32_chop(float input)
	{
		// read the exponent and decide how much we need to shift the mantissa down
		int32_t shift = 23-fpExponent(input);
		// read the mantissa and shift it down to remove all fractional values
		int32_t result = fpMantissa(input)>>shift;
		// set the sign of the new result
		result = flipSign(result, fpSign(input));
		// if the exponent was negative, (-1<input<1) we must return zero
		result &= ~fpExponentSign(input);
		// return the result
		return result;                  
	}

	// 把一个浮点数的小数部分“四舍”掉，把整数部分转换为一个32位有符号整数
	// 如realToInt32_floor(2.35) = 2  realToInt32_floor(-2.35) = -3
	inline int32_t realToInt32_floor(float input)
	{ 
		// read the exponent and decide how much we need to shift the mantissa down
		int shift = 23-fpExponent(input);
		// read the mantissa and shift it down to remove all fractional values
		int result = fpMantissa(input)>>shift;
		// set the sign of the new result
		result = flipSign(result, fpSign(input));
		// if the exponent was negative, (-1<input<1) we must return zero
		result &= ~fpExponentSign(input);

		// if the original value is negative, and any fractional values are present,
		// decrement the result by one
		result -= fpSign(input) && (fpExponentSign(input) || (fpPureMantissa(input) & ((1<<shift)-1)));

		// return the result
		return result;                  
	}


	// 把一个浮点数的小数部分“五入”掉，把整数部分转换为一个32位有符号整数
	// 如realToInt32_floor(2.35) = 3  realToInt32_floor(-2.35) = -2
	inline int32_t realToInt32_ceiling(float input)
	{ 
		// read the exponent and decide how much we need to shift the mantissa down
		int shift = 23-fpExponent(input);
		// read the mantissa and shift it down to remove all fractional values
		int result = fpMantissa(input)>>shift;
		// set the sign of the new result
		result = flipSign(result, fpSign(input));
		// if the exponent was negative, (-1<input<1) we must return zero
		result &= ~fpExponentSign(input);

		// if the original value is positive and not zero, and any fractional values are present,
		// increment the result by one
		result += (!fpSign(input) && fpBits(input)) && (fpExponentSign(input) || (fpPureMantissa(input) & ((1<<shift)-1)));

		// return the result
		return result;                  
	}

	inline int32_t realToInt32(double input)
	{
		return realToInt32((float)input);
	}

	inline int32_t realToInt32_chop(double input)
	{
		return realToInt32_chop((float)input);
	}

	inline int32_t realToInt32_floor(double input)
	{
		return realToInt32_floor((float)input);
	}

	inline int32_t realToInt32_ceiling(double input)
	{
		return realToInt32_ceiling((float)input);
	}

}
#endif // h3d_numeric_tools_h__