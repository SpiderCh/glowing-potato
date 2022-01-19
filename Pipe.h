#pragma once

#include <fcntl.h>
#include <unistd.h>

class Pipe
{
public:
	Pipe()
	{
		int result = pipe(m_pipefd);
		if (result < 0)
			throw std::runtime_error("Cannot create pipe");
	}

	~Pipe()
	{
		close(m_pipefd[0]);
		close(m_pipefd[1]);
	}

	int GetPipe(int num) const
	{
		if (num > 1 || num < 0)
			throw std::invalid_argument("Out of range");

		return m_pipefd[num];
	}

private:
	int m_pipefd[2];
};