#include "EEPROMController.h"

EEPROMController::EEPROMController()
{
	EEPROM.begin(512);
	delay(10);
}

bool EEPROMController::saveWifi(String & ssid, String & password)
{
	for (int i = 64; i < 192; ++i)
		EEPROM.write(i, 0);

	EEPROM.end();
	EEPROM.begin(512);
	delay(10);

	if (ssid.length() > 64 || password.length() > 64)
	{
		Serial.print(F("eeprom overflow"));
		return false;
	}

	for (int i = 0; i < ssid.length(); ++i)
		EEPROM.write(64 + i, ssid[i]);

	for (int i = 0; i < password.length(); ++i) 
		EEPROM.write(64 + 64 + i, password[i]);

	bool state = EEPROM.commit();
	Serial.print(F("Save to eeprom: "));
	Serial.println(state);
	return state;
}

void EEPROMController::readWifi(String & ssid, String & password)
{
	char ssidArr[64];
	char passArr[64];

	for (int i = 0; i < 64; ++i)
		ssidArr[i] = char(EEPROM.read(i + 64));

	for (int i = 0; i < 64; ++i)
		passArr[i] = char(EEPROM.read(i + 64 + 64));

	ssid = String(ssidArr);
	password = String(passArr);
}

void EEPROMController::displayWiFi()
{
	char ssidArr[64];
	char passArr[64];

	for (int i = 0; i < 64; ++i)
		ssidArr[i] = char(EEPROM.read(i + 64));

	for (int i = 0; i < 64; ++i)
		passArr[i] = char(EEPROM.read(i + 64 + 64));

	Serial.printf_P(PSTR("Wifi ssid: %s\nWifi pass: %s\n"), ssidArr, passArr);
}

bool EEPROMController::setConfig(bool config)
{
	EEPROM[0] = config;
	EEPROM[205] = 0;

	EEPROM[1] = (char)90;
	return EEPROM.commit();
}

void EEPROMController::resetConfig()
{
	bool response = setConfig(false);
	Serial.print(F("EEPROM: "));
	Serial.println(response);
	WiFi.disconnect(true);
	delay(100);
	ESP.restart();
}

bool EEPROMController::getConfig()
{
	return EEPROM[0];
}

bool EEPROMController::isStaticAddres()
{
	return EEPROM[205];
}

bool EEPROMController::setStaticAddres(bool status)
{
	EEPROM[205] = status;
	return EEPROM.commit();
}

bool EEPROMController::setIpConfig()
{
	IPAddress ip = WiFi.localIP();
	IPAddress gateway = WiFi.gatewayIP();
	IPAddress subnet = WiFi.subnetMask();
	Serial.println(ip);
	Serial.println(gateway);
	Serial.println(subnet);
	for (uint8_t i = 0; i < 4; i++)
		EEPROM[193 + i] = ip[i];
	for (uint8_t i = 0; i < 4; i++)
		EEPROM[197 + i] = gateway[i];
	for (uint8_t i = 0; i < 4; i++)
		EEPROM[201 + i] = subnet[i];
	return EEPROM.commit();
}

IPAddress EEPROMController::getIp()
{
	IPAddress ip(EEPROM[193], EEPROM[194], EEPROM[195], EEPROM[196]);
	return ip;
}

IPAddress EEPROMController::getGateway()
{
	IPAddress ip(EEPROM[197], EEPROM[198], EEPROM[199], EEPROM[200]);
	return ip;
}

IPAddress EEPROMController::getSubnet()
{
	IPAddress ip(EEPROM[201], EEPROM[202], EEPROM[203], EEPROM[204]);
	return ip;
}

bool EEPROMController::setIp(IPAddress & ip)
{
	for (uint8_t i = 0; i < 4; i++)
		EEPROM[193 + i] = ip[i];
	return EEPROM.commit();
}