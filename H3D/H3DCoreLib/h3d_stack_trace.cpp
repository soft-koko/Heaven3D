#include "h3d_pch.h"
#include "h3d_stack_trace.h"
#include "h3d_debug.h"

namespace Heaven3D
{
	typedef std::vector<uint32_t> ADDRESS_VECTOR ;

	// 当前的进程句柄
	static HANDLE g_hProcess = 0;

	static DWORD __stdcall get_mod_base( HANDLE process_handle, DWORD address )
	{
		IMAGEHLP_MODULE image_help ;
		image_help.SizeOfStruct = sizeof( IMAGEHLP_MODULE );

		if( SymGetModuleInfo(g_hProcess, address, &image_help ) )
		{
			return( image_help.BaseOfImage );
		}
		else
		{
			MEMORY_BASIC_INFORMATION mem_base_info;

			if( 0 != VirtualQueryEx( process_handle,(LPCVOID)address ,
				&mem_base_info , sizeof( mem_base_info )  ) )
			{
				uint32_t name_len = 0 ;
				TCHAR file[ MAX_PATH ];

				name_len = GetModuleFileName((HINSTANCE)mem_base_info.AllocationBase,
					file, MAX_PATH );

				HANDLE file_handle = NULL ;

				if( 0 != name_len )
				{
					file_handle = CreateFile( file,GENERIC_READ,
						FILE_SHARE_READ,NULL,OPEN_EXISTING,0,0);
				}

				SymLoadModule( g_hProcess, file_handle,
					(name_len ? file : NULL), NULL,
					(uint32_t)mem_base_info.AllocationBase, 0);

				return((uint32_t)mem_base_info.AllocationBase );
			}
		}
		return( 0 );
	}

	static uint32_t convert_address( uint32_t address, LPTSTR output_buff )
	{
		char temp [MAX_PATH + sizeof( IMAGEHLP_SYMBOL )];
		PIMAGEHLP_SYMBOL pIHSymbol =(PIMAGEHLP_SYMBOL)&temp ;
		IMAGEHLP_MODULE image_help ;
		LPTSTR curr_pos = output_buff ;

		ZeroMemory( pIHSymbol, MAX_PATH + sizeof( IMAGEHLP_SYMBOL ) );
		ZeroMemory( &image_help, sizeof( IMAGEHLP_MODULE ) );

		pIHSymbol->SizeOfStruct = sizeof( IMAGEHLP_SYMBOL );
		pIHSymbol->Address = address ;
		pIHSymbol->MaxNameLength = MAX_PATH ;

		image_help.SizeOfStruct = sizeof( IMAGEHLP_MODULE );

		// Always stick the address in first.
		curr_pos += wsprintf( curr_pos, _T( "0x%08X " ), address );

		// Get the module name.
		if( 0 != SymGetModuleInfo( g_hProcess, address, &image_help ) )
		{
			// Strip off the path.
			LPTSTR name = _tcsrchr( image_help.ImageName, _T( '\\' ) );
			
			if( NULL != name )
			{
				name++;
			}
			else
			{
				name = image_help.ImageName ;
			}

			curr_pos += wsprintf( curr_pos, _T( "%s: " ), name );
		}
		else
		{
			curr_pos += wsprintf( curr_pos, _T( "<unknown module>: " ) );
		}

		// Get the function.
		uint32_t disp ;
		
		if( 0 != SymGetSymFromAddr( g_hProcess, address, (PDWORD)&disp, pIHSymbol ) )
		{
			if( 0 == disp )
			{
				curr_pos += wsprintf( curr_pos, _T( "%s" ), pIHSymbol->Name);
			}
			else
			{
				curr_pos += wsprintf( curr_pos, _T( "%s + %d bytes" ), pIHSymbol->Name,disp);
			}

			// If I got a symbol, give the source and line a whirl.
			IMAGEHLP_LINE imagehlp_line ;
			ZeroMemory( &imagehlp_line, sizeof( IMAGEHLP_LINE ) );
			imagehlp_line.SizeOfStruct = sizeof( IMAGEHLP_LINE );

			if( 0 != SymGetLineFromAddr( g_hProcess, address, (PDWORD)&disp,&imagehlp_line   ) )
			{
				// Put this on the next line and indented a bit.
				curr_pos += wsprintf( curr_pos, _T("\n\t\t%s, Line %d"),
					imagehlp_line.FileName, imagehlp_line.LineNumber );
				
				if( 0 != disp )
				{
					curr_pos += wsprintf( curr_pos,_T( " + %d bytes" ), disp);
				}
			}
		}
		else
		{
			curr_pos += wsprintf( curr_pos, _T("<unknown symbol>") );
		}

		// Tack on a CRLF.
		curr_pos += wsprintf( curr_pos, _T( "\n" ) );
		return( curr_pos - output_buff );
	}

	void BuildStackTrace( TCHAR* _string,uint32_t  _size, uint32_t  num_skip  )
	{
		HANDLE process_handle = GetCurrentProcess();

		// if the symbol handler is not initialized, 
		// set it up now.
		if( !g_hProcess && SymInitialize(process_handle, NULL, FALSE))
		{
			uint32_t dwOpts = SymGetOptions( );

			// Turn on load lines.
			SymSetOptions( dwOpts|SYMOPT_LOAD_LINES      );

			g_hProcess = process_handle;
		}

		ADDRESS_VECTOR vAddrs;
		CONTEXT context;

		context.ContextFlags = CONTEXT_FULL ;

		if( GetThreadContext( GetCurrentThread( ), &context ) )
		{
			STACKFRAME stackFrame ;
			uint32_t   dwMachine ;
			ZeroMemory( &stackFrame, sizeof( STACKFRAME ) );
			
			stackFrame.AddrPC.Mode = AddrModeFlat ;
			dwMachine                = IMAGE_FILE_MACHINE_I386 ;
			stackFrame.AddrPC.Offset    = context.Eip   ;
			stackFrame.AddrStack.Offset = context.Esp   ;
			stackFrame.AddrStack.Mode   = AddrModeFlat ;
			stackFrame.AddrFrame.Offset = context.Ebp   ;
			stackFrame.AddrFrame.Mode   = AddrModeFlat ;

			// Loop for the first 512 stack elements.
			for( uint32_t i = 0 ; i < 512 ; i++ )
			{
				if( FALSE == StackWalk( dwMachine,
					process_handle,process_handle,
					&stackFrame,&context,NULL,
					SymFunctionTableAccess,
					get_mod_base,NULL) )
					
				{
					break ;
				}

				if( i > num_skip )
				{
					// Also check that the address is not zero.  Sometimes
					//  StackWalk returns TRUE with a frame of zero.
					if( 0 != stackFrame.AddrPC.Offset )
					{
						vAddrs.push_back( stackFrame.AddrPC.Offset );
					}
				}
			}

			// Now start converting the addresses.
			uint32_t dwSizeLeft = _size ;
			uint32_t dwSymSize ;

			TCHAR szSym[ MAX_PATH * 2 ];
			LPTSTR szCurrPos = _string ;

			ADDRESS_VECTOR::iterator loop ;
			for( loop =  vAddrs.begin( );
				loop != vAddrs.end( )  ;
				loop++                     )
			{

				dwSymSize = convert_address( *loop, szSym );
				if( dwSizeLeft < dwSymSize )
				{
					break ;
				}
				_tcscpy( szCurrPos, szSym );
				szCurrPos += dwSymSize ;
				dwSizeLeft -= dwSymSize ;
			}
		}
	}
}