#include "Hoja3.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <windows.h>

void Hoja3::initFMOD(FMOD_INITFLAGS flags = FMOD_INIT_NORMAL)
{
	ERRCHECK(System_Create(&system)); // Creamos el objeto system
	// 128 canales (numero maximo que podremos utilizar simultaneamente)
	ERRCHECK(system->init(128, flags, 0)); // Inicializacion de FMOD
}

void Hoja3::setListener(FMOD_VECTOR listenerPos = { 0,0,0 }, FMOD_VECTOR listenerVel = { 0,0,0 },
	FMOD_VECTOR up = { 0,1,0 }, FMOD_VECTOR at = { 1,0,0 })
{
	// posicion del listener
	// velocidad del listener
	// vector up: hacia la ``coronilla''
	// vector at: hacia donde mira
	// colocamos listener
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));
}

void Hoja3::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

void Hoja3::clearScreen(char fill)
{
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void Hoja3::renderEj6(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, Channel* channel)
{
	//Clear console
	clearScreen();
	//Renderizado del estado actual del "juego"
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((listenerPos->x == j && listenerPos->y == i)) {
				//Es Listener
				std::cout << "L ";
			}
			else if (pos->x == j && pos->y == i) {
				//es source
				std::cout << "S ";
			}
			else {
				//es vacio
				std::cout << ". ";
			}
		}
		std::cout << "\n";
	}

	float volume; channel->getVolume(&volume);
	float inside, outside, outsideVolume; channel->get3DConeSettings(&inside, &outside, &outsideVolume);
	float minDist, maxDist; channel->get3DMinMaxDistance(&minDist, &maxDist);
	std::cout << "Cone Out:" << outside << "   In:" << inside << "   Outer gain:" << outsideVolume << "    Music vol :" << volume << "\n";
	std::cout << "MinDist:" << minDist << "   MaxDist:" << maxDist << "\n";

}

Hoja3::Hoja3() : system(nullptr)
{

}

Hoja3::~Hoja3()
{
}

void Hoja3::ejercicio_01()
{
	initFMOD();

	Sound* loopSound;
	Sound* gun1;
	Sound* gun2;

	ERRCHECK(system->createSound("assets\\Battle.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &loopSound));
	ERRCHECK(system->createSound("assets\\Gun1.wav", FMOD_3D, 0, &gun1));
	ERRCHECK(system->createSound("assets\\Gun2.wav", FMOD_3D, 0, &gun2));

	Channel* loopChannel;
	Channel* chn1;
	Channel* chn2;

	//Empezamos el loop de musica inmediatamente
	ERRCHECK(system->playSound(loopSound, 0, false, &loopChannel));
	//setteamos la posicion al listener (Por defecto en el 0,0 de todo, el eje y apunta a la conorilla y empieza mirando hacia
	// el eje x)
	setListener();

	auto t_start_1 = std::chrono::high_resolution_clock::now();
	auto t_start_2 = std::chrono::high_resolution_clock::now();


	int random1 = rand() % 5;
	int random2 = rand() % 5;
	//double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

	bool end = false;
	while (!end) {
		//Salir
		if (_kbhit()) {
			int key = _getch();
			if ((key == 'Q') || (key == 'q')) {
				end = true;
				continue;
			}
		}
		auto currTime = std::chrono::high_resolution_clock::now();

		if (random1 * 1000.0f <= std::chrono::duration<double, std::milli>(currTime - t_start_1).count()) {
			ERRCHECK(system->playSound(gun1, 0, true, &chn1));
			FMOD_VECTOR pos = { (-(rand() % 10)), 0 , (10 - (rand() % 20)) };
			FMOD_VECTOR vel = { 1,0,0 };
			ERRCHECK(chn1->set3DAttributes(&pos, &vel));
			ERRCHECK(chn1->setPaused(false));
			t_start_1 = currTime;
			random1 = rand() % 5;
		}
		else if (random2 * 1000.0f <= std::chrono::duration<double, std::milli>(currTime - t_start_2).count()) {
			ERRCHECK(system->playSound(gun2, 0, true, &chn2));
			FMOD_VECTOR pos = { ((rand() % 10)), 0 , (10 - (rand() % 20)) };
			FMOD_VECTOR vel = { 1,0,0 };
			ERRCHECK(chn2->set3DAttributes(&pos, &vel));
			ERRCHECK(chn2->setPaused(false));
			t_start_2 = currTime;
			random2 = rand() % 5;
		}

		//Actualizar
		system->update();
	}

	ERRCHECK(loopSound->release());
	ERRCHECK(gun1->release());
	ERRCHECK(gun2->release());

	ERRCHECK(system->release());
}

void Hoja3::ejercicio_02()
{
	initFMOD();
	Sound* motor;
	Channel* chn1;

	ERRCHECK(system->createSound("assets\\motor.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &motor));
	ERRCHECK(system->playSound(motor, 0, false, &chn1));

	while (true) {

		if (_kbhit()) {
			int key = _getch();
			//Salir 27 == esc en ASCII
			if (key == 27) {
				break;
			}
			else if (key == 'a' || key == 'A') {
				float pitch;
				chn1->getPitch(&pitch);
				chn1->setPitch(pitch + 0.1f);
			}
			else if (key == 's' || key == 'S') {
				float pitch;
				chn1->getPitch(&pitch);
				if (pitch * 0.1f > 0)chn1->setPitch(pitch - 0.1f);
			}
		}

		ERRCHECK(system->update());
	}
	ERRCHECK(motor->release());
	ERRCHECK(system->release());
}

void Hoja3::ejercicio_03()
{
	initFMOD();
	Sound* paso;
	Channel* chn1;

	ERRCHECK(system->createSound("assets\\footstep_edit.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &paso));
	ERRCHECK(system->playSound(paso, 0, true, &chn1));
	setListener();

	FMOD_VECTOR pos = { 10, 0, 0 };
	FMOD_VECTOR vel = { 0,0,0 };
	ERRCHECK(chn1->set3DAttributes(&pos, &vel));
	ERRCHECK(chn1->setPaused(false));


	while (true) {
		if (_kbhit()) {
			int key = _getch();
			FMOD_VECTOR posAux;
			FMOD_VECTOR velAux;
			chn1->get3DAttributes(&posAux, &velAux);
			std::cout << "Pos x : " << posAux.x << " z : " << posAux.z << "\n";
			//Salir 27 == esc en ASCII
			if (key == 27) {
				break;
			}
			else if (key == 'W' || key == 'w') {
				posAux.x += 0.5;
			}
			else if (key == 'S' || key == 's') {
				posAux.x -= 0.5;
			}
			else if (key == 'A' || key == 'a') {
				posAux.z -= 0.5;
			}
			else if (key == 'D' || key == 'd') {
				posAux.z += 0.5;
			}

			chn1->set3DAttributes(&posAux, &velAux);
		}
		ERRCHECK(system->update());
	}
	ERRCHECK(paso->release());
	ERRCHECK(system->release());
}

void Hoja3::ejercicio_04()
{
	initFMOD();
	constexpr int NUM_SHOTS = 7;

	Sound** disparos = new Sound * [NUM_SHOTS];

	for (int i = 0; i < NUM_SHOTS; i++) {
		std::string s = "assets\\shoots\\0";
		s.append(std::to_string(i+1));
		s.append(".wav");
		ERRCHECK(system->createSound(s.c_str(), FMOD_3D, 0, &disparos[i]));
	}
	Channel* chn1;

	while (true) {
		if (_kbhit()) {
			int key = _getch();

			//Salir 27 == esc en ASCII
			if (key == 27) {
				break;
			}
			else if (key == 'Q' || key == 'q') {
				ERRCHECK(system->playSound(disparos[rand() % NUM_SHOTS], 0, true, &chn1));
				ERRCHECK(chn1->setPitch(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0f))));
				ERRCHECK(chn1->setPaused(false));
			}
		}
	}

	for (int i = 0; i < NUM_SHOTS; i++) {
		ERRCHECK(disparos[i]->release());
	}
	ERRCHECK(system->release());
}

void Hoja3::ejercicio_04_v2()
{
	initFMOD();
	
	constexpr int MAX_ELEMS = 7;
	Sound* sounds[MAX_ELEMS];
	Channel* channels[MAX_ELEMS];
	
	for (int i = 0; i < MAX_ELEMS; i++) {
		std::string path = "assets\\shoots\\0";
		path = path.append(std::to_string(i + 1) + ".wav");

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

void Hoja3::ejercicio_05()
{
	initFMOD();
	Sound* piano;

	char keys [] = {'Z','S','X','D','C','V','G','B','H','N','J','M'};

	ERRCHECK(system->createSound("assets\\piano.ogg", FMOD_3D , 0, &piano));
	int octava = 0;
	while (true) {
		time_t currTime = time(nullptr);
		if (_kbhit()) {
			int key = _getch();
			if (key == 27) break;
			else if (toupper(key) == 'P') {
				octava++;
			}
			else if (octava > 0 && toupper(key) == 'O') {
				octava--;
			}
			else {
				for (int i = 0; i < 12; i++) {
					if (toupper(key) == keys[i]) {
						Channel* chn1;
						ERRCHECK(system->playSound(piano, 0, true, &chn1));
						ERRCHECK(chn1->setPitch(std::pow(2.0f, (i + 12.0f*octava)/12.0f)));
						ERRCHECK(chn1->setPaused(false));
					}
				}
			}
		}
		ERRCHECK(system->update());
	}
	ERRCHECK(piano->release());
	ERRCHECK(system->release());
}

void Hoja3::ejercicio_06()
{
	constexpr int dimensions = 15;
	constexpr float VARIATION = 0.1f;

	initFMOD();

	Sound* sonido;
	//LISTENER
	FMOD_VECTOR listenerPos = { 0,0,0 }; FMOD_VECTOR listenerVel = { 0,0,0 };
	FMOD_VECTOR up = { 0,1,0 }; FMOD_VECTOR at = { 1,0,0 };
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));

	//SOURCE
	FMOD_VECTOR pos = { dimensions / 2,dimensions / 2,0 }, vel = { 0,0,0 };
	ERRCHECK(system->createSound("assets\\scooter.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sonido));

	Channel* channel;
	ERRCHECK(system->playSound(sonido, 0, true, &channel));
	ERRCHECK(channel->set3DAttributes(&pos, &vel));
	//atenuacion
	float minDistance = 1.0f, maxDistance = 100.0f;
	ERRCHECK(channel->set3DMinMaxDistance(minDistance, maxDistance));
	//Quitamos la pausa
	ERRCHECK(channel->setPaused(false));
	
	//Renderizado inicial del estado, solo lo updateamos cuando pulsamos una tecla que cambia la posicion
	//o del listener o del source
	renderEj6(dimensions, &listenerPos, &pos, channel);


	while (true) {
		//Gestion de input para mover al listener
		if (_kbhit()) {
			int key = _getch();

			float volume; ERRCHECK(channel->getVolume(&volume));
			float inside, outside, outsideVolume; ERRCHECK(channel->get3DConeSettings(&inside, &outside, &outsideVolume));
			float minDist, maxDist; ERRCHECK(channel->get3DMinMaxDistance(&minDist, &maxDist));

			bool teclaImportante = true;
			//Movimiento del listener
			if (key == 27) break;
			else if (std::toupper(key) == 'W' && listenerPos.y > 0) listenerPos.y -= 1;
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
	
	ERRCHECK(sonido->release());
	ERRCHECK(system->release());
}
