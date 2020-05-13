#include "Player.h"

Player::Player(SDL_Rect src, SDL_Rect dst) : Sprite(src, dst)
{

}

void Player::move_X(float deltaTime)
{
	m_r_dst.x += 1 * m_i_direction;
}
void Player::move_Y(float deltaTime)
{
	m_r_dst.y += 1 * m_i_direction;
}

void Player::set_direction(int m_i_direction) { this->m_i_direction = m_i_direction; }
