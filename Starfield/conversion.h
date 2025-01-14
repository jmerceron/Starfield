#pragma once


Uint32 SDL_ColorToRGBA8888(SDL_Color color, SDL_PixelFormat* format);

SDL_PixelFormat* ConvertToPixelFormat(Uint32 pixelFormatEnum);
