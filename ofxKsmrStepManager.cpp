//
//  ofxKsmrStepManager.cpp
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#include "ofxKsmrStepManager.h"

void ofxKsmrStepManager::setup(string portName, int baud){
	serial.setup(portName, baud);
}

void ofxKsmrStepManager::setup(int portNum, int baud){
	serial.setup(portNum, baud);
}

void ofxKsmrStepManager::addStepper(string name, int numStep, int SPIch){

	steppers.push_back( virtualSteppingMotor(name, numStep, SPIch) );

}

void ofxKsmrStepManager::sendSPIByteAll(unsigned char byte){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();
	for (int i = 2;i < steppers.size()+2;i++){
		sig[i] = byte;
	}

	serial.writeBytes(sig, steppers.size()+2);
}

void ofxKsmrStepManager::sendSPIByteSelected(unsigned char byte){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();

	for (int i = 2;i < steppers.size()+2;i++){
		if (steppers[i - 2].sendEnable) sig[i] = byte;
		else sig[i] = 0x0;
	}

	serial.writeBytes(sig, steppers.size()+2);
}

void ofxKsmrStepManager::sendSPIByteSingle(unsigned char byte, int ch){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();
	for (int i = 2;i < steppers.size()+2;i++){
		if (i - 2 == ch) sig[i] = byte;
		else sig[i] = 0x0;
	}

	serial.writeBytes(sig, steppers.size()+2);
}

virtualSteppingMotor &ofxKsmrStepManager::getMotor(int num){

	num = ofClamp(num, 0, steppers.size() - 1);
	return steppers[num];

}

virtualSteppingMotor &ofxKsmrStepManager::getMotor(string name){

	for (int i = 0;i < steppers.size();i++){
		if (steppers[i].getName() == name){
			return steppers[i];
		}
	}
	return steppers[0];

}

void ofxKsmrStepManager::resetAllDevices(){
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x00);

	sendSPIByteAll(0xc0);
}

void ofxKsmrStepManager::setupEasy(){

	resetAllDevices();

	sendSPIByteAll(0x07);//最大回転スピード
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x25);

	sendSPIByteAll(0x05);//加速度
	sendSPIByteAll(0x02);
	sendSPIByteAll(0x20);

	sendSPIByteAll(0x06);//減速度
	sendSPIByteAll(0x02);
	sendSPIByteAll(0x20);


	setMicroSteps(7);
	absPos(0);
}

void ofxKsmrStepManager::setMicroSteps(int involution0to7){
	sendSPIByteAll(0x16);
	sendSPIByteAll(involution0to7);
	microStepInv = involution0to7;
}

void ofxKsmrStepManager::selectStepperOne(int ch,bool enable){
	setStepperAll(false);
	steppers[ch].sendEnable = enable;
}

void ofxKsmrStepManager::setStepperAll(bool enable){
	for (int i = 0;i < steppers.size();i++){
		steppers[i].sendEnable = enable;
	}
}

void ofxKsmrStepManager::setStepperSingle(int ch,bool enable){
	ofClamp(ch, 0, steppers.size() - 1);
	steppers[ch].sendEnable = enable;
}

void ofxKsmrStepManager::absPos(int pos){
	sendSPIByteSelected(0x01);

	int val = pos;
	unsigned char data[3];

	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sendSPIByteSelected(data[2]);
	sendSPIByteSelected(data[1]);
	sendSPIByteSelected(data[0]);
}

void ofxKsmrStepManager::run(int speed, bool dir){
	if (dir) sendSPIByteSelected(0x50);
	else	sendSPIByteSelected(0x51);
	
	int val = speed;
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sendSPIByteSelected(data[2]);
	sendSPIByteSelected(data[1]);
	sendSPIByteSelected(data[0]);
}

void ofxKsmrStepManager::move(int step, bool dir){
	if (dir) sendSPIByteSelected(0x40);
	else	sendSPIByteSelected(0x41);

	int val = step * pow(2.0f,microStepInv);
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sendSPIByteSelected(data[2]);
	sendSPIByteSelected(data[1]);
	sendSPIByteSelected(data[0]);
}

void ofxKsmrStepManager::go_to(int pos){
	sendSPIByteSelected(0x60);

	int val = pos;
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sendSPIByteSelected(data[2]);
	sendSPIByteSelected(data[1]);
	sendSPIByteSelected(data[0]);
}

void ofxKsmrStepManager::softStop(){
	sendSPIByteSelected(0xb0);
}

void ofxKsmrStepManager::hardStop(){
	sendSPIByteSelected(0xb8);
}

void ofxKsmrStepManager::gohome(){
	sendSPIByteSelected(0x70);
}

void ofxKsmrStepManager::setupEasyFromPreset(ofxKsmrStepPreset preset){

	resetAllDevices();

	if (preset == KSMR_STEP_P_PMSA_B56D5){
		setParam_maxSpeed(0x0025);
		setParam_Accel(0x0220);
		setParam_Decel(0x0220);
	}

	if (preset == KSMR_STEP_SM_42BYG011_25){
		setParam_maxSpeed(0x0041);
	}

	setMicroSteps(7);
	absPos(0);
}

void ofxKsmrStepManager::setParam_maxSpeed(int bit_10){
	sendSPIByteAll(0x07);
	unsigned char sig;

	sig = (bit_10 >> 8) & 0x03;
	sendSPIByteAll(sig);

	sig = bit_10 & 0xFF;
	sendSPIByteAll(sig);
}

void ofxKsmrStepManager::setParam_minSpeed(int bit_13){
	sendSPIByteAll(0x08);

	unsigned char sig;

	sig = (bit_13 >> 8) & 31;
	sendSPIByteAll(sig);

	sig = bit_13 & 0xFF;
	sendSPIByteAll(sig);
}

void ofxKsmrStepManager::setParam_Accel(int bit_12){
	sendSPIByteAll(0x05);

	unsigned char sig;

	sig = (bit_12 >> 8) & 0x0F;
	sendSPIByteAll(sig);

	sig = bit_12 & 0xFF;
	sendSPIByteAll(sig);
}

void ofxKsmrStepManager::setParam_Decel(int bit_12){
	sendSPIByteAll(0x06);

	unsigned char sig;

	sig = (bit_12 >> 8) & 0x0F;
	sendSPIByteAll(sig);

	sig = bit_12 & 0xFF;
	sendSPIByteAll(sig);
}

void ofxKsmrStepManager::setParam_AbsPos(int bit_22){
	absPos(bit_22);
}