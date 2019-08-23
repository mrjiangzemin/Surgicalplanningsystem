#include "model.h"

using namespace std;

Model::Model(string path)
{
	//初始化矩阵
		/*glm::mat4 model = glm::mat4(1.0f);
		this->modelmat = model;*/
		//this->modelvec3 = glm::vec3(0.0f, 0.0f, 0.0f);
	Model::loadModel(path);
}
void Model::loadModel(string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}
void Model::Move(glm::vec3 location, float length) {
	Mesh mesh ;
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Move(location,length,mesh);
	meshes.push_back(mesh);
}
void Model::Draw(Shader &shader)
{
	
	glm::mat4 inversemodel = glm::inverse(this->modelmat);
	shader.setMat4("model", this->modelmat);
	shader.setMat4("inversemodel", inversemodel);
	shader.setFloat("rgb", this->rgb.x, this->rgb.y, this->rgb.z, this->rgb.w);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
		
}
void Model::setmodelmat(glm::mat4 &modelmat) {
	/*for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			std::cout << modelmat[i][k]<<" ";
		}
		std::cout << std::endl;
	}*/
	this->modelmat = modelmat;
	/*for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			std::cout << this->modelmat[i][k] << " ";
		}
		std::cout << std::endl;
	}*/

}
glm::mat4 Model::getmodelmat() {
	return this->modelmat;
}
glm::vec3 Model::getmodelvec3() {
	return this->modelvec3;
}
void Model::setmodelvec3(glm::vec3 modelvec3) {
	this->modelvec3 = modelvec3;
}
void Model::setrgb(float r, float g, float b, float t)
{
	this->rgb.x = r;
	this->rgb.y = g;
	this->rgb.z = b;
	this->rgb.w = t;
}
//废弃的方法
void Model::Draw(Shader &shader, std::vector<glm::mat4> models) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		shader.setMat4("model", models[i]);
		meshes[i].Draw(shader);
		//std::cout << meshes[i].vertices.size() << std::endl;
	}
		
}
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// ������λ�á����ߺ���������
		//����
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		//����
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		//打印法向量
		//std::cout << vector.x << vector.y << vector.z << std::endl;
		vertex.Normal = vector;
		//��������
		if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// �������
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}
vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (auto j = textures_loaded.begin(); j != textures_loaded.end(); j++)
		{
			if (strcmp(j->path.data(), str.C_Str()) == 0)
			{
				textures.push_back(*j);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �������û�б����أ��������
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
		}
	}
	return textures;
}
unsigned int Model::TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

Model::~Model()
{
}
