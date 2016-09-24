/*
 * This file is part of RaceFlight.
 *
 * RaceFlight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RaceFlight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RaceFlight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once


#include "drivers/io.h"
#include "platform.h"

typedef struct sonarHardware_s {
    uint16_t trigger_pin;
    GPIO_TypeDef* trigger_gpio;
    uint16_t echo_pin;
    GPIO_TypeDef* echo_gpio;
    ioTag_t triggerIO;
    ioTag_t echoIO;
} sonarHardware_t;

typedef struct sonarRange_s {
    int16_t maxRangeCm;
    
    int16_t detectionConeDeciDegrees; 
    int16_t detectionConeExtendedDeciDegrees; 
} sonarRange_t;

#define SONAR_GPIO GPIOB

#define HCSR04_MAX_RANGE_CM 400 
#define HCSR04_DETECTION_CONE_DECIDEGREES 300 
#define HCSR04_DETECTION_CONE_EXTENDED_DECIDEGREES 450 

void hcsr04_init(const sonarHardware_t *sonarHardware, sonarRange_t *sonarRange);
void hcsr04_start_reading(void);
int32_t hcsr04_get_distance(void);