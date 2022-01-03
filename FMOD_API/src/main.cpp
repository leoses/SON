#include "Hoja3.h"
#include "Hoja4.h"
#include "EjerciciosExamen.h"

int main(int argc, char* argv[]) {

	EjerciciosExamen* ejercicios = new EjerciciosExamen();
	ejercicios->ejercicio1();
	delete ejercicios;
	return 0;
}