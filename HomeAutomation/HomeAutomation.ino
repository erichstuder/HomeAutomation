/*
 Name:		HomeAutomation.ino
 Created:	02.09.2017 19:33:59
 Author:	erich
*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

#define LED_PIN 13 
BridgeServer server;
String msg;

volatile bool lampState;



void setup() {
	lampState = false;
	digitalWrite(LED_PIN, LOW);

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
				lampState = true;
				client.print("on");
			}
			else if (msg.startsWith("off")) {
				digitalWrite(LED_PIN, LOW);
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