


#include "sdl.h"
#include "SDL_render.h"


// Function to convert SDL_Color to Uint32 with SDL_PIXELFORMAT_RGBA8888
Uint32 SDL_ColorToRGBA8888(SDL_Color color, SDL_PixelFormat* format)
{
	return SDL_MapRGBA(format, color.r, color.g, color.b, color.a);
}

SDL_PixelFormat* ConvertToPixelFormat(Uint32 pixelFormatEnum)
{
	SDL_PixelFormat* format = SDL_AllocFormat(pixelFormatEnum);
	if (format == NULL)
	{
		SDL_Log("Failed to allocate pixel format: %s", SDL_GetError());
		return NULL;
	}
	return format;
}