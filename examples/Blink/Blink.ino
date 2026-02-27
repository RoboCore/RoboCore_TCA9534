/*******************************************************************************
* RoboCore TCA9534 Library (v1.0)
* 
* Simple program to blink a LED with the TCA9534.
* The LED must be connected to the pin P0 of the TCA9534.
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

const uint16_t DELAY_TIME = 1000; // [ms]
const uint8_t PIN_LED = 0; // pin P0

const uint8_t TCA_PIN = TCA9534::map_pin(PIN_LED);

void setup() {
  // configure the serial communication
  Serial.begin(115200);

  // configure the I2C communication
  Wire.begin();

  // configure the TCA
  Serial.print("TCA config: ");
  Serial.println(tca.config(TCA_PIN));
}

void loop() {
  tca.set_pin(PIN_LED, HIGH);
  delay(DELAY_TIME);
  tca.set_pin(PIN_LED, LOW);
  delay(DELAY_TIME);
}
