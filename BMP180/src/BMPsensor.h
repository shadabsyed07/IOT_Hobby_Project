/*
 * BMPsensor.h
 *
 *  Created on: Jun 12, 2017
 *      Author: shadab-syed
 */

#ifndef BMPSENSOR_H_
#define BMPSENSOR_H_
#include "I2CDevice.h"

namespace exploringBMP180{


class BMPsensor:protected I2CDevice{
public:

	BMPsensor(unsigned int I2CBus, unsigned int I2CAddress=0x77);
	virtual void readCalibrationData();
	virtual float getTemperature(){return Temperature;}
	virtual float getPressure(){return Pressure;}
	virtual short getAltitude(){return Altitude;}
	virtual void displayTPA(); //display temperature, pressure and altitude on console
	virtual void displayAllReg();
	virtual ~BMPsensor();

private:
	unsigned int I2CBus, I2CAddress;
	unsigned char *registers;
	float Temperature, Pressure;
	float Altitude;
	long UT, UP;
	//Calibration variables
	short AC1, AC2, AC3, B1, B2, MB, MC, MD;
	unsigned short AC4, AC5, AC6;

	short combineRegisters(unsigned char msb, unsigned char lsb);
	void calculateTPA();
	virtual int updateRegisters();

};
} // end of namespace exploringBMP180

#endif /* BMPSENSOR_H_ */
