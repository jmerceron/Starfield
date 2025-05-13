#include "Player_Actions.h"

Player player;

void init_objects_size_on_screen()
{
    player.width = 50;
    player.height = 50;
    player.x = 400;
    player.y = 300;
    player.velocity = 0;
}

void update_objects_size_on_screen()
{
    player.width = 50;
    player.height = 50;
}

void Player::update_player(int screen_width)
{
    x += velocity;
    if (x < 0) x = 0;
    if (x > screen_width - width) x = screen_width - width;
}