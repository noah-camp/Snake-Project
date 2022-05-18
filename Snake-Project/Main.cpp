#include <glad.h>
#include <glfw3.h>

#include <iostream>

#include "Shader.h"
#include "Rectangle.h"

#include <string>

// ------------------------------------------  VARIABLE SETUP   -------------------------------------------------------- //

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

float triangle_left[] = {
	-0.1f,  0.0f, 0.0f,
	-0.05f, 0.1f, 0.0f,
	 0.0f,  0.0f, 0.0f
};

float triangle_right[] = {
	// positions       // colors
	0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.05f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

//float vertices[] = {
//	 -0.1f,  0.0f, 0.0f,  
//	 -0.05f, 0.1f, 0.0f,  
//	0.0f, 0.0f, 0.0f,  
//	0.05f,  0.1f, 0.0f,
//	0.1f, 0.0f, 0.0f
//};
//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 2,   // first triangle
//	2, 3, 4    // second triangle
//};

//float vertices[] = {
//	 0.15f,  0.15f, 0.0f,  // top right
//	 0.15f, -0.15f, 0.0f,  // bottom right
//	-0.15f, -0.15f, 0.0f,  // bottom left
//	-0.15f,  0.15f, 0.0f   // top left 
//};
//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

unsigned int EBO;

unsigned int VBO;	// Vertex buffer object

unsigned int vertexShader;	// Vertex shader object

unsigned int fragmentShader;	// Fragment shader object

unsigned int fragmentShader1;
unsigned int fragmentShader2;

unsigned int shaderProgram;		// Shader program object

unsigned int shaderProgram1;
unsigned int shaderProgram2;

unsigned int VAO;	// Vertex array object

unsigned int VBO_left;

unsigned int VBO_right;

unsigned int VAO_left;

unsigned int VAO_right;

int  success;
char infoLog[512];

unsigned int testShader;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\0";

const char* shader1VertexPath = "C:\\Users\\campagnm\\source\\repos\\Snake-Project\\Snake-Project\\shader1.vs";
const char* shader1FragmentPath = "C:\\Users\\campagnm\\source\\repos\\Snake-Project\\Snake-Project\\shader1.fs";
const char* shader2FragmentPath = "C:\\Users\\campagnm\\source\\repos\\Snake-Project\\Snake-Project\\shader2.fs";

int main()
{
	// ------------------------------------------  INITIALIZATION   ---------------------------------------------------- //

	glfwInit();		// Initialize GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// Major version number
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// Minor version number
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// We want to use the core profile

	// ------------------------------------------  WINDOW CREATION  ---------------------------------------------------- //

	GLFWwindow* window = glfwCreateWindow(800, 600, "ExampleWindow", NULL, NULL);	// Create a window object
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);		// Make window the main context on the current thread

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	// Initialize GLAD
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);		// Tells OpenGL size of rendering window and left corner position

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// Tells GLFW to call our resize function whenever a window is resized

	// ------------------------------------------  SHADER SETUP     ---------------------------------------------------- //

	Shader shader1(shader1VertexPath, shader1FragmentPath);
	Shader shader2(shader1VertexPath, shader2FragmentPath);


	glGenVertexArrays(1, &VAO_left);
	glGenVertexArrays(1, &VAO_right);

	glBindVertexArray(VAO_left);
	glGenBuffers(1, &VBO_left);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_left);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_left), triangle_left, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	// Enable vertex attribute

	glBindVertexArray(VAO_right);
	glGenBuffers(1, &VBO_right);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_right);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_right), triangle_right, GL_STATIC_DRAW);

	//glGenBuffers(1, &EBO);		// Generate Element Buffer Object
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// which vertex attrib (location); size of the vertex attrib (vec3); data type; normalization;
	// stride (space between consecutive vertex attributes), can leave 0 if tightly packed (let OpenGL find it); offset where position data begins
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	// Enable vertex attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	// Render Loop //
	while (!glfwWindowShouldClose(window) /* Checks if GLFW has been asked to close*/ )
	{
		// Input
		processInput(window);

		//glUseProgram(shaderProgram);	// Use shader program

		// Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// Sets color value used to clear color buffer; State-setting
		glClear(GL_COLOR_BUFFER_BIT);	// Clears color buffer; State-using

		//glUseProgram(shaderProgram1);
		shader1.use();

		glBindVertexArray(VAO_left);		// Binds vertex arary
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glUseProgram(shaderProgram2);
		shader2.use();

		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO_right);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check and call events and swap the buffers
		glfwSwapBuffers(window);	// Swaps color buffer (swaps back buffer to front buffer)
		glfwPollEvents();	// Checks if any events are triggered, updates the window state, and calls corresponding functions

		glBindVertexArray(0);
	}

	glfwTerminate();	// Properly exits application and cleans up resources

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)	// Reset size of rendering window when called
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)	// Input processing function
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)	// Checks if specified key is pressed in window (returns GLFW_RELEASE if not pressed)
		glfwSetWindowShouldClose(window, true);		// Closes GLFW
}