#pragma once
#include<vector>
#include<glm.hpp>
#include "gtc/matrix_transform.hpp"
#include <gtc/type_ptr.hpp>
#include<string>
#include"shader_m.h"

//��������
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
//�洢���� �����ظ�����
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;  // ���Ǵ��������·������������������бȽ�
};

class Mesh
{
public:
	Mesh();
	//��������
	std::vector<Vertex> vertices;
	//��ըʱ���ٶ�
	std::vector<glm::vec3> boom_V;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	//���캯��
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);
	
	void Move(glm::vec3 location, float length,Mesh &mesh);
	void setup(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	~Mesh();
private:
	// ��Ⱦ���� 
	unsigned int VAO, VBO, EBO;
	void setupMesh();
	

};


