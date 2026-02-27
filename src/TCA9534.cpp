#include <stdint.h>
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

// ----------------------------------------
// Libraries

#include "TCA9534.h"

// ----------------------------------------
// ----------------------------------------

// Constructor
// @param (i2c) : the I2C/TWI bus used for communication [TwoWire]
// @param (address) : the I2C address of the device [uint8_t] (default 0x20)
TCA9534::TCA9534(TwoWire & i2c, const uint8_t address) :
#ifdef TCA_DEBUG
  _debugger(nullptr),
#endif
  _address(address),
  _error(false),
  _initialized(false),
  _mask_config(0x00),
  _mask_output(0x00)
{
  this->_i2c = &i2c;
}

// ----------------------------------------
// ----------------------------------------

// Configure the TCA
// @param (mask_config) : the configuration mask (bitwise) [uint8_t] (default 0x00)
//                        1=output / 0=input - Opposite of the register, to set by default as input.
//                        (Bitwise operations are easier with 1 instead of 0, therefore it is better to choose the outputs with 1.)
// @param (mask_output) : the output mask, when applicable (bitwise) [uint8_t] (default 0x00)
//                        Set the initial states of the outputs (0=LOW / 1=HIGH).
// @param (force) : true to force the initialization [bool] (default false)
// @returns true if successful [bool]
// Note: the I2C/TWI bus must be initialized before configuring the object.
// Note: the force feature shouldn't be necessary, but it might be helpful in some cases.
bool TCA9534::config(uint8_t mask_config, uint8_t mask_output, bool force) {
  // check if already initialized
  if (this->_initialized) {
    if (force) {
      this->_initialized = false; // reset
    } else {
      return true;
    }
  }

  // Note (for future reference)
  // <Wire.begin()> could be used to make sure the I2C communication is
  // configured and active, but it might be problematic if several calls
  // to <Wire.begin()> would occur in the same program, as it resets
  // the driver. Instead, the library uses a reference to the bus and
  // tries to communicate over it as master.

  this->_error = true; // default

  // update the configuration mask
  // 0=output / 1=input
  mask_config = ~mask_config;

  // configure the GPIO
  this->_i2c->beginTransmission(this->_address);
  this->_i2c->write(TCA_REGISTER_CONFIG);
  this->_i2c->write(mask_config);
  if (this->_i2c->endTransmission() == 0) {
    this->_initialized = true; // set
    this->_mask_config = mask_config; // update
    
    // check for all inputs
    if (mask_config == 0xFF){
      this->_error = false; // reset
    } else {
      mask_output &= ~mask_config; // only set the outputs (mainly for debug)

      // send the data
      this->_i2c->beginTransmission(this->_address);
      this->_i2c->write(TCA_REGISTER_OUTPUT);
      this->_i2c->write(mask_output);
      if (this->_i2c->endTransmission() == 0) {
        this->_error = false; // reset
        this->_mask_output = mask_output; // update
      }
    }
  } else {
#ifdef TCA_DEBUG
    if (this->_debugger != nullptr) {
      this->_debugger->println("Couldn't access the TCA");
    }
#endif // TCA_DEBUG
  }

  return this->_initialized;
}

// ----------------------------------------

// Check if there is an error flagged
// @returns true if there is an error [bool]
bool TCA9534::error(void){
  return this->_error;
}


// ----------------------------------------

// Map a pin to a mask
// @param (pin) : the pin, from 0 to 7 [uint8_t]
// @returns the mask or 0x00 on error [uint8_t]
uint8_t TCA9534::map_pin(const uint8_t pin){
  // check the upper limit
  if (pin > 7){
    return 0x00;
  }

  return (1 << pin);
}

// ----------------------------------------

// Read the inputs
// @param (mask) : the input mask to read (bitwise) [uint8_t] (default 0xFF)
//                 The configuration mask (input/output) is automatically applied,
//                 but a second input mask can also be used.
// @returns the state of the inputs (bitwise) or TCA_ERROR on error [uint8_t]
uint8_t TCA9534::read(const uint8_t mask) {
  // Notes on the error value
  //  - 0 doesn't work as a return value because it also means no input.
  //  - 0xFF can work as a return value, but it can be intepreted as all inputs (bitwise).
  //  - -1 can work as a return value, but it can be intepreted as all inputs (bitwise).
  //  - TCA_ERROR is an option to keep the value as [uint8_t], but with an error flag.
  
  this->_error = true; // default

  // check if initialized
  if (!this->_initialized) {
    return TCA_ERROR;
  }

  // get the data
  uint8_t data = TCA_ERROR;
  this->_i2c->beginTransmission(this->_address);
  this->_i2c->write(TCA_REGISTER_INPUT);
  if (this->_i2c->endTransmission() == 0) {
    if (this->_i2c->requestFrom(this->_address, static_cast<uint8_t>(1)) > 0) {
      this->_error = false; // reset
      data = this->_i2c->read();
      data &= this->_mask_config;
      data &= mask;
    }
  }

  // check for error
  if (this->_error) {
#ifdef TCA_DEBUG
    if (this->_debugger != nullptr) {
      this->_debugger->println("Couldn't read the TCA");
    }
#endif // TCA_DEBUG
  }

  return data;
}

// ----------------------------------------

// Read the inputs
// @param (pin) : the pin to read, from 0 to 7 [uint8_t]
// @returns the state of the input (HIGH/LOW) or TCA_ERROR on error [uint8_t]
uint8_t TCA9534::read_pin(const uint8_t pin) {
  // get the corresponding mask
  uint8_t mask = this->map_pin(pin);
  if (mask == 0x00){
    return TCA_ERROR;
  }

  // read the pin
  uint8_t data = this->read(mask);

  // check for error
  if (this->_error) {
#ifdef TCA_DEBUG
    if (this->_debugger != nullptr) {
      this->_debugger->println("Error while reading the pin");
    }
#endif // TCA_DEBUG
  } else if (data != TCA_ERROR) {
    // convert the result to a more common value
    if (data > 0){
      data = HIGH;
    } else {
      data = LOW;
    }
  }

  return data;
}

// ----------------------------------------

// Set the output pins
// @param (mask) : the output mask to set (bitwise) [uint8_t]
// @returns true if successful [bool]
bool TCA9534::set(uint8_t mask){
  
  this->_error = true; // default

  // check if already initialized
  if (!this->_initialized) {
    return false;
  }

  // update the mask
  mask &= ~this->_mask_config;

  // send the data
  this->_i2c->beginTransmission(this->_address);
  this->_i2c->write(TCA_REGISTER_OUTPUT);
  this->_i2c->write(mask);
  if (this->_i2c->endTransmission() == 0) {
    this->_error = false;
    this->_mask_output = mask; // update
  }

  return !this->_error;
}

// ----------------------------------------

// Set the debugger of the object
// @param (stream) : the stream to print to [Stream &]
#ifdef TCA_DEBUG
void TCA9534::set_debugger(Stream & stream) {
  this->_debugger = &stream;
}
#endif // TCA_DEBUG

// ----------------------------------------

// Set an output pin
// @param (pin) : the pin to set, from 0 to 7 [uint8_t]
// @returns true if successful [bool]
bool TCA9534::set_pin(const uint8_t pin, const uint8_t value){
  // get the corresponding mask
  uint8_t mask = this->map_pin(pin);
  if (mask == 0x00){
    return false;
  }

  // update the output mask, considering the other output pins
  if (value == LOW) {
    mask = ~mask;
    mask &= this->_mask_output;
  } else {
    mask |= this->_mask_output;
  }

  return this->set(mask);
}

// ----------------------------------------
