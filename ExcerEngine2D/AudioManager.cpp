#include "AudioManager.h"

void AudioManager::set_music_volume(int volume) // clamps volume from 0 to 128
{
	volume = (volume = 0 ? 0 : (volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume));
	Mix_VolumeMusic(volume);
}

void AudioManager::set_sounds_volume(int idx, int volume) // clamps volume from 0 to 128, choose index from m_v_sounds vector to select which sounds chunk to change volume of
{
	volume = (volume = 0 ? 0 : (volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume));
	Mix_VolumeChunk(m_v_sounds[idx], volume);
}

void AudioManager::play_music(int idx, int loops)
{
	Mix_PlayMusic(m_v_music[idx], loops);
}

void AudioManager::play_sound(int idx, int channel, int loops)
{
	Mix_PlayChannel(channel, m_v_sounds[idx], loops);
}

void AudioManager::load_music(const char* file)
{
	Mix_Music* music = Mix_LoadMUS(file);
	if (music != nullptr)
		m_v_music.push_back(music);
	else
		std::cout << "[FAIL] Music Loading\n";
}

void AudioManager::load_sound(const char* file)
{
	Mix_Chunk* sound = Mix_LoadWAV(file);
	if (sound != nullptr)
		m_v_sounds.push_back(sound);
	else
		std::cout << "[FAIL] Sound Loading\n";
}

void AudioManager::toggle_music()
{
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

void AudioManager::resume_music()
{
	Mix_ResumeMusic();
}

void AudioManager::pause_music()
{
	Mix_PauseMusic();
}

bool AudioManager::is_paused()
{
	if (Mix_PausedMusic() == 1)
		return true;
	else
		return false;
}

void AudioManager::clear_music()
{
	for (int i = 0; i < (int)m_v_music.size(); i++)
	{
		Mix_FreeMusic(m_v_music[i]);
		m_v_music[i] = nullptr;
	}
	m_v_music.clear();
	m_v_music.shrink_to_fit();
}

void AudioManager::clear_sounds()
{
	for (int i = 0; i < (int)m_v_sounds.size(); i++)
	{
		Mix_FreeChunk(m_v_sounds[i]);
		m_v_sounds[i] = nullptr;
	}
	m_v_sounds.clear();
	m_v_sounds.shrink_to_fit();
}

void AudioManager::clear_newest_sound()
{
	if (!m_v_sounds.empty())
	{
		Mix_FreeChunk(m_v_sounds.back());
		m_v_sounds.back() = nullptr;
		m_v_sounds.shrink_to_fit();
	}
	else
		std::cout << "Can not clear newest sound -- m_v_sounds vector empty.\n";
}

AudioManager::~AudioManager()
{
	std::cout << "Destroying Audio Manager\n";
	clear_music();
	clear_sounds();
}