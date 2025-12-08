#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//Math Libraries
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//other files
#include "CameraClass.h"
#include "GraphNode.h"
#include "CelestialBody.h"
#include "Mesh.h"
#include "shaderClass.h" //shader program class
#include "VBO.h" //vertex buffer class`
#include "EBO.h" //element (index) buffer class
#include "VAO.h" //vertex array object class

#define WINDOW_WIDTH 1280.0f
#define WINDOW_HEIGHT 720.0f

//frame times
float deltaTime;
float lastFrameTime;

Camera mainCamera;

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


	//this movement is actually terrible, if two movement buttons are pressed, their vector direction is not normalized
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mainCamera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mainCamera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mainCamera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mainCamera.ProcessKeyboard(RIGHT, deltaTime);
	}
}


float lastX = WINDOW_WIDTH / 2, lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	mainCamera.ProcessMouseMovement(xOffset, yOffset);
}


const float goldenRatio = (1 + sqrt(5)) / 2;
const int pointCount = 250;
//fibonacci sphere vertex construction - was looking at delaunay triangulation for creating the element array of faces (this will take too long, I just need a sphere-like thing so will use a icosohedron
glm::vec3 GetSpherePointFromIndex(int index)
{
	float j = index + 0.5;
	float theta = 2 * glm::pi<float>() * j / goldenRatio;
	float phi = glm::acos(1 - 2 * j / pointCount);
	float x = glm::cos(theta) * glm::sin(phi);
	float y = glm::sin(theta) * glm::sin(phi);
	float z = glm::cos(phi);
	return glm::vec3(x, y, z);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetScrollCallback(window, scrollCallback);



	//------------------Set Up The Data We Send To The GPU------------------
	// A buffer is used so that all data is sent at once, reducing latency as the GPU is much faster than the CPU
	
	GLfloat cubeVertices[] = {
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

	
	const GLfloat halfgr = goldenRatio / 2;
	GLfloat icosahedronVertices[] = {
		//x			y			z			r	  g		b
		-0.5,		halfgr,		0.f,		1.f, 0.f, 0.f, //0
		0.5,		halfgr,		0.f,		1.f, 0.f, 0.f, //1
		-0.5,		-halfgr,	0.f,		1.f, 0.f, 0.f, //2
		0.5,		-halfgr,	0.f,		1.f, 0.f, 0.f, //3
		0.f,		-0.5f,		-halfgr,	1.f, 0.f, 0.f, //4
		0.f,		0.5f,		-halfgr,	1.f, 0.f, 0.f, //5
		0.f,		-0.5f,		halfgr,		1.f, 0.f, 0.f, //6
		0.f,		0.5f,		halfgr,		1.f, 0.f, 0.f, //7
		halfgr,		0.f,		0.5f,		1.f, 0.f, 0.f, //8
		halfgr,		0.f,		-0.5f,		1.f, 0.f, 0.f, //9
		-halfgr,	0.f,		0.5f,		1.f, 0.f, 0.f, //10
		-halfgr,	0.f,		-0.5f,		1.f, 0.f, 0.f  //11
	};

	GLuint icosahedronIndices[] = {
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5,
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};


	/*float sphereVertices[pointCount * 3 * 2];
	for (int i = 0; i < pointCount; i++)
	{
		glm::vec3 current = GetSpherePointFromIndex(i);
		//layout 1: aPos
		sphereVertices[i * 2] = current.x;
		sphereVertices[(i * 2) + 1] = current.y;
		sphereVertices[(i * 2) + 2] = current.z;

		//layout 2: VertexColour
		sphereVertices[(i * 2) + 3] = 1.f;
		sphereVertices[(i * 2) + 4] = 1.f;
		sphereVertices[(i * 2) + 5] = 1.f;
	}*/


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
	

//--------------------------------------END OF VERTEX/ELEMENT DATA--------------------------------------
		
	//create and compile complete shader program from default.vert and default.frag vertex and fragment shaders
	Shader shaderProgram("default.vert", "default.frag");

	//Generate Vertex Array Object and Bind it
	VAO VAO1;
	VAO1.Bind();

	//Generate Vertex Buffer Object and link the vertex data defined above into it
	VBO VBO1(icosahedronVertices, sizeof(icosahedronVertices));

	//defining the location of vertex attributes(Layout) - 0: position, 1: colour
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));

	//Generate Element (index) Buffer Object and link the tri data from indices defined above into it
	EBO EBO1(icosahedronIndices, sizeof(icosahedronIndices));


	//prevent accidentally modifying VBO/VAO/EBO in lines below this
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	//----------------------------------------------------------Main Loop----------------------------------------------------------
	float modulatedValue;
	glm::mat4 transform;
	//Mesh Cobra(VAO1, EBO1, shaderProgram);
	Mesh Planet(VAO1, EBO1, shaderProgram);
	Node Scene;

	//initialize the Game Scene
	for (int i = 0; i < 10; i++)
	{
		CelestialBody* current = new CelestialBody(&Planet);
		if ((i % 2) == 0)
		{
			current->Initialize(objectPositions[i], objectPositions[i] * 5.f, 1000.f);
		}
		else
		{
			current->Initialize(-objectPositions[i], objectPositions[i] * 5.f, 1000.f);
		}
		
		current->Parent(&Scene);
	}

	bool firstFrame = true;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime(); //time since Program execution has started a(seconds)
		deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;

		//input
		processInput(window);

		//rendering (pipeline (* for where we can inject custom shader)
		//*Vertex Shader -> *Geometry Shader -> Shape Assembly -> Rasterizer -> *Fragment(per pixel) Shader -> Tests + Blending


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //state *using* function, actually reset the buffer specified (in this case, the colour buffer) to the current state, retrieving the clearing colour

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable color blending for transparency

		shaderProgram.Activate();
		
		//when creating a tranformation matrix, order is important - translate -> rotate -> scale
		//View Matrix *is* the camera
		shaderProgram.SetMat4("view", mainCamera.GetViewMatrix());


		glm::mat4 projection(glm::perspective(glm::radians(70.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f));
		shaderProgram.SetMat4("projection", projection);
		

		//float angle = 20.0f * deltaTime;

		Scene.ProcessPhysics(deltaTime);


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		Scene.Render(glm::mat4(1.0f), firstFrame);
		

		
		//check and call events + buffer swap to display next frame
		glfwPollEvents(); 
		glfwSwapBuffers(window);
		glfwSwapInterval(1);
		firstFrame = false;
	}

	//release all allocated resources after main render loop ends
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	Scene.Destroy(true);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}