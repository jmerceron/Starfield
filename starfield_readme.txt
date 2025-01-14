Tested with SDL2
	SDL2-2.30.11
	2025-01-02
Tested with SDL_TTF
	SDL2_ttf-devel-2.24.1-VC.zip
	2025-01-02 17:09	11M	 


SDL: Copy x86 libs in lib directory
	Add include path
	Add Lib path
SDL_TTF: Copy x86 libs in lib directory
	Add include path
	Add Lib path


Get font fonts.google.com.
Choose a font (e.g., "Roboto", "Open Sans").
Download the font files you like (usually .ttf or .otf).
Put them in the opath of your choice, and update the path of the .ttf file with TTF_OpenFont.By default, we have:



First, add also SDL directory to DEVSPACE
(now SDL.h will be found)

Second, add SDL2.dll to Windows/system directory
(now dll linking post compilation won't crash)

Third, add also SDL_TTF directory to DEVSPACE
(now SDL_ttf.h will be found)

Fourth, add SDL2_ttf.dll to Windows/system directory
(now dll linking post compilation won't crash)






