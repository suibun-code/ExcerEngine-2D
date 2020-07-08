//GLM
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include "XSprite.h"

void XSprite::initRenderData()
{
	//gVBO data
	GLfloat vertexData[] =
	{
		1280.f, 0.f,	//top right
		0.f,    0.f,	//top left
		0.f,    720.f,	//bottom left
		1280.f, 720.f,	//bottom right
	};

	//gIBO data
	GLuint indexData[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0  //second triangle
	};

	//cVBO data
	GLfloat colorData[] =
	{
		1.f, 0.f, 0.f, //top left
		1.f, 0.f, 0.f, //top right
		0.f, 1.f, 0.f, //bottom right
		1.f, 0.f, 0.f  //bottom left
	};

	//tVBO data
	GLfloat textureData[] =
	{
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		1.f, 1.f, //bottom right
		0.f, 1.f  //bottom left
	};

	posAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "position"); //position
	colAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "color"); //color
	alphaUni = glGetUniformLocation(shaderUtil.get_shaders(), "alpha"); //alpha
	texAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "texcoord"); //texture
	timeUni = glGetUniformLocation(shaderUtil.get_shaders(), "time"); //time

	glGenVertexArrays(1, &gVAO);
	glGenBuffers(1, &gIBO);
	glGenBuffers(1, &gVBO);
	glGenBuffers(1, &cVBO);
	glGenBuffers(1, &tVBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	//unbind buffer and vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

XSprite::XSprite(ShaderUtil shaderUtil)
{

}

XSprite::~XSprite()
{
}

void XSprite::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	shaderUtil.use_shaders();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	GLint uniModel = glGetUniformLocation(shaderUtil.get_shaders(), "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glActiveTexture(GL_TEXTURE0);
	texture.BindTexture();

	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
