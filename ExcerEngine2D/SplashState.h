#pragma once

#include "FSM.h"
#include "Engine.h"
#include "ConsoleIO.h"

class SplashState : public State
{
private:
	std::vector<Image*> m_v_images;

	template <class T>
	void clean_vector(std::vector<T> vec);

public:
	SplashState() : State("Splash") {}
	void enter();
	void update(float deltaTime);
	void render();
	void resume() {}
	void exit();
};

template<class T>
inline void SplashState::clean_vector(std::vector<T> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
	vec.clear();
	vec.shrink_to_fit();
}
