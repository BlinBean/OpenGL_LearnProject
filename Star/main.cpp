#define GLEW_STATIC	//GLEWʹ��STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma region shaderCode
const char* vertexShaderSource =
"#version 330 core													\n"
"layout(location = 0) in vec3 position;								\n"//��0��λ����ȡ����
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {//�����סESC
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc,char* argv[]) {
	
#pragma region GLFW
	glfwInit();//��ʼ��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//��ʾʹ�õ�openGL���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//ʹ�õ�openGL�ΰ汾//Ҳ����ʹ��openGL3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ָ������//�ɱ����ˮ��

	//---------------open GLFW window--------------
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "star", NULL, NULL);
	if (window == NULL) {
		std::cout << "δ��������" << std::endl;
		glfwTerminate();//��ֹGLFW
		return -1;
	}
	glfwMakeContextCurrent(window);//ʹ�����window
	//-----------------------------------------
#pragma endregion
#pragma region GLEW
	//-----------init glew--------------
	glewExperimental = true;//ʵ������
	if (glewInit() != GLEW_OK) {//glew�������³ɹ��Ļ�������GLEW_OK
		std::cout << "GLEW ��ʼ��ʧ��" << std::endl;
		glfwTerminate();//��ֹGLFW
		return -1;
	}
	//-------------------------------
#pragma endregion

	glViewport(0, 0, 1920, 1080);//���ڿɻ��ƴ�С��ǰ����Ϊ��ʼ��
	glEnable(GL_CULL_FACE);//�����޳�
	glCullFace(GL_BACK);//�����޳�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����ģʽ��Ϊ�߿�
#pragma region VBO&VAO&EBO
	unsigned int VAO, VBO,EBO;
	glGenVertexArrays(1, &VAO);//����һ��VAO������VAO ID
	glBindVertexArray(VAO);//ArrayBuffer��VAO(״̬��|GL)

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//buffer��VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���û���������ݸ��Ƶ���ǰ�󶨵�buffer��
	//-----------
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//��ELEMENT_ARRAY_BUFFER
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);//��������ֵ
#pragma endregion
#pragma region Shader
	unsigned int vertexShader,fragmentShader,shaderProgram;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����vertexShader������ID
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//��shaderԴ�봫��//�ڶ�������Ϊ������м����ֶ�
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);//��Դ������shader
	glCompileShader(fragmentShader);

	//��vertex��fragment���Ϊprogram
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);//��vertex��ӵ�program
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);//����

	//�󶨳��������Ҫvertex��fragment
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(void*)0);//���Ͷ��㣨�������ԣ�//���ô洢��0�ţ�������������<1,1,1>,�������ͣ��Ƿ��һ�������ݼ�ļ����һ�������С��,ƫ����
	//ÿץ3��Ϊһ������
	glEnableVertexAttribArray(0);//���ö�������//0�����ԣ�vertex��//1��color

	while (!glfwWindowShouldClose(window))//�Ƿ�رմ���
	{
		//input
		processInput(window);//����һ��ѭ����ȡ�û��Ƿ��˳�

		//-----------rendering--------------
		glClearColor(0.,0.3f,0.3f,1);//����Ϊʲô��ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����

			//����VAO
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		//glDrawArrays(GL_TRIANGLES, 0, 6);//��������//�ӵ�0����ʼ������6�����㣨���ǣ�
			//����EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);//��Ҫ��������ֵ����//ƫ����
		//-------------
		//swapBuffer
		glfwSwapBuffers(window);//����ǰ�󻺳�//ǰ����������Ⱦ/�󻺳����ڻ���
		glfwPollEvents();//��ȡ�û�����//��Ҫһ��ʱ��//ͨ���������
	}

	glfwTerminate();//�˳�ʱ�����Դ

	return 0;
} 
