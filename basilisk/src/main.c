#include <basilisk-mod.h>

#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	bs_enableValidation();
	bsgfx_enableValidation();
	bsmod_enableValidation();

	bs_ini();

	bs_Object* window_object = BS_WINDOW(BSGFX_WINDOWS, -1, 0);
	bs_window(window_object, 1024, 1024, "basilisk");

	bs_Atlas atlas = { .count = 1 };
	bs_atlasSize(&atlas, 0);

	bsgfx_ini("Basilisk", 1920, 1080, argc, argv);

	return 0;
}