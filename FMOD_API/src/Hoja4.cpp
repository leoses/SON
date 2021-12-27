#include "Hoja4.h"
#include "Utils.h"
#include "StereoSound.h"
#include <iostream>
#include <conio.h>
#include <chrono>

void Hoja4::initFMOD(FMOD_INITFLAGS flags = FMOD_INIT_NORMAL)
{
	ERRCHECK(System_Create(&system)); // Creamos el objeto system
	// 128 canales (numero maximo que podremos utilizar simultaneamente)
	ERRCHECK(system->init(128, flags, 0)); // Inicializacion de FMOD
}

void Hoja4::setListener(FMOD_VECTOR listenerPos = { 0,0,0 }, FMOD_VECTOR listenerVel = { 0,0,0 },
	FMOD_VECTOR up = { 0,1,0 }, FMOD_VECTOR at = { 1,0,0 })
{
	// posicion del listener
	// velocidad del listener
	// vector up: hacia la ``coronilla''
	// vector at: hacia donde mira
	// colocamos listener
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));
}

void Hoja4::render(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, Channel* channel)
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

Hoja4::Hoja4() : system(nullptr)
{
}

Hoja4::~Hoja4()
{
}

void Hoja4::ejercicio_01()
{
	initFMOD();

	Sound* s1;
	ERRCHECK(system->createSound("assets\\scooter.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &s1));
	StereoSound* stereoSound = new StereoSound(system, "assets\\stereo\\footstep.wav", FMOD_3D | FMOD_LOOP_NORMAL);
	stereoSound->playSound();

	auto t_start = std::chrono::high_resolution_clock::now();
	while (true) {
		auto t_end = std::chrono::high_resolution_clock::now();
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
		if (_kbhit()) {
			int key = _getch();
			if (std::toupper(key) == 27) break;
			else if (std::toupper(key) == 'W') stereoSound->fadeOut(5000, 0.2f); //in millsecs
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

void Hoja4::ejercicio_02()
{
	initFMOD();
	constexpr int DIMENSIONS = 15;
	constexpr float VARIATION = 0.1f;

	Sound* sonido;

	//LISTENER
	FMOD_VECTOR listenerPos = { 0,0,0 }; FMOD_VECTOR listenerVel = { 0,0,0 };
	FMOD_VECTOR up = { 0,1,0 }; FMOD_VECTOR at = { 1,0,0 };
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));

	//SOURCE
	FMOD_VECTOR pos = { DIMENSIONS / 2,DIMENSIONS / 2,0 }, vel = { 0,0,0 };
	ERRCHECK(system->createSound("assets\\scooter.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &sonido));

	Channel* channel;
	ERRCHECK(system->playSound(sonido, 0, true, &channel));
	ERRCHECK(channel->set3DAttributes(&pos, &vel));
	//atenuacion
	float minDistance = 1.0f, maxDistance = 100.0f;
	ERRCHECK(channel->set3DMinMaxDistance(minDistance, maxDistance));
	//Quitamos la pausa
	ERRCHECK(channel->setPaused(false));

	//Orientacion de la fuente sonora
	float angle = 0.0f;
	FMOD_VECTOR orientation = { std::cos(angle), std::sin(angle), 0 };
	ERRCHECK(channel->set3DConeOrientation(&orientation));

	//IMPORTANTE AQUI EL OUTSIDE VOLUME QUE SI NO NO SE APRECIA CUANDO ESTAS DENTRO O FUERA DEL CONO
	float interiorAngle = 30.0f, exteriorAngle = 60.0f, outsideVolume = 1.0f;
	ERRCHECK(channel->set3DConeSettings(interiorAngle, exteriorAngle, outsideVolume));

	//REVERB
	FMOD::Reverb3D* reverb;
	ERRCHECK(system->createReverb3D(&reverb));
	FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CONCERTHALL;
	reverb->setProperties(&prop2);
	FMOD_VECTOR posRevb = { DIMENSIONS / 2, DIMENSIONS / 2, 0.0f };
	float mindist = 2.5f, maxdist = 5.0f;
	reverb->set3DAttributes(&posRevb, mindist, maxdist);
	reverb->setActive(true);

	//GEOMETRIA (NO FUNCA LOL)
	FMOD::Geometry* geo;
	ERRCHECK(system->createGeometry(1, 4, &geo));
	int polygonIndex;
	FMOD_VECTOR v [4]= {  { 0,11,0 },  { 10,11,0 },  { 10,10,0 }, {0, 10, 0}, };
	ERRCHECK(geo->addPolygon(0.0f, 0.0f, true, 4, v, &polygonIndex));
	FMOD_VECTOR geoPos = { DIMENSIONS / 2, DIMENSIONS / 2, 0 };
	ERRCHECK(geo->setPosition(&geoPos));
	ERRCHECK(geo->setActive(true));

	//Primer renderizado
	render(DIMENSIONS, &listenerPos, &pos, channel);
	std::cout << "Orientation x : " << orientation.x << " y: " << orientation.y << "\n";
	std::cout << "Cone Settings interior : " << interiorAngle << " exteriorAngle: " << exteriorAngle << "\n";

	while (true) {

		if (_kbhit()) {
			bool importantKey = true;
			int key = _getch();
			float minDist, maxDist; ERRCHECK(channel->get3DMinMaxDistance(&minDist, &maxDist));

			if (key == 27) break;
			//MoverListener
			else if (std::toupper(key) == 'W' && listenerPos.y > 0) listenerPos.y -= 1;
			else if (std::toupper(key) == 'S' && listenerPos.y < DIMENSIONS - 1) listenerPos.y += 1;
			else if (std::toupper(key) == 'A' && listenerPos.x > 0) listenerPos.x -= 1;
			else if (std::toupper(key) == 'D' && listenerPos.x < DIMENSIONS - 1) listenerPos.x += 1;
			//MoverSource
			else if (std::toupper(key) == 'I' && pos.y > 0) pos.y -= 1;
			else if (std::toupper(key) == 'K' && pos.y < DIMENSIONS - 1) pos.y += 1;
			else if (std::toupper(key) == 'J' && pos.x > 0) pos.x -= 1;
			else if (std::toupper(key) == 'L' && pos.x < DIMENSIONS - 1) pos.x += 1;
			//Direccion del sonido de la fuente
			else if (std::toupper(key) == 'P') angle += 10.0f;
			else if (std::toupper(key) == 'O') angle -= 10.0f;
			//Angulos de los conos interiores y exteriores
			else if (std::toupper(key) == 'F' && interiorAngle - VARIATION > 0) interiorAngle -= VARIATION;
			else if (std::toupper(key) == 'G' && interiorAngle + VARIATION < exteriorAngle) interiorAngle += VARIATION;
			else if (std::toupper(key) == 'N' && exteriorAngle - VARIATION > interiorAngle) exteriorAngle -= VARIATION;
			else if (std::toupper(key) == 'M' && exteriorAngle + VARIATION < 360.f) exteriorAngle += VARIATION;
			//MinMaxDistance
			else if (std::toupper(key) == 'Z' && minDist - VARIATION > 0)minDist -= VARIATION;
			else if (std::toupper(key) == 'X' && minDist + VARIATION < maxDist) minDist += VARIATION;
			else if (std::toupper(key) == 'C' && maxDist - VARIATION > minDist)maxDist -= VARIATION;
			else if (std::toupper(key) == 'V') maxDist += VARIATION;
			//Cambiar source de posicion (espejo)
			else if (std::toupper(key) == 'E') pos.y = (DIMENSIONS - pos.y - 1);
			else if (std::toupper(key) == 'R') pos.x = (DIMENSIONS - pos.x - 1);
			else importantKey = false;

			if (importantKey) {
				orientation = { std::cos(angle * (3.141592f / 180.0f)), std::sin(angle * (3.141592f / 180.0f)), 0 };
				ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &up, &at));
				ERRCHECK(channel->set3DConeOrientation(&orientation));
				ERRCHECK(channel->set3DConeSettings(interiorAngle, exteriorAngle, outsideVolume));
				ERRCHECK(channel->set3DAttributes(&pos, &vel));
				render(DIMENSIONS, &listenerPos, &pos, channel);
				std::cout << "Orientation x : " << orientation.x << " y: " << orientation.y << "\n";
				std::cout << "Cone Settings interior : " << interiorAngle << " exteriorAngle: " << exteriorAngle << "\n";
			}
		}

		ERRCHECK(system->update());
	}

	ERRCHECK(reverb->release());
	ERRCHECK(sonido->release());
	ERRCHECK(system->release());
}

