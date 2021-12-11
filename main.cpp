#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>


bool compress()
{
	std::cout << "Compress" << std::endl;
	return true;
}

bool decompress()
{
	std::cout << "Decompress" << std::endl;
	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		return 1;
	}

	std::string command{ argv[1] };

	if (command == "compress")
	{
		compress();
	}
	else if(command == "decompress")
	{
		decompress();
	}
	else
	{
		return 2;
	}

	char* inputFileName = argv[2];
	std::ifstream ifs{ inputFileName };
	if (!ifs.is_open())
	{
		return 3;
	}

	char* outputFileName = argv[3];
	std::ofstream outs{ outputFileName };
	if (!outs.is_open())
	{
		return 4;
	}

	return 0;
}
