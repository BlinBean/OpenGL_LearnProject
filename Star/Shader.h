#pragma once
#include <string>
class Shader
{
public:
	std::string vertexString,fragmentString;
	const char* vertexSource;
	const char* fragmentSourcs;
	unsigned int ID;//program ID

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	//~Shader();//��������//ɾ������ʱִ��//��������ǰ�ͷ���Դ

private:
	void checkCompileError(unsigned int ID,std::string type);
};

