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
		1280.f, 0.f, //top right
		0.f,    0.f, //top left
		0.f,    720.f, //bottom left
		1280.f, 720.f, //bottom right

		// 0.5f,  0.5f, //top right
		//-0.5f,  0.5f, //top left
		//-0.5f, -0.5f, //bottom left
		// 0.5f, -0.5f, //bottom right

		//-0.25f,  0.25f, //top left
		// 0.75f,  0.25f, //top right
		// 0.75f, -0.75f, //bottom right
		//-0.25f, -0.75f, //bottom left
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

	//time
	t_start = std::chrono::high_resolution_clock::now();

	//blending
	glEnable(GL_CULL_FACE);
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

	posAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "position"); //position
	colAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "color"); //color
	monoAlpha = glGetUniformLocation(shaderUtil.get_shaders(), "alpha"); //alpha
	texAttrib = glGetAttribLocation(shaderUtil.get_shaders(), "texcoord"); //texture
	uniTime = glGetUniformLocation(shaderUtil.get_shaders(), "time"); //time

	/***TEST VARIABLES***/

	glViewport(0, 0, 1280, 720);

	//PROJECTION
	projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -5.f, 5.f);
	GLint uniProj = glGetUniformLocation(shaderUtil.get_shaders(), "projection");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projection));

	//VIEW
	view = glm::lookAt(
		glm::vec3(0.f, 0.f, 3.f),
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f, 1.f, 0.f)
	);
	uniView = glGetUniformLocation(shaderUtil.get_shaders(), "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	//MODEL
	model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::vec4 result = model * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//printf("%f, %f, %f\n", result.x, result.y, result.z);

	uniModel = glGetUniformLocation(shaderUtil.get_shaders(), "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	transformObject(0.5f, glm::vec3(0.f, 0.f, 1.f), 0.f, glm::vec3(500.f, 0.f, 0.f)); //currently muse be used before MVP calculation.

	MVP = projection * view * model;
	uniMVP = glGetUniformLocation(shaderUtil.get_shaders(), "MVP");
	glUniformMatrix4fv(uniMVP, 1, GL_FALSE, glm::value_ptr(MVP));

	/***END TEST VARIABLES***/

	glUniform1f(monoAlpha, .5f);

	//***TEXTURES***

	glGenTextures(2, textures); // generate texture

	int mode = GL_RGB;
	SDL_Surface* testSurface = nullptr;

	//TEXTURE1

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]); //bind texture

	testSurface = IMG_Load("res/img/sample.png");

	mode = GL_RGB;

	if (testSurface->format->BytesPerPixel == 4)
		mode = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, testSurface->w, testSurface->h, 0, mode, GL_UNSIGNED_BYTE, testSurface->pixels);
	SDL_FreeSurface(testSurface);

	glUniform1i(glGetUniformLocation(shaderUtil.get_shaders(), "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //wrapping repeat on X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //wrapping clamp to border on 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //interpolation method for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //interpolation method for scaling up

	//TEXTURE2

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	testSurface = IMG_Load("res/img/sample2.png");

	mode = GL_RGB;

	if (testSurface->format->BytesPerPixel == 4)
		mode = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, testSurface->w, testSurface->h, 0, mode, GL_UNSIGNED_BYTE, testSurface->pixels);
	SDL_FreeSurface(testSurface);

	glUniform1i(glGetUniformLocation(shaderUtil.get_shaders(), "tex2"), 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //wrapping repeat on X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //wrapping clamp to border on 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //interpolation method for scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //interpolation method for scaling up

	//glBindTexture(GL_TEXTURE_2D, 0);

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
	State::update(deltaTime);
}

void OpenGLTestState::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//time
	t_now = std::chrono::high_resolution_clock::now();
	time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
	glUniform1f(uniTime, time);

	//enable vertex position
	glEnableVertexAttribArray(gVertexPos2DLocation);

	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//glBindVertexArray(gVAO2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//disable vertex position
	glDisableVertexAttribArray(gVertexPos2DLocation);

	State::render();
}

void OpenGLTestState::handle_state_events(const SDL_Event* event)
{
	State::handle_state_events(event);
}

void OpenGLTestState::exit()
{
	glBindTexture(textures[0], 0);
	glBindTexture(textures[1], 0);
	glDeleteTextures(2, textures);
	glDeleteBuffers(1, &gVBO);
	glDeleteBuffers(1, &gIBO);
	glDeleteBuffers(1, &cVBO);
	glDeleteBuffers(1, &tVBO);
}

void OpenGLTestState::transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation)
{
		model = glm::mat4(1.0f);
		model = glm::translate(model, translation);
		model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
		model = glm::scale(model, glm::vec3(scale));
		//MVP = projection * view * model;
		//glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}
