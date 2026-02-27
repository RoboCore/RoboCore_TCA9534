/*******************************************************************************
* RoboCore TCA9534 Library (v1.0)
* 
* Simple program to read all pins of the TCA9534.
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

#include <TCA9534.h>

TCA9534 tca(Wire); // by default at address 0x20

uint8_t value = 0;
uint8_t value_last = 0;

void setup() {
  // configure the serial communication
  Serial.begin(115200);

  // configure the I2C communication
  Wire.begin();

  // configure the TCA
  Serial.print("TCA config: ");
  Serial.println(tca.config());
}

void loop() {
  value = tca.read();

  if (value != value_last){
    Serial.print("New value: 0b");
    Serial.println(value, BIN);

    value_last = value; // update
  }
}
