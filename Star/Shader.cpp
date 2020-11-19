#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define GLEW_STATIC	
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//using namespace std;

Shader::Shader(const char* vertexPath,const char* fragmentPath)//::�����ռ䣨������Shader
{
	std::ifstream vertexFile,fragmentFile;
	std::stringstream vertexSStream,fragmentSStream;//String ����

	vertexFile.open(vertexPath);//����
	fragmentFile.open(fragmentPath);

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//�򲻿��򻵵�
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);//�򲻿��򻵵�
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw std::exception("open file error");
		}
		vertexSStream << vertexFile.rdbuf();//��������
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();//����string
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();//����Ϊchar*
		fragmentSourcs = fragmentString.c_str();

		//��Shader���������
		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glShaderSource(fragment, 1, &fragmentSourcs, NULL);//һ���ַ�����ûʲô���ȣ�Ϊnull
		glCompileShader(vertex);
		glCompileShader(fragment);
		//���
		checkCompileError(vertex, "VERTEX");
		checkCompileError(fragment, "FRAGMENT");

		//�󶨳���
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		//���
		checkCompileError(ID, "PROGRAM");
		//�󶨺�ɾ��ԭ��Shader
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

	if (type != "PROGRAM") {	//�ж������Ƿ�ʱPROGRAM
		glGetShaderiv(ID,GL_COMPILE_STATUS,&success);//���ǵĻ���SHADER//���shader��COMPILE
		if (!success) {//COMPILEʧ��
			glGetShaderInfoLog(ID,512,NULL,infoLog);//�����Ϣ
			std::cout << "Shader compile error:" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(ID,GL_LINK_STATUS,&success );//�������
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "Program link error:" << infoLog << std::endl;
		}
	}
}


//Shader::~Shader()
//{
//}
