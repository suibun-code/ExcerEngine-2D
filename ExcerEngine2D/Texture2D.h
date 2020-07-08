#pragma once

#include "glew.h"

class Texture2D
{
public:
	GLuint texID; //used for texture operations to reference this particular texture
	GLuint width, height; //width and height of loaded image

	//TEXTURE FORMAT
	GLuint internalFormat; //format of texture object
	GLuint imageFormat; //format of loaded image;

	//TEXTURE CONFIGURATION
	GLuint WRAP_S; //what wrapper to use when coordinate outside of texture on S (X) axis
	GLuint WRAP_T; //what wrapper to use when coordinate outside of texture on T (Y) axis
	GLuint Filter_Min; //filter to use when scaling down the image
	GLuint Filter_Max; //filter to use when scaling up the image

	Texture2D();

	//generates texture from image data
	void GenerateTexture(GLuint width, GLuint height, unsigned char* data);

	//binds texture as the current active GL_TEXTURE_2D texture object
	void BindTexture() const;
};

