#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>

/// <summary>
/// The function loads the entire file into memory. It is not memory efficient, but it will ensure 
/// that the program will work even if the input and output files are the same. 
/// </summary>
/// <param name="file">vector that represents the entire file</param>
/// <param name="ifs">opened input file stream</param>
void read_file(std::vector<std::string>& file, std::ifstream& ifs)
{
	std::string strLine;

	while (std::getline(ifs, strLine))
	{
		file.push_back(strLine);
	}
}

/// <summary>
/// A function that compresses a file according to specified rules.
/// </summary>
/// <param name="file">vector that represents the entire file</param>
/// <param name="ofs">opened output file stream</param>
/// <returns>true if the function was successful, false if an error occurred</returns>
bool compress(std::vector<std::string>& file, std::ofstream& ofs)
{
	std::regex regex1(R"(^[a-zA-Z]*$)");

	uint64_t line_num = 0;

	for (std::string& strLine : file)
	{
		line_num++;

		if (strLine.length() == 0)
		{
			ofs << std::endl;
			continue;
		}

		if (!regex_match(strLine, regex1))
		{
			std::cout << "Error on line " << line_num << std::endl;
			return false;
		}

		std::string outLine = "";

		uint64_t new_position = 0;
		uint64_t last_position = 0;

		while (new_position != std::string::npos)
		{
			outLine.push_back(strLine[new_position]);
			last_position = new_position;
			new_position = strLine.find_first_not_of(strLine[last_position], last_position);
			if (new_position == std::string::npos)
			{
				outLine.append(std::to_string(strLine.length() - last_position));
			}
			else
			{
				outLine.append(std::to_string(new_position - last_position));
			}
		}

		ofs << outLine << std::endl;
	}

	return true;
}

/// <summary>
/// A function that decompresses a file according to specified rules.
/// </summary>
/// <param name="file">vector that represents the entire file</param>
/// <param name="ofs">opened output file stream</param>
/// <returns>true if the function was successful, false if an error occurred</returns>
bool decompress(std::vector<std::string>& file, std::ofstream& ofs)
{
	std::regex regex1(R"(^[a-zA-Z0-9]*$)");
	std::string letters{ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" };

	uint64_t line_num = 0;

	for (std::string& strLine : file)
	{
		line_num++;

		if (strLine.length() == 0)
		{
			ofs << std::endl;
			continue;
		}

		if (!regex_match(strLine, regex1))
		{
			std::cout << "Error on line " << line_num << std::endl;
			return false;
		}

		std::string outLine = "";

		uint64_t new_position_letter = strLine.find_first_of(letters, 0);
		uint64_t last_position_letter = 0;

		if (new_position_letter != 0 || new_position_letter == std::string::npos)
		{
			std::cout << "Error on line " << line_num << std::endl;
			return false;
		}

		uint64_t number = 0;

		while (new_position_letter != std::string::npos)
		{
			last_position_letter = new_position_letter;
			new_position_letter = strLine.find_first_of(letters, last_position_letter + 1);

			uint64_t start = last_position_letter + 1;
			uint64_t letters_count = 0;

			if (new_position_letter == std::string::npos)
			{
				letters_count = strLine.length();
			}
			else
			{
				letters_count = new_position_letter - last_position_letter - 1;
			}

			try
			{
				number = std::stoll(strLine.substr(start, letters_count));
			}
			catch (const std::exception&)
			{
				std::cout << "Error on line " << line_num << std::endl;
				return false;
			}

			outLine.append(std::string(number, strLine[last_position_letter]));
		}

		ofs << outLine << std::endl;
	}

	return true;
}

/// <summary>
/// Main function of program. Function reads the file and calls the processing function. 
/// </summary>
/// <param name="argc">Argument count</param>
/// <param name="argv">Argument vector</param>
/// <returns>0 if the program worked properly, something else otherwise</returns>
int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		return 1;
	}

	std::string command{ argv[1] };
	if (!(command == "compress" || command == "decompress"))
	{
		return 2;
	}

	const char* inputFileName = argv[2];
	std::ifstream ifs{ inputFileName };
	if (!ifs.is_open())
	{
		return 3;
	}

	std::vector<std::string> file;
	read_file(file, ifs);
	
	if (!ifs.eof())
	{
		return 7;
	}
	ifs.close();

	const char* outputFileName = argv[3];
	std::ofstream ofs{ outputFileName };
	if (!ofs.is_open())
	{
		return 4;
	}

	if (command == "compress")
	{
		if (!compress(file, ofs))
		{
			return 5;
		}
	}
	else if (command == "decompress")
	{
		if (!decompress(file, ofs))
		{
			return 6;
		}
	}

	ofs.close();

	return 0;
}
