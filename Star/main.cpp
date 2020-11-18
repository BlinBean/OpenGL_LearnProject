#define GLEW_STATIC	//GLEW使用STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma region shaderCode
const char* vertexShaderSource =
"#version 330 core													\n"
"layout(location = 0) in vec3 position;								\n"//从0号位置挖取数据
"void main() {														\n"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);}	\n";
const char* fragmentShaderSource =
"#version 330 core													\n"
"out vec4 color;													\n"
"void main(){														\n"
"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}							\n";
#pragma endregion

GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.8f,	0.8f, 0.0f
};
unsigned int indices[] = {
	0,1,2,
	2,1,3
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

	glViewport(0, 0, 1920, 1080);//窗口可绘制大小，前两个为起始点
	glEnable(GL_CULL_FACE);//开启剔除
	glCullFace(GL_BACK);//背面剔除
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//多边形模式设为线框
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
#pragma region Shader
	unsigned int vertexShader,fragmentShader,shaderProgram;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建vertexShader，返回ID
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//将shader源码传入//第二个参数为传入的有几个字段
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);//将源码编译进shader
	glCompileShader(fragmentShader);

	//将vertex和fragment打包为program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);//将vertex添加到program
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);//连接

	//绑定程序后不在需要vertex和fragment
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);//解释顶点（顶点属性）//设置存储在0号，顶点属性数量<1,1,1>,数据类型，是否归一化，数据间的间隔（一个顶点大小）,偏移量
	//每抓3个为一个顶点
	glEnableVertexAttribArray(0);//启用顶点属性//0号属性（vertex）//1号color

	while (!glfwWindowShouldClose(window))//是否关闭窗口
	{
		//input
		processInput(window);//从下一个循环获取用户是否退出

		//-----------rendering--------------
		glClearColor(0.,0.3f,0.3f,1);//清屏为什么颜色
		glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲

			//画出VAO
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
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
