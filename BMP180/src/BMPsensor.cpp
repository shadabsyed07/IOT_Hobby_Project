/*
 * BMPsensor.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: shadab-syed
 */

#include "BMPsensor.h"
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
using namespace std;

namespace exploringBMP180{

#define AC1_ADD 0xAA
#define AC2_ADD 0xAC
#define AC3_ADD 0xAE
#define AC4_ADD 0xB0
#define AC5_ADD 0xB2
#define AC6_ADD 0xB4
#define B1_ADD  0xB6
#define B2_ADD  0xB8
#define MB_ADD  0xBA
#define MC_ADD  0xBC
#define MD_ADD  0xBE



#define READ_TEMP 0x2E
#define READ_PRES 0x34
#define WRITE_REG
#define READ_REG_MSB 0xF6
#define READ__REG_LSB 0xF7
#define READ_REG_XLSB 0xF8

#define OSS 0
static long X1,X2,B5;


BMPsensor::BMPsensor(unsigned int I2CBus, unsigned int I2CAddress):
I2CDevice(I2CBus, I2CAddress){
	this->I2CBus = I2CBus;
	this->I2CAddress = I2CAddress;
	this->registers = NULL;
	this->Temperature = 0.0f;
	this->Pressure = 0.0f;
	this->Altitude = 0.0f;
	this->UT = 0;
	this->UP = 0;
	this->AC1 = 0;
	this->AC2 = 0;
	this->AC3 = 0;
	this->AC4 = 0;
	this->AC5 = 0;
	this->AC6 = 0;
	this->B1 = 0;
	this->B2 = 0;
	this->MB = 0;
	this->MC = 0;
	this->MD = 0;
	this->readCalibrationData();
	// TODO Auto-generated constructor stub
}

BMPsensor::~BMPsensor() {
	// TODO Auto-generated destructor stub
}


void BMPsensor::calculateTPA(){
	this->writeRegister(0xF4,READ_TEMP);
	usleep(4500);
		//wait 4.5 msec
		this->registers = this->readRegisters(2,READ_REG_MSB);
	this->UT= (this->registers[0] << 8) + this->registers[1];

	//Read UP
	this->writeRegister(0xF4,READ_PRES+(OSS << 6));
		usleep(4500);
		//wait 4.5 msec
		this->registers = this->readRegisters(3,READ_REG_MSB);
		this->UP = ((this->registers[0] << 16)
				       + (this->registers[1] << 8)
					       + (this->registers[2]))
		       					     >> (8-OSS);  //Uncompensated pressure
	//calculating true temperature
	X1 = (this->UT - this->AC6)*this->AC5/32768;   //refer datasheet
    X2 = (this->MC * 2048)/(X1+this->MD);
    B5 = X1+X2;
    this->Temperature = ((B5+8)/16.0)/10.0;

    //calculating pressure

	long B6 = B5 - 4000;
    X1 = (this->B2*(B6*B6/4096))/2048;
    X2 = (this->AC2*B6)/2048;
    long X3 = X1+X2;
    long B3 = (((this->AC1*4+X3)<<OSS)+2)/4;
    X1 = long(pow(this->AC3, B6)/8192);
    X2 = (this->B1*(B6*B6/4096))/65536;
    X3 = ((X1+X2)+2)/4;
    unsigned long B4;
    B4 = this->AC4*((unsigned long)(X3+32768)/32768);
    unsigned long B7 = ((unsigned long)this->UP-B3)*(50000>>OSS);
    	if(B7 < 0x80000000){this->Pressure = (B7*2)/B4;}

    	else {this->Pressure = (B7/B4)*2;}
    X1 = (this->Pressure/256)*(this->Pressure/256);
    X1 = (X1*3038)/65536;
    X2 = (-7357*this->Pressure)/65536;
    this->Pressure = this->Pressure + (X1 + X2 + 3791)/8.0;
    this->Altitude = 44330.0*(1-pow((this->Pressure/101325.0),(1/5.255)));
    //calculating pressure


}
void BMPsensor::displayTPA(){
	this->calculateTPA();
	cout << "Temperature          : " << this->Temperature << " C"<< endl;
	cout << "Pressure             : " << this->Pressure <<" Pa" << endl;
	cout << "Altitude             : " << this->Altitude << " m"<< endl;

	//calculating Absolute pressure
	float PressureAtSeaLevel = this->Pressure/(pow((1-(this->Altitude/44330.0)),5.255));
	cout << "Pressure at Sea Level: " << PressureAtSeaLevel << endl;
}
void BMPsensor::readCalibrationData(){
	this->registers = this->readRegisters(2,AC1_ADD);
	this->AC1 = (this->registers[0] << 8) + this->registers[1];

	this->registers = this->readRegisters(2,AC2_ADD);
	this->AC2 = (this->registers[0] << 8) + this->registers[1];

	this->registers = this->readRegisters(2,AC3_ADD);
	this->AC3 = (this->registers[0] << 8) + this->registers[1];

	this->registers = this->readRegisters(2,AC4_ADD);
	this->AC4 = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,AC5_ADD);
	this->AC5 = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,AC6_ADD);
	this->AC6 = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,B1_ADD);
	this->B1 = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,B2_ADD);
	this->B2 = (this->registers[0] << 8) + this->registers[1];

	this->registers = this->readRegisters(2,MB_ADD);
	this->MB = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,MC_ADD);
	this->MC = (this->registers[0] << 8) + this->registers[1];


	this->registers = this->readRegisters(2,MD_ADD);
	this->MD = (this->registers[0] << 8) + this->registers[1];

}

int BMPsensor::updateRegisters(){
	this->readCalibrationData();
	return 0;
}

void BMPsensor::displayAllReg(){
	cout << "AC1: " << this->AC1 << endl;
	cout << "AC2: " << this->AC2 << endl;
	cout << "AC3: " << this->AC3 << endl;
	cout << "AC4: " << this->AC4 << endl;
	cout << "AC5: " << this->AC5 << endl;
	cout << "AC6: " << this->AC6 << endl;
	cout << "B1 : " << this->B1  << endl;
	cout << "B2 : " << this->B2  << endl;
	cout << "MB : " << this->MB  << endl;
	cout << "MC : " << this->MC  << endl;
	cout << "MD : " << this->MD  << endl;
	cout << "UT : " << this->UT  << endl;
	cout << "UP : " << this->UP  << endl;
}

} /*end of namespace exploringBMP180*/
