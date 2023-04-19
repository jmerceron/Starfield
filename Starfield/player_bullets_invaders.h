#pragma once





///////////////////////////////////////
// Player Ship class //////////////////
///////////////////////////////////////
class Player {
public:
    int x, y;
    int width, height;
    int velocity;

    void update(int screen_width)
    {
        // Check if the player collides with the left or right walls
        if (x < 0 && velocity < 0)
        {
            velocity = 0;
        }
        if (x + width > screen_width && velocity > 0)
        {
            velocity = 0;
        }

        // Update player position based on velocity
        x += velocity;
    }
};
Player mPlayer;


///////////////////////////////////////
// Player ship bullets ////////////////
///////////////////////////////////////
struct PlayerBullet
{
    int x; // X-coordinate of the bullet
    int y; // Y-coordinate of the bullet
    int width; // Width of the bullet
    int height; // Height of the bullet
    int velocity_x;
    int velocity_y;
    bool active;
    SDL_Texture* texture; // Texture for the bullet
    SDL_Rect rect; // SDL_Rect representing the position and size of the bullet
};
const int nbPlayerBullets = 1000;
PlayerBullet mPlayerBullet[nbPlayerBullets];


// Update the bullet's position based on game logic
void fn_vPlayerBullets_Update()
{
    for (int i = 0; i < nbPlayerBullets; i++)
    {
        if (mPlayerBullet[i].active)
        {
            // Update bullet's position based on velocity
            mPlayerBullet[i].x += mPlayerBullet[i].velocity_x;
            mPlayerBullet[i].y += mPlayerBullet[i].velocity_y;

            // Update rect to reflect current position
            mPlayerBullet[i].rect.x = mPlayerBullet[i].x;
            mPlayerBullet[i].rect.y = mPlayerBullet[i].y;
        }
    }
}


// Invaders class
class Invader {
public:
    int x, y;
    int width, height;
    bool destroyed;

    void update()
    {
        if (!destroyed)
        {
            // Update block state (e.g. check for destruction)
            // Check if the ball collides with the block
            for (int i = 0; i < nbPlayerBullets; i++)
            {
                if (mPlayerBullet[i].active)
                {
                    if (mPlayerBullet[i].x > x
                        && mPlayerBullet[i].x < x + width
                        && mPlayerBullet[i].y > y
                        && mPlayerBullet[i].y < y + height)
                    {
                        // If so, mark the block as destroyed
                        destroyed = true;
                        // rearrange list of player bullets
                        for (int j = 0; j < nbPlayerBullets; j++)
                        {
                            if (mPlayerBullet[j].active == 0)
                            {
                                j--;
                                mPlayerBullet[i].x = mPlayerBullet[j].x;
                                mPlayerBullet[i].y = mPlayerBullet[j].y;
                                mPlayerBullet[i].width = mPlayerBullet[j].width;
                                mPlayerBullet[i].height = mPlayerBullet[j].height;
                                mPlayerBullet[i].velocity_x = mPlayerBullet[j].velocity_x;
                                mPlayerBullet[i].velocity_y = mPlayerBullet[j].velocity_y;
                                mPlayerBullet[i].active = mPlayerBullet[j].active;
                                mPlayerBullet[j].active = 0;
                                break;
                            }
                        }
                        i--;
                    }
                }
            }
        }
    }
};
std::vector<Invader> vInvaders;


// Invaders bullets
struct InvaderBullet
{
    int x; // X-coordinate of the bullet
    int y; // Y-coordinate of the bullet
    int width; // Width of the bullet
    int height; // Height of the bullet
    bool active;
    SDL_Texture* texture; // Texture for the bullet
    SDL_Rect rect; // SDL_Rect representing the position and size of the bullet
};

int InvadersBullets_velocity = 1;
const int nbInvadersBullets = 10;
InvaderBullet mInvaderBullet[nbInvadersBullets];


// Update the bullet's position based on game logic
void fn_vInvadersBullets_Update()
{
    for (int i = 0; i < nbInvadersBullets; i++)
    {
        // Update bullet's position based on velocity
        mInvaderBullet[i].y += InvadersBullets_velocity;

        // Update rect to reflect current position
        mInvaderBullet[i].rect.x = mInvaderBullet[i].x;
        mInvaderBullet[i].rect.y = mInvaderBullet[i].y;

    }
}




void fn_vInitObjectsSizeOnScreen()
{
    // Create the paddle
    mPlayer.x = game_screen_width / 2;
    mPlayer.y = game_screen_height - game_screen_height / 9;
    mPlayer.width = game_screen_width / 12;
    mPlayer.height = game_screen_height / 24;
    mPlayer.velocity = 0;

    // Create the player bullets
    for (int i = 0; i < nbPlayerBullets; i++)
    {
        mPlayerBullet[i].active = 0;
        mPlayerBullet[i].x = game_screen_width / 2;
        mPlayerBullet[i].y = game_screen_height - game_screen_height / 9;
        mPlayerBullet[i].width = game_screen_width / 96;
        mPlayerBullet[i].height = game_screen_height / 48;
        mPlayerBullet[i].velocity_x = 0;
        mPlayerBullet[i].velocity_y = -5;
    }

    // Create the invaders
    for (int i = 1; i < 11; i++) {
        for (int j = 2; j < 6; j++) {
            Invader invader;
            invader.x = i * (game_screen_width / 12);
            invader.y = j * (game_screen_height / 18);
            invader.width = (game_screen_width / 18);
            invader.height = (game_screen_height / 24);
            invader.destroyed = false;
            vInvaders.push_back(invader);
        }
    }
}

void fn_vUpdateObjectsSizeOnScreen()
{
    // update the Paddle size on screen
    mPlayer.x = game_screen_width / 2;
    mPlayer.y = game_screen_height - game_screen_height / 9;
    mPlayer.width = game_screen_width / 12;
    mPlayer.height = game_screen_height / 24;
    //mPlayer.velocity = 0;

    // update the Player Bullets
    for (int i = 0; i < nbPlayerBullets; i++)
    {
        //        mPlayerBullet[i].active = 0;
        mPlayerBullet[i].x = game_screen_width / 2;
        mPlayerBullet[i].y = game_screen_height - game_screen_height / 9;
        mPlayerBullet[i].width = game_screen_width / 96;
        mPlayerBullet[i].height = game_screen_height / 48;
        mPlayerBullet[i].velocity_x = 0;
        mPlayerBullet[i].velocity_y = -5;
    }

    // update the invaders size on screen
    int k = 0;
    for (int i = 1; i < 11; i++) {
        for (int j = 2; j < 6; j++) {
            vInvaders[k].x = i * (game_screen_width / 12);
            vInvaders[k].y = j * (game_screen_height / 18);
            vInvaders[k].width = (game_screen_width / 18);
            vInvaders[k].height = (game_screen_height / 24);
            k++;
        }
    }
}
