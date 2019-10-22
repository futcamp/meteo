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

#ifndef __METEO_READER_H__
#define __METEO_READER_H__

#include <memory>

#include "hdk.h"


#define TEMP_SENSORS_COUNT  2

#define TEMP_SENSOR_DS18B20 "ds18b20"
#define TEMP_SENSOR_DHT22   "dht22"

#define TEMP_READ_RETRY 5
#define TEMP_READ_DELAY 500

#define TEMP_MIN_VALID  -50
#define TEMP_MAX_VALID  50
#define HUM_MIN_VALID   0
#define HUM_MAX_VALID   100

#define TEMP_FAIL_VAL   -127
#define HUM_FAIL_VAL    0
#define PRES_FAIL_VAL   0


class IMeteoReader
{
public:
    virtual int8_t readTemp(uint8_t pin) = 0;
    virtual bool readTempHum(uint8_t pin, int8_t &temp, uint8_t &hum) = 0;
    virtual bool readPressure(uint16_t &pres) = 0;
};


class MeteoReader: public IMeteoReader
{
public:
    MeteoReader(const std::shared_ptr<IHdk> &hdk);

    /*
     * @breif Reading temperature
     * 
     * @param [in] chan Channel number
     * 
     * @returns temperature value
     */
    int8_t readTemp(uint8_t chan);

    /*
     * @breif Reading temperature and humidity
     * 
     * @param [in] chan Channel number
     * @param [out] temp Temperature value
     * @param [out] hum Humidity value
     * 
     * @returns true If reading ok
     * @returns false If reading fail
     */
    bool readTempHum(uint8_t chan, int8_t &temp, uint8_t &hum);

    /*
     * @breif Reading pressure from BMP280 sensor
     * 
     * @param [out] pres pressure in mm
     * 
     * returns true if success
     * returns false if fail
     */
    bool readPressure(uint16_t &pres);

private:
    const std::shared_ptr<IHdk> hdk_;
    uint8_t pins_[TEMP_SENSORS_COUNT];
};


#endif
