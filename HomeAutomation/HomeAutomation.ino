/*
 Name:		HomeAutomation.ino
 Created:	02.09.2017 19:33:59
 Author:	erich
*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <SoftwareSerial.h>

#define XBEE_BAUD_RATE 9600

#define LED_PIN 13 
BridgeServer server;
String msg;

volatile bool lampState;

SoftwareSerial xbeeSerial(3, 2); // RX, TX
const byte zigBeeLampOn[]  = { 0x7E, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x6F, 0x6E, 0x17 };
const byte zigBeeLampOff[] = { 0x7E, 0x00, 0x11, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x6F, 0x66, 0x66, 0xB9};


void setup() {
	lampState = false;
	digitalWrite(LED_PIN, LOW);

	xbeeSerial.begin(XBEE_BAUD_RATE);

	Bridge.begin();
	//Console.begin();
	//while (!Console);
	server.listenOnLocalhost();
	server.begin();
}

void loop() {
	//*********Read new message from the client**************
	BridgeClient client = server.accept(); //check new clients

	if (client) {
		String command = client.readStringUntil('/');  //read the incoming data
		if (command == "msg") {
			msg = client.readStringUntil('/');             // read the incoming data

			if (msg.startsWith("on")) {
				digitalWrite(LED_PIN, HIGH);
				xbeeSerial.write(zigBeeLampOn, 20);
				lampState = true;
				client.print("on");
			}
			else if (msg.startsWith("off")) {
				digitalWrite(LED_PIN, LOW);
				xbeeSerial.write(zigBeeLampOff, 21);
				lampState = false;
				client.print("off");
			}
			else if (msg.startsWith("state")) {
				if (lampState == true) {
					client.print("on");
				}
				else {
					client.print("off");
				}
			}
		}
		client.flush();
		client.stop();
	}
	delay(200);
}