#pragma once
#include "fmod.hpp" // para utilizar el wrapper C++

using namespace FMOD;

class Hoja3
{
private:
	System* system;
	void initFMOD(FMOD_INITFLAGS flags);
	void setListener(FMOD_VECTOR listenerPos, FMOD_VECTOR listenerVel, FMOD_VECTOR up, FMOD_VECTOR at);

	void renderEj6(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, Channel* channel);

public:
	Hoja3();
	~Hoja3();

	void ejercicio_01();
	void ejercicio_02();
	void ejercicio_03();
	void ejercicio_04();
	void ejercicio_04_v2();
	void ejercicio_05();
	void ejercicio_06();
};

