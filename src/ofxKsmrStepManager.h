//
//  ofxKsmrStepManager.h
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#ifndef __sheep_ramSceneTest__ofxKsmrStepManager__
#define __sheep_ramSceneTest__ofxKsmrStepManager__

#define USE_OSC

#include "ofMain.h"
#include "virtualSteppingMotor.h"
#include "ofxOsc.h"

enum ofxKsmrStepPreset{
	KSMR_STEP_P_PMSA_B56D5,		//ShinanoKenshi
	KSMR_STEP_SM_42BYG011_25	//MERCURY MOTOR
};

class ofxKsmrStepManager{
public:
	ofxKsmrStepManager(){
		useOsc = false;
		sendByteSimply = false;
	}

	/*==== System Config ===*/
	void setup(string portName,int baud);
	void setup(int portNum, int baud);
	void setupOsc(string address, int port);

	void addStepper(int numStep);
	void addStepper(string name,int numStep,int SPIch);

	/*=== L6470 Config ===*/
	void resetAllDevices();

	void setupEasy();
	void setupEasyFromPreset(ofxKsmrStepPreset preset);

	void setMicroSteps		(int involution0to7);
	void setParam_maxSpeed(int bit_10);
	void setParam_minSpeed(int bit_13);
	void setParam_Accel(int bit_12);
	void setParam_Decel(int bit_12);
	void setParam_AbsPos(int bit_22);

	void setParam_kVal_all(int bit_8);
	void setParam_kVal_Acc(int bit_8);
	void setParam_kVal_Dec(int bit_8);
	void setParam_kVal_Run(int bit_8);
	void setParam_kVal_Hold(int bit_8);

	/*=== Slave Select Flags ===*/
	void selectStepperOne	(int ch,bool enable);
	void setStepperAll		(bool enable);
	void setStepperSingle	(int ch,bool enable);

	/*=== SPI Translate ===*/
	void sendSPIPacketAll		(unsigned char* bytes,int length);
	void sendSPIPacketSelected	(unsigned char* bytes,int length);

	void sendSPIMultiByte	(unsigned char* bytes,int length);

	void sendSPIByteAll		(unsigned char byte);
	void sendSPIByteSingle	(unsigned char byte,int ch);
	void sendSPIByteSelected(unsigned char byte);

	/*=== moving Function ===*/
	void absPos				(int pos);
	void run				(int speed,bool dir);
	void move				(int step,bool dir);
	void go_to				(int pos);

	void multi_go_to		(int* pos);

	void softStop();
	void hardStop();
	void gohome();

	/*=== Utility Functions ===*/
	int   getDegToStep(float degree,int Ch = 0);
	float getStepToDeg(int Step,int Ch = 0);

	int microStepInv;

	virtualSteppingMotor &getMotor(int num);
	virtualSteppingMotor &getMotor(string name);

	vector<virtualSteppingMotor> steppers;
	ofSerial serial;

	bool			useOsc;
	bool			sendByteSimply;

	ofxOscSender	sender;

	void sendBytesOnline(unsigned char* buffer, int length);
};

#endif /* defined(__sheep_ramSceneTest__ofxKsmrStepManager__) */
