#include "mesh.h"
using namespace std;
Mesh::Mesh() {
	
}
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	
	setupMesh();
}
void Mesh::setup(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	
	setupMesh();
}
void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// ����λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// ���㷨��j'j
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// ������������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
										  // ��ȡ������ţ�diffuse_textureN �е� N��
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = to_string(specularNr++);

		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Mesh::Move(glm::vec3 location, float length,Mesh &mesh) {
	//测试点数量
	long long count = 0;
	vector<Vertex> vertices1;
	vector<Vertex> vertices2;
	vector<unsigned int> indices1;
	vector<Texture> textures1;
	indices1 = this->indices;
	textures1 = this->textures;
	//cout << vertices.size()<<endl;

	for (long long i = 0; i < vertices.size(); i++) {
		//std::cout << vertices[i].Position.y<<" ";
		if (i >= vertices.size() || i < 0) { cout << "vetcor下标越界" << endl; break; }
		
		if (vertices[i].Position.y >= -130) {
			//std::cout << "Y";

			vertices1.push_back(vertices[i]);
			//vertices.erase(vertices.begin() + i);
			//i--;
			count++;
			//vertices[i].Position += (location * glm::vec3(100.0f, 100.0f, 100.0f));
		}
		else {
			vertices2.push_back(vertices[i]);
		}
		
	}
	vertices.clear();
	this->vertices = vertices2;
	//cout << count;
	mesh.setup(vertices1, indices1, textures1);
	
	setupMesh();
	std::cout << "yes";
}




Mesh::~Mesh()
{
}
