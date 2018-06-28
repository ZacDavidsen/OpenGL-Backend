#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ModelLoader.h"
#include "Matrix.h"

//struct MTLdata {
//	Vec3 color;
//	Vec3 diffuseColor;
//	Vec3 specularColor;
//	float alpha;
//};

#define DEBUGGING_STUFFN

int parseVertex(std::ifstream& stream, int& vertexIndex, int& texIndex, int& normalIndex);
Mat::Vector<8>* makeVertex(Vec3* vertex, Vec2* texCoord, Vec3* norm);

void parseObjFile(const char* filename, float*& verticesOut, int& trianglesOut, int options)
{
	std::vector<Vec3*> vertices;
	std::vector<Vec2*> texCoords;
	std::vector<Vec3*> normals;
	std::vector<Mat::Vector<8>*> attribArray;
	std::vector<Mat::Vector<8>*> uniqueVertices;
	trianglesOut = 0;
	//std::unordered_map<std::string, MTLdata> materials = std::unordered_map<std::string, MTLdata>();

	std::ifstream objFile(filename);
	std::string linePrefix;

	while (!objFile.eof())
	{
		objFile >> linePrefix;

		if (objFile.fail())
		{
			//objFile.clear();
			//std::getline(objFile, linePrefix);
			//std::cout << linePrefix << std::endl;
			break;
		}

		if (linePrefix == "v") {
			Vec3* vert = new Vec3();
			objFile >> vert->operator[](0) >> vert->operator[](1) >> vert->operator[](2);
			vertices.push_back(vert);
		}
		else if (linePrefix == "vt" && options & ModelLoaderInclude::TexCoords) {
			Vec2* tex = new Vec2();
			objFile >> tex->operator[](0) >> tex->operator[](1);
			texCoords.push_back(tex);
		}
		else if (linePrefix == "vn" && options & ModelLoaderInclude::Normals) {
			Vec3* norm = new Vec3();
			objFile >> norm->operator[](0) >> norm->operator[](1) >> norm->operator[](2);
			normals.push_back(norm);
		}
		else if (linePrefix == "f") {
			int vertexIndex, texIndex, normalIndex;

			parseVertex(objFile, vertexIndex, texIndex, normalIndex);
			if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
			if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

			Mat::Vector<8>* first = makeVertex(vertices[vertexIndex - 1],
											   texIndex ? texCoords[texIndex - 1] : nullptr,
											   normalIndex ? normals[normalIndex - 1] : nullptr);

			parseVertex(objFile, vertexIndex, texIndex, normalIndex);
			if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
			if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

			Mat::Vector<8>* last = makeVertex(vertices[vertexIndex - 1],
											  texIndex ? texCoords[texIndex - 1] : nullptr,
											  normalIndex ? normals[normalIndex - 1] : nullptr);

			uniqueVertices.push_back(first);
			uniqueVertices.push_back(last);

			while (parseVertex(objFile, vertexIndex, texIndex, normalIndex))
			{
				if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
				if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

				attribArray.push_back(first);
				attribArray.push_back(last);

				last = makeVertex(vertices[vertexIndex - 1],
								  texIndex ? texCoords[texIndex - 1] : nullptr,
								  normalIndex ? normals[normalIndex - 1] : nullptr);

				attribArray.push_back(last);
				uniqueVertices.push_back(last);

				trianglesOut++;
			}
		}
		else if (linePrefix == "l") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else if (linePrefix == "mtllib") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else if (linePrefix == "usemtl") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else if (linePrefix == "o") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else if (linePrefix == "g") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else if (linePrefix == "s") {
			//ignoring this for now
			objFile.ignore(1000, '\n');
		}
		else {
			//Some other prefix we weren't expecting, so ignore it
			objFile.ignore(1000, '\n');
		}
	}

	for (Vec3* vec : vertices) {
		delete vec;
	}
	vertices.clear();

	for (Vec2* vec : texCoords) {
		delete vec;
	}
	texCoords.clear();

	for (Vec3* vec : normals) {
		delete vec;
	}
	normals.clear();

	verticesOut = new float[attribArray.size() * 8];

	for (int i = 0; i < attribArray.size(); i++) 
	{
		Mat::Vector<8>* vec = attribArray[i];
		//copy stuff to output array
		for (int j = 0; j < 8; j++) 
		{
			verticesOut[i * 8 + j] = vec->operator[](j);
		}
	}
	attribArray.clear();

	for (Mat::Vector<8>* vec : uniqueVertices) {
		delete vec;
	}
	uniqueVertices.clear();
}

int parseVertex(std::ifstream& stream, int& vertexIndex, int& texIndex, int& normalIndex)
{
	texIndex = 0; 
	normalIndex = 0;

	stream >> vertexIndex;

	//if it fails to get vertex, then there should be a comment or a new line
	//either way, fail 
	if (stream.fail()) 
	{
		stream.clear();
		return 0;
	}

	if (stream.get() == '/')
	{
		stream >> texIndex;
		//if it failed, then we expect next character to be '/', so we just ignore it
		if (stream.fail())
		{
			stream.clear();
		}
		if (stream.get() == '/')
		{
			stream >> normalIndex;
		}
	}

	return 1;
}

Mat::Vector<8>* makeVertex(Vec3* vertex, Vec2* texCoord, Vec3* norm) 
{
	float values[8];

	values[0] = vertex->operator[](0);
	values[1] = vertex->operator[](1);
	values[2] = vertex->operator[](2);

	if (texCoord)
	{
		values[3] = texCoord->operator[](0);
		values[4] = texCoord->operator[](1);
	}
	else
	{
		values[3] = 0;
		values[4] = 0;
	}

	if (norm)
	{
		values[5] = norm->operator[](0);
		values[6] = norm->operator[](1);
		values[7] = norm->operator[](2);
	}
	else
	{
		values[5] = 0;
		values[6] = 0;
		values[7] = 0;
	}

	return new Mat::Vector<8>(values);
}
