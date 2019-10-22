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

#include "meteo.h"
#include "hdk.h"
#include "configs.h"

#include <assert.h>


MeteoReader::MeteoReader(const std::shared_ptr<IHdk> &hdk):
                         hdk_(std::move(hdk))
{
    pins_[0] = CFG_SENSOR_PIN_0;
    pins_[1] = CFG_SENSOR_PIN_1;
}

int8_t MeteoReader::readTemp(uint8_t chan)
{
    int8_t temp;

    for (uint8_t i = 0; i < TEMP_READ_RETRY; i++)
    {
        temp = hdk_->readTempDS18B20(chan);
        if ((temp < TEMP_MIN_VALID) || (temp > TEMP_MAX_VALID))
        {
            hdk_->sleep(TEMP_READ_DELAY);
        }
        else
        {
            break;
        }
    }

    return temp;
}

bool MeteoReader::readTempHum(uint8_t chan, int8_t &temp, uint8_t &hum)
{
    bool retVal;

    for (uint8_t i = 0; i < TEMP_READ_RETRY; i++)
    {
        hdk_->readTempHumDHT22(pins_[chan], temp, hum);

        if ((temp < TEMP_MIN_VALID) || (temp > TEMP_MAX_VALID) ||
            (hum <= HUM_MIN_VALID) || (hum > HUM_MAX_VALID))
        {
            retVal = false;
            hdk_->sleep(TEMP_READ_DELAY);
        }
        else
        {
            retVal = true;
            break;
        }
    }

    return retVal;
}

bool MeteoReader::readPressure(uint16_t &pres)
{
    return hdk_->readPressureBMP280(pres);
}
