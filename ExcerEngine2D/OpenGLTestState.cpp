#include "OpenGLTestState.h"

void OpenGLTestState::enter()
{
	//IBO data
	GLuint indexData[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0  //second triangle
	};

	//VBO data
	GLfloat vertexData[] =
	{
		-0.25f,  0.25f, //top left
		 0.75f,  0.25f, //top right
		 0.75f, -0.75f, //bottom right
		-0.25f, -0.75f, //bottom left
	};

	GLfloat colorData[] =
	{
		1.f, 0.f, 0.f, //top left
		1.f, 0.f, 0.f, //top right
		0.f, 1.f, 0.f, //bottom right
		1.f, 0.f, 0.f  //bottom left
	};

	GLfloat textureData[] =
	{
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		1.f, 1.f, //bottom right
		0.f, 1.f  //bottom left
	};

	//test
	GLuint indexData2[] =
	{
		0, 1, 2, //first triangle
		2, 3, 0	 //second triangle
	};

	GLfloat vertexData2[] =
	{
		-0.75f,  0.75f, //top left
		 0.25f,  0.75f, //top right
		 0.25f, -0.25f, //bottom right
		-0.75f, -0.25f, //bottom left
	};

	GLfloat colorData2[] =
	{
		0.f, 1.f, 0.f, //top left
		0.f, 0.f, 1.f, //top right
		0.f, 0.f, 1.f, //bottom right
		0.f, 0.f, 1.f  //bottom left
	};

	GLfloat textureData2[] =
	{
		0.f, 0.f, //top left
		1.f, 0.f, //top right
		1.f, 1.f, //bottom right
		0.f, 1.f  //bottom left
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//load program
	shaderUtil.load_shaders("shaders/vs.shader", "shaders/fs.shader");
	//bind program
	shaderUtil.use_shaders();

	//get vertex attribute location
	gVertexPos2DLocation = glGetAttribLocation(shaderUtil.get_shaders(), "position");

	//clear to this color
	glClearColor(.5f, .5f, .5f, 1.f);

	GLint posAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "position");
	GLint colAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "color");
	GLint monoAlpha = glGetUniformLocation(shaderUtil.get_shaders(), "alpha");
	GLint texAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "texcoord");
	glUniform1f(monoAlpha, .5f);

	SDL_Surface* testSurface = IMG_Load("res/img/sample.png");

	glGenTextures(1, &tex); // generate texture
	glBindTexture(GL_TEXTURE_2D, tex); //bind texture

	int mode = GL_RGB;

	if (testSurface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, mode, testSurface->w, testSurface->h, 0, mode, GL_UNSIGNED_BYTE, testSurface->pixels);
	SDL_FreeSurface(testSurface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //wrapping repeat on X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //wrapping clamp to border on 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //interpolation method for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //interpolation method for scaling up

	//float borderColor[] = { 1.f, 1.f, 1.f, 1.f }; //border color if clamping to border
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); //set the border color

	glGenerateMipmap(GL_TEXTURE_2D); //generate mipmap

	//create and bind VAO
	gVAO = 0;
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	//create IBO
	gIBO = 0;
	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	//create vertex VBO
	gVBO = 0;
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	//create color VBO
	cVBO = 0;
	glGenBuffers(1, &cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	//create texture VBO
	tVBO = 0;
	glGenBuffers(1, &tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	//***********************************************************************

	gVAO2 = 0;
	glGenVertexArrays(1, &gVAO2);
	glBindVertexArray(gVAO2);

	GLuint gIBO2 = 0;
	glGenBuffers(1, &gIBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData2), indexData2, GL_STATIC_DRAW);

	GLuint gVBO2 = 0;
	glGenBuffers(1, &gVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2), vertexData2, GL_STATIC_DRAW);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	GLuint cVBO2 = 0;
	glGenBuffers(1, &cVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData2), colorData2, GL_STATIC_DRAW);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colAttrib);

	//create texture VBO
	GLuint tVBO2 = 0;
	glGenBuffers(1, &tVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData2), textureData2, GL_STATIC_DRAW);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(texAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, NULL); // Can optionally unbind the buffer to avoid modification.
	glBindVertexArray(NULL); // Can optionally unbind the vertex array to avoid modification.
}

void OpenGLTestState::update(float deltaTime)
{
}

void OpenGLTestState::render()
{
	//clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//enable vertex position
	glEnableVertexAttribArray(gVertexPos2DLocation);

	//set array data
	glBindVertexArray(gVAO);

	//render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//set array data
	glBindVertexArray(gVAO2);

	//render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//disable vertex position
	glDisableVertexAttribArray(gVertexPos2DLocation);
}

void OpenGLTestState::exit()
{
}
