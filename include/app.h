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

#ifndef __APP_H__
#define __APP_H__

#include "hdk.h"
#include "webserver.h"


class App
{
public:
    App(const std::shared_ptr<IHdk> &hdk,
        const std::shared_ptr<IWebServer> &server);

    /*
     * @breif Start application
     */
    void start();

    /*
     * @breif Main application loop
     */
    void loop();

private:
    const std::shared_ptr<IHdk> hdk_;
    const std::shared_ptr<IWebServer> server_;
};


#endif
