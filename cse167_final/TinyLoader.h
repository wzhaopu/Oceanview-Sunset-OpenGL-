
#ifndef TINY_LOADER_H
#define TINY_LOADER_H

//STL libraries
#include <string>
#include <vector>

//glm libraries
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffusal;
	glm::vec3 specular;
	glm::vec3 exponent;

	float transparency;
};


class TinyLoader
{
public:
	TinyLoader(std::string objFile, std::string mtlFile);

	void loadObjFile(std::string filePath);
	void loadMtlFIle(std::string filePath);

	std::vector<glm::vec3> &getVertices();
	std::vector<glm::vec3> &getNormals();
	std::vector<glm::vec2> &getTexcoords();

	std::string &getTextureName();

	Material *getMaterialStruct();
protected:
	//bool mtlFileWasRead = false;
	void parseVertex(std::vector<glm::vec3> &vertices, const std::string &vertexString);
	void parseIndices(std::string indexString);
	void parseTexCoords(std::vector<glm::vec2> &texCoords, const std::string &texCoordsString);
	void generateTexCoords(std::vector<glm::vec2> &fTexCoords, std::vector<glm::vec2> &texCoords, int index);
	void generateVector(std::vector<glm::vec3> &fVertices, std::vector<glm::vec3> &vertices, int index);

	void parseMaterials(glm::vec3 &material, const std::string &matString);

	//Used to temporarily store unprocessed vertex data (stored just like it was declared in the file)
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	//Final vertex data in format suitable for opengl
	std::vector<glm::vec2> fTexcoords;
	std::vector<glm::vec3> fVertices;
	std::vector<glm::vec3> fNormals;

	Material material;

	//Texture name if any retrived from mtl file
	std::string textureName;
};

#endif
