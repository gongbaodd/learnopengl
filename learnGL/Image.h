#ifndef IMAGE_H
#define IMAGE_H

#include "stb_image.h"
#include <iostream>

struct Image {
	int width;
	int height;
	int nrChannels;
	unsigned char* data;
	const char* path;
};

void bindImage(const char* path, void callback(Image im)) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {
		callback(Image{
			width,
			height,
			nrChannels,
			data,
			path
			});
		std::cout << "Load Image, " << path << "\t" << width << "x" << height << std::endl;
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}
#endif // !IMAGE_H
