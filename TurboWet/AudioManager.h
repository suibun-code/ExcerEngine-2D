#pragma once

#include <iostream>
#include <vector>

#include "SDL_mixer.h"

class AudioManager
{
private:
	std::vector<Mix_Music*> m_v_music;
	std::vector<Mix_Chunk*> m_v_sounds;

public:
	~AudioManager();

	void set_music_volume(int volume);
	void set_sounds_volume(int idx, int volume);
	void play_music(int idx = 0, int loops = -1);
	void play_sound(int idx, int channel = -1, int loops = 0);
	void load_music(const char* file);
	void load_sound(const char* file);
	void toggle_music();
	void resume_music();
	void pause_music();
	bool is_paused();
	void clear_music();
	void clear_sounds();
	void clear_newest_sound();
};

