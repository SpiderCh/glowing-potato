#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <array>
#include <stdexcept>

#include "Pipe.h"

class ClientDescriptor
{
public:
	ClientDescriptor() = default;

	ClientDescriptor(int socket, sockaddr_in addr)
		: m_socket(socket)
		, m_addr(addr)
	{}

	ClientDescriptor(const ClientDescriptor &) = default;

	ClientDescriptor(ClientDescriptor && cd)
		: m_socket(cd.m_socket)
		, m_addr(cd.m_addr)
	{
		cd.m_socket = -1;
	}

	~ClientDescriptor()
	{
		close(m_socket);
	}

	ClientDescriptor & operator=(ClientDescriptor && cd)
	{
		m_socket = cd.m_socket;
		m_addr = cd.m_addr;

		cd.m_socket = -1;
		return *this;
	}

	int GetSocket() const
	{
		return m_socket;
	}

private:
	int m_socket = -1;
	sockaddr_in m_addr;
};

class Socket
{
public:
	Socket(int port)
		: m_socket(socket(AF_INET, SOCK_STREAM, 0))
	{
		if(m_socket < 0)
			throw std::runtime_error("Cannot create socket");

		m_serverAddr.sin_addr.s_addr = INADDR_ANY;
		m_serverAddr.sin_port = htons( port );
		m_serverAddr.sin_family = AF_INET;

		if(int res = bind(m_socket,(struct sockaddr *)&m_serverAddr , sizeof(m_serverAddr)); res < 0)
			throw std::runtime_error("Cannot bind");

		if(listen(m_socket, 3) < 0)
			throw std::runtime_error("Cannot start listening");
	}

	void WaitAndProcessClients()
	{
		std::array<ClientDescriptor, 2> clients;
		int client_count = 0;
		while(true)
		{
			int client_socket;
			sockaddr_in client_addr;
			int addrlen = sizeof(client_addr);
			if ((client_socket = accept(m_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen)) == 0)
				continue;

			clients[client_count] = std::move(ClientDescriptor(client_socket, client_addr));
			++client_count;


			if (client_count < 2)
				continue;

			ProcessClients(clients);
			break;
		}
	}

private:
	void ProcessClients(const std::array<ClientDescriptor, 2> & clients)
	{
		Pipe p;

		ssize_t written_to_pipe = 0;
		do
		{
			written_to_pipe = splice(clients[1].GetSocket(), NULL, p.GetPipe(1), NULL, 512, SPLICE_F_MORE | SPLICE_F_MOVE);
			if (written_to_pipe > 0)
				splice(p.GetPipe(0), NULL, clients[0].GetSocket(), 0, written_to_pipe, SPLICE_F_MORE | SPLICE_F_MOVE);
		}
		while(written_to_pipe > 0);
	}

private:
	int m_socket;
	struct sockaddr_in m_serverAddr;
};

constexpr int PORT = 140;

int main(int, char**)
{
	Socket s(PORT);
	s.WaitAndProcessClients();
	return 0;
}