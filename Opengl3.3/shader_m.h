#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>

class Shader
{
public:
	//程序ID
	unsigned int ID;

	// 构造器读取并构建着色器
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	// 使用、激活程序
	void use();
	// uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	//
	void setFloat(const std::string &name, float value1, float value2, float value3, float value4) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	~Shader();

private:
	void checkCompileErrors(unsigned int shader, std::string type);


};

