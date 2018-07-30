#pragma once

namespace ShaderLoad
{
	int load(const char *fileName, char *buffer, int bufSize);
	int loadProgram(const char *program, char *vertBuf, char *fragBuf, int bufSize);
}

