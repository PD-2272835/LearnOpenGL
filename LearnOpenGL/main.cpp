#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include"shaderClass.h" //shader program class
#include"VBO.h" //vertex buffer class
#include"EBO.h" //element (index) buffer class
#include"VAO.h" //vertex array object class

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tommy sucks peen", NULL, NULL);
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

	//vertex data, describe vertices in normalized world coords
	GLfloat vertices[] = {
		//Layout 0 - aPos									Layout 1 - aColor
		-0.5f,	-0.5f * float(sqrt(3)) / 3,		0.0f,		1.0f, 0.0f, 0.0f,	//lower left corner : 0		(red)
		0.5f,	-0.5f * float(sqrt(3)) / 3,		0.0f,		0.0f, 1.0f, 0.0f,	//lower right corner : 1	(green)
		0.0f,	 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		0.0, 0.0f, 1.0f,	//top corner : 2			(blue)
		
		//just need the three far corners for using vertex data with shaders
		/*-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	 //inner left : 3
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	 //inner right : 4
		0.0f,  -0.5f * float(sqrt(3)) / 3, 0.0f, */	 //inner bottom : 5
	};

	//describe the triangles as an order of vertices using the index of each vertex in the VBO
	GLuint serpinsky[] = {
		0, 3, 5, //left tri
		5, 4, 1, //right tri
		3, 4, 2 //top tri
	};

	GLuint triangle[] = {
		0, 1, 2 //whole triangle
	};



	//create and compile complete shader program from default.vert and default.frag vertex and fragment shaders
	Shader shaderProgram("default.vert", "default.frag");

	//Generate Vertex Array Object and Bind it
	VAO VAO1;
	VAO1.Bind();

	//Generate Vertex Buffer Object and link the vertex data defined above into it
	VBO VBO1(vertices, sizeof(vertices));

	//defining the location of vertex attributes - 0: position, 1: colour
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, GL_FALSE, 6 * (sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));

	//Generate Element (index) Buffer Object and link the tri data from indices defined above into it
	EBO EBO1(triangle, sizeof(triangle));


	//prevent accidentally modifying VBO/VAO/EBO in lines below this
	VAO1.UnBind();
	VBO1.Unbind();
	EBO1.Unbind();

	/*
	VAO VAO2;
	VAO2.Bind();
	EBO EBO2(indices, sizeof(indices));
	VAO2.LinkVBO(VBO1, 0); //reuse first vetex buffer data
	VAO2.UnBind();
	*/




	//----------------------------------------------------------Main Loop----------------------------------------------------------

	float timeValue;
	float modulatedValue;
	GLuint alphaModulatorLocation;


	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//rendering (pipeline (* for where we can inject custom shader)
		//*Vertex Shader -> *Geometry Shader -> Shape Assembly -> Rasterizer -> *Fragment(per pixel) Shader -> Tests + Blending
		glClearColor(0.245f, 0.04f, 0.2f, 0.8f); //state *setting* function, specifiying the colour used to reset the colorBuffer
		glClear(GL_COLOR_BUFFER_BIT); //state *using* function, actually reset the buffer specified (in this case, the colour buffer) to the current state, retrieving the clearing colour
		shaderProgram.Activate();

		glEnable(GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //enable color blending for transparency

		timeValue = glfwGetTime(); //time since Program execution has started a(seconds)
		modulatedValue = (sin(timeValue) / 2.0f) + 0.5f;
		shaderProgram.SetFloat("alphaModulator", modulatedValue);

		
		//std::cout << modulatedValue << std::endl; //this works, why does the shader not? unless it's to do with it interpolating vertex colour values?
		
		VAO1.Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		VAO1.UnBind();
		
		//VAO2 undefined as I'm not using that buffer data for shaders 
		/*
		VAO2.Bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		VAO2.UnBind(); */
		
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

