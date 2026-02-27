/*******************************************************************************
* RoboCore TCA9534 Library (v1.0)
* 
* Simple I2C scanner.
* 
* Copyright 2026 RoboCore.
* Based on several examples on the internet.
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

#include <Wire.h>

void setup() {
  // configure the serial communication
  Serial.begin(115200);
  Serial.println("I2C Scanner");

  // configure the I2C communication
  Wire.begin();
}

void loop() {
  Serial.print("I2C addresses found:");
  for (uint8_t address = 1 ; address < 127 ; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.printf(" 0x%X", address);
    }
    delay(50);
  }
  Serial.println();

  delay(1000);
}
