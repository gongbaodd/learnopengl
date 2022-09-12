#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"

const unsigned SCR_WIDTH = 800;
const unsigned SCR_HEIGHT = 600;

GLuint createArrayObject() {
	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	return VAO;
}

GLuint createBuffer(float* vertices, unsigned len) {
	unsigned VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);

	return VBO;
}

GLuint createElement(unsigned* indices, unsigned len) {
	unsigned EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, indices, GL_STATIC_DRAW);
	return EBO;
}

void bindArrayObject() {

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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

GLuint createTexture() {
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}

struct Image {
	int width;
	int height;
	int nrChannels;
	unsigned char* data;
	const char* path;
};

void bindImage(const char* path) {
	int width, height, nrChannels;

	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "Load Image, " << path << "\t" << width << "x" << height << std::endl;
	
	
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

int main() {
	auto window = initWindow();

	if (NULL == window) {
		return -1;
	}
	
	Shader ourShader("./shader.vs", "./shader.fs");

	//  d - a
	//  |   |
	//  c - b	

	float vertices[] = {
		// pos           // color       // texture
		 0.5,  0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, // a
		 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, // b
		-0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, // c
		-0.5,  0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0  // d
	};

	unsigned indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	GLuint VAO = createArrayObject();
	GLuint VBO = createBuffer(vertices, sizeof(vertices));
	GLuint EBO = createElement(indices, sizeof(indices));
	bindArrayObject();

	GLuint texture1 = createTexture();
	bindImage("container.jpg");

	/*GLuint texture2 = createTexture();
	bindImage("awesomeface.png", [](Image img) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	});*/

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.ID);

	glfwTerminate();

	return 0;
}


