#include "EjerciciosExamen.h"
#include "Utils.h"
#include <conio.h>
void EjerciciosExamen::initFMOD(FMOD_INITFLAGS flags = FMOD_INIT_NORMAL)
{
	ERRCHECK(System_Create(&system)); // Creamos el objeto system
// 128 canales (numero maximo que podremos utilizar simultaneamente)
	ERRCHECK(system->init(128, flags, 0)); // Inicializacion de FMOD
}

void EjerciciosExamen::render(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, FMOD_VECTOR* centerPos, float variation)
{
	//Clear console
	clearScreen();
	//Renderizado del estado actual del "juego"
	int mitad = dimensions / 2;
	for (float i = centerPos->z - mitad; i < mitad; i += variation) {
		for (float j = centerPos->x - mitad; j < mitad; j += variation) {
			if ((listenerPos->x == j && listenerPos->z == i)) {
				//Es Listener
				std::cout << "L ";
			}
			else if (pos->x == j && pos->z == i) {
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
}

EjerciciosExamen::EjerciciosExamen()
{
}

EjerciciosExamen::~EjerciciosExamen()
{
}

void EjerciciosExamen::ejercicio1()
{
	initFMOD();
	Sound* loopSound;
	Channel* musica;

	//Apartado 1
	ERRCHECK(system->createSound("assets\\examen\\musica.ogg", FMOD_2D | FMOD_LOOP_NORMAL, 0, &loopSound));
	//Empezamos el loop de musica inmediatamente
	ERRCHECK(system->playSound(loopSound, 0, false, &musica));
	ERRCHECK(musica->setVolume(0.1f));

	//Apartado 2: posicion del listener y loop del latido 3D

	//LISTENER
	FMOD_VECTOR listenerPos = { 0,0,0 }; FMOD_VECTOR listenerVel = { 0,0,0 };
	FMOD_VECTOR up = { 0,1,0 }; FMOD_VECTOR at = { 1,0,0 };
	ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up));

	//SOURCE
	Sound* latidoSound;
	Channel* latido;
	FMOD_VECTOR latidoPos = { 0,0,4 }, latidoVel = { 0,0,0 };
	float pitch;
	ERRCHECK(system->createSound("assets\\examen\\latido.wav", FMOD_3D | FMOD_LOOP_NORMAL, 0, &latidoSound));
	ERRCHECK(system->playSound(latidoSound, 0, true, &latido));
	ERRCHECK(latido->set3DAttributes(&latidoPos, &latidoVel));
	ERRCHECK(latido->getPitch(&pitch));

	//CONE SETTINGS
	//Orientacion de la fuente sonora
	FMOD_VECTOR orientation = { 0,0,1 };
	ERRCHECK(latido->set3DConeOrientation(&orientation));
	float interiorAngle = 30.0f, exteriorAngle = 60.0f, outsideVolume = 0.3f;
	ERRCHECK(latido->set3DConeSettings(interiorAngle, exteriorAngle, outsideVolume));

	ERRCHECK(latido->setPaused(false));

	FMOD_VECTOR center = { 0,0,0 };
	//Renderizado inicial
	render(15, &listenerPos, &latidoPos, &center, 0.5f);

	while (true) {
		ERRCHECK(system->update());

		if (_kbhit()) {
			int key = _getch();
			if (key == 'q')break;
			else if (key == 'p' && pitch - 0.1f > 0.0f) {
				pitch -= 0.1;
				ERRCHECK(latido->setPitch(pitch));
			}
			else if (key == 'P') {
				pitch += 0.1;
				ERRCHECK(latido->setPitch(pitch));
			}
			else if (key == 'a') {
				listenerPos.x -= 0.5;
				ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up));
			}
			else if (key == 's') {
				listenerPos.z += 0.5;
				ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up));
			}
			else if (key == 'd') {
				listenerPos.x += 0.5;
				ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up));
			}
			else if (key == 'w') {
				listenerPos.z -= 0.5;
				ERRCHECK(system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &at, &up));
			}
			//Solo volvemos a renderizar cuando ha habido un cambio en el estado (posiciones, pitch etc)
			else continue;

			render(15, &listenerPos, &latidoPos, &center, 0.5f);
		}
	}

	//Liberamos recursos y cerramos el sistema
	ERRCHECK(latidoSound->release());
	ERRCHECK(loopSound->release());
	ERRCHECK(system->release());

}
