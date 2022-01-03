#pragma once
#include "fmod.hpp"

using namespace FMOD;
class EjerciciosExamen
{
private:
	System* system;
	void initFMOD(FMOD_INITFLAGS flags);
	void render(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, FMOD_VECTOR* centerPos, float variation);

public:
	EjerciciosExamen();
	~EjerciciosExamen();

	void ejercicio1();
};

