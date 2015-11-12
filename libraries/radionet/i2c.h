/*
 Copyright (C) 2015 Dave Berkeley projects2@rotwang.co.uk

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA
*/

#include "pinio.h"

    /*
     *  I2C Interface
     */

typedef struct {
    PinIo*      sda;        //  SDA pin
    PinIo*      scl;        //  SCL pin
    uint8_t     addr;       //  device address
    void (*delay)();        //  optional scl delay fn
    void (*trig)();         //  optional START debug fn

    //  Used internally
    int16_t     retries;    //  Polled mode retries
}   I2C;


// High level I2C functions
void i2c_init(I2C* i2c);
bool i2c_is_present(I2C* i2c);

    /*
     *  Interface to Flash Memory.
     */

bool i2c_load(I2C* i2c, uint16_t page, uint8_t offset, void* buff, int count);
bool i2c_save(I2C* i2c, uint16_t page, uint8_t offset, const void* buff, int count);

typedef struct {
    uint16_t    pages;
    uint16_t    page_size;
}   _FlashInfo;

typedef struct {
    I2C* i2c;
    _FlashInfo info;
}   FlashIO;

// Detect the size of EEPROM
void i2c_probe(FlashIO* io);

// FIN
