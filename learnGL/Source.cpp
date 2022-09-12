
#include <iostream>
#include "shader.h"
#include "Window.h"
#include "Image.h"

int main() {
	GLFWwindow* win = initWindow();

	Shader ourShader("shader.vs", "shader.fs");

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

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	bindImage("container.jpg", [](Image im) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.width, im.height, 0, GL_RGB, GL_UNSIGNED_BYTE, im.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	});

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	bindImage("awesomeface.png", [](Image im) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, im.width, im.height, 0, GL_RGB, GL_UNSIGNED_BYTE, im.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	});

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	// ourShader.setInt("texture2", 1);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);

	auto a = [texture1]() {};
	
	whileOpen(win, [texture1, texture2, &ourShader, VAO]() {
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	});

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
