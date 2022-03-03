#pragma region Head


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
#define STASSID "lishiyuan"
#define STAPSK  "761834925"
#endif	

#define PORT 25523
#define SERIAL_BAUD 115200

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char* ssid = STASSID;
const char* password = STAPSK;

void Reverse(uint8_t pin);

WiFiServer Server(PORT);
WiFiClient Client;
char IPStr[30];

void SetupU8g2();
void FlushDisplay(char** lines, int count);
void SetupWiFi();

#pragma endregion

#pragma region DFS

#define DW(p,v) digitalWrite(p,v)
#define DR(p) digitalRead(p)

#define AR(p) analogRead(p)
#define AW(p,v) analogWrite(p,v)

#define PM(p,m) pinMode(p,m)

#pragma endregion


#pragma region Lines
typedef struct _Lines
{
	char *L1;
	char *L2;
	char *L3;
	char *L4;
	char *L5;
}Lines, * PLines;
PLines Data = NULL;

void U8g2Println(const char*l)
{
	int x = 0;
	char* i = (char*)Data;
	for (; x < 5; i += sizeof(char*), x++)
	{
		if (strlen(i) == 0) 
		{ strcpy(i, l);
		char* mm[] = { Data->L1 ,Data->L2,Data->L3,Data->L4,Data->L5 };
		FlushDisplay(mm, 5);

		return; }
	}

	char* temp = Data->L1;
	Data->L1 = Data->L2;
	Data->L2 = Data->L3;
	Data->L3 = Data->L4;
	Data->L4 = Data->L5;

	strcpy(temp, l);

	Data->L5 = temp;

	char* mm[] = { Data->L1 ,Data->L2,Data->L3,Data->L4,Data->L5 };
	FlushDisplay(mm, 5);

}

#pragma endregion


void SoftAPInit()
{
	/// <summary>
	WiFi.mode(WiFiMode::WIFI_AP);

	IPAddress softLocal(192, 168, 1, 1);   // 1 ��������WIFI IP��ַ
	IPAddress softGateway(192, 168, 1, 1);
	IPAddress softSubnet(255, 255, 255, 0);
	WiFi.softAPConfig(softLocal, softGateway, softSubnet);

	String apName = "Acite_White_Host";  // 2 ����WIFI����
	const char* softAPName = apName.c_str();

	WiFi.softAP(softAPName, "11223344");      // 3����wifi  ���� +���� adminadmin

	U8g2Println(softAPName);
	U8g2Println("11223344");

	Serial.println("Setuped SoftAP.");
	/// </summary>
}

void setup() {
	Serial.begin(SERIAL_BAUD);
	SetupU8g2();
	//SetupWiFi();
	SoftAPInit();
	PM(D4, OUTPUT);
	DW(D4, HIGH);
	


	strcpy(IPStr, WiFi.localIP().toString().c_str());

	Server.begin();

}
int xxb = 0;
void loop() {
	xxb++;
	char bb[10];
	itoa(xxb, bb, 10);
	Serial.println(bb);
	U8g2Println(bb);
	if (!Client.connected()) {
		digitalWrite(D4, LOW);
		if (Server.hasClient())
		{
			Client = Server.available();
			digitalWrite(D4, HIGH);
		}
		else
			return;
	}
	String dd = Client.readStringUntil('\0');
	Client.println("OK");
}


void Reverse(uint8_t pin)
{
	digitalWrite(pin, !digitalRead(pin));
}

void FlushDisplay(char** lines, int count)
{
	u8g2.clearBuffer();
	for (int i = 0, y_base = 12; i < count; i++, y_base += 11)
	{
		u8g2.drawStr(2, y_base, lines[i]);
	}
	u8g2.sendBuffer();
}

void SetupU8g2()
{
	Data = (PLines)malloc(sizeof(Lines));
	memset(Data, 0, sizeof(Lines));
	Data->L1 = new char[32];
	Data->L2 = new char[32];
	Data->L3 = new char[32];
	Data->L4 = new char[32];
	Data->L5 = new char[32];

	u8g2.begin();
	u8g2.setFontMode(1);	// Transparent
	u8g2.setFontDirection(0);
	u8g2.setFont(u8g2_font_6x13_tf);
}

void SetupWiFi()
{
	wifi_station_set_hostname("Acite_White_Host");

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
}