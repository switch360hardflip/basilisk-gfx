#include <basilisk-mod.h>

#include <windows.h>
#include <stdio.h>

void bs_setupTrampoline();
int main(int argc, char* argv[]) {
	bs_setupTrampoline();

	bs_Atlas atlas = { .count = 1 };
	bs_atlasSize(&atlas, 0);

	bs_ini();

	bsgfx_ini("Basilisk", 1920, 1080, argc, argv);

	return 0;
}