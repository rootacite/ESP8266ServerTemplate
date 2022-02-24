#include <Wire.h> 

#include <ESP8266Wifi.h>
#include <WiFiUdp.h>

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#ifndef STASSID
#define STASSID "HUAWEI0260W"
#define STAPSK  "HUAWEI666"
#endif	

#define PORT 32212
#define SERIAL_BAUD 115200

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char* ssid = STASSID;
const char* password = STAPSK;

void Reverse(uint8_t pin);

WiFiServer Server(PORT);
WiFiClient Client;
char IPStr[30];

void FlushDisplay(char **lines, int count)
{
	u8g2.clearBuffer();
	for (int i = 0, y_base = 12; i < count; i++, y_base += 11)
	{
		u8g2.drawStr(2, y_base, lines[i]);
	}
	u8g2.sendBuffer();
}

void setup() {
	u8g2.begin();
	Serial.begin(SERIAL_BAUD);
	u8g2.setFontMode(1);	// Transparent
	u8g2.setFontDirection(0);
	u8g2.setFont(u8g2_font_6x12_tf);

	pinMode(A0, INPUT);
	pinMode(D4, OUTPUT);

	wifi_station_set_hostname("Acite_White_Host");
	digitalWrite(D4, LOW);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	char* n1[] = { "Connecting......" };
	
	FlushDisplay(n1, 1);

	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
		Reverse(D4);
	}

	digitalWrite(D4, LOW);
	strcpy(IPStr, WiFi.localIP().toString().c_str());

	char* n2[] = { "Connected.",IPStr,"Client Disconnected!" };
	FlushDisplay(n2, 3);
	Server.begin();

}

void loop() {
	int bb = analogRead(A0);

	if (!Client.connected()) {
		digitalWrite(D4, LOW);
		char* n2[] = { "Connected.",IPStr,"Client Disconnected!" };
		FlushDisplay(n2, 3);
		if (Server.hasClient())
		{
			Client = Server.available();
			digitalWrite(D4, HIGH);
			char* n3[] = { "Connected.",IPStr,"Client Connected!" };
			FlushDisplay(n3, 3);
		}
		else
			return;
	}


	//Client.print(buf);
	//Client.readStringUntil('\n');
}


void Reverse(uint8_t pin)
{
	digitalWrite(pin, !digitalRead(pin));
}