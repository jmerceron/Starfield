

#include "sdl.h"
#include "SDL_render.h"

#include <iostream>
#include <windows.h>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <cstdint>

#include "conversion.h"
#include "Explosion.h"


/*particle structure*/
typedef struct
{
	Uint16 xpos, ypos, xdir, ydir;
	Uint8 colorindex;
	Uint8 dead;
} PARTICLE;

#define NUMBER_OF_PARTICLES 500
#define EXPLOSION_WIDTH 480
#define EXPLOSION_HEIGHT 360

static PARTICLE particles[NUMBER_OF_PARTICLES];
static Uint8 explosion[EXPLOSION_WIDTH * EXPLOSION_HEIGHT];
static SDL_Color explosion_to_texture[EXPLOSION_WIDTH * EXPLOSION_HEIGHT];
static SDL_Color colors_explosion[256];
static SDL_Texture* explosion_texture;


void fn_vInit_particle(PARTICLE* particle)
{
	/* randomly init particles, generate them in the center of the screen */

	particle->xpos = (EXPLOSION_WIDTH >> 1) - 20 + (int)(40.0 * (rand() / (RAND_MAX + 1.0)));
	particle->ypos = (EXPLOSION_HEIGHT >> 1) - 20 + (int)(40.0 * (rand() / (RAND_MAX + 1.0)));
	particle->xdir = -10 + (int)(20.0 * (rand() / (RAND_MAX + 1.0)));
	particle->ydir = -17 + (int)(19.0 * (rand() / (RAND_MAX + 1.0)));
	particle->colorindex = 255;
	particle->dead = 0;
}


void fn_vRandomly_Init_Particles()
{
	int i;

	/* randomly init particles, generate them in the center of the screen */

	for (i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		fn_vInit_particle(particles + i);
	}

}



void fn_vInit_Explosion(SDL_Renderer* renderer)
{
	Uint32 i;

	/* create a suitable fire palette, this is crucial for a good effect */
	/* black to blue, blue to red, red to yellow, yellow to white*/
	for (i = 0; i < 32; ++i)
	{
		/* black to blue, 32 values*/
		colors_explosion[i].b = i << 1;

		/* blue to red, 32 values*/
		colors_explosion[i + 32].r = i << 3;
		colors_explosion[i + 32].b = 64 - (i << 1);

		/*red to yellow, 32 values*/
		colors_explosion[i + 64].r = 255;
		colors_explosion[i + 64].g = i << 3;

		/* yellow to white, 162 */
		colors_explosion[i + 96].r = 255;
		colors_explosion[i + 96].g = 255;
		colors_explosion[i + 96].b = i << 2;
		colors_explosion[i + 128].r = 255;
		colors_explosion[i + 128].g = 255;
		colors_explosion[i + 128].b = 64 + (i << 2);
		colors_explosion[i + 160].r = 255;
		colors_explosion[i + 160].g = 255;
		colors_explosion[i + 160].b = 128 + (i << 2);
		colors_explosion[i + 192].r = 255;
		colors_explosion[i + 192].g = 255;
		colors_explosion[i + 192].b = 192 + i;
		colors_explosion[i + 224].r = 255;
		colors_explosion[i + 224].g = 255;
		colors_explosion[i + 224].b = 224 + i;
	}


	// init particles
	fn_vRandomly_Init_Particles();

	// create texture to render Plasma effect
	explosion_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, EXPLOSION_WIDTH, EXPLOSION_HEIGHT);


}


int fm_iRun_Explosion_RendererStyle(SDL_Renderer* renderer)
{


	Uint32 buf, index, temp;
	int i, j;

	
	/* move and draw particles into fire array */
	for (i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		if (!particles[i].dead)
		{
			particles[i].xpos += particles[i].xdir;
			particles[i].ypos += particles[i].ydir;

			/* is particle dead? */

			if ((particles[i].ypos >= EXPLOSION_HEIGHT - 3) || particles[i].colorindex == 0 ||
				particles[i].xpos <= 1 || particles[i].xpos >= EXPLOSION_WIDTH - 3)
			{
				particles[i].dead = 1;
				continue;
			}

			/* gravity takes over */

			particles[i].ydir++;

			/* particle cools off */

			particles[i].colorindex--;

			/* draw particle */

			temp = particles[i].ypos * EXPLOSION_WIDTH + particles[i].xpos;

			explosion[temp] = particles[i].colorindex;
			explosion[temp - 1] = particles[i].colorindex;
			explosion[temp + EXPLOSION_WIDTH] = particles[i].colorindex;
			explosion[temp - EXPLOSION_WIDTH] = particles[i].colorindex;
			explosion[temp + 1] = particles[i].colorindex;
		}
	}

	/* create fire effect */
	for (i = 1; i < EXPLOSION_HEIGHT - 2; i++)
	{
		index = (i - 1) * EXPLOSION_WIDTH;

		for (j = 1; j < EXPLOSION_WIDTH - 2; j++)
		{
			buf = index + j;

			temp = explosion[buf];
			temp += explosion[buf + 1];
			temp += explosion[buf - 1];
			buf += EXPLOSION_WIDTH;
			temp += explosion[buf - 1];
			temp += explosion[buf + 1];
			buf += EXPLOSION_WIDTH;
			temp += explosion[buf];
			temp += explosion[buf + 1];
			temp += explosion[buf - 1];

			temp >>= 3;

			if (temp > 4)
			{
				temp -= 4;
			}
			else
			{
				temp = 0;
			}

			explosion[buf - EXPLOSION_WIDTH] = temp;
		}
	}

	// get colors in !
	// TO_DO_LIST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// convert color index into color
//	static Uint8 explosion[EXPLOSION_WIDTH * EXPLOSION_HEIGHT];
//	static SDL_Color explosion_to_texture[EXPLOSION_WIDTH * EXPLOSION_HEIGHT];
	SDL_Color red = { 255, 0, 0, 255 }; // Full red, no green, no blue, fully opaque
	for (i = 0; i<EXPLOSION_HEIGHT; i++)
	{
		temp = i * EXPLOSION_WIDTH;
		for (j = 0; j<EXPLOSION_WIDTH; j++)
		{
//			explosion_to_texture[temp + j] = red;
			explosion_to_texture[temp + j] = colors_explosion[explosion[temp + j]];
		}
	}



	// start copy in texture
	void* pixels;
	int pitch;
	SDL_LockTexture(explosion_texture, NULL, &pixels, &pitch);

	// Convert SDL_PIXELFORMAT_RGBA8888 to SDL_PixelFormat*
	SDL_PixelFormat* format = ConvertToPixelFormat(SDL_PIXELFORMAT_RGBA8888);

	Uint32* image, * image_start;
	image = (Uint32*)pixels;
	image_start = (Uint32*)pixels;

	/* draw fire array to screen from bottom to top*/
	for (i = 0 ; i < EXPLOSION_HEIGHT ; i++)
	{
		temp = i * EXPLOSION_WIDTH;
		for (j = 0 ; j < EXPLOSION_WIDTH ; j++)
		{
			*image = SDL_ColorToRGBA8888(explosion_to_texture[temp + j], format);
			image++;
		}
	}


	// unlock texture, we finished updating it
	SDL_UnlockTexture(explosion_texture);

	// Render the texture to the specified rectangle
	SDL_RenderCopy(renderer, explosion_texture, NULL, NULL);

	return 0;
}





int fm_iRun_Explosion_RendererStyle_XY(SDL_Renderer* renderer, int ix, int iy, int iwidth, int iheight)
{


	Uint32 buf, index, temp;
	int i, j;


	/* move and draw particles into fire array */
	for (i = 0; i < NUMBER_OF_PARTICLES; i++)
	{
		if (!particles[i].dead)
		{
			particles[i].xpos += particles[i].xdir;
			particles[i].ypos += particles[i].ydir;

			/* is particle dead? */

			if ((particles[i].ypos >= EXPLOSION_HEIGHT - 3) || particles[i].colorindex == 0 ||
				particles[i].xpos <= 1 || particles[i].xpos >= EXPLOSION_WIDTH - 3)
			{
				particles[i].dead = 1;
				continue;
			}

			/* gravity takes over */

			particles[i].ydir++;

			/* particle cools off */

			particles[i].colorindex--;

			/* draw particle */

			temp = particles[i].ypos * EXPLOSION_WIDTH + particles[i].xpos;

			explosion[temp] = particles[i].colorindex;
			explosion[temp - 1] = particles[i].colorindex;
			explosion[temp + EXPLOSION_WIDTH] = particles[i].colorindex;
			explosion[temp - EXPLOSION_WIDTH] = particles[i].colorindex;
			explosion[temp + 1] = particles[i].colorindex;
		}
	}

	/* create fire effect */
	for (i = 1; i < EXPLOSION_HEIGHT - 2; i++)
	{
		index = (i - 1) * EXPLOSION_WIDTH;

		for (j = 1; j < EXPLOSION_WIDTH - 2; j++)
		{
			buf = index + j;

			temp = explosion[buf];
			temp += explosion[buf + 1];
			temp += explosion[buf - 1];
			buf += EXPLOSION_WIDTH;
			temp += explosion[buf - 1];
			temp += explosion[buf + 1];
			buf += EXPLOSION_WIDTH;
			temp += explosion[buf];
			temp += explosion[buf + 1];
			temp += explosion[buf - 1];

			temp >>= 3;

			if (temp > 4)
			{
				temp -= 4;
			}
			else
			{
				temp = 0;
			}

			explosion[buf - EXPLOSION_WIDTH] = temp;
		}
	}

	// convert color index into color
	SDL_Color red = { 255, 0, 0, 255 }; // Full red, no green, no blue, fully opaque
	for (i = 0; i < EXPLOSION_HEIGHT; i++)
	{
		temp = i * EXPLOSION_WIDTH;
		for (j = 0; j < EXPLOSION_WIDTH; j++)
		{
			//			explosion_to_texture[temp + j] = red;
			explosion_to_texture[temp + j] = colors_explosion[explosion[temp + j]];
		}
	}



	// start copy in texture
	void* pixels;
	int pitch;
	SDL_LockTexture(explosion_texture, NULL, &pixels, &pitch);

	// Convert SDL_PIXELFORMAT_RGBA8888 to SDL_PixelFormat*
	SDL_PixelFormat* format = ConvertToPixelFormat(SDL_PIXELFORMAT_RGBA8888);

	Uint32* image, * image_start;
	image = (Uint32*)pixels;
	image_start = (Uint32*)pixels;

	/* draw explosion array to screen from top to bottom*/
	for (i = 0; i < EXPLOSION_HEIGHT; i++)
	{
		temp = i * EXPLOSION_WIDTH;
		for (j = 0; j < EXPLOSION_WIDTH; j++)
		{
			*image = SDL_ColorToRGBA8888(explosion_to_texture[temp + j], format);
			image++;
		}
	}


	// unlock texture, we finished updating it
	SDL_UnlockTexture(explosion_texture);


	// Define the destination rectangle on the screen
	SDL_Rect destRect;
	destRect.x = ix;
	destRect.y = iy;
	destRect.w = iwidth;
	destRect.h = iheight;
	// Render the texture to the specified rectangle
	SDL_RenderCopy(renderer, explosion_texture, NULL, &destRect);

	return 0;
}


