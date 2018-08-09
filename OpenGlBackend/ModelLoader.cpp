#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ModelLoader.h"
//TODO: uncouple this from Matrix so that it's a standalone utility
#include "Matrix.h"

//struct MTLdata {
//	Vec3 color;
//	Vec3 diffuseColor;
//	Vec3 specularColor;
//	float alpha;
//};

struct Vertex {
	Vec3* coordinate;
	Vec2* texCoords;
	Vec3* normal;
};

int getVertexIndices(std::ifstream& stream, int& vertexIndex, int& texIndex, int& normalIndex);

void parseObjFile(const char* filename, float*& verticesOut, int& trianglesOut, int options)
{
	std::vector<Vec3*> vertices;
	std::vector<Vec2*> texCoords;
	std::vector<Vec3*> normals;
	std::vector<Vec3*> calculatedNormals;
	std::vector<Vertex*> attribArray;
	std::vector<Vertex*> uniqueVertices;
	trianglesOut = 0;
	//std::unordered_map<std::string, MTLdata> materials = std::unordered_map<std::string, MTLdata>();

	std::ifstream objFile(filename);
	std::string linePrefix;

	while (!objFile.eof())
	{
		objFile >> linePrefix;

		//Extracting formatted data doesn't trigger eof, it just fails
		//To work around this for now, I just assume that if it can't extract 
		//the line start, then there's no more data to read.
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
			//TODO: fix this to handle 3 texture coordinates correctly!
			//Currently avoids errors since it ignores the rest of the line
		}
		else if (linePrefix == "vn" && options & ModelLoaderInclude::Normals) {
			Vec3* norm = new Vec3();
			objFile >> norm->operator[](0) >> norm->operator[](1) >> norm->operator[](2);
			normals.push_back(norm);
		}
		else if (linePrefix == "f") {
			int vertexIndex, texIndex, normalIndex;

			getVertexIndices(objFile, vertexIndex, texIndex, normalIndex);
			if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
			if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

			Vertex* first = new Vertex;
			first->coordinate = vertices[vertexIndex - 1];
			first->texCoords = texIndex ? texCoords[texIndex - 1] : nullptr;
			first->normal = normalIndex ? normals[normalIndex - 1] : nullptr;

			getVertexIndices(objFile, vertexIndex, texIndex, normalIndex);
			if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
			if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

			Vertex* last = new Vertex;
			last->coordinate = vertices[vertexIndex - 1];
			last->texCoords = texIndex ? texCoords[texIndex - 1] : nullptr;
			last->normal = normalIndex ? normals[normalIndex - 1] : nullptr;

			uniqueVertices.push_back(first);
			uniqueVertices.push_back(last);

			bool doCalculateNormals = options & ModelLoaderInclude::Normals && normalIndex == 0;
			Vertex* second = last;
			Vec3* normal = nullptr;

			//separating out the third vertex, since we know it should always be there, and we 
			//might need to calculate the normal
			getVertexIndices(objFile, vertexIndex, texIndex, normalIndex);
			if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
			if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

			attribArray.push_back(first);
			attribArray.push_back(last);

			last = new Vertex;
			last->coordinate = vertices[vertexIndex - 1];
			last->texCoords = texIndex ? texCoords[texIndex - 1] : nullptr;
			last->normal = normalIndex ? normals[normalIndex - 1] : nullptr;

			attribArray.push_back(last);
			uniqueVertices.push_back(last);

			trianglesOut++;

			if (doCalculateNormals) {
				normal = new Vec3(Mat::cross(*second->coordinate - *first->coordinate, 
											 *last->coordinate - *first->coordinate));
				first->normal = normal;
				second->normal = normal;
				last->normal = normal;

				calculatedNormals.push_back(normal);
			}

			while (getVertexIndices(objFile, vertexIndex, texIndex, normalIndex))
			{
				if (!(options & ModelLoaderInclude::TexCoords)) texIndex = 0;
				if (!(options & ModelLoaderInclude::Normals)) normalIndex = 0;

				attribArray.push_back(first);
				attribArray.push_back(last);

				last = new Vertex;
				last->coordinate = vertices[vertexIndex - 1];
				last->texCoords = texIndex ? texCoords[texIndex - 1] : nullptr;
				last->normal = normalIndex ? normals[normalIndex - 1] : nullptr;

				if (doCalculateNormals && normalIndex == 0) last->normal = normal;

				attribArray.push_back(last);
				uniqueVertices.push_back(last);

				trianglesOut++;
			}
			//Since extracting formatted input trims leading whitespace, calling getVertexIndices
			//until it fails trims off the newline if there's no comment on the line.  Put it back
			//so that the catch-all objFile.ignore doesn't ignore the entire next line
			objFile.unget();
		}
		else if (linePrefix == "l") {
			//ignoring this for now
		}
		else if (linePrefix == "mtllib") {
			//ignoring this for now
		}
		else if (linePrefix == "usemtl") {
			//ignoring this for now
		}
		else if (linePrefix == "o") {
			//ignoring this for now
		}
		else if (linePrefix == "g") {
			//ignoring this for now
		}
		else if (linePrefix == "s") {
			//ignoring this for now
		}
		else {
			//Some other prefix we weren't expecting, so ignore it
		}
		//once we've read all the data we want from the line, just ignore the rest, 
		//since it's either junk or a comment anyway
		objFile.ignore(1000, '\n');
	}

	int attribElements = 3;
	if (options & ModelLoaderInclude::TexCoords) attribElements += 2;
	if (options & ModelLoaderInclude::Normals) attribElements += 3;

	//TODO: make output size dependent on the options given (no unused space for texcoords for example)
	verticesOut = new float[attribArray.size() * attribElements];

	for (unsigned int i = 0; i < attribArray.size(); i++) 
	{
		int index = 0;
		Vertex* vec = attribArray[i];
		//copy stuff to output array
		int j;
		for (j = 0; j < 3; j++, index++) 
		{
			verticesOut[i * attribElements + index] = vec->coordinate->operator[](j);
		}
		if (options & ModelLoaderInclude::TexCoords)
		{
			for (j = 0; j < 2; j++, index++)
			{
				Vec2* tex = vec->texCoords;
				if (tex != nullptr)
					verticesOut[i * attribElements + index] = tex->operator[](j);
				else
					verticesOut[i * attribElements + index] = 0;
			}
		}
		if (options & ModelLoaderInclude::Normals)
		{
			for (j = 0; j < 3; j++, index++)
			{
				Vec3* norm = vec->normal;
				if (norm != nullptr)
					verticesOut[i * attribElements + index] = norm->operator[](j);
				else
					verticesOut[i * attribElements + index] = 0;
			}
		}
	}
	attribArray.clear();

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

	for (Vec3* vec : calculatedNormals) {
		delete vec;
	}
	calculatedNormals.clear();

	for (Vertex* vec : uniqueVertices) {
		delete vec;
	}
	uniqueVertices.clear();
}

int getVertexIndices(std::ifstream& stream, int& vertexIndex, int& texIndex, int& normalIndex)
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
