#pragma once

struct Player {
    int x;
    int y;
    int width;
    int height;
    int velocity;

    void update_player(int screen_width);
};

extern Player player;

void init_objects_size_on_screen();
void update_objects_size_on_screen();