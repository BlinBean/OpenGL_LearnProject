#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define GLEW_STATIC	
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//using namespace std;

Shader::Shader(const char* vertexPath,const char* fragmentPath)//::命名空间（作用域）Shader
{
	std::ifstream vertexFile,fragmentFile;
	std::stringstream vertexSStream,fragmentSStream;//String 缓冲

	vertexFile.open(vertexPath);//开档
	fragmentFile.open(fragmentPath);

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//打不开或坏档
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//打不开或坏档
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw std::exception("open file error");
		}
		vertexSStream << vertexFile.rdbuf();//读到缓冲
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();//读到string
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();//解析为char*
		fragmentSourcs = fragmentString.c_str();

		//将Shader编译进代码
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glShaderSource(fragment, 1, &fragmentSourcs, NULL);//一个字符串，没什么长度，为null
		glCompileShader(vertex);
		glCompileShader(fragment);
		//检查
		checkCompileError(vertex, "VERTEX");
		checkCompileError(fragment, "FRAGMENT");

		//绑定程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//检查
		checkCompileError(ID, "PROGRAM");
		//绑定后删除原有Shader
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	catch (const std::exception & ex)
	{
		printf(ex.what());
	}

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::checkCompileError(unsigned int ID, std::string type)
{
	int success;
	char infoLog[512];

	if (type != "PROGRAM") {	//判断类型是否时PROGRAM
		glGetShaderiv(ID,GL_COMPILE_STATUS,&success);//不是的话查SHADER//检查shader的COMPILE
		if (!success) {//COMPILE失败
			glGetShaderInfoLog(ID,512,NULL,infoLog);//输出信息
			std::cout << "Shader compile error:" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(ID,GL_LINK_STATUS,&success );//检查连接
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "Program link error:" << infoLog << std::endl;
		}
	}
}


//Shader::~Shader()
//{
//}
