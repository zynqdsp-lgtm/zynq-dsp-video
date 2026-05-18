#include "driver.h"
#include "platform.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "xil_printf.h"
#include <stdio.h>
#include <unistd.h>
#include <xstatus.h>

int main() {
  init_platform();

  print("Start\n\r");

  int Status;

  Status = driver_init();
  if (Status != XST_SUCCESS) {
    printf("ERROR driver init\r\n");
  }

  ssd1306_Init();

  ssd1306_DrawRectangle(2, 2, 126, 62, White);

  ssd1306_SetCursor(32, 20);

  ssd1306_WriteString("ZYNQ", Font_16x26, White);

  ssd1306_UpdateScreen();

  sleep(3);

  ssd1306_Fill(Black);

  char buffer[10];

  for (int i = 0; i < 1000; i++) {

    sprintf(buffer, "%03d", i);
    ssd1306_SetCursor(37, 20);
    ssd1306_WriteString(buffer, Font_16x26, White);
    ssd1306_UpdateScreen();
    usleep(100000);
  }

  print("Done\n\r");

  cleanup_platform();
  return 0;
}
