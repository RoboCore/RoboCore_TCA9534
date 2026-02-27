#ifndef TCA9534_H
#define TCA9534_H

/*******************************************************************************
* RoboCore TCA9534 Library (v1.0)
* 
* Library to use the TCA9534 I2C I/O expander.
* 
* Copyright 2026 RoboCore.
* 
* 
* This file is part of the TCA9534 library by RoboCore ("RoboCore-TCA9534-lib").
* 
* "RoboCore-TCA9534-lib" is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* "RoboCore-TCA9534-lib" is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with "RoboCore-TCA9534-lib". If not, see <https://www.gnu.org/licenses/>
*******************************************************************************/

#define TCA_DEBUG

// ----------------------------------------
// Libraries

extern "C" {
#include <stdint.h>
}

#include <Wire.h>

#ifdef TCA_DEBUG
#include <Stream.h>
#endif

// ----------------------------------------
// Macros

#ifndef HIGH
#define HIGH (1)
#define LOW (0)
#endif

#define TCA_REGISTER_INPUT   (0x00)
#define TCA_REGISTER_OUTPUT  (0x01)
#define TCA_REGISTER_CONFIG  (0x03)

#define TCA_ERROR   (0x81)

#define TCA_PIN_0   (0x01)
#define TCA_PIN_1   (0x02)
#define TCA_PIN_2   (0x04)
#define TCA_PIN_3   (0x08)
#define TCA_PIN_4   (0x10)
#define TCA_PIN_5   (0x20)
#define TCA_PIN_6   (0x40)
#define TCA_PIN_7   (0x80)

// ----------------------------------------
// Class - TCA9534

class TCA9534 {
  public:
    TCA9534(TwoWire &, const uint8_t = 0x20);

    bool config(uint8_t = 0x00, uint8_t = 0x00, bool = false);
    bool error(void);
    static uint8_t map_pin(const uint8_t);
    uint8_t read(const uint8_t = 0xFF);
    uint8_t read_pin(const uint8_t);
    bool set(uint8_t);
    bool set_pin(const uint8_t, const uint8_t);
  
#ifdef TCA_DEBUG
    void set_debugger(Stream &);
#endif

  private:
    TwoWire * _i2c;
    uint8_t _address;
    bool _error;
    bool _initialized;
    uint8_t _mask_config;
    uint8_t _mask_output;

#ifdef TCA_DEBUG
    Stream * _debugger;
#endif
};

// ----------------------------------------

#endif // TCA9534_H
