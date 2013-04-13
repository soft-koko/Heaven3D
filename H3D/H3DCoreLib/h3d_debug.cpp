#define CDEBUG_CPP
#define CORE_DLL

#include "h3d_pch.h"
#include "h3d_debug.h"
#include "h3d_debug_channel.h"
#include "h3d_default_output_channel.h"
#include "h3d_stack_trace.h"
//   类
namespace Heaven3D
{
	static H3DDefaultOutputChannel g_defaultChannel;
	H3DDebugMessageHandler Heaven3D::g_Debug;

#ifdef _DEBUG
#define _STACKTRACE
#endif



	/*	H3DDebugMessageHandler
	-----------------------------------------------------------------

	Default Constructor

	-----------------------------------------------------------------
	*/
	H3DDebugMessageHandler::H3DDebugMessageHandler()
		:open_channels_(0)
	{
		memset(channels_, 0, sizeof(channels_));
		channels_[0] = &g_defaultChannel;
		open_channel(0);
	}


	/*	~H3DDebugMessageHandler
	-----------------------------------------------------------------

	Default Destructor

	-----------------------------------------------------------------
	*/
	H3DDebugMessageHandler::~H3DDebugMessageHandler()
	{
		// close the default system channel
		close_channel(0);

		// panic if channels have been left open.
		debug_assert(open_channels_.isEmpty(), "debug channels left open");
	}


	void H3DDebugMessageHandler::set_channel_message_filter(int index, u32Flags flags)
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		debug_assert(channels_[index], "invalid channel index");

		channels_[index]->messageFilter = flags;
	}

	void H3DDebugMessageHandler::add_channel_message_filter(int index, u32Flags flags)
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		debug_assert(channels_[index], "invalid channel index");

		channels_[index]->messageFilter.setFlags(flags);
	}

	void H3DDebugMessageHandler::remove_channel_message_filter(int index, u32Flags flags)
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		debug_assert(channels_[index], "invalid channel index");

		channels_[index]->messageFilter.clearFlags(flags);
	}

	bool H3DDebugMessageHandler::set_channel(int index, H3DDebugMessageChannel* pChannel)
	{
		// channel 0 is reserved. index must be in range 1-nMaxChannels
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");

		if(!channels_[index])
		{
			channels_[index] = pChannel;
			return true;
		}
		// the channel is already in use
		return false;
	}

	void H3DDebugMessageHandler::clear_channel(int index)
	{
		// channel 0 is reserved. index must be in range 1-nMaxChannels
		debug_assert(index > 0 && index<nMaxChannels, "invalid channel index");
		debug_assert(!TEST_BIT(open_channels_,index), "the channel is currently open");

		channels_[index] = 0;
		open_channels_.clearBit(index);
	}

	bool H3DDebugMessageHandler::open_channel(int index)
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");

		if(!TEST_BIT(open_channels_,index))
		{
			if(channels_[index] && channels_[index]->open())
			{
				open_channels_.setBit(index);
				return true;
			}

			// either there is no channel, or it failed to open
			return false;
		}

		// the channel is already open
		return true;
	}

	void H3DDebugMessageHandler::close_channel(int index)
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");

		if(TEST_BIT(open_channels_,index))
		{
			if(channels_[index])
			{
				channels_[index]->close();
				open_channels_.clearBit(index);
			}
		}
	}


	u32Flags H3DDebugMessageHandler::channel_filter(int index)const
	{
		debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
		debug_assert(channels_[index], "invalid channel index");

		return channels_[index]->messageFilter;
	}


	// route text input using the internal default message flag
	void H3DDebugMessageHandler::output(const TCHAR* text, ...)
	{
		// build the va_list of optional arguments
		va_list		arg_list;
		va_start(arg_list, text);

		// call the va_list version of output
		process_message(k_defaultMessageFlag, text, arg_list);

		// end the optional argument list use
		va_end(arg_list);
	}

	// route text input using the message flag provided
	void H3DDebugMessageHandler::output(uint32_t messageFlags, const TCHAR* text, ...)
	{
		// build the va_list of optional arguments
		va_list		arg_list;
		va_start(arg_list, text);

		// call the va_list version of output
		process_message(messageFlags, text, arg_list);

		// end the optional argument list use
		va_end(arg_list);
	}

	// the function which handles the actual routing of text messages
	void H3DDebugMessageHandler::process_message(uint32_t messageFlags, const TCHAR* text, va_list arg_list)
	{
		// are any channels currently open?
		if( !open_channels_  )
			return;

		// build our output string
		TCHAR buffer[nMaxOutputStringSize+1];
		int Size = _vsntprintf(buffer, nMaxOutputStringSize, text, arg_list);

		if( Size <= 0 )
			return;

		// if a string was built...

		// run through all the channels
		for(int i=0; i<nMaxChannels; ++i)
		{
			// if the channel is open...
			if( TEST_BIT(open_channels_,i) && 
				channels_[i]->messageFilter.testAny(messageFlags))
			{
				// then send the message through
				channels_[i]->write(buffer);
			}
		}
	}

	ERROR_RESULT notify_error(uint32_t errorCode, const TCHAR* fileName, int lineNumber)
	{
		// if no error code is provided, get the last known error
		if(errorCode == 0)
		{
			errorCode = GetLastError();
		}

		// use DirectX to supply a string and description for our error.
		// This will handle all known DirectX error codes (HRESULTs)
		// as well as Win32 error codes normally found via FormatMessage
		const TCHAR* pErrorString = DXGetErrorString(errorCode);
		const TCHAR* pErrorDescription = DXGetErrorDescription(errorCode);

		// pass the data on to the message box
		ERROR_RESULT result = display_error(	_T("Debug Error!"),
			pErrorString,
			pErrorDescription,
			fileName,
			lineNumber);

		// Put the incoming last error back.
		SetLastError(errorCode);

		return(result);
	}

	ERROR_RESULT notify_assertion(const TCHAR* condition, const TCHAR* description, const TCHAR* fileName, int lineNumber)
	{
		// pass the data on to the message box
		ERROR_RESULT result = display_error(_T("Assert Failed!"),
			condition,
			description,
			fileName,
			lineNumber);

		return(result);

	}

	ERROR_RESULT display_error(const TCHAR* errorTitle,
		const TCHAR* errorText,
		const TCHAR* errorDescription,
		const TCHAR* fileName, 
		int lineNumber)
	{
		const	int		MODULE_NAME_SIZE = 255;
		TCHAR moduleName[MODULE_NAME_SIZE];

		// attempt to get the module name
		if(!GetModuleFileName(NULL, moduleName, MODULE_NAME_SIZE))
		{
			_tcscpy(moduleName, _T("<unknown application>"));
		}

		// if stack tracing is enabled, build a string containing the 
		// unwound stack information
#ifdef _STACKTRACE
		const	int		STACK_STRING_SIZE = 255;
		TCHAR stackText[STACK_STRING_SIZE];

		BuildStackTrace(stackText, STACK_STRING_SIZE, 2);
#else
		TCHAR stackText[] = _T("<stack trace disabled>");
#endif

		// build a collosal string containing the entire asster message
		const	int		MAX_BUFFER_SIZE = 1024;
		TCHAR	buffer[MAX_BUFFER_SIZE];

		int Size = _sntprintf(	buffer, 
			MAX_BUFFER_SIZE, 
			_T(	"%s\n\n"						\
			"Program : %s\n"               \
			"File : %s\n"                  \
			"Line : %d\n"                  \
			"Error: %s\n"			\
			"Comment: %s\n"						\
			"\nStack:\n%s\n\n"						\
			"Abort to exit (or debug), Retry to continue,\n"\
			"Ignore to disregard all occurances of this error\n"),
			errorTitle,
			moduleName,
			fileName,
			lineNumber,
			errorText,
			errorDescription,
			stackText
			);


		// place a copy of the message into the clipboard
		if(OpenClipboard(NULL))
		{
			uint32_t bufferLength = _tcsclen(buffer);
			HGLOBAL hMem = GlobalAlloc(GHND|GMEM_DDESHARE, bufferLength+1);

			if(hMem)
			{
				uint8_t* pMem = (uint8_t*)GlobalLock(hMem);
				memcpy(pMem, buffer, bufferLength);
				GlobalUnlock(hMem);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
			}

			CloseClipboard();
		}


		// find the top most window of the current application
		HWND hWndParent = GetActiveWindow ( ) ;
		if( NULL != hWndParent )
		{
			hWndParent = GetLastActivePopup ( hWndParent ) ;
		}

		// put up a message box with the error
		int iRet = MessageBox ( hWndParent,
			buffer,
			_T( "ERROR NOTIFICATION..." ),
			MB_TASKMODAL
			|MB_SETFOREGROUND
			|MB_ABORTRETRYIGNORE
			|MB_ICONERROR);

		// Figure out what to do on the return.
		if(iRet == IDRETRY)
		{
			// ignore this error and continue
			return (VR_CONTINUE);
		}
		if(iRet == IDIGNORE)
		{
			// ignore this error and continue,
			// plus never stop on this error again (handled by the caller)
			return (VR_IGNORE);
		}

		// The return has to be IDABORT, but does the user want to enter the debugger
		// or just exit the application?
		iRet = MessageBox( hWndParent,
			"Do you wish to debug the last error?",
			_T( "DEBUG OR EXIT?" ),
			MB_TASKMODAL
			|MB_SETFOREGROUND
			|MB_YESNO
			|MB_ICONQUESTION);

		if(iRet == IDYES)
		{
			// inform the caller to break on the current line of execution
			return (VR_BREAKPOINT);
		}

		// must be a full-on termination of the app
		ExitProcess ( (UINT)-1 ) ;
		return (VR_ABORT);
	}
}