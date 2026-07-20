#include <basilisk-mod.h>

#include <windows.h>
#include <stdio.h>

volatile long has_performed_tracked_changes = 1;

static void basilisk_loadScene() {

}


static DWORD WINAPI _bsmod_tickAsync(void* param) {
	while (1) {
		InterlockedExchange(&has_performed_tracked_changes, 0);
		bsmod_onTrack();
		InterlockedExchange(&has_performed_tracked_changes, 1);
		Sleep(1000);
	}
}

static void basilisk_ini() {
	_bsmod_onIni();

	bsmod_onTrack();
	CreateThread(NULL, 0, _bsmod_tickAsync, NULL, 0, NULL);

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