/********************************************************************
created:	2012/02/18
created:	18:2:2012   0:32
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_debug.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_debug_h__
#define h3d_debug_h__
//   类
#include "h3d_bit_flags.h"

namespace Heaven3D
{

#ifdef ASSERT
#undef ASSERT
#endif

#ifdef assert
#undef assert
#endif

#ifdef VERIFY
#undef VERIFY
#endif

#ifdef verify
#undef verify
#endif

#ifdef TRACE
#undef TRACE
#endif


#define Debug g_Debug


#ifdef _DEBUG_OUTPUT
	//
	// DEBUG OUTPUT ENABLED
	//
#define debug_print Debug.output

#define debug_assert(x, comment) {\
	static bool _ignoreAssert = false;\
	if(!_ignoreAssert && !(x)) \
	{\
	ERROR_RESULT _err_result = notify_assertion(_text(#x), _text(comment), __FILE__, __LINE__);\
	if(_err_result == VR_IGNORE)\
	{\
	_ignoreAssert = true;\
}\
				else if(_err_result == VR_BREAKPOINT)\
	{\
	_asm{int 3};\
}\
}}

#define debug_error(x) {\
	static bool _ignoreError = false;\
	if(!_ignoreError) \
	{\
	ERROR_RESULT _err_result = notify_error((x), \
	__FILE__, __LINE__);\
	if(_err_result == VR_IGNORE)\
	{\
	_ignoreError = true;\
}\
				else if(_err_result == VR_BREAKPOINT)\
	{\
	_asm{int 3};\
}\
}}	


#else

	// depending on the compiler version, 
	// we either set unused function definitions
	// to (0) or __noop
#ifndef NOP_FUNCTION
#if(_MSC_VER >= 1210)
#define NOP_FUNCTION __noop
#else
#define NOP_FUNCTION (void)(0)
#endif
#endif

#define debug_print (void)NOP_FUNCTION 
#define debug_assert(x, comment)
#define debug_error(x)

#endif


#define compiler_assert(x) {\
	const int _value = (x) ? 1:0;\
	switch (x)\
	{\
	case 0: \
	case _value: \
	default: break;\
};}


#define ASSERT(x) debug_assert(x,"")
#define assert(x) debug_assert(x,"")
#define error(x) debug_error(x)
#define TRACE debug_print

	// Assert function return values
	enum ERROR_RESULT
	{
		VR_IGNORE = 0,
		VR_CONTINUE,
		VR_BREAKPOINT,
		VR_ABORT
	};

	extern ERROR_RESULT notify_error(uint32_t errorCode, const TCHAR* fileName, int lineNumber);
	extern ERROR_RESULT notify_assertion(const TCHAR* condition, const TCHAR* description, const TCHAR* fileName, int lineNumber);
	extern ERROR_RESULT display_error( const TCHAR* errorTitle,const TCHAR* errorText,const TCHAR* errorDescription,const TCHAR* fileName, int lineNumber);


	/*	H3DDebugMessageHandler
	-----------------------------------------------------------------

	H3DDebugMessageHandler is a singleton interface to handle the output of 
	text messages for debugging purposes. A Total of 32 channels
	are available for output. Channel Zero is reserved for standard
	debug output. All other channels can be routed to files, etc.

	-----------------------------------------------------------------
	*/
	class H3DDebugMessageChannel;

	class H3DDebugMessageHandler
	{
	public:

		// Data Types & Constants...
		enum
		{
			nMaxOutputStringSize = 2048
		};

		enum DEBUG_CHANNELS
		{
			nSystemChannelBit = 0,
			nMaxChannels = 32,
		};

		enum MESSAGE_FLAGS
		{
			k_defaultMessageFlag	 = FLAG(0),
		};

		H3DDebugMessageHandler();
		~H3DDebugMessageHandler();
		bool set_channel(int index, H3DDebugMessageChannel* pChannel);
		void clear_channel(int index);
		bool open_channel(int index);
		void close_channel(int index);
		void set_channel_message_filter(int index, u32Flags flags);
		void add_channel_message_filter(int index, u32Flags flags);
		void remove_channel_message_filter(int index, u32Flags flags);

		void output(const TCHAR* text, ...);
		void output(uint32_t messageFlags, const TCHAR* text, ...);
		void process_message(uint32_t messageFlags, const TCHAR* text, va_list arg_list);

		bool is_channel_set(int index)const;
		bool is_channel_open(int index)const;
		u32Flags channel_filter(int index)const;
	private:
		H3DDebugMessageChannel*		channels_[nMaxChannels];
		u32Flags					open_channels_;
	};

	extern H3DDebugMessageHandler g_Debug;

	inline bool H3DDebugMessageHandler::is_channel_set(int index)const
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		return channels_[index] != 0;
	}

	inline bool H3DDebugMessageHandler::is_channel_open(int index)const
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		return TEST_BIT(open_channels_,index);
	}

}
#endif
