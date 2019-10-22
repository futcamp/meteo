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

#include "app.h"
#include "configs.h"


App::App(const std::shared_ptr<IHdk> &hdk,
         const std::shared_ptr<IWebServer> &server):
         hdk_(std::move(hdk)),
         server_(std::move(server))
{
}

void App::start()
{
    hdk_->connectWifi(CFG_WIFI_SSID, CFG_WIFI_PASSWD);
    server_->start();
}

void App::loop()
{
    server_->process();
}
