#include "Hoja3.h"

/*
#pragma region HOJA_4
void hoja4Ejercicio1() {
	System* system = initFMOD();
	Sound* s1;
	ERRCHECK(system->createSound("assets\\scooter.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &s1));
	StereoSound* stereoSound = new StereoSound(system, "assets\\stereo\\footstep.wav", FMOD_3D | FMOD_LOOP_NORMAL);
	stereoSound->playSound();
	
	bool bucle = true;
	auto t_start = std::chrono::high_resolution_clock::now();
	while (bucle) {
		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		if (_kbhit()) {
			int key = _getch();
			if (std::toupper(key) == 'E') bucle = false;
			else if (std::toupper(key) == 'W') stereoSound->fadeOut(5000, 0.2); //in millsecs
			else if (std::toupper(key) == 'Q') stereoSound->fadeIn(5000, 1.0); //in millsecs
			else if (std::toupper(key) == 'R') stereoSound->mixSounds(system, s1, 10000); //in millsecs
			else if (std::toupper(key) == 'A') stereoSound->fadeInFancy(5); //in secs
			else if (std::toupper(key) == 'S') stereoSound->fadeOutFancy(5);//in secs
		}

		stereoSound->update(elapsed_time_ms);
		system->update();
		t_start = t_end;
	}

	delete stereoSound;
	ERRCHECK(system->release());
}
#pragma endregion
*/

int main(int argc, char* argv[]) {

	Hoja3* ejercicios = new Hoja3();
	ejercicios->ejercicio_06();
	delete ejercicios;
	return 0;
}