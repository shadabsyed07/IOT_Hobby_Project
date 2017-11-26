//============================================================================
// Name        : BMP180.cpp
// Author      : Shadab Syed
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "BMPsensor.h"
#include <unistd.h>
#include <pthread.h>
using namespace std;
using namespace exploringBMP180;

int main() {
	BMPsensor Sensor(1,0x77);
	Sensor.displayTPA();
	return 0;
}
