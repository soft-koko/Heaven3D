/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:28
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_bit_flags.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_bit_flags_h__
#define h3d_bit_flags_h__

#pragma warning( disable : 4800 )

namespace Heaven3D
{

#define FLAG(x) (1<<(x))
#define TEST_BIT(flag, bit) (((flag.value) & FLAG((bit))) !=0)
#define TEST_ALL(flag, set) (((flag.value) & (set)) == (set))
#define TEST_ANY(flag, set) (((flag.value) & (set)) !=0)
#define SET_BIT(flag, bit) ((flag.value) |= FLAG((bit)))
#define CLEAR_BIT(flag, bit) ((flag.value) &= ~FLAG((bit)))
#define SET_ALL(flag, set) ((flag.value) |= (set))
#define CLEAR_ALL(flag, set) ((flag.value) &= ~(set))

	template <class T>
	class H3DBitFlags
	{
	public:

		T value;

		H3DBitFlags();
		H3DBitFlags(T data);
		H3DBitFlags(const H3DBitFlags& Src);
		~H3DBitFlags();

		H3DBitFlags& operator=( const H3DBitFlags& Src);
		H3DBitFlags& operator=( T Src);
		operator T()const {return(value);};
		bool operator==(const H3DBitFlags& Src)const;
		bool operator!=(const H3DBitFlags& Src)const;

		void set(T settings);
		void clear();
		void setFlags(T settings);
		void clearFlags(T settings);
		void setBit(int bit);
		void clearBit(int bit);
		void setBit(int bit, bool setting);

		bool isEmpty()const;
		bool testBit(int bit)const;
		bool testFlags(T test)const;
		bool testAny(T test)const;

		int	totalBits()const;
		int	totalSet()const;
	};

	typedef H3DBitFlags<uint8_t> u8Flags;	// 8 bits of flags
	typedef H3DBitFlags<uint16_t> u16Flags; // 16 bits of flags
	typedef H3DBitFlags<uint32_t> u32Flags; // 32 bits of flags


	template <class T>
	inline H3DBitFlags<T>::H3DBitFlags()
		:value(0)
	{
	}

	template <class T>
	inline H3DBitFlags<T>::H3DBitFlags(T settings)
		:value(settings)
	{
	}

	template <class T>
	inline H3DBitFlags<T>::H3DBitFlags( const H3DBitFlags& Src)
		:value(Src.value)
	{
	}

	template <class T>
	inline H3DBitFlags<T>::~H3DBitFlags()
	{
	}

	template <class T>
	inline H3DBitFlags<T>& H3DBitFlags<T>::operator=( const H3DBitFlags<T>& Src)
	{
		value = Src.value;
		return(*this);
	}

	template <class T>
	inline H3DBitFlags<T>& H3DBitFlags<T>::operator=( T Src)
	{
		value = Src;
		return(*this);
	}

	template <class T>
	bool H3DBitFlags<T>::operator==( const H3DBitFlags& Src)const
	{
		return(value == Src.value);
	}

	template <class T>
	bool H3DBitFlags<T>::operator!=( const H3DBitFlags& Src)const
	{
		return(value != Src.value);
	}

	template <class T>
	inline void H3DBitFlags<T>::set(T settings)
	{
		value = settings;
	}

	template <class T>
	inline void H3DBitFlags<T>::clear()
	{
		value = 0;
	}

	template <class T>
	inline void H3DBitFlags<T>::setFlags(T settings)
	{
		value |= settings;
	}

	template <class T>
	inline void H3DBitFlags<T>::clearFlags(T settings)
	{
		value &= ~settings;
	}

	template <class T>
	inline void H3DBitFlags<T>::setBit(int bit)
	{
		value |= (1<<bit);
	}

	template <class T>
	inline void H3DBitFlags<T>::setBit(int bit, bool setting)
	{
		if(setting)
		{
			value |= (1<<bit);
		}
		else
		{
			clearBit(bit);
		}
	}

	template <class T>
	inline void H3DBitFlags<T>::clearBit(int bit)
	{
		value &= (~(1<<bit));
	}

	template <class T>
	inline bool H3DBitFlags<T>::isEmpty()const
	{
		return(value == static_cast<T>(0));
	}

	template <class T>
	inline bool H3DBitFlags<T>::testBit(int bit)const
	{
		return(value & (1<<bit));
	}

	template <class T>
	inline bool H3DBitFlags<T>::testFlags(T test)const
	{
		return((value & test) == test);
	}

	template <class T>
	inline bool H3DBitFlags<T>::testAny(T test)const
	{
		return(value & test);
	}

	template <class T>
	inline int H3DBitFlags<T>::totalBits()const
	{
		return(sizeof(T)<<3);
	}

	template <class T>
	inline int H3DBitFlags<T>::totalSet()const
	{
		int count=0;
		int total = totalBits();
		T TestValue = value;

		for(int i=total;i;--i)
		{
			count += (TestValue & 1);
			TestValue >>= 1;
		}
		return(count);
	}
}
#pragma warning( default : 4800 )

#endif // h3d_bit_flags_h__