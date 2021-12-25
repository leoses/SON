//#include <iostream>
//#include <conio.h>
//#include <stdio.h>
//#include <ctime>
//#include <string>
//#include <windows.h>
//#include <chrono>

#include "Hoja3.h"


//void ejemploClase(System* system) {
//	FMOD_RESULT result;
//	Sound* sound1;
//	const char* path = "assets\\Battle.wav";
//	result = system->createSound(
//		path, // path al archivo de sonido
//		FMOD_DEFAULT, // valores (por defecto en este caso: sin loop, 2D)
//		0, // informacion adicional (nada en este caso)
//		&sound1); // handle al buffer de sonido
//	ERRCHECK(result);
//
//	Channel* channel;
//	result = system->playSound(
//		sound1, // buffer que se "engancha" a ese canal
//		0, // grupo de canales, 0 sin agrupar (agrupado en el master)
//		false, // arranca sin "pause" (se reproduce directamente)
//		&channel); // devuelve el canal que asigna
//		// el sonido ya se esta reproduciendo
//	ERRCHECK(result);
//
//
//	while (true) {
//		result = system->update();
//		ERRCHECK(result);
//	}
//
//}

/*

#pragma region HOJA_3

void hoja3Ejercicio1() {
	System* system = initFMOD();

	FMOD_RESULT result;
	Sound* sound1;
	Sound* sound2;
	Sound* sound3;

	ERRCHECK(system->createSound("assets\\Battle.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sound1));
	ERRCHECK(system->createSound("assets\\Gun1.wav", FMOD_3D, 0, &sound2));
	ERRCHECK(system->createSound("assets\\Gun2.wav", FMOD_3D, 0, &sound3));

	Channel* channel;
	ERRCHECK(system->playSound(sound1, 0, false, &channel));

	Channel* channel2;
	ERRCHECK(system->playSound(sound2, 0, true, &channel2));
	ERRCHECK(channel2->setPosition(rand() % 500, FMOD_TIMEUNIT_MS));
	ERRCHECK(channel2->setPaused(false));

	Channel* channel3;
	ERRCHECK(system->playSound(sound3, 0, true, &channel3));
	ERRCHECK(channel3->setPosition(rand() % 500, FMOD_TIMEUNIT_MS));

	while (true) {
		ERRCHECK(system->update());
	}

	ERRCHECK(sound1->release());
	ERRCHECK(sound2->release());
	ERRCHECK(sound3->release());

}

void hoja3Ejercicio2() {
	System* system = initFMOD();
	Sound* sound1;
	ERRCHECK(system->createSound("assets\\motor.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sound1));

	Channel* channel;
	ERRCHECK(system->playSound(sound1, 0, false, &channel));

	while (true) {
		if (_kbhit()) {
			int key = _getch();
			if ((key == 'Q') || (key == 'q')) break;
			else if ((key == 'A') || (key == 'a')) {
				float p;
				ERRCHECK(channel->getPitch(&p));
				std::cout << "Pitch: " << p << "\n";
				ERRCHECK(channel->setPitch(p + 0.1f));
			}
			else if (key == 'S' || key == 's') {
				float p;
				ERRCHECK(channel->getPitch(&p));
				std::cout << "Pitch: " << p << "\n";
				if (p - 0.1 > 0)ERRCHECK(channel->setPitch(p - 0.1f));
			}

			ERRCHECK(system->update());
		}
	}

	ERRCHECK(sound1->release());
	ERRCHECK(system->release());
}

void hoja3Ejercicio3() {
	System* system = initFMOD();
	Sound* sound1;
	FMOD_VECTOR pos = { 19,0,0 }, // posicion
		vel = { 0,0,0 }; // velocidad
	ERRCHECK(system->createSound("assets\\footstep_edit.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sound1));

	Channel* channel;
	ERRCHECK(system->playSound(sound1, 0, true, &channel));
	ERRCHECK(channel->set3DAttributes(&pos, &vel));

	FMOD_VECTOR listenerPos = { 0,0,0 }, // posicion del listener
		listenerVel = { 0,0,0 }, // velocidad del listener
		up = { 0,1,0 }, // vector up: hacia la ``coronilla''
		at = { 1,0,0 }; // vector at: hacia donde mira
	// colocamos listener
	system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at);

	//Despausamos el canal
	ERRCHECK(channel->setPaused(false));

	while (true) {
		if (_kbhit()) {
			int key = _getch();
			if ((key == 'Q') || (key == 'q')) break;
			else {
				FMOD_VECTOR actPos, actVel;
				ERRCHECK(channel->get3DAttributes(&actPos, &actVel));

				if ((key == 'A') || (key == 'a')) {
					actPos.x -= 0.1;
				}
				if (key == 'D' || key == 'd') {
					actPos.x += 0.1;
				}
				if (key == 'S' || key == 's') {
					actPos.z -= 0.1;
				}
				if (key == 'W' || key == 'w') {
					actPos.z += 0.1;
				}

				std::cout << "Posicion actual de la fuente, x: " << actPos.x
					<< " y: " << actPos.y << " z: " << actPos.z << "\n";

				ERRCHECK(channel->set3DAttributes(&actPos, &actVel));
			}

			ERRCHECK(system->update());
		}
	}

	ERRCHECK(sound1->release());
	ERRCHECK(system->release());
}

void hoja3Ejercicio4() {
	System* system = initFMOD();
	const int MAX_ELEMS = 7;
	Sound* sounds[MAX_ELEMS];

	Channel* channels[MAX_ELEMS];
	for (int i = 0; i < MAX_ELEMS; i++) {
		std::string path = "assets\\shoots\\0";
		path = path.append(std::to_string(i + 1) + ".wav" );

		std::cout << path << "\n";
		ERRCHECK(system->createSound(path.c_str(), FMOD_3D, 0, &sounds[i]));
		ERRCHECK(system->playSound(sounds[i], 0, true, &channels[i]));
	}
	time_t lastTime = time(nullptr);


	while (true) {
		time_t currTime = time(nullptr);
		if (_kbhit()) {
			int key = _getch();
			if ((key == 'Q') || (key == 'q')) break;
		}
		else if (lastTime + 1000 < currTime) {
			channels[rand() % MAX_ELEMS]->setPaused(false);
			lastTime = currTime;
		}
	}
}

void hoja3Ejercicio5() {
	System* system = initFMOD();
	Sound* sound1;
	ERRCHECK(system->createSound("assets\\piano.ogg", FMOD_3D, 0, &sound1));

	Channel* channel;
	ERRCHECK(system->playSound(sound1, 0, true, &channel));
	char teclas[] = { 'Z', 'S', 'X','D','C','V', 'G', 'B','H' ,'N', 'J', 'M', 'Q','2','W','3','E','R','5','T','6','Y','7','U','I' };
	const int MAX_TECLAS = 25;
	int octava = 0;
	while (true) {
		if (_kbhit()) {
			int key = _getch();
			float pitch = 1.0f;

			int i = 0;
			for (; i < MAX_TECLAS; i++) {
				if (std::toupper(key) == teclas[i]) {
					pitch = (octava * 3.0f) + (pow(2.0f, (float)i / 12.0f));
					break;
				}
			}

			if (i < MAX_TECLAS) {
				std::cout << "Pitch actual: " << pitch << "\n";
				ERRCHECK(system->playSound(sound1, 0, true, &channel));
				ERRCHECK(channel->setPitch(pitch));
				ERRCHECK(channel->setPaused(false));
				ERRCHECK(system->update());
			}
			else {
				if (std::toupper(key) == 'P') {
					octava++;
					std::cout << "Octava actual: " << octava << "\n";
				}
				else if (std::toupper(key) == 'O' && octava > 0) {
					octava--;
					std::cout << "Octava actual: " << octava << "\n";
				}
			}

		}
	}


	ERRCHECK(sound1->release());
	ERRCHECK(system->release());
}

void clearScreen(char fill = ' ') {
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void renderEj6(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, Channel* channel) {
	//Clear console
	clearScreen();
	//Renderizado del estado actual del "juego"
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((listenerPos->x == j && listenerPos->y == i)) {
				//Es Listener
				std::cout << "L";
			}
			else if (pos->x == j && pos->y == i) {
				//es source
				std::cout << "S";
			}
			else {
				//es vacio
				std::cout << ".";
			}
		}
		std::cout << "\n";
	}

	float volume; channel->getVolume(&volume);
	float inside, outside, outsideVolume; channel->get3DConeSettings(&inside, &outside, &outsideVolume);
	float minDist, maxDist; channel->get3DMinMaxDistance(&minDist, &maxDist);
	std::cout << "Cone Out:" << outside << "   In:" << inside << "   Outer gain:" << outsideVolume << "    Music vol :" << volume << "\n";
	std::cout << "MinDist:" <<minDist << "   MaxDist:" << maxDist << "\n";

}

void hoja3Ejercicio6() {

	const int dimensions = 20;

	System* system = initFMOD();
	Sound* sound1;
	
	//POSICION INICIAL SOURCE
	FMOD_VECTOR pos = { dimensions/2,dimensions /2,0 }, // posicion
		vel = { 0,0,0 }; // velocidad
	ERRCHECK(system->createSound("assets\\scooter.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sound1));

	Channel* channel;
	ERRCHECK(system->playSound(sound1, 0, true, &channel));
	ERRCHECK(channel->set3DAttributes(&pos, &vel));
	//atenuacion
	float minDistance = 1.0f, maxDistance = 100.0f;
	ERRCHECK(channel->set3DMinMaxDistance(minDistance, maxDistance));
	//Quitamos la pausa
	ERRCHECK(channel->setPaused(false));
	
	//Posicion inincial listener
	FMOD_VECTOR listenerPos = { dimensions/2,dimensions/2 + dimensions/4,0 }, // posicion del listener
		listenerVel = { 0,0,0 }, // velocidad del listener
		up = { 0,0,1 }, // vector up: hacia la ``coronilla''
		at = { 0,1,0 }; // vector at: hacia donde mira
	
	// colocamos listener
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));

	//Renderizado inicial del estado, solo lo updateamos cuando pulsamos una tecla que cambia la posicion
	//o del listener o del source
	renderEj6(dimensions, &listenerPos, &pos, channel);

	constexpr float VARIATION =  0.1f;

	while (true) {

		//Gestion de input para mover al listener
		if (_kbhit()) {
			int key = _getch();

			float volume; ERRCHECK(channel->getVolume(&volume));
			float inside, outside, outsideVolume; ERRCHECK(channel->get3DConeSettings(&inside, &outside, &outsideVolume));
			float minDist, maxDist; ERRCHECK(channel->get3DMinMaxDistance(&minDist, &maxDist));

			bool teclaImportante = true;
			//Movimiento del listener
			if (std::toupper(key) == 'W' && listenerPos.y > 0) listenerPos.y -= 1;
			else if (std::toupper(key) == 'S' && listenerPos.y < dimensions - 1) listenerPos.y += 1;
			else if (std::toupper(key) == 'A' && listenerPos.x > 0) listenerPos.x -= 1;
			else if (std::toupper(key) == 'D' && listenerPos.x < dimensions - 1) listenerPos.x += 1;
			else if (std::toupper(key) == 'Z' && volume > 0) channel->setVolume(volume - 0.1);
			//Volumen del source
			else if (std::toupper(key) == 'X') channel->setVolume(volume + VARIATION);
			else if (std::toupper(key) == 'Z' && volume - VARIATION > 0) channel->setVolume(volume - VARIATION);
			//InnerCone
			else if (std::toupper(key) == 'C' && inside - VARIATION > 0) inside -= VARIATION;
			else if (std::toupper(key) == 'V' && inside + VARIATION < outside) inside += VARIATION;
			//OutsideCone
			else if (std::toupper(key) == 'Q' && outside - VARIATION > inside && outside - VARIATION > 0)outside -= VARIATION;
			else if (std::toupper(key) == 'E' && outside < 360.0f) outside += VARIATION;
			//OutsideVolume
			else if (std::toupper(key) == 'R' && outsideVolume - VARIATION > 0)outsideVolume -= VARIATION;
			else if (std::toupper(key) == 'T') outsideVolume += VARIATION;
			//MinMaxDistance
			else if (std::toupper(key) == 'F' && minDist - VARIATION > 0)minDist -= VARIATION;
			else if (std::toupper(key) == 'G' && minDist + VARIATION < maxDist) minDist += VARIATION;
			else if (std::toupper(key) == 'B' && maxDist - VARIATION > minDist)maxDist -= VARIATION;
			else if (std::toupper(key) == 'N') maxDist += VARIATION;

			else teclaImportante = false;

			if (teclaImportante) {
				ERRCHECK(channel->set3DMinMaxDistance(minDist, maxDist));
				ERRCHECK(channel->set3DConeSettings(inside, outside, outsideVolume));
				renderEj6(dimensions, &listenerPos, &pos, channel);
				system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at);
			}
	
		}

		//Updateamos el sistema de FMOD
		ERRCHECK(system->update());

	}


	ERRCHECK(sound1->release());
	ERRCHECK(system->release());
}
#pragma endregion

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

	//hoja4Ejercicio1();
	Hoja3* ejercicios = new Hoja3();
	ejercicios->ejercicio_03();
	delete ejercicios;
	return 0;
}