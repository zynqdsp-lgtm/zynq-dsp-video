#ifndef DRIVER_H
#define DRIVER_H

#include <stddef.h>
#include <stdint.h>


int driver_init(void);
void ssd1306_Reset(void);
void ssd1306_WriteCommand(uint8_t byte);
void ssd1306_WriteData(uint8_t *buffer, size_t buff_size);

#endif