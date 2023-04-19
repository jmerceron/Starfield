#pragma once




static Uint8 fire[SCREEN_WIDTH * SCREEN_HEIGHT];
static SDL_Color colors[256];


void fn_vDemoScene_ColorPalette_Fire_init()
{
    int i;

    /* create a suitable fire palette, this is crucial for a good effect */
    /* black to blue, blue to red, red to yellow, yellow to white*/

    for (i = 0; i < 32; ++i)
    {
        /* black to blue, 32 values*/
        colors[i].b = i << 1;

        /* blue to red, 32 values*/
        colors[i + 32].r = i << 3;
        colors[i + 32].b = 64 - (i << 1);

        /*red to yellow, 32 values*/
        colors[i + 64].r = 255;
        colors[i + 64].g = i << 3;

        /* yellow to white, 162 */
        colors[i + 96].r = 255;
        colors[i + 96].g = 255;
        colors[i + 96].b = i << 2;
        colors[i + 128].r = 255;
        colors[i + 128].g = 255;
        colors[i + 128].b = 64 + (i << 2);
        colors[i + 160].r = 255;
        colors[i + 160].g = 255;
        colors[i + 160].b = 128 + (i << 2);
        colors[i + 192].r = 255;
        colors[i + 192].g = 255;
        colors[i + 192].b = 192 + i;
        colors[i + 224].r = 255;
        colors[i + 224].g = 255;
        colors[i + 224].b = 224 + i;
    }
}


void fn_vDemoScene_Fire_Render(SDL_Renderer* renderer)
{
    //    Uint8* image;
    Uint16 temp;
    Uint8 index;
    int i, j;

    //    SDL_SetPalette(renderer, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);


        /* draw random bottom line in fire array*/

    j = SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
    for (i = 0; i < SCREEN_WIDTH - 1; i++)
    {
        int random = 1 + (int)(16.0 * (rand() / (RAND_MAX + 1.0)));
        if (random > 9) /* the lower the value, the intenser the fire, compensate a lower value with a higher decay value*/
            fire[j + i] = 255; /*maximum heat*/
        else
            fire[j + i] = 0;
    }

    /* move fire upwards, start at bottom*/

    for (index = 0; index < 60; ++index)
    {
        for (i = 0; i < SCREEN_WIDTH - 1; ++i)
        {
            if (i == 0) /* at the left border*/
            {
                temp = fire[j];
                temp += fire[j + 1];
                temp += fire[j - SCREEN_WIDTH];
                temp /= 3;
            }
            else if (i == SCREEN_WIDTH - 1) /* at the right border*/
            {
                temp = fire[j + i];
                temp += fire[j - SCREEN_WIDTH + i];
                temp += fire[j + i - 1];
                temp /= 3;
            }
            else
            {
                temp = fire[j + i];
                temp += fire[j + i + 1];
                temp += fire[j + i - 1];
                temp += fire[j - SCREEN_WIDTH + i];
                temp >>= 2;
            }
            if (temp > 1)
                temp -= 1; /* decay */

            fire[j - SCREEN_WIDTH + i] = temp;
        }
        j -= SCREEN_WIDTH;
    }

    /*start in the right bottom corner*/
    /* draw fire array to screen from bottom to top + 300*/

    for (i = SCREEN_HEIGHT - 3; i >= 300; --i)
    {
        for (j = SCREEN_WIDTH - 1; j >= 0; --j)
        {
            //            SDL_Rect FirePixelRect = { j, i, 3, 3 };
            SDL_Rect FirePixelRect = { game_screen_width - 3 * j,
                                        game_screen_height + i - (SCREEN_HEIGHT),
                                        3, 3 };
            SDL_SetRenderDrawColor(renderer, colors[fire[i * SCREEN_WIDTH + j]].r,
                colors[fire[i * SCREEN_WIDTH + j]].g,
                colors[fire[i * SCREEN_WIDTH + j]].b, 255);
            SDL_RenderFillRect(renderer, &FirePixelRect);

            /*game_screen_width game_screen_height*/
            /*SCREEN_WIDTH SCREEN_HEIGHT*/
        }
    }
}



