#include "driver.h"
#include "xgpiops.h"
#include "xparameters.h"
#include "xspips.h"
#include <unistd.h>


#define SSD1306_Reset_Pin 54
#define SSD1306_CS_Pin 55
#define SSD1306_DC_Pin 56

XGpioPs_Config *GpioConfigPtr;
XGpioPs GpioInstance;

XSpiPs_Config *SpiConfigPtr;
XSpiPs SpiInstance;

int driver_init(void) {
  int Status;
  /* Initialize the Gpio driver. */
  GpioConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_BASEADDR);
  if (GpioConfigPtr == NULL) {
    return XST_FAILURE;
  }

  Status = XGpioPs_CfgInitialize(&GpioInstance, GpioConfigPtr,
                                 GpioConfigPtr->BaseAddr);
  if (Status != XST_SUCCESS) {
    return XST_FAILURE;
  }

  /* Set the direction for the specified pin to be output */
  /*1 for Output Direction, 0 for Input Direction */

  XGpioPs_SetDirectionPin(&GpioInstance, SSD1306_Reset_Pin, 0x1);
  XGpioPs_SetDirectionPin(&GpioInstance, SSD1306_CS_Pin, 0x1);
  XGpioPs_SetDirectionPin(&GpioInstance, SSD1306_DC_Pin, 0x1);

  /*Set the Output Enable of the specified pin.*/
  /* 1 for Enabling Output Enable. 0 for Disabling Output Enable*/
  XGpioPs_SetOutputEnablePin(&GpioInstance, SSD1306_Reset_Pin, 0x1);
  XGpioPs_SetOutputEnablePin(&GpioInstance, SSD1306_CS_Pin, 0x1);
  XGpioPs_SetOutputEnablePin(&GpioInstance, SSD1306_DC_Pin, 0x1);

  /*
   * Initialize the SPI driver so that it's ready to use
   */

  SpiConfigPtr = XSpiPs_LookupConfig(XPAR_XSPIPS_0_BASEADDR);
  if (NULL == SpiConfigPtr) {
    return XST_FAILURE;
  }

  Status = XSpiPs_CfgInitialize(&SpiInstance, SpiConfigPtr,
                                SpiConfigPtr->BaseAddress);
  if (Status != XST_SUCCESS) {
    return XST_FAILURE;
  }

  /*
   * Set the Spi device as a master
   */
  XSpiPs_SetOptions(&SpiInstance, XSPIPS_MASTER_OPTION);

  /*
   * Set the Spi clock rate: 166.67 MHz/XSPIPS_CLK_PRESCALE_32 = 5.2 MHz
   */

  XSpiPs_SetClkPrescaler(&SpiInstance, XSPIPS_CLK_PRESCALE_32);

  return XST_SUCCESS;
}

void ssd1306_Reset(void) {
  // CS = High (not selected)

  /*Write data to the specified pin.*/
  XGpioPs_WritePin(&GpioInstance, SSD1306_CS_Pin, 0x1);

  // Reset the OLED
  XGpioPs_WritePin(&GpioInstance, SSD1306_Reset_Pin, 0x0);

  usleep(10000);

  XGpioPs_WritePin(&GpioInstance, SSD1306_Reset_Pin, 0x1);

  usleep(10000);
}

// Send a byte to the command register
void ssd1306_WriteCommand(uint8_t byte) {

  // select OLED
  XGpioPs_WritePin(&GpioInstance, SSD1306_CS_Pin, 0x0);

  // command
  XGpioPs_WritePin(&GpioInstance, SSD1306_DC_Pin, 0x0);

  /* Transfers specified data on the SPI bus in polled mode. */
  /* No receive buffer is specified since there is nothing toreceive*/
  XSpiPs_PolledTransfer(&SpiInstance, &byte, NULL, 1);

  XGpioPs_WritePin(&GpioInstance, SSD1306_CS_Pin, 0x1);
  // un-select OLED
}

// Send data
void ssd1306_WriteData(uint8_t *buffer, size_t buff_size) {

  // select OLED
  XGpioPs_WritePin(&GpioInstance, SSD1306_CS_Pin, 0x0);

  // data
  XGpioPs_WritePin(&GpioInstance, SSD1306_DC_Pin, 0x1);

  XSpiPs_PolledTransfer(&SpiInstance, buffer, NULL, buff_size);

  XGpioPs_WritePin(&GpioInstance, SSD1306_CS_Pin, 0x1);
  // un-select OLED
}