#pragma once

namespace ShaderLoad
{
	int load(char *fileName, char *buffer, int bufSize);
	int loadProgram(char *program, char *vertBuf, char *fragBuf, int bufSize);
}

