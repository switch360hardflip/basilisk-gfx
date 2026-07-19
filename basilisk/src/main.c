#include <basilisk-mod.h>

#include <windows.h>
#include <stdio.h>

static void basilisk_loadScene() {

}

static void basilisk_ini() {
	bsmod_onTrack();

	bsgfx_loadScene("engine");
}

int main(int argc, char* argv[]) {
	bs_enableValidation();
	bsgfx_enableValidation();
	bsmod_enableValidation();

	bsgfx_Callbacks* callbacks = bsgfx_callbacks();
	*callbacks = (bsgfx_Callbacks) {
		.loadScene = basilisk_loadScene,
		.ini = basilisk_ini,
	};

	bsgfx_ini("Basilisk", 1920, 1080, argc, argv);

	return 0;
}