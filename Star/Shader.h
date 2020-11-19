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
	//~Shader();//析构函数//删除对象时执行//跳出程序前释放资源

private:
	void checkCompileError(unsigned int ID,std::string type);
};

