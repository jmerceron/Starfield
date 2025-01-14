#pragma once


#define INVADER_DESTRUCTION_MAX_COUNT   10
#define INVADER_DESTRUCTION_TIMER       60

struct stExplosion_location 
{
    int     x, y;
    int     width, height;
    int     timer;
    bool    bActive;
    bool    bIgnite;
};


stExplosion_location stExplosion_location_Elements[INVADER_DESTRUCTION_MAX_COUNT];


int iLast_ResolutionX = SCREEN_WIDTH;
int iLast_ResolutionY = SCREEN_HEIGHT;





///////////////////////////////////////
// Destruction Functions //////////////
///////////////////////////////////////

void fn_vDestruction_Init()
{
    for (int i = 0; i < INVADER_DESTRUCTION_MAX_COUNT; i++)
    {
        stExplosion_location_Elements[i].bActive = 0;
    }
}




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
                       

                        // add location for explosion FX !!!!!!!!!!!!!!!!!!!!!! TO_DO_LIST
                        for (int k = 0; k < INVADER_DESTRUCTION_MAX_COUNT; k++)
                        {
                            if (!stExplosion_location_Elements[k].bActive)
                            {
                                stExplosion_location_Elements[k].bActive = 1;
                                stExplosion_location_Elements[k].bIgnite = 1;
                                stExplosion_location_Elements[k].x = x;
                                stExplosion_location_Elements[k].y = y;
                                stExplosion_location_Elements[k].width = width;
                                stExplosion_location_Elements[k].height = height;
                                break;
                            }
                        }

                        
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
    // init base resolution
    iLast_ResolutionX = game_screen_width;
    iLast_ResolutionY = game_screen_height;
    
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

    // calculate ratio
    float fRatioX = ((float)game_screen_width / (float)iLast_ResolutionX);
    float fRatioY = ((float)game_screen_height / (float)iLast_ResolutionY);


    // update last resolution
    iLast_ResolutionX = game_screen_width;
    iLast_ResolutionY = game_screen_height;


    // update the Paddle size on screen
    mPlayer.x = (int)(mPlayer.x * fRatioX);
    mPlayer.y = (int)(mPlayer.y * fRatioY);
    mPlayer.width = game_screen_width / 12;
    mPlayer.height = game_screen_height / 24;
    //mPlayer.velocity = 0;

    // update the Player Bullets
    for (int i = 0; i < nbPlayerBullets; i++)
    {
//        mPlayerBullet[i].active = 0;
        mPlayerBullet[i].x = (int)(mPlayerBullet[i].x * fRatioX);
        mPlayerBullet[i].y = (int)(mPlayerBullet[i].y * fRatioY);
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



void fn_vUpdateBullets_spawn5()
{
    int iFirstAvailableSlot = 0;
    for (int i = 0; i < nbPlayerBullets; i++)
    {
        if (mPlayerBullet[i].active == 0)
        {
            iFirstAvailableSlot = i;
            break;
        }
    }
    int i = iFirstAvailableSlot;
    mPlayerBullet[i].active = 1;
    mPlayerBullet[i].x = mPlayer.x + mPlayer.width / 2;
    mPlayerBullet[i].y = mPlayer.y;
    mPlayerBullet[i].width = game_screen_width / 96;
    mPlayerBullet[i].height = game_screen_height / 48;
    mPlayerBullet[i].velocity_x = 0;
    mPlayerBullet[i].velocity_y = -5;
    mPlayerBullet[i + 1].active = 1;
    mPlayerBullet[i + 1].x = mPlayer.width / 2 + mPlayer.x + game_screen_width / 24;
    mPlayerBullet[i + 1].y = mPlayer.y;
    mPlayerBullet[i + 1].width = game_screen_width / 96;
    mPlayerBullet[i + 1].height = game_screen_height / 48;
    mPlayerBullet[i + 1].velocity_x = 0;
    mPlayerBullet[i + 1].velocity_y = -5;
    mPlayerBullet[i + 2].active = 1;
    mPlayerBullet[i + 2].x = mPlayer.width / 2 + mPlayer.x - game_screen_width / 24;
    mPlayerBullet[i + 2].y = mPlayer.y;
    mPlayerBullet[i + 2].width = game_screen_width / 96;
    mPlayerBullet[i + 2].height = game_screen_height / 48;
    mPlayerBullet[i + 2].velocity_x = 0;
    mPlayerBullet[i + 2].velocity_y = -5;
    mPlayerBullet[i + 3].active = 1;
    mPlayerBullet[i + 3].x = mPlayer.width / 2 + mPlayer.x + 2 * game_screen_width / 24;
    mPlayerBullet[i + 3].y = mPlayer.y;
    mPlayerBullet[i + 3].width = game_screen_width / 96;
    mPlayerBullet[i + 3].height = game_screen_height / 48;
    mPlayerBullet[i + 3].velocity_x = 0;
    mPlayerBullet[i + 3].velocity_y = -5;
    mPlayerBullet[i + 4].active = 1;
    mPlayerBullet[i + 4].x = mPlayer.width / 2 + mPlayer.x - 2 * game_screen_width / 24;
    mPlayerBullet[i + 4].y = mPlayer.y;
    mPlayerBullet[i + 4].width = game_screen_width / 96;
    mPlayerBullet[i + 4].height = game_screen_height / 48;
    mPlayerBullet[i + 4].velocity_x = 0;
    mPlayerBullet[i + 4].velocity_y = -5;
}


void fn_vUpdateBullets_spawn1(int locationX, int locationY)
{
    // create missile to the mouse position
    int iFirstAvailableSlot = 0;
    for (int i = 0; i < nbPlayerBullets; i++)
    {
        if (mPlayerBullet[i].active == 0)
        {
            iFirstAvailableSlot = i;
            break;
        }
    }
    int i = iFirstAvailableSlot;
    mPlayerBullet[i].active = 1;
    mPlayerBullet[i].x = locationX;
    mPlayerBullet[i].y = locationY;
    mPlayerBullet[i].width = game_screen_width / 96;
    mPlayerBullet[i].height = game_screen_height / 48;
    mPlayerBullet[i].velocity_x = 0;
    mPlayerBullet[i].velocity_y = -5;

}

