#include "ShaderLoader.h"
#include <iostream>
#include <fstream>
#include <string>

namespace ShaderLoad
{
	int load(const char *fileName, char *buffer, int bufSize)
	{
		std::ifstream inFile(fileName);

		inFile.get(buffer, bufSize, '\0');

		if (inFile.fail())
		{
			return -1;
		}

		return 1;
	}

	int loadProgram(const char *prog, char *vertBuf, char *fragBuf, int bufSize)
	{
		std::string program(prog);
		std::ifstream inFile;
		int progLen = program.length();

		if (progLen > 120)
		{
			return -1;
		}

		inFile.open((program + ".vert").c_str());
		inFile.get(vertBuf, bufSize, '\0');
		inFile.close();

		inFile.open((program + ".frag").c_str());
		inFile.get(fragBuf, bufSize, '\0');
		inFile.close();
		return 1;
	}
}
