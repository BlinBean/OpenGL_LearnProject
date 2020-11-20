#define GLEW_STATIC	//GLEWʹ��STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//#pragma region shaderCode
//const char* vertexShaderSource =
//"#version 330 core													\n"//openGL �汾
//"layout(location = 0) in vec3 position;								\n"//��0��λ����ȡ����//����shader�ı���
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
//"uniform vec4 ourColor;												\n"//��CPU��������
//"void main(){														\n"
////"	FragColor = ourColor;											\n"
//"	FragColor = vertexColor;										\n"
//"}	\n";
//#pragma endregion

GLfloat vertices[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
};
unsigned int indices[] = {
	0,2,1,
	2,0,3
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



	glViewport(0, 0, 800, 600);//���ڿɻ��ƴ�С��ǰ����Ϊ��ʼ��
	glEnable(GL_CULL_FACE);//�����޳�
	glCullFace(GL_BACK);//�����޳�
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����ģʽ��Ϊ�߿�
	Shader* myShader=new Shader("vertexSource.txt","fragmentSource.txt");//��new�����Ļ�����ָ���//ָ��ǵ�delete

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

#pragma region texture
	unsigned int TexBufferA;
	glGenTextures(1, &TexBufferA);
	glActiveTexture(GL_TEXTURE0);//�л���ͼͨ��0��֮�����ͼ�ȴ���0��
	glBindTexture(GL_TEXTURE_2D,TexBufferA);//GL_TEXTURE_2DΪͨ��//ֻҪ�󶨺�shader���sampler2d�Ϳ��Զ�ȡ��

	int width, height, nrChannel;
	//��GPU��������//�������ݺ��˷Ѵ�������һ�δ���
	stbi_set_flip_vertically_on_load(true);//y�ᷴ��
	unsigned char* data = stbi_load("container.jpg",&width,&height,&nrChannel,0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);//��ͨ����������//mipmapΪ��0��//�趨�����ʽRGB//�߾�Ϊ0//ԭͼ��ʽRGB//data��ʽ(char�����byte��
		glGenerateMipmap(GL_TEXTURE_2D);//Ϊͨ��������ݴ���mipmap
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
		glGenerateMipmap(GL_TEXTURE_2D);//Ϊͨ��������ݴ���mipmap
	}
	else {
		std::cout << "load image error" << std::endl;
	}
	stbi_image_free(data2);


#pragma endregion


//#pragma region Shader
//	unsigned int vertexShader,fragmentShader,shaderProgram;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);//����vertexShader������ID
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//��shaderԴ�봫��תΪ������//�ڶ�������Ϊ������м����ֶ�
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(vertexShader);//��Դ������shader
//	glCompileShader(fragmentShader);
//
//	//��vertex��fragment���Ϊprogram
//	shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram,vertexShader);//��vertex��ӵ�program
//	glAttachShader(shaderProgram,fragmentShader);
//	glLinkProgram(shaderProgram);//����
//
//	//�󶨳��������Ҫvertex��fragment
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//#pragma endregion

	//�ڶ���
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),(void*)0);//���Ͷ��㣨�������ԣ�//���ô洢��0�ţ�������������<1,1,1>,�������ͣ��Ƿ��һ�������ݼ�ļ��,ƫ����
	//ÿץ3��Ϊһ������
	glEnableVertexAttribArray(0);//���ö�������//0�����ԣ�vertex��//1��color
	
	//����ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3*sizeof(float)));//���Ͷ��㣨�������ԣ�//���ô洢��1�ţ�������������<1,1,1>,�������ͣ��Ƿ��һ�������ݼ�ļ��,ƫ����
//ÿץ3��Ϊһ������
	glEnableVertexAttribArray(1);
	//��UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);



	while (!glfwWindowShouldClose(window))//�Ƿ�رմ���
	{
		//input
		processInput(window);//����һ��ѭ����ȡ�û��Ƿ��˳�

		//-----------rendering--------------
		glClearColor(0.,0.3f,0.3f,1);//����Ϊʲô��ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����

			//����VAO
		glBindVertexArray(VAO);
		//float timeValue = glfwGetTime();//���ʱ��
		//float greenValue = (sin(timeValue) / 2) + 0.5;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");//��shaderProgram�ҵ�λ��
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation,0,greenValue,0,1);//Ҫ��ȷ��program��ʹ��//�������λ�ú�ֵ
			//--------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBufferA);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, TexBufferB);

		myShader->use();//ʹ�õ�ǰProgram

		//��shader��uniform��������
		glUniform1i(glGetUniformLocation(myShader->ID,"ourTexture"),0);
		glUniform1i(glGetUniformLocation(myShader->ID,"ourFace"),3);

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
