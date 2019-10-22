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

#include <Arduino.h>
#include <functional>
#include <ESP8266WiFi.h>

#include "webserver.h"
#include "meteo.h"
#include "configs.h"


WebServer::WebServer(const std::shared_ptr<ESP8266WebServer> &server,
                     const std::shared_ptr<IMeteoReader> &temp):
                     server_(std::move(server)),
                     temp_(std::move(temp))
{
}

void WebServer::start()
{
    server_->on("/", std::bind(&WebServer::indexHandler, this));
    server_->on("/meteo", std::bind(&WebServer::meteoHandler, this));
    server_->onNotFound(std::bind(&WebServer::notFoundHandler, this));
    server_->begin();
}

void WebServer::process()
{
    server_->handleClient();
}

/*
 * Private functions
 */

String WebServer::getDevId()
{
    String id = "";

    for (auto c : WiFi.macAddress())
        if (c != ':')
            id += c;
    id.toLowerCase();

    return id;
}

void WebServer::indexHandler()
{
    String response;

    response = "{\"module\":\"meteo\",\"id\":\"" + getDevId() +
               "\",\"board\":\"" + CFG_BOARD_NAME + "\"}";

    server_->send(WEB_SERVER_RESPONSE_OK, "application/json", response);
}

void WebServer::meteoHandler()
{
    String  response;
    int8_t  temp = TEMP_FAIL_VAL;
    uint8_t hum = 0;
    uint8_t chan = 0;
    uint16_t pres = 0;

    chan = static_cast<uint8_t>(server_->arg("chan").toInt());

    if (server_->arg("type") == TEMP_SENSOR_DS18B20)
    {
        temp = temp_->readTemp(chan);
        response = "{\"temp\":" + String(temp) + "}";
    }
    else if (server_->arg("type") == TEMP_SENSOR_DHT22)
    {
        if (!temp_->readTempHum(chan, temp, hum))
        {
            temp = TEMP_FAIL_VAL;
            hum = HUM_FAIL_VAL;
        }
        response = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + "}";
    }
    else if (server_->arg("type") == "all")
    {
        if (!temp_->readTempHum(chan, temp, hum))
        {
            temp = TEMP_FAIL_VAL;
            hum = HUM_FAIL_VAL;
        }
        if (!temp_->readPressure(pres))
            pres = PRES_FAIL_VAL;

        response = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + ",\"pres\":" + 
                    String(pres) + "}";
    }
    else
    {
        response = "{\"temp\":" + String(TEMP_FAIL_VAL) + "}";
    }

    server_->send(WEB_SERVER_RESPONSE_OK, "application/json", response);
}

void WebServer::notFoundHandler()
{
    String response;

    response = "{\"module\":\"meteo\",\"error\":\"Not found\"}";

    server_->send(WEB_SERVER_RESPONSE_OK, "application/json", response);
}
