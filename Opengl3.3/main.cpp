#include <glad/glad.h>
//GLFW是一个专门针对OpenGL的C语言库，它提供了一些渲染物体所需的最低限度的接口。
//它允许用户创建OpenGL上下文，定义窗口参数以及处理用户输入
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
//#include <glm/gtc/type_ptr.hpp>
#include "gtc/type_ptr.hpp"
#include "shader_m.h"
#include"model.h"
#include "camera.h"
#include <gl/glut.h>
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback2(GLFWwindow* window, double xpos, double ypos);
void mouse_callback1(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//鼠标按钮事件
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//鼠标点击选取3D坐标
void Get3Dpos(int x, int y, glm::vec3 *pp, glm::mat4 modelandview, glm::mat4 poj);
//根据三个不共线点计算平面
glm::vec4 CalculatePlane(std::vector<glm::vec3>points);
//根据四个不共面点计算球面
glm::vec4 CalculateGlobe(std::vector<glm::vec3>points);


void processInput(GLFWwindow *window);
//计算切割后的剩余面积

void CalculateVolume(Xyz value, glm::vec4 Functionparameters);

void printMat4(glm::mat4 mat) {
	
		for (int k = 0; k < 4; k++) {
			for (int y = 0; y < 4; y++) {
				std::cout <<mat[k][y]<< " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	
}

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
//不同的鼠标模式
bool flagbutton = true;
//定点输出
bool flagbutton2 = true;
bool flagbutton3 = true;
//Functionparameters uniform变量的初始化
bool flagbutton4 = true;
//切割模式的选择
int flagbutton5 = 1;

bool flagbutton6 = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float radians = 0;
float radians1 = 0;
float radians2 = 0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//view 和 model矩阵相乘的结果的全局变量，用来取点
glm::mat4 modelandview;
//投影矩阵
glm::mat4 projection;
//存储变换后的models;
glm::mat4  models[8];
//存储不同的平移矩阵
std::vector<glm::vec3> translates;
//存储model
std::vector<Model> MODELS;
//存储选择的点
std::vector<glm::vec3> points;
//光源位置
glm::vec3 lightPos(300.0f, 300.0f, 300.0f);
int main()
{
	translates.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//鼠标移动事件
	//glfwSetCursorPosCallback(window, mouse_callback);
	//按键时间
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	glfwSetScrollCallback(window, scroll_callback);
	//设置光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);
	//让颜色直接用作材质
	//glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Shader ourShader("D:\\OpenGlProject\\lampvs.txt", "D:\\OpenGlProject\\lampfs.txt","D:\\OpenGlProject\\lampgs.txt");
	Shader ourShader("D:\\OpenGlProject\\lampvs.txt", "D:\\OpenGlProject\\lampfs.txt", "D:\\OpenGlProject\\lampgs.txt");
	//Shader normalShader("D:\\OpenGlProject\\normalvs.txt", "D:\\OpenGlProject\\normalfs.txt", "D:\\OpenGlProject\\normalgs.txt");
	//Shader LampShader("D:\\OpenGlProject\\lvs.txt", "D:\\OpenGlProject\\lfs.txt");
	/*Shader myShader("lampvs.txt", "lampfs.txt");
	Shader allShader("vs.txt", "fs.txt");*/

	/*Model ourModel("F:\\手术规划系统\\liverstllib\\liver.stl");
	Model myModel("F:\\手术规划系统\\liverstllib\\动脉.stl");*/
	/*Model allModel("E:\\Course\\Third_year\\计算机图形学\\手术规划系统\\model\\brain\\all.stl");*/
	Model *model1 = new	Model("F:\\手术规划系统\\liverstllib\\0300292\\肝脏.stl");
	Model *model2 = new	Model("F:\\手术规划系统\\liverstllib\\0300292\\门静脉.stl");
	Model *model3 = new	Model("F:\\手术规划系统\\liverstllib\\0300292\\腔静脉.stl");
	Model *model4 = new	Model("F:\\手术规划系统\\liverstllib\\0300292\\肿瘤.stl");
	Model *model5 = new	Model("F:\\手术规划系统\\liverstllib\\0300292\\动脉.stl");
	
	//设定颜色RGB值
	(*model1).setrgb(1.0f, 1.0f, 1.0f, 0.5f);
	(*model2).setrgb(0.0f, 0.0f, 1.0f, 1.0f);
	(*model3).setrgb(1.0f, 1.0f, 0.0f, 1.0f);
	(*model4).setrgb(0.0f, 1.0f, 0.0f, 1.0f);
	(*model5).setrgb(1.0f, 0.0f, 0.0f, 1.0f);

	MODELS.push_back(*model1);
	MODELS.push_back(*model2);
	MODELS.push_back(*model3);
	MODELS.push_back(*model4);
	MODELS.push_back(*model5);
	
	//MODELS[1].setmodelvec3(glm::vec3(3.0f, 0.0f, 0.0f));
	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，
	//如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//输入
		processInput(window);

		glClearColor(0.0f, 0.3f, 0.05f, 1.0f);
		//清空深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourShader.use();
	

		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 inverseview = glm::inverse(view);
		glm::mat4 inversepoj = glm::inverse(projection);
		//printMat4(projection);
		//printMat4(inversepoj);
		//printMat4(view);
		////printMat4(inverseview);
		//glm::mat4 res1 = view *projection;
		//printMat4(res1);
		//glm::mat4 res2 = res1 * inversepoj;
		//printMat4(res2);
		
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		ourShader.setMat4("inversepoj", inversepoj);
		ourShader.setMat4("inverseview", inverseview);
		ourShader.setFloat("time", glfwGetTime());
		glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
		/*lightColor.x = sin(glfwGetTime()*2.0f);
		lightColor.y = sin(glfwGetTime()*0.7f);
		lightColor.z = sin(glfwGetTime()*1.3f);*/
		ourShader.setVec3("lightColor", lightColor);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setVec3("viewPos", camera.Position);

		
		for (unsigned int i = 0; i < MODELS.size(); i++) {
			glm::mat4 outModel = glm::mat4(1.0f);
			//构造平移矩阵
			outModel = glm::translate(outModel,MODELS[i].getmodelvec3());
			//构造拉伸矩阵
			outModel = glm::scale(outModel, glm::vec3(0.01f, 0.01f, 0.01f));
			outModel = glm::rotate(outModel, glm::radians(radians), glm::vec3(0.0f, 1.0f, 0.0f));
			outModel = glm::rotate(outModel, glm::radians(radians1), glm::vec3(0.0f, 0.0f, 1.0f));
			outModel = glm::rotate(outModel, glm::radians(radians2), glm::vec3(1.0f, 0.0f, 0.0f));
			//MODELS[i].modelmat = outModel;
			//models[i] = outModel;
			MODELS[i].setmodelmat(outModel);
			//ourShader.setMat4("model", outModel);
			//std::cout << "yes";
			modelandview = view * outModel;
		}
		
		//旋转函数
		if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)) {
			//std::cout << (float)glfwGetTime();
			//todo: move函数对所有的model
			
			radians = (radians + 1.0f);
			if (radians >= 360.0f) {
				radians = (radians - 360.0f);
			}
			
			
			//std::cout << "chenggong";
		}
		if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)) {
			radians1 = (radians1 + 1.0f);
			if (radians1 >= 360.0f) {
				radians1 = (radians1 - 360.0f);
			}
			
		}
		if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)) {
			radians2 = (radians2 + 1.0f);
			if (radians2 >= 360.0f) {
				radians2 = (radians2 - 360.0f);
			}

		}
		
		if ((glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)) {
			//std::cout << models.size() << " " << translates.size() << std::endl;
			for (int i = 0; i < translates.size(); i++) {
				for (int k = 0; k < 4; k++) {
					for (int y = 0; y < 4; y++) {
						std::cout << models[i][y][k] << " " ;
					}
					std::cout << std::endl;
				}
				std::cout << std::endl;
				std::cout << std::endl;
			}

		}
		if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)) {
			if (flagbutton5 == 1) {
				flagbutton5 = 2;
				std::cout << "Switch to sphere cutting now!" << std::endl;
			}
			else {
				flagbutton5 = 1;
				std::cout << "Switch to plane cutting now!" << std::endl;
			}
		}
		//设置全局变量Functionparameters
		if (flagbutton4 == true) {
			ourShader.setFloat("cutmode", 0.0f);
			ourShader.setFloat("Functionparameters", 0, 0, 0,0);
		}
		//平面切割
		if (points.size() == 3&&flagbutton5==1) {
			flagbutton3 = false;
			flagbutton4 = false;
			glm::vec4 mes = CalculatePlane(points);
			ourShader.setFloat("cutmode", 1.0f);
			ourShader.setFloat("Functionparameters", mes.x,mes.y,mes.z,mes.w);
			if (flagbutton6) {
				std::cout << model1->maxx << " " << model1->minx << " " << model1->maxy << " " << model1->miny << " " << model1->maxz << " " << model1->minz << " " << std::endl ;
				CalculateVolume(model1->volume(), mes);
				flagbutton6 = false;
			}
			
			/*ourShader.setFloat("point1", points[0].x, points[0].y, points[0].z, 0);
			ourShader.setFloat("point2", points[1].x, points[1].y, points[1].z, 0);
			ourShader.setFloat("point3", points[2].x, points[2].y, points[2].z, 0);*/
		}
		//球面切割
		if (points.size() == 4 && flagbutton5 == 2) {
			flagbutton3 = false;
			flagbutton4 = false;
			glm::vec4 mes = CalculateGlobe(points);
			ourShader.setFloat("cutmode", 2.0f);
			ourShader.setFloat("Functionparameters", mes.x, mes.y, mes.z, mes.w);

			/*ourShader.setFloat("point1", points[0].x, points[0].y, points[0].z, 0);
			ourShader.setFloat("point2", points[1].x, points[1].y, points[1].z, 0);
			ourShader.setFloat("point3", points[2].x, points[2].y, points[2].z, 0);*/
		}
		//使用黑色标志出已经选择的点
		if (!points.empty()) {
			int i = 0;
			while (i < points.size()) {
				std::string s;
				if (i == 0) {
					s = "point1";
				}
				else if (i == 1) {
					s = "point2";
				}
				else if (i == 2) {
					s = "point3";
				}
				
				ourShader.setFloat(s, points[i].x, points[i].y, points[i].z, 0);
				i++;
			}
		}
		//ourShader.setMat4("model", outModel);
		/*if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			myModel.Draw(myShader);
		}*/
		for (int i = 0; i < MODELS.size(); i++) {
			//std::cout << MODELS.size();
			MODELS[i].Draw(ourShader);

		}

		//normalShader.use();
		//normalShader.setMat4("projection", projection);
		//normalShader.setMat4("view", view);
		//for (int i = 0; i < MODELS.size(); i++) {
		//	//std::cout << MODELS.size();
		//	MODELS[i].Draw(normalShader);

		//}
		
		//Shader.use();
		//MODELS[8].Draw(Shader);
		
		
		//myModel.Draw(ourShader,models);
		//ourModel.Draw(ourShader,models);
		/*myModel.Draw(myShader);*/
		/*	allShader.setMat4("model", inModel);
			if (!glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
				allModel.Draw(allShader);
			}*/
		//glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
		//它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}


glm::vec4 CalculatePlane(std::vector<glm::vec3> points)
{
	float a, b, c, d;
	glm::vec4 result;
	a = (points[1].y - points[0].y)*(points[2].z - points[0].z) - (points[1].z - points[0].z)*(points[2].y - points[0].y);
	b = (points[2].x - points[0].x)*(points[1].z - points[0].z) - (points[1].x - points[0].x)*(points[2].z - points[0].z);
	c = (points[1].x - points[0].x)*(points[2].y - points[0].y) - (points[2].x - points[0].x)*(points[1].y - points[0].y);
	d = (a*points[0].x + b * points[0].y + c * points[0].z)*-1;
	result.x = a;
	result.y = b;
	result.z = c;
	result.w = d;
	//输出所计算的函数
	//std::cout << result.x << " " << result.y << " " << result.z << " " << std::endl;
	return result;
}
glm::vec4 CalculateGlobe(std::vector<glm::vec3> points) {
	double resultR;
	glm::vec4 result;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double a, b, c, A, B, C, a1, b1, c1, A1, B1, C1, a2, b2, c2, A2, B2, C2;
	double P, Q, R;
	double x, y, z;
	x1 = points[0].x;
	y1 = points[0].y;
	z1 = points[0].z;

	x2 = points[1].x;
	y2 = points[1].y;
	z2 = points[1].z;

	x3 = points[2].x;
	y3 = points[2].y;
	z3 = points[2].z;

	x4 = points[3].x;
	y4 = points[3].y;
	z4 = points[3].z;

	a = (x1 - x2);
	b = (y1 - y2);
	c = (z1 - z2);
	A = (x1 * x1 - x2 * x2);
	B = (y1 * y1 - y2 * y2);
	C = (z1 * z1 - z2 * z2);
	a1 = (x3 - x4);
	b1 = (y3 - y4);
	c1 = (z3 - z4);
	A1 = (x3 * x3 - x4 * x4);
	B1 = (y3 * y3 - y4 * y4);
	C1 = (z3 * z3 - z4 * z4);
	a2 = (x2 - x3);
	b2 = (y2 - y3);
	c2 = (z2 - z3);
	A2 = (x2 * x2 - x3 * x3);
	B2 = (y2 * y2 - y3 * y3);
	C2 = (z2 * z2 - z3 * z3);
	P = (A + B + C) / 2;
	Q = (A1 + B1 + C1) / 2;
	R = (A2 + B2 + C2) / 2; //D是系数行列式，运用克拉默法则
	double D = a * b1 * c2 + a2 * b * c1 + c * a1 * b2 - (a2 * b1 * c + a1 * b * c2 + a * b2 * c1);
	double Dx = P * b1 * c2 + b * c1 * R + c * Q * b2 - (c * b1 * R + P * c1 * b2 + Q * b * c2);
	double Dy = a * Q * c2 + P * c1 * a2 + c * a1 * R - (c * Q * a2 + a * c1 * R + c2 * P * a1);
	double Dz = a * b1 * R + b * Q * a2 + P * a1 * b2 - (a2 * b1 * P + a * Q * b2 + R * b * a1);
	x = Dx / D;
	y = Dy / D;
	z = Dz / D;
	resultR = sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y)+(z1-z)*(z1-z));
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = resultR;
	return result;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

void CalculateVolume(Xyz value, glm::vec4 Functionparameters)
{

	float high = value.maxz - value.minz;

	float z1 = (Functionparameters.x*value.maxx + Functionparameters.y*value.maxy + Functionparameters.w) / -Functionparameters.z;

	float z2 = (Functionparameters.x*value.maxx + Functionparameters.y*value.miny + Functionparameters.w) / -Functionparameters.z;

	float z3 = (Functionparameters.x*value.minx + Functionparameters.y*value.maxy + Functionparameters.w) / -Functionparameters.z;

	float z4 = (Functionparameters.x*value.minx + Functionparameters.y*value.miny + Functionparameters.w) / -Functionparameters.z;

	z1 = z1 - value.minz;

	z2 = z2 - value.minz;

	z3 = z3 - value.minz;

	z4 = z4 - value.minz;

	float result = 100 * (z1 + z2 + z3 + z4) / (4 * high);

	std::cout << "切除面积为：" << result << "%" << std::endl;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//bool flagbutton = true;
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		if (flagbutton == true) {
			glfwSetCursorPosCallback(window, mouse_callback);
			flagbutton = false;
			std::cout << "FirstMouseMode" << std::endl;
		}
		else {
			std::cout << "SecondMouseMode" << std::endl;
			glfwSetCursorPosCallback(window, mouse_callback1);
			flagbutton = true;
		}
		
		
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (flagbutton == true&&flagbutton2==true&&flagbutton3==true) {
			glfwSetCursorPosCallback(window, mouse_callback2);
			flagbutton2 = false;
			
		}
		else if (flagbutton == true && flagbutton2 == false) {
			glfwSetCursorPosCallback(window, mouse_callback1);
			flagbutton2 = true;
		}
		break;
	default:
		return;
	}
	return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void mouse_callback1(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	//camera.ProcessMouseMovement(xoffset, yoffset);
}
void mouse_callback2(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec3 pp;
	Get3Dpos(xpos,ypos,&pp,modelandview,projection);
	glfwSetCursorPosCallback(window, mouse_callback1);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
//根据屏幕坐标得到视点空间坐标
void Get3Dpos(int x, int y, glm::vec3 *pp,glm::mat4 modelandview,glm::mat4 poj) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	
	GLfloat winX, winY, winZ;
	GLdouble object_x, object_y, object_z;
	int mouse_x = x;
	int mouse_y = y;
	//这个方法并不能获取矩阵，新管线的矩阵由自己定义
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	//初始化modelview和projection
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			modelview[i * 4 + k] = modelandview[i][k];
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			projection[i * 4 + k] = poj[i][k];
		}
	}
	//打印modelview
	std::cout << "modelview:" << std::endl;
	for (int i = 0; i < 16; i++) {
		std::cout << modelview[i]<<" ";
		if ((i + 1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
	//glGetDoublev(GL_PROJECTION_MATRIX, projection);
	//打印projection
	std::cout << "projection:" << std::endl;
	for (int i = 0; i < 16; i++) {
		std::cout << projection[i] << " ";
		if ((i + 1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
	//lGetIntegerv(GL_VIEWPORT, viewport);
	//打印viewport
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = SCR_WIDTH;
	viewport[3] = SCR_HEIGHT;
	std::cout << "viewport:" << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << viewport[i] << " ";
		if ((i + 1) % 4 == 0) {
			std::cout << std::endl;
		}
	}
	winX = (float)mouse_x;
	winY = (float)viewport[3] - (float)mouse_y  ;
	//winY = (float)mouse_y;
	glReadBuffer(GL_FRONT);
	glReadPixels(mouse_x, winY, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	std::cout << winZ <<" "<< winX<< " " << winY<<std::endl;
	gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);
	(*pp).x = object_x;
	(*pp).y = object_y;
	(*pp).z = object_z;
	std::cout<<"  "<<object_x << "  " <<object_y << "  " <<object_z<<std::endl;
	points.push_back(*pp);
	

}

//根据视点空间坐标得到世界空间坐标
glm::vec3 GetWorldPos(int x, int y,Camera camera) {
	//得到观察空间的坐标
	glm::vec3 pp;
	//Get3Dpos(x, y, &pp);

	//求视点的UVN系统
	glm::vec3 U, V, N;                   //fVector3为自定义向量类
	glm::vec3 up = { 0.0,1.0,0.0 };
	glm::vec3 eye = camera.Position;
	glm::vec3 direction = { 0.0,0.0,0.0 };            //视点坐标与观察点坐标

	N = eye - direction;               //矢量减法
	//U = N.cross(up);                    //矢量叉乘
	U = glm::cross(N, up);
	//V = N.cross(U);
	V = glm::cross(N, U);

	//N.normalize();                      //矢量归一化
	//U.normalize();
	//V.normalize();
	glm::normalize(N);
	glm::normalize(U);
	glm::normalize(V);

	//求世界坐标
	glm::vec3 worldpos ;
	worldpos.x = U.x * pp.x + V.x * pp.y + N.x * pp.z + eye.x;
	worldpos.y = U.y * pp.x + V.y * pp.y + N.y * pp.z + eye.y;
	worldpos.z = U.z * pp.x + V.z * pp.y + N.z * pp.z + eye.z;
	return worldpos;
}

