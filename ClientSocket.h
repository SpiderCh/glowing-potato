#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "File.h"

namespace Client
{

class Socket
{
public:
	Socket()
		: m_socket(socket(AF_INET, SOCK_STREAM, 0))
	{
		if(m_socket < 0)
			throw std::runtime_error("Cannot create socket");

		m_serverAddr.sin_family = AF_INET;
		m_serverAddr.sin_port = htons(140);
		m_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if(connect(m_socket, (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0)
			throw std::runtime_error("Cannot create connection");
	}

	~Socket()
	{
		close(m_socket);
	}

	int GetSocket() const
	{
		return m_socket;
	}

	void Send(const File & f)
	{
		ssize_t write_file_size = 0;
		off_t offset = 0;
		while(write_file_size < f.GetSize())
			write_file_size += sendfile(m_socket, f.GetDesriptor(), &offset, 512);
	}

private:
	int m_socket;
	struct sockaddr_in m_serverAddr;
};

}