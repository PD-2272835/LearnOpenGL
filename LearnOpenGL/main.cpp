#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include"shaderClass.h" //shader program class
#include"VBO.h" //vertex buffer class
#include"EBO.h" //element (index) buffer class
#include"VAO.h" //vertex array object class

#define WINDOW_WIDTH 1280.0f
#define WINDOW_HEIGHT 720.0f


//each time the window is resized, set the viewport width and height to the new resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	//close the process if escape key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//--------------Initialize-------------------
	std::cout << "starting OpenGL window" << std::endl;
	//create a new openGL context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create a window object
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "The Legend Of Cube: Tears of the Cube (lawsuit incoming)", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create window" << std::endl;
		glfwTerminate(); //fail gracefully if the window fails to create
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize GLAD (manages function pointers for OpenGL)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); //establish rendering viewport (same size as window)

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //bind the window resize callback



	//------------------Set Up The Data We Send To The GPU------------------
	// A buffer is used so that all data is sent at once, reducing latency as the GPU is much faster than the CPU


	GLfloat vertices[] = {
		//Layout 0 - aPos
		-1.0f,		-1.0f,		-1.0f,		1.0f, 0.0f, 0.0f,//0
		1.0f,		-1.0f,		-1.0f,		0.5f, 0.5f, 0.0f,//1
		1.0f,		1.0f,		-1.0f,		0.0f, 0.5f, 0.5f,//2
		-1.0f,		1.0f,		-1.0f,		0.0f, 0.0f, 1.0f,//3

		-1.0f,		-1.0f,		1.0f,		1.0f, 0.0f, 0.0f,//4
		1.0f,		-1.0f,		1.0f,		0.5f, 0.5f, 0.0f,//5
		1.0f,		1.0f,		1.0f,		0.0f, 0.5f, 0.5f,//6
		-1.0f,		1.0f,		1.0f,		0.0f, 0.0f, 1.0f//7
	};


	GLuint cubeIndices[] = {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	GLfloat cobraVertices[] = {
		//x			y			z			r	  g		b
		0.4f,		0.0f,		0.9f,		1.0f, 0.0f, 0.0f,	//0
		-0.4f,		0.0f,		0.9f,		0.0f, 1.0f, 0.0f,	//1
		0.0f,		0.3f,		0.0f,		0.0f, 0.0f, 1.0f,	//2*
		-1.9f,		0.0f,		-0.7f,		1.0f, 0.0f, 0.0f,	//3
		1.9f,		0.0f,		-0.7f,		0.0f, 1.0f, 0.0f,	//4
		-1.4f,		0.2f,		-1.0f,		0.0f, 0.0f, 1.0f,	//5*
		1.4f,		0.2f,		-1.0f,		1.0f, 0.0f, 0.0f,	//6*
		2.0f,		0.0f,		-1.0f,		0.0f, 1.0f, 0.0f,	//7
		-2.0f,		0.0f,		-1.0f,		0.0f, 0.0f, 1.0f,	//8
		0.0f,		0.3f,		-1.0f,		1.0f, 0.0f, 0.0f,	//9*

		0.0f,		-0.3f,		0.0f,		0.0f, 1.0f, 0.0f,	//10*
		-1.5f,		-0.2f,		-1.0f,		0.0f, 0.0f, 1.0f,	//11*
		1.5f,		-0.2f,		-1.0f,		1.0f, 0.0f, 0.0f,	//12*
		0.0f,		-0.3f,		-1.0f,		0.0f, 1.0f, 0.0f,	//13*
	};

	GLuint cobraIndices[] = {
		0, 1, 2, //F0
		1, 2, 5, //F1
		0, 2, 6, //F2
		1, 5, 3, //F3
		0, 6, 4, //F4
		2, 5, 9, //F5
		2, 6, 9, //F6
		3, 5, 8, //F7
		4, 6, 7, //F8

		0, 1, 10, //F9
		1, 10, 11, //F10
		0, 10, 12, //F11
		1, 11, 3, //F12
		0, 12, 4, //F13
		10, 11, 13, //F14
		10, 12, 13, //F15
		3, 11, 8, //F16
		4, 12, 7, //F17
	};

	glm::vec3 objectPositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	//create and compile complete shader program from default.vert and default.frag vertex and fragment shaders
	Shader shaderProgram("default.vert", "default.frag");

	//Generate Vertex Array Object and Bind it
	VAO VAO1;
	VAO1.Bind();

	//Generate Vertex Buffer Object and link the vertex data defined above into it
	VBO VBO1(cobraVertices, sizeof(cobraVertices));

	//defining the location of vertex attributes(Layout) - 0: position, 1: colour
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));

	//Generate Element (index) Buffer Object and link the tri data from indices defined above into it
	EBO EBO1(cobraIndices, sizeof(cobraIndices));


	//prevent accidentally modifying VBO/VAO/EBO in lines below this
	VAO1.UnBind();
	VBO1.Unbind();
	EBO1.Unbind();


	//--------------------------------------------Camera--------------------------------------------
	glm::vec3 up = glm::vec3(0, 1, 0); //world space up

	glm::vec3 cameraPos = glm::vec3(0.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f);

	//camera coordinate system
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;

	glm::mat4 view;


	//----------------------------------------------------------Main Loop----------------------------------------------------------

	float timeValue;
	float modulatedValue;
	GLuint alphaModulatorLocation;
	glm::mat4 transform;

	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//rendering (pipeline (* for where we can inject custom shader)
		//*Vertex Shader -> *Geometry Shader -> Shape Assembly -> Rasterizer -> *Fragment(per pixel) Shader -> Tests + Blending
		//glClearColor(0.245f, 0.04f, 0.2f, 0.8f); //state *setting* function, specifiying the colour used to reset the colorBuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //state *using* function, actually reset the buffer specified (in this case, the colour buffer) to the current state, retrieving the clearing colour

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable color blending for transparency

		shaderProgram.Activate();
		
		
		timeValue = glfwGetTime(); //time since Program execution has started a(seconds)
		modulatedValue = (sin(timeValue) / 2.0f) + 0.5f;
		shaderProgram.SetFloat("alphaModulator", modulatedValue);
		

		cameraDirection = glm::normalize(cameraPos - cameraTarget);
		cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		cameraUp = glm::cross(cameraDirection, cameraRight); //this will already return a normalized vector as direction and right are both unit vectors themselves
		
		//initialize the identity matrix (1.0f on the diagonal)
		glm::mat4 view = glm::mat4(1.0f); //view matrix *is* the camera

		const float radius = 30.0f;
		cameraPos.x = sin(timeValue) * radius;
		cameraPos.z = cos(timeValue) * radius;
		view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);


		shaderProgram.SetMat4("view", view);


		glm::mat4 projection = glm::perspective(glm::radians(70.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
		shaderProgram.SetMat4("projection", projection);

		//when creating a tranformation matrix, order is important - translate -> rotate -> scale
		
		
		VAO1.Bind();
		EBO1.Bind();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for (int i = 0; i < 10; i++) {

			transform = glm::mat4(1.0f);
			transform = glm::translate(transform, objectPositions[i]);
			float angle = 20.0f * i;
			transform = glm::rotate(transform, glm::radians(angle), objectPositions[i]);


			shaderProgram.SetMat4("model", transform);
			glDrawElements(GL_TRIANGLES, sizeof(cobraIndices), GL_UNSIGNED_INT, 0);
		}

		VAO1.UnBind();
		EBO1.Unbind();
		
		
		//check and call events + buffer swap to display next frame
		glfwPollEvents(); 
		glfwSwapBuffers(window);
	}

	//release all allocated resources after main render loop ends
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

