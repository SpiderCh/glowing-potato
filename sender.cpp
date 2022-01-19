#include <fcntl.h>

#include <string>
#include <iostream>
#include <stdexcept>

#include "ClientSocket.h"
#include "File.h"

int main(int argc, char ** argv)
{
	if (argc < 2)
		return 1;

	File f(argv[1], O_RDONLY);
	Client::Socket s;
	s.Send(f);
	return 0;
}