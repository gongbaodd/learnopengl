#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = ""
	"# version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}";

const char* fragmentShaderSource = ""
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
	"}";

const unsigned SCR_WIDTH = 800;
const unsigned SCR_HEIGHT = 600;

unsigned createArrayObject() {
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	return VAO;
}

unsigned createBuffer(float* vertices, unsigned len) {
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);

	return VBO;
}

void bindArrayObject() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int createVertexShader() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return vertexShader;
}

unsigned int createFragmentShader() {

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return fragmentShader;
}

unsigned createShaderProgram(unsigned vertexShader, unsigned fragmentShader) {
	unsigned shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "GLFW Failed" << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD Failed" << std::endl;
		return NULL;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	auto window = initWindow();

	if (NULL == window) {
		return -1;
	}
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	auto vertexShader = createVertexShader();
	// fragment shader
	auto fragmentShader = createFragmentShader();
	// link shaders
	auto shaderProgram = createShaderProgram(vertexShader, fragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	/*
	 a--b
	 |  |
	 c--d
	*/

	float rect[] = {
		// right top
		0.5f, 0.5f, 0.0f,  // b
		0.5f, -0.5f, 0.0f, // d
		-0.5f, 0.5f, 0.0f, // a
		// left bottom
		// 0.5f, -0.5f, 0.0f, // duplicated d
		-0.5f, -0.5f, 0.0f,   // c
		// -0.5f, 0.5f, 0.0f, // duplicated a
	};

	unsigned indices[] = {
		0, 1, 2, // bda 
		1, 2, 3  // dac
	};


	unsigned VAO = createArrayObject();
	unsigned VBO = createBuffer(rect, sizeof(rect));

	unsigned EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	bindArrayObject();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(0, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}


