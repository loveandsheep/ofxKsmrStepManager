//
//  ofxKsmrStepManager.h
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#ifndef __sheep_ramSceneTest__ofxKsmrStepManager__
#define __sheep_ramSceneTest__ofxKsmrStepManager__

#include "ofMain.h"
#include "virtualSteppingMotor.h"

enum ofxKsmrStepPreset{
	KSMR_STEP_P_PMSA_B56D5, //ShinanoKenshi
	KSMR_STEP_SM_42BYG011_25 //MERCURY MOTOR
};

class ofxKsmrStepManager{
public:
	void setup(string portName,int baud);
	void setup(int portNum, int baud);

	void addStepper(string name,int numStep,int SPIch);

	void setupEasy();
	void setupEasyFromPreset(ofxKsmrStepPreset preset);

	void resetAllDevices();
	void setMicroSteps		(int involution0to7);

	void selectStepperOne	(int ch,bool enable);
	void setStepperAll		(bool enable);
	void setStepperSingle	(int ch,bool enable);

	void sendSPIByteAll		(unsigned char byte);
	void sendSPIByteSingle	(unsigned char byte,int ch);
	void sendSPIByteSelected(unsigned char byte);

	void absPos				(int pos);
	void run				(int speed,bool dir);
	void move				(int step,bool dir);
	void go_to				(int pos);

	void softStop();
	void hardStop();
	void gohome();

	void setParam_maxSpeed(int bit_10);
	void setParam_minSpeed(int bit_13);
	void setParam_Accel(int bit_12);
	void setParam_Decel(int bit_12);
	void setParam_AbsPos(int bit_22);

	int microStepInv;

	virtualSteppingMotor &getMotor(int num);
	virtualSteppingMotor &getMotor(string name);

	vector<virtualSteppingMotor> steppers;
	ofSerial serial;
};

#endif /* defined(__sheep_ramSceneTest__ofxKsmrStepManager__) */
