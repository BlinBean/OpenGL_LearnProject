#define GLEW_STATIC	//GLEW使用STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//#pragma region shaderCode
//const char* vertexShaderSource =
//"#version 330 core													\n"//openGL 版本
//"layout(location = 0) in vec3 position;								\n"//从0号位置挖取数据//汇入shader的变量
//"layout(location = 1) in vec3 aColor;								\n"
//"out vec4 vertexColor;												\n"
//"void main() {														\n"
//"	gl_Position = vec4(position.x, position.y, position.z, 1.0);	\n"
////"	vertexColor=vec4(1.0,0,0,1.0);									\n"
//"	vertexColor=vec4(aColor,1);												\n"
//"}	\n";
//const char* fragmentShaderSource =
//"#version 330 core													\n"
//"out vec4 FragColor;												\n"
//"in vec4 vertexColor;												\n"
//"uniform vec4 ourColor;												\n"//从CPU传入数据
//"void main(){														\n"
////"	FragColor = ourColor;											\n"
//"	FragColor = vertexColor;										\n"
//"}	\n";
//#pragma endregion

GLfloat vertices[] = {
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};
unsigned int indices[] = {
	0,2,1,
	2,0,3
};

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//如果按住ESC
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc,char* argv[]) {

#pragma region GLFW
	glfwInit();//初始化
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//提示使用的openGL主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//使用的openGL次版本//也就是使用openGL3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//指定配置//可编程流水线

	//---------------open GLFW window--------------
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "star", NULL, NULL);
	if (window == NULL) {
		std::cout << "未创建窗口" << std::endl;
		glfwTerminate();//终止GLFW
		return -1;
	}
	glfwMakeContextCurrent(window);//使用这个window
	//-----------------------------------------
#pragma endregion
#pragma region GLEW
	//-----------init glew--------------
	glewExperimental = true;//实验性质
	if (glewInit() != GLEW_OK) {//glew操作底下成功的话，返回GLEW_OK
		std::cout << "GLEW 初始化失败" << std::endl;
		glfwTerminate();//终止GLFW
		return -1;
	}
	//-------------------------------
#pragma endregion



	glViewport(0, 0, 800, 600);//窗口可绘制大小，前两个为起始点
	glEnable(GL_CULL_FACE);//开启剔除
	glCullFace(GL_BACK);//背面剔除
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//多边形模式设为线框
	Shader* myShader=new Shader("vertexSource.txt","fragmentSource.txt");//用new方法的话是用指针的//指针记得delete

#pragma region VBO&VAO&EBO
	unsigned int VAO, VBO,EBO;
	glGenVertexArrays(1, &VAO);//产生一个VAO，返还VAO ID
	glBindVertexArray(VAO);//ArrayBuffer绑定VAO(状态机|GL)

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//buffer绑定VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//把用户定义的数据复制到当前绑定的buffer上
	//-----------
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);//传入索引值
#pragma endregion

#pragma region texture
	unsigned int TexBufferA;
	glGenTextures(1, &TexBufferA);
	glActiveTexture(GL_TEXTURE0);//切换贴图通道0，之后的贴图先存在0上
	glBindTexture(GL_TEXTURE_2D,TexBufferA);//GL_TEXTURE_2D为通道//只要绑定后shader里的sampler2d就可以读取到

	int width, height, nrChannel;
	//向GPU传入数据//传送数据很浪费带宽，建议一次传完
	stbi_set_flip_vertically_on_load(true);//y轴反向
	unsigned char* data = stbi_load("container.jpg",&width,&height,&nrChannel,0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);//朝通道传输数据//mipmap为第0层//设定保存格式RGB//边距为0//原图格式RGB//data格式(char存的是byte）
		glGenerateMipmap(GL_TEXTURE_2D);//为通道里的数据创建mipmap
	}
	else {
		std::cout << "load image error" << std::endl;
	}
	stbi_image_free(data);


	unsigned int TexBufferB;
	glGenTextures(1, &TexBufferB);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TexBufferB);
	unsigned char* data2 = stbi_load("awesomeface.png",&width,&height,&nrChannel,0);

	if (data2) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);//为通道里的数据创建mipmap
	}
	else {
		std::cout << "load image error" << std::endl;
	}
	stbi_image_free(data2);


#pragma endregion


//#pragma region Shader
//	unsigned int vertexShader,fragmentShader,shaderProgram;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建vertexShader，返回ID
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将shader源码传入转为二进制//第二个参数为传入的有几个字段
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(vertexShader);//将源码编译进shader
//	glCompileShader(fragmentShader);
//
//	//将vertex和fragment打包为program
//	shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram,vertexShader);//将vertex添加到program
//	glAttachShader(shaderProgram,fragmentShader);
//	glLinkProgram(shaderProgram);//连接
//
//	//绑定程序后不在需要vertex和fragment
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//#pragma endregion

	//挖顶点
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);//解释顶点（顶点属性）//设置存储在0号，顶点属性数量<1,1,1>,数据类型，是否归一化，数据间的间隔,偏移量
	//每抓3个为一个顶点
	glEnableVertexAttribArray(0);//启用顶点属性//0号属性（vertex）//1号color
	
	//挖颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));//解释顶点（顶点属性）//设置存储在1号，顶点属性数量<1,1,1>,数据类型，是否归一化，数据间的间隔,偏移量
//每抓3个为一个顶点
	glEnableVertexAttribArray(1);
	//挖UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);



	while (!glfwWindowShouldClose(window))//是否关闭窗口
	{
		//input
		processInput(window);//从下一个循环获取用户是否退出

		//-----------rendering--------------
		glClearColor(0.,0.3f,0.3f,1);//清屏为什么颜色
		glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲

			//画出VAO
		glBindVertexArray(VAO);
		//float timeValue = glfwGetTime();//获得时间
		//float greenValue = (sin(timeValue) / 2) + 0.5;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");//从shaderProgram找到位置
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation,0,greenValue,0,1);//要在确定program后使用//传入变量位置和值
			//--------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBufferA);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, TexBufferB);

		myShader->use();//使用当前Program

		//向shader的uniform传输数据
		glUniform1i(glGetUniformLocation(myShader->ID,"ourTexture"),0);
		glUniform1i(glGetUniformLocation(myShader->ID,"ourFace"),3);

		//glDrawArrays(GL_TRIANGLES, 0, 6);//画成三角//从第0个开始画，画6个顶点（三角）
			//画出EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);//需要传入索引值类型//偏移量
		//-------------
		//swapBuffer
		glfwSwapBuffers(window);//交换前后缓冲//前缓冲用于渲染/后缓冲用于绘制
		glfwPollEvents();//获取用户操作//需要一点时间//通常放在最后
	}

	glfwTerminate();//退出时清除资源

	return 0;
} 
