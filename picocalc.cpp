#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"

// SPI Defines for LCD
#define SPI_LCD spi1
#define PIN_SCK  10
#define LCD_MISO 11
#define LCD_MOSI 12
#define LCD_CS   13
#define LCD_DC   14
#define LCD_RST  15

int main() {
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // SPI initialisation. 75mhz is apparently the sweet spot here
    spi_init(SPI_LCD, 75000*1000);
    gpio_set_function(LCD_MISO, GPIO_FUNC_SPI);
    gpio_set_function(LCD_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI, GPIO_FUNC_SPI);
    
    // Chip select and reset are active-low, so initialise it to a driven-high state
    gpio_set_dir(LCD_CS,  GPIO_OUT);
    gpio_set_dir(LCD_RST, GPIO_OUT);
    gpio_put(LCD_CS,  1);
    gpio_put(LCD_RST, 1);

    

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void write_disp(uint8_t data, bool dcx){
    if(&dcx) gpio_put(LCD_DC, 1);
    gpio_put(LCD_CS, 1);
    spi_write_blocking(SPI_LCD, &data, 8);
    gpio_put(LCD_DC, 0);
    gpio_put(LCD_CS, 0);
}

void init_disp(){
    write_disp(0x53, false);
    write_disp(0b00000100, true);
}
