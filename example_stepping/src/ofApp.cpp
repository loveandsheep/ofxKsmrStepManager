#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	myMotor.serial.listDevices();
	myMotor.setup("tty.usbmodem1411", 57600);

	myMotor.addStepper(200);

	ofSleepMillis(3000);

	myMotor.resetAllDevices();
	myMotor.setParam_maxSpeed(30);
	myMotor.setParam_kVal_all(0xFF);
	myMotor.setParam_Accel(0x0050);
	myMotor.setParam_Decel(0x0050);
	myMotor.setMicroSteps(0);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	myMotor.setStepperAll(true);

	/*go absolute position 90 degree from home*/
	if (key == '1') myMotor.go_to(myMotor.getDegToStep(90));

	/*go relative position 90 degree from current position*/
	if (key == '2') myMotor.move(myMotor.getDegToStep(90), true);

	/*go absolute home position ( = go_to(0) )*/
	if (key == '3') myMotor.gohome();

	/*stop hard*/
	if (key == 's') myMotor.hardStop();

	/*stop softly*/
	if (key == 'd') myMotor.softStop();

	myMotor.setStepperAll(false);

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
