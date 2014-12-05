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

class ofxKsmrStepManager{
public:
	void setup(string portName,int baud);
	void setup(int portNum, int baud);

	void addStepper(string name,int numStep,int SPIch);
	void setStepperFlag(int num, bool enable);

	void setupAll();
	void testStep();

	void setupEasy();

	void resetAllDevices();
	void setMicroSteps(int involution0to7);

	void selectStepperOne(int ch,bool enable);
	void setStepperAll(bool enable);
	void setStepperSingle(int ch,bool enable);

	void sendSPIByteAll(unsigned char byte);
	void sendSPIByteSingle(unsigned char byte,int ch);
	void sendSPIByteSelected(unsigned char byte);

	void absPos(int pos);
	void run(int speed,bool dir);
	void move(int step,bool dir);
	void go_to(int pos);

	void softStop();
	void hardStop();
	void gohome();

	int microStepInv;

	virtualSteppingMotor &getMotor(int num);
	virtualSteppingMotor &getMotor(string name);

	vector<virtualSteppingMotor> steppers;
	ofSerial serial;
};

#endif /* defined(__sheep_ramSceneTest__ofxKsmrStepManager__) */
