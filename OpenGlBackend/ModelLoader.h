#pragma once

enum ModelLoaderInclude {
	//This will break if you expect 3 texture coordinates!
	TexCoords = 0x1,
	Normals = 0x2,
//	AmbientColor = 0x4,
//	DiffuseColor = 0x8,
//	SpecularColor = 0x10,
//	Alpha = 0x20,
//	Illumination = 0x40
};

void parseObjFile(const char* filename, float*& verticesOut, int& trianglesOut, int options = 0);