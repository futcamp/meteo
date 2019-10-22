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

#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__

#include <Arduino.h>
#include <ESP8266WebServer.h>

#include "meteo.h"


#define WEB_SERVER_RESPONSE_OK  200


class IWebServer
{
public:
    virtual void start() = 0;
    virtual void process() = 0;
};


class WebServer: public IWebServer
{
public:
    WebServer(const std::shared_ptr<ESP8266WebServer> &server,
              const std::shared_ptr<IMeteoReader> &temp);

    /*
     * @breif Starting web server
     */
    void start();

    /*
     * @breif Processing web handlers
     */
    void process();

private:
    const std::shared_ptr<ESP8266WebServer> server_;
    const std::shared_ptr<IMeteoReader> temp_;

    /*
     * @breif Get device identifier
     * 
     * @return id string
     */
    String getDevId();

    /*
     * @breif Main handler
     */
    void indexHandler();

    /*
     * @breif Get temperature and humidity handler
     */
    void meteoHandler();

    /*
     * @breif Not found handler
     */
    void notFoundHandler();
};


#endif
