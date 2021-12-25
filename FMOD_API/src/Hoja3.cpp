#include "Hoja3.h"
#include <iostream>
#include <conio.h>
#include <chrono>

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
				if(pitch * 0.1f > 0)chn1->setPitch(pitch - 0.1f);
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

	FMOD_VECTOR pos = { 10, 0, 0};
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
