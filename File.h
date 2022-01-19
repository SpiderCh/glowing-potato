#pragma once

#include <unistd.h>
#include <sys/stat.h>

class File
{
public:
	File(const char * filePath, int flags, mode_t mode)
		: m_fileDescriptor(open(filePath, flags, mode))
	{
		if (m_fileDescriptor < 0)
			throw std::invalid_argument("Cannot open file by path: " + std::string(filePath));
		struct stat st;
		stat(filePath, &st);
		m_fileSize = st.st_size;
	}

	File(const char * filePath, int flags)
		: m_fileDescriptor(open(filePath, flags))
	{
		if (m_fileDescriptor < 0)
			throw std::invalid_argument("Cannot open file by path: " + std::string(filePath));
		struct stat st;
		stat(filePath, &st);
		m_fileSize = st.st_size;
	}

	~File()
	{
		close(m_fileDescriptor);
	}

	int GetDesriptor() const
	{
		return m_fileDescriptor;
	}

	off_t GetSize() const
	{
		return m_fileSize;
	}

private:
	const int m_fileDescriptor;
	off_t m_fileSize;
};