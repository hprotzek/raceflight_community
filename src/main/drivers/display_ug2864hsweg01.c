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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"

#include "bus_i2c.h"
#include "system.h"

#include "display_ug2864hsweg01.h"

#ifndef OLED_I2C_INSTANCE
#define OLED_I2C_INSTANCE I2CDEV_1
#endif

#define INVERSE_CHAR_FORMAT 0x7f 
#define NORMAL_CHAR_FORMAT  0x00 

unsigned char CHAR_FORMAT = NORMAL_CHAR_FORMAT;

static const uint8_t multiWiiFont[][5] = { 
        { 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x4F, 0x00, 0x00 }, 
                { 0x00, 0x07, 0x00, 0x07, 0x00 }, 
                { 0x14, 0x7F, 0x14, 0x7F, 0x14 }, 
                { 0x24, 0x2A, 0x7F, 0x2A, 0x12 }, 
                { 0x23, 0x13, 0x08, 0x64, 0x62 }, 
                { 0x36, 0x49, 0x55, 0x22, 0x50 }, 
                { 0x00, 0x05, 0x03, 0x00, 0x00 }, 
                { 0x00, 0x1C, 0x22, 0x41, 0x00 }, 
                { 0x00, 0x41, 0x22, 0x1C, 0x00 }, 
                { 0x14, 0x08, 0x3E, 0x08, 0x14 }, 
                { 0x08, 0x08, 0x3E, 0x08, 0x08 }, 
                { 0x00, 0x50, 0x30, 0x00, 0x00 }, 
                { 0x08, 0x08, 0x08, 0x08, 0x08 }, 
                { 0x00, 0x60, 0x60, 0x00, 0x00 }, 
                { 0x20, 0x10, 0x08, 0x04, 0x02 }, 
                { 0x3E, 0x51, 0x49, 0x45, 0x3E }, 
                { 0x00, 0x42, 0x7F, 0x40, 0x00 }, 
                { 0x42, 0x61, 0x51, 0x49, 0x46 }, 
                { 0x21, 0x41, 0x45, 0x4B, 0x31 }, 
                { 0x18, 0x14, 0x12, 0x7F, 0x10 }, 
                { 0x27, 0x45, 0x45, 0x45, 0x39 }, 
                { 0x3C, 0x4A, 0x49, 0x49, 0x30 }, 
                { 0x01, 0x71, 0x09, 0x05, 0x03 }, 
                { 0x36, 0x49, 0x49, 0x49, 0x36 }, 
                { 0x06, 0x49, 0x49, 0x29, 0x1E }, 
                { 0x00, 0x36, 0x36, 0x00, 0x00 }, 
                { 0x00, 0x56, 0x36, 0x00, 0x00 }, 
                { 0x08, 0x14, 0x22, 0x41, 0x00 }, 
                { 0x14, 0x14, 0x14, 0x14, 0x14 }, 
                { 0x00, 0x41, 0x22, 0x14, 0x08 }, 
                { 0x02, 0x01, 0x51, 0x09, 0x06 }, 
                { 0x32, 0x49, 0x79, 0x41, 0x3E }, 
                { 0x7E, 0x11, 0x11, 0x11, 0x7E }, 
                { 0x7F, 0x49, 0x49, 0x49, 0x36 }, 
                { 0x3E, 0x41, 0x41, 0x41, 0x22 }, 
                { 0x7F, 0x41, 0x41, 0x22, 0x1C }, 
                { 0x7F, 0x49, 0x49, 0x49, 0x41 }, 
                { 0x7F, 0x09, 0x09, 0x09, 0x01 }, 
                { 0x3E, 0x41, 0x49, 0x49, 0x7A }, 
                { 0x7F, 0x08, 0x08, 0x08, 0x7F }, 
                { 0x00, 0x41, 0x7F, 0x41, 0x00 }, 
                { 0x20, 0x40, 0x41, 0x3F, 0x01 }, 
                { 0x7F, 0x08, 0x14, 0x22, 0x41 }, 
                { 0x7F, 0x40, 0x40, 0x40, 0x40 }, 
                { 0x7F, 0x02, 0x0C, 0x02, 0x7F }, 
                { 0x7F, 0x04, 0x08, 0x10, 0x7F }, 
                { 0x3E, 0x41, 0x41, 0x41, 0x3E }, 
                { 0x7F, 0x09, 0x09, 0x09, 0x06 }, 
                { 0x3E, 0x41, 0x51, 0x21, 0x5E }, 
                { 0x7F, 0x09, 0x19, 0x29, 0x46 }, 
                { 0x46, 0x49, 0x49, 0x49, 0x31 }, 
                { 0x01, 0x01, 0x7F, 0x01, 0x01 }, 
                { 0x3F, 0x40, 0x40, 0x40, 0x3F }, 
                { 0x1F, 0x20, 0x40, 0x20, 0x1F }, 
                { 0x3F, 0x40, 0x38, 0x40, 0x3F }, 
                { 0x63, 0x14, 0x08, 0x14, 0x63 }, 
                { 0x07, 0x08, 0x70, 0x08, 0x07 }, 
                { 0x61, 0x51, 0x49, 0x45, 0x43 }, 
                { 0x00, 0x7F, 0x41, 0x41, 0x00 }, 
                { 0x02, 0x04, 0x08, 0x10, 0x20 }, 
                { 0x00, 0x41, 0x41, 0x7F, 0x00 }, 
                { 0x04, 0x02, 0x01, 0x02, 0x04 }, 
                { 0x40, 0x40, 0x40, 0x40, 0x40 }, 
                { 0x01, 0x02, 0x04, 0x00, 0x00 }, 
                { 0x20, 0x54, 0x54, 0x54, 0x78 }, 
                { 0x7F, 0x48, 0x44, 0x44, 0x38 }, 
                { 0x38, 0x44, 0x44, 0x44, 0x20 }, 
                { 0x38, 0x44, 0x44, 0x48, 0x7F }, 
                { 0x38, 0x54, 0x54, 0x54, 0x18 }, 
                { 0x08, 0x7E, 0x09, 0x01, 0x02 }, 
                { 0x06, 0x49, 0x49, 0x49, 0x3F }, 
                { 0x7F, 0x08, 0x04, 0x04, 0x78 }, 
                { 0x00, 0x44, 0x7D, 0x40, 0x00 }, 
                { 0x20, 0x40, 0x44, 0x3D, 0x00 }, 
                { 0x7F, 0x10, 0x28, 0x44, 0x00 }, 
                { 0x00, 0x41, 0x7F, 0x40, 0x00 }, 
                { 0x7C, 0x04, 0x18, 0x04, 0x7C }, 
                { 0x7C, 0x08, 0x04, 0x04, 0x78 }, 
                { 0x38, 0x44, 0x44, 0x44, 0x38 }, 
                { 0x7C, 0x14, 0x14, 0x14, 0x08 }, 
                { 0x08, 0x14, 0x14, 0x18, 0x7C }, 
                { 0x7C, 0x08, 0x04, 0x04, 0x08 }, 
                { 0x48, 0x54, 0x54, 0x54, 0x20 }, 
                { 0x04, 0x3F, 0x44, 0x40, 0x20 }, 
                { 0x3C, 0x40, 0x40, 0x20, 0x7C }, 
                { 0x1C, 0x20, 0x40, 0x20, 0x1C }, 
                { 0x3C, 0x40, 0x30, 0x40, 0x3C }, 
                { 0x44, 0x28, 0x10, 0x28, 0x44 }, 
                { 0x0C, 0x50, 0x50, 0x50, 0x3C }, 
                { 0x44, 0x64, 0x54, 0x4C, 0x44 }, 
                { 0x00, 0x08, 0x36, 0x41, 0x00 }, 
                { 0x00, 0x00, 0x7F, 0x00, 0x00 }, 
                { 0x00, 0x41, 0x36, 0x08, 0x00 }, 
                { 0x02, 0x01, 0x02, 0x04, 0x02 }, 
                { 0x3E, 0x55, 0x55, 0x41, 0x22 }, 
                { 0x00, 0x00, 0x00, 0x00, 0x00 }, 
                { 0x00, 0x00, 0x79, 0x00, 0x00 }, 
                { 0x18, 0x24, 0x74, 0x2E, 0x24 }, 
                { 0x48, 0x7E, 0x49, 0x42, 0x40 }, 
                { 0x5D, 0x22, 0x22, 0x22, 0x5D }, 
                { 0x15, 0x16, 0x7C, 0x16, 0x15 }, 
                { 0x00, 0x00, 0x77, 0x00, 0x00 }, 
                { 0x0A, 0x55, 0x55, 0x55, 0x28 }, 
                { 0x00, 0x01, 0x00, 0x01, 0x00 }, 
                { 0x00, 0x0A, 0x0D, 0x0A, 0x04 }, 
                { 0x08, 0x14, 0x2A, 0x14, 0x22 }, 
                { 0x04, 0x04, 0x04, 0x04, 0x1C }, 
                { 0x00, 0x08, 0x08, 0x08, 0x00 }, 
                { 0x01, 0x01, 0x01, 0x01, 0x01 }, 
                { 0x00, 0x02, 0x05, 0x02, 0x00 }, 
                { 0x44, 0x44, 0x5F, 0x44, 0x44 }, 
                { 0x00, 0x00, 0x04, 0x02, 0x01 }, 
                { 0x7E, 0x20, 0x20, 0x10, 0x3E }, 
                { 0x06, 0x0F, 0x7F, 0x00, 0x7F }, 
                { 0x00, 0x18, 0x18, 0x00, 0x00 }, 
                { 0x00, 0x40, 0x50, 0x20, 0x00 }, 
                { 0x00, 0x0A, 0x0D, 0x0A, 0x00 }, 
                { 0x22, 0x14, 0x2A, 0x14, 0x08 }, 
                { 0x17, 0x08, 0x34, 0x2A, 0x7D }, 
                { 0x17, 0x08, 0x04, 0x6A, 0x59 }, 
                { 0x30, 0x48, 0x45, 0x40, 0x20 }, 
                { 0x42, 0x00, 0x42, 0x00, 0x42 }, 
                { 0x7E, 0x42, 0x00, 0x42, 0x00 }, 
                { 0x7E, 0x7E, 0x00, 0x42, 0x00 }, 
                { 0x7E, 0x7E, 0x7E, 0x42, 0x00 }, 
                { 0x7E, 0x7E, 0x7E, 0x7E, 0x00 }, 
                { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E }, 
                { 0x5A, 0x00, 0x00, 0x00, 0x5A }, 
                { 0x5A, 0x40, 0x40, 0x40, 0x5A }, 
                { 0x7A, 0x60, 0x60, 0x60, 0x7A }, 
                { 0x7A, 0x70, 0x70, 0x70, 0x7A }, 
                { 0x7A, 0x78, 0x78, 0x78, 0x7A }, 
                { 0x7A, 0x7C, 0x7C, 0x7C, 0x7A }, 
                { 0x7A, 0x7E, 0x7E, 0x7E, 0x7A }, 
        };

#define OLED_address   0x3C     

static bool i2c_OLED_send_cmd(uint8_t command)
{
    return i2cWrite(OLED_I2C_INSTANCE, OLED_address, 0x80, command);
}

static bool i2c_OLED_send_byte(uint8_t val)
{
    return i2cWrite(OLED_I2C_INSTANCE, OLED_address, 0x40, val);
}

void i2c_OLED_clear_display(void)
{
    i2c_OLED_send_cmd(0xa6);              
    i2c_OLED_send_cmd(0xae);              
    i2c_OLED_send_cmd(0x20);              
    i2c_OLED_send_cmd(0x00);              
    i2c_OLED_send_cmd(0xb0);              
    i2c_OLED_send_cmd(0x40);              
    i2c_OLED_send_cmd(0);                 
    i2c_OLED_send_cmd(0x10);              
    for(uint16_t i = 0; i < 1024; i++) {  
        i2c_OLED_send_byte(0x00);  
    }
    i2c_OLED_send_cmd(0x81);              
    i2c_OLED_send_cmd(200);               
    i2c_OLED_send_cmd(0xaf);              
}

void i2c_OLED_clear_display_quick(void)
{
    i2c_OLED_send_cmd(0xb0);              
    i2c_OLED_send_cmd(0x40);              
    i2c_OLED_send_cmd(0);                 
    i2c_OLED_send_cmd(0x10);              
    for(uint16_t i = 0; i < 1024; i++) {      
        i2c_OLED_send_byte(0x00);  
    }
}

void i2c_OLED_set_xy(uint8_t col, uint8_t row)
{
    i2c_OLED_send_cmd(0xb0 + row);                      
    i2c_OLED_send_cmd(0x00 + ((CHARACTER_WIDTH_TOTAL * col) & 0x0f));         
    i2c_OLED_send_cmd(0x10 + (((CHARACTER_WIDTH_TOTAL * col) >> 4) & 0x0f));  
}

void i2c_OLED_set_line(uint8_t row)
{
    i2c_OLED_send_cmd(0xb0 + row); 
    i2c_OLED_send_cmd(0);          
    i2c_OLED_send_cmd(0x10);       
}

void i2c_OLED_send_char(unsigned char ascii)
{
    unsigned char i;
    uint8_t buffer;
    for (i = 0; i < 5; i++) {
        buffer = multiWiiFont[ascii - 32][i];
        buffer ^= CHAR_FORMAT;  
        i2c_OLED_send_byte(buffer);
    }
    i2c_OLED_send_byte(CHAR_FORMAT);    
}

void i2c_OLED_send_string(const char *string)
{
    
    while (*string) {
        i2c_OLED_send_char(*string);
        string++;
    }
}

/**
* according to http:
*/
#if 1
bool ug2864hsweg01InitI2C(void)
{

    
    if (!i2c_OLED_send_cmd(0xAE)) {
        return false;
    }

    i2c_OLED_send_cmd(0xD4); 
    i2c_OLED_send_cmd(0x80); 
    i2c_OLED_send_cmd(0xA8); 
    i2c_OLED_send_cmd(0x3F); 
    i2c_OLED_send_cmd(0xD3); 
    i2c_OLED_send_cmd(0x00); 
    i2c_OLED_send_cmd(0x40); 
    i2c_OLED_send_cmd(0x8D); 
    i2c_OLED_send_cmd(0x14); 
    i2c_OLED_send_cmd(0xA1); 
    i2c_OLED_send_cmd(0xC8); 
    i2c_OLED_send_cmd(0xDA); 
    i2c_OLED_send_cmd(0x12); 
    i2c_OLED_send_cmd(0x81); 
    i2c_OLED_send_cmd(0xCF); 
    i2c_OLED_send_cmd(0xD9); 
    i2c_OLED_send_cmd(0xF1); 
    i2c_OLED_send_cmd(0xDB); 
    i2c_OLED_send_cmd(0x40); 
    i2c_OLED_send_cmd(0xA4); 
    i2c_OLED_send_cmd(0xA6); 
    i2c_OLED_send_cmd(0xAF); 

    i2c_OLED_clear_display();

    return true;
}
#else
void ug2864hsweg01InitI2C(void)
{
    i2c_OLED_send_cmd(0xae);    
    i2c_OLED_send_cmd(0xa4);          

    delay(50);



    i2c_OLED_send_cmd(0x20);            
    i2c_OLED_send_cmd(0x02); 

    i2c_OLED_send_cmd(0xa1); 

    i2c_OLED_send_cmd(0xC8); 
    i2c_OLED_send_cmd(0xa6);            

    i2c_OLED_send_cmd(0x81); 
    i2c_OLED_send_cmd(0xaf); 





#ifdef DISPLAY_FONT_DSIZE
    i2c_OLED_send_cmd(0xd6);            
    i2c_OLED_send_cmd(0x01);
#else


#endif
    delay(20);
    i2c_OLED_send_cmd(0xaf);          
    delay(20);
    i2c_OLED_clear_display();
}

#endif