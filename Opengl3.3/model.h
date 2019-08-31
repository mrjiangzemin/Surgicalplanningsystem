#pragma once

#include<vector>
#include"shader_m.h"
#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"stb_image.h"
#include<string>

struct Xyz {
	float maxx, minx;
	float maxy, miny;
	float maxz, minz;
};

class Model
{
public:
	Model(std::string path);
	void Draw(Shader &shader);
	void Draw(Shader &shader,std::vector<glm::mat4> models);
	void Move(glm::vec3 location,float length);
	glm::mat4 getmodelmat();
	void setmodelmat(glm::mat4 &modelmat);
	glm::vec3 getmodelvec3();
	void setmodelvec3(glm::vec3 modelvec3);
	void setrgb(float r, float g, float b,float t);
	Xyz	 volume();
	~Model();
	glm::mat4 modelmat;
	glm::vec3 modelvec3;
	glm::vec4 rgb;


	float maxx, minx;
	float maxy, miny;
	float maxz, minz;
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	unsigned int TextureFromFile(const char *path, const std::string &directory);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


