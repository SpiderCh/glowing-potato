#include <fcntl.h>

#include <string>
#include <iostream>
#include <stdexcept>

#include "Pipe.h"
#include "ClientSocket.h"
#include "File.h"


int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		std::cout << "Too few argumenys" << std::endl;
		return 1;
	}

	try
	{
		File f(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		Client::Socket s;
		Pipe p;

		ssize_t written_to_pipe = 0;
		off_t written_to_file = 0;
		do
		{
			written_to_pipe = splice(s.GetSocket(), NULL, p.GetPipe(1), NULL, 512, SPLICE_F_MORE | SPLICE_F_MOVE);
			if (written_to_pipe > 0)
				splice(p.GetPipe(0), NULL, f.GetDesriptor(), &written_to_file, written_to_pipe, SPLICE_F_MORE | SPLICE_F_MOVE);
		}
		while(written_to_pipe > 0);
	}
	catch(const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}