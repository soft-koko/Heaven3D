/********************************************************************
created:	2012/02/18
created:	18:2:2012   1:15
filename: 	e:\Work\Heaven3D\H3DCoreLib\h3d_file.h
author:		Xiong Xinke

purpose:	
*********************************************************************/
#ifndef h3d_file_h__
#define h3d_file_h__

#include "h3d_string.h"
//   类
namespace Heaven3D
{
	class H3DFile
	{
	public:
		enum FILE_ACCESS
		{
			READ_ONLY,
			WRITE_ONLY,
			READ_WRITE,
			APPEND_ONLY,
			READ_AND_APPEND
		};

		H3DFile();
		H3DFile(const H3DString& FileName, uint32_t access_rights=READ_WRITE);
		~H3DFile();

		bool open();
		bool open(const H3DString& FileName, uint32_t access_rights=READ_WRITE);
		bool create();
		bool create(const H3DString& FileName);
		void close();

		H3DFile& operator>>(int8_t& data);
		H3DFile& operator>>(uint8_t& data);
		H3DFile& operator>>(int16_t& data);
		H3DFile& operator>>(uint16_t& data);
		H3DFile& operator>>(int32_t& data);
		H3DFile& operator>>(uint32_t& data);
		H3DFile& operator>>(float& data);
		H3DFile& operator>>(bool& data);

		H3DFile& operator<<(int8_t data);
		H3DFile& operator<<(uint8_t data);
		H3DFile& operator<<(int16_t data);
		H3DFile& operator<<(uint16_t data);
		H3DFile& operator<<(int32_t data);
		H3DFile& operator<<(uint32_t data);
		H3DFile& operator<<(float data);
		H3DFile& operator<<(bool data);

		// writing functions
		bool write(const void *src, uint32_t bytes_to_write);
		bool write(const H3DString& string, uint32_t* bytes_written=0);
		bool write(int8_t value);
		bool write(uint8_t value);
		bool write(int16_t value);
		bool write(uint16_t value);
		bool write(int32_t value);
		bool write(uint32_t value);
		bool write(float value);
		bool write(bool value);

		// reading functions
		bool read(void *dest, uint32_t bytes_to_read);
		bool read(H3DString& string);
		bool read(int8_t& value);
		bool read(uint8_t& value);
		bool read(int16_t& value);
		bool read(uint16_t& value);
		bool read(int32_t& value);
		bool read(uint32_t& value);
		bool read(float& value);
		bool read(bool& value);

		// file position
		uint32_t position();
		bool set_position(uint32_t NewPos);
		bool seek(long Offset);

		// accessors...
		bool isFileOpen();
		const H3DString& name();
		uint32_t size();

	private:
		uint32_t	file_access_;
		FILE*		file_handle_;
		H3DString		file_name_;

		H3DFile( const H3DFile& src);
		H3DFile& operator=( const H3DFile& src);
		bool operator == ( const H3DFile&  src) const;
	};

	inline H3DFile::H3DFile():file_handle_(0)
	{
		file_name_.erase();
	}

	inline H3DFile::H3DFile(const H3DString& file_name, uint32_t access_rights)
		:file_handle_(0)
		,file_name_(file_name)
		,file_access_(access_rights)
	{
	}

	inline H3DFile::~H3DFile()
	{
		close();
	}

	inline H3DFile& H3DFile::operator>>(int8_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(uint8_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(int16_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(uint16_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(int32_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(uint32_t& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(float& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator>>(bool& data)
	{
		read((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(int8_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(uint8_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(int16_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(uint16_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(int32_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(uint32_t data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(float data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline H3DFile& H3DFile::operator<<(bool data)
	{
		write((void*)&data, sizeof(data));
		return(*this);
	}

	inline bool H3DFile::isFileOpen()
	{
		return (file_handle_ ? true : false);
	}

	inline const H3DString& H3DFile::name()
	{
		return file_name_;
	}

}

#endif // h3d_file_h__
