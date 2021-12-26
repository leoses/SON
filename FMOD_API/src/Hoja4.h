#pragma once
#include "fmod.hpp"

using namespace FMOD;
class Hoja4
{
private:
	System* system;
	void initFMOD(FMOD_INITFLAGS flags);
	void setListener(FMOD_VECTOR listenerPos, FMOD_VECTOR listenerVel, FMOD_VECTOR up, FMOD_VECTOR at);
	void render(int dimensions, FMOD_VECTOR* listenerPos, FMOD_VECTOR* pos, Channel* channel);

public:
	Hoja4();
	~Hoja4();

	void ejercicio_01();
	void ejercicio_02();
};

