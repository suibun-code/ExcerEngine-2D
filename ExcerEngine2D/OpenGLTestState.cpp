#include "OpenGLTestState.h"

void OpenGLTestState::enter()
{
	//SPRITE CLASS TESTING***********************
	OGLSprite* sprite = new OGLSprite(0, 0, 1280, 720, 0, 0);
	allSprites.push_back(sprite);
	std::cout << "YO: " << allSprites.front()->spriteSize.w << "\n";

	//time
	t_start = std::chrono::high_resolution_clock::now();

	//clear to this color
	glClearColor(.5f, .5f, .5f, 1.f);

	glUniform1f(monoAlpha, .5f);
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
	glEnableVertexAttribArray(allSprites.front()->get_vertpos2d());

	glBindVertexArray(allSprites.front()->get_sprite_VAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//glBindVertexArray(gVAO2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	//disable vertex position
	glDisableVertexAttribArray(allSprites.front()->get_vertpos2d());

	State::render();
}

void OpenGLTestState::handle_state_events(const SDL_Event* event)
{
	State::handle_state_events(event);
}

void OpenGLTestState::exit()
{
	while(!allSprites.empty())
		allSprites.front()->destroy_sprite();

	//glBindTexture(textures[0], 0);
	//glBindTexture(textures[1], 0);
	//glDeleteTextures(2, textures);
	//glDeleteBuffers(1, &gVBO);
	//glDeleteBuffers(1, &gIBO);
	//glDeleteBuffers(1, &cVBO);
	//glDeleteBuffers(1, &tVBO);
}

//void OpenGLTestState::transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation)
//{
//		model = glm::translate(model, translation);
//		model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
//		model = glm::scale(model, glm::vec3(scale));
//		//MVP = projection * view * model;
//		//glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(MVP));
//		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
//		//glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projection));
//		//glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
//}
//
//void OpenGLTestState::transformObject(float scale)
//{
//	model = glm::scale(model, glm::vec3(scale));
//}
//
//void OpenGLTestState::transformObject(glm::vec3 rotationAxis, float rotationAngle)
//{
//	model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
//}
//
//void OpenGLTestState::transformObject(glm::vec3 translation)
//{
//	model = glm::translate(model, translation);
//}
