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

void ofxKsmrStepManager::setupAll(){

}

void ofxKsmrStepManager::testStep(){
	static int c = 0;
	c++;

	sendSPIByteAll(0x41);
	int val = (200 + c % 2 * 200) * 128;
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sendSPIByteAll(data[2]);
	sendSPIByteAll(data[1]);
	sendSPIByteAll(data[0]);
}

void ofxKsmrStepManager::addStepper(string name, int numStep, int SPIch){

	steppers.push_back( virtualSteppingMotor(name, numStep, SPIch) );

}

void ofxKsmrStepManager::setStepperFlag(int num, bool enable){

	if (num < 0 || num >= steppers.size()) return;
	steppers[num].sendEnable = enable;

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

//	sendSPIByteAll(0xD8);

//	sendSPIByteAll(0x09);//モータ停止中の電圧設定
//	sendSPIByteAll(0xFF);

//	sendSPIByteAll(0x0a);//モータ低速回転時の電圧設定
//	sendSPIByteAll(0xFF);

	sendSPIByteAll(0x0b);//加速中の電圧設定
	sendSPIByteAll(0xFF);

	sendSPIByteAll(0x0c);//減速中の電圧設定
	sendSPIByteAll(0xFF);

//	sendSPIByteAll(0x13);//オーバーカレント時の電流スレッショルド
//	sendSPIByteAll(0x0F);

//	sendSPIByteAll(0x14);//ストール時の電流スレッショルド
//	sendSPIByteAll(0x7F);

	sendSPIByteAll(0x07);//最大回転スピード
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x05);

	sendSPIByteAll(0x05);//加速度
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x05);

	setMicroSteps(0);

//	sendSPIByteAll(0x03);
//	sendSPIByteAll(0x00);
//	sendSPIByteAll(0x00);
//	sendSPIByteAll(0x00);
	absPos(0);
	absPos(0);
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