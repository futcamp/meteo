//*******************************************************************
//
// Future Camp Project
//
// Copyright(C) 2019 Sergey Denisov.
//
// Written by Sergey Denisov aka LittleBuster(DenisovS21@gmail.com)
// Github:  https://github.com/LittleBuster
//          https://github.com/futcamp
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public Licence 3
// as published by the Free Software Foundation; either version 3
// of the Licence, or(at your option) any later version.
//
//*******************************************************************

#include "hdk.h"
#include "configs.h"

#include <ESP8266WiFi.h>
#include <Arduino.h>


Hdk::Hdk(const std::shared_ptr<DallasTemperature> &ds,
         const std::shared_ptr<DHTesp> &dht,
         const std::shared_ptr<OneWire> &owire,
         const std::shared_ptr<Adafruit_BMP280> &bmp):
         ds_(std::move(ds)),
         dht_(std::move(dht)),
         owire_(std::move(owire)),
         bmp_(std::move(bmp))
{
}

void Hdk::switchPinMode(uint8_t pin, HdkPinModes mode)
{
    switch (mode)
    {
        case HDK_PIN_MODE_IN:
            pinMode(pin, INPUT);
            break;

        case HDK_PIN_MODE_OUT:
            pinMode(pin, OUTPUT);
            break;

        case HDK_PIN_MODE_IN_PULL_UP:
            pinMode(pin, INPUT_PULLUP);
            break;
    }
}

void Hdk::pinHigh(uint8_t pin)
{
    digitalWrite(pin, HIGH);
}

void Hdk::pinLow(uint8_t pin)
{
    digitalWrite(pin, LOW);
}

bool Hdk::readPin(uint8_t pin)
{
    if (digitalRead(pin) == LOW)
        return true;

    return false;
}

int8_t Hdk::readTempDS18B20(uint8_t pin)
{
    owire_->begin(pin);
    ds_->requestTemperatures();
    return static_cast<int8_t>(ds_->getTempCByIndex(HDK_DS18B20_DEFAULT_SENSOR));
}

void Hdk::readTempHumDHT22(uint8_t pin, int8_t &temp, uint8_t &hum)
{
    dht_->setup(pin, DHTesp::DHT22);
    temp = static_cast<int8_t>(dht_->getTemperature());
    hum = static_cast<uint8_t>(dht_->getHumidity());
}

bool Hdk::readPressureBMP280(uint16_t &pres)
{
#ifdef CFG_BOARD_ESP8266
    Wire.begin(CFG_I2C_SDA_PIN, CFG_I2C_SCL_PIN);

    if (!bmp_->begin(HDK_BMP_SENSOR_ADDR))
        return false;

    pres = (uint16_t)(bmp_->readPressure() * 750 / 100000);
#endif

    return true;
}

void Hdk::connectWifi(const String &ssid, const String &passwd)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);

    while (WiFi.status() != WL_CONNECTED) 
    {
        sleep(500);
    }
}

unsigned long int Hdk::getCurSeconds()
{
    return millis() / 1000;
}

void Hdk::sleep(unsigned millis)
{
    delay(millis);
}
