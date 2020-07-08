#include "Texture2D.h"

Texture2D::Texture2D()
	: texID(0), width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB), WRAP_S(GL_REPEAT), WRAP_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	internalFormat = GL_RGB;

	//if (testSurface->format->BytesPerPixel == 4)
	//	mode = GL_RGBA;

	glGenTextures(1, &this->texID);
}

void Texture2D::GenerateTexture(GLuint width, GLuint height, unsigned char* data)
{
	this->width = width;
	this->height = height;

	//create texture
	glBindTexture(GL_TEXTURE_2D, this->texID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

	//set wrapper and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WRAP_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WRAP_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::BindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, this->texID);
}