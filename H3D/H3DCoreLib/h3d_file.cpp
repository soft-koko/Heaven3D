#include "h3d_pch.h"
#include "h3d_string.h"
#include "h3d_debug.h"
#include "h3d_file.h"
//   类
namespace Heaven3D
{
	bool H3DFile::open()
	{
		debug_assert(file_name_.size() != 0, "No filename provided");
		return open(file_name_, file_access_);
	}

	bool H3DFile::open(const H3DString& file_name, uint32_t access_rights)
	{
		TCHAR file_access[3];

		close();

		switch(access_rights)
		{
		case READ_ONLY:
			_tcscpy(file_access,_T("rb"));
			break;
		case WRITE_ONLY:
			_tcscpy(file_access,_T("wb"));
			break;
		case APPEND_ONLY:
			_tcscpy(file_access,_T("ab"));
			break;
		case READ_AND_APPEND:
			_tcscpy(file_access,_T("a+b"));
			break;

		default:
		case READ_WRITE:
			_tcscpy(file_access,_T("r+b"));
			break;

		}

		file_handle_ = _tfopen( file_name.c_str() ,file_access);

		if(file_handle_==0)
		{
			return (false);
		}

		file_access_=access_rights;
		file_name_=file_name;

		return true;
	}

	bool H3DFile::create()
	{
		return open(file_name_);
	}

	bool H3DFile::create(const H3DString& file_name)
	{
		close();

		file_handle_ = _tfopen( file_name.c_str(),_T("w+b"));

		if(file_handle_==0)
		{
			return (false);
		}

		file_access_=READ_WRITE;
		file_name_=file_name;

		return true;
	}

	void H3DFile::close()
	{
		if(file_handle_)
		{
			fflush( file_handle_);
			fclose( file_handle_);
			file_handle_ = 0;
		}
	}

	bool H3DFile::read(void *dest, uint32_t bytes_to_read)
	{
		unsigned long BytesRead = 0;
		debug_assert(file_handle_, "There is no file handle open");
		BytesRead = fread(dest, 1, bytes_to_read, (FILE*)file_handle_);
		return (BytesRead == bytes_to_read);
	}

	bool H3DFile::write(const void *Src, uint32_t bytes_to_write)
	{
		unsigned long BytesWritten = 0;
		debug_assert(file_handle_, "There is no file handle open");

		if(bytes_to_write && Src)
		{
			BytesWritten = fwrite(Src, 1, bytes_to_write, (FILE*)file_handle_);
		}

		return (BytesWritten == bytes_to_write);
	}

	bool H3DFile::write(int32_t value)
	{
		return write(&value, sizeof(int32_t));
	}

	bool H3DFile::write(float value)
	{
		return write(&value, sizeof(float));
	}

	bool H3DFile::write(uint32_t value)
	{
		return write(&value, sizeof(uint32_t));
	}

	bool H3DFile::write(bool value)
	{
		return write(&value, sizeof(bool));
	}

	bool H3DFile::read(int32_t& value)
	{
		return read(&value, sizeof(int));
	}

	bool H3DFile::read(float& value)
	{
		return read(&value, sizeof(float));
	}

	bool H3DFile::read(uint32_t& value)
	{
		return read(&value, sizeof(uint32_t));
	}

	bool H3DFile::read(bool& value)
	{
		return read(&value, sizeof(bool));
	}

	uint32_t H3DFile::position()
	{
		debug_assert(file_handle_, "There is no file handle open");
		return ftell(file_handle_);
	}

	bool H3DFile::set_position(uint32_t NewPos)
	{
		debug_assert(file_handle_, "There is no file handle open");
		return fseek(file_handle_, (long)NewPos, SEEK_SET) ? false : true;
	}

	bool H3DFile::seek(long Offset)
	{
		debug_assert(file_handle_, "There is no file handle open");
		return fseek(file_handle_, (long)Offset, SEEK_CUR) ? false : true;
	}

	bool H3DFile::write(const H3DString& string, uint32_t* bytes_written)
	{
		char* name =0;
		int32_t string_length=string.size()+1;

		if(write(string_length))
		{
			if(!string_length ||write((void*)string.c_str(), string_length))
			{
				if(bytes_written)
				{
					*bytes_written = string_length + sizeof(string_length);
				}
				return true;
			}
		}
		return false;
	}

	bool H3DFile::read(H3DString& string)
	{
		bool result = false;
		TCHAR* name =0;
		int32_t string_length=0;

		if(read(string_length))
		{
			if(string_length)
			{
				name = new char[string_length];
				if(read(name, string_length))
				{
					string = name;
					delete [](name);
					result = true;
				}
			}
			else
			{
				string.erase();
				result = true;
			}
		}
		return result;
	}

	uint32_t H3DFile::size()
	{
		debug_assert(file_handle_, "There is no file handle open");
		uint32_t CurrentPos = position();
		fseek( file_handle_, (long)0, SEEK_END);
		uint32_t size = position();
		set_position(CurrentPos);
		return size;
	}
}