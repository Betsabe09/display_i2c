#include "mbed.h"
#include Display_I2C.h"

// Definiciones privadas
#define DISPLAY_IR_CLEAR_DISPLAY   0b00000001
#define DISPLAY_IR_ENTRY_MODE_SET  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL 0b00001000
#define DISPLAY_IR_FUNCTION_SET    0b00100000
#define DISPLAY_IR_SET_DDRAM_ADDR  0b10000000

#define DISPLAY_IR_ENTRY_MODE_SET_INCREMENT 0b00000010
#define DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT  0b00000000

#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON  0b00000100
#define DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_OFF 0b00000000
#define DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF  0b00000000

#define DISPLAY_IR_FUNCTION_SET_4BITS    0b00000000
#define DISPLAY_IR_FUNCTION_SET_2LINES   0b00001000
#define DISPLAY_IR_FUNCTION_SET_5x8DOTS  0b00000000

#define DISPLAY_PIN_RS  0
#define DISPLAY_PIN_EN  1
#define DISPLAY_PIN_D4  2
#define DISPLAY_PIN_D5  3
#define DISPLAY_PIN_D6  4
#define DISPLAY_PIN_D7  5

#define PCF8574_I2C_ADDRESS 0x27

// Declaración de objetos globales
I2C i2c(PB_9, PB_8);  // SDA, SCL
static display_t display;
static bool initialized = false;

// Función para escribir un comando en el display
static void displayWriteCommand(uint8_t command) {
    uint8_t data = command;
    i2c.write(PCF8574_I2C_ADDRESS, (const char*)&data, 1);
}

// Función para escribir datos en el display
static void displayWriteData(uint8_t data) {
    uint8_t cmd = 0x01 | data;  // Ejemplo de forma de enviar datos
    i2c.write(PCF8574_I2C_ADDRESS, (const char*)&cmd, 1);
}

// Inicialización del display
void displayInit(displayConnection_t connection) {
    display.connection = connection;
    i2c.frequency(100000); // Configura la frecuencia del I2C

    // Configura el display en modo de 4 bits, 2 líneas
    displayWriteCommand(DISPLAY_IR_FUNCTION_SET | DISPLAY_IR_FUNCTION_SET_4BITS | DISPLAY_IR_FUNCTION_SET_2LINES | DISPLAY_IR_FUNCTION_SET_5x8DOTS);
    displayWriteCommand(DISPLAY_IR_DISPLAY_CONTROL | DISPLAY_IR_DISPLAY_CONTROL_DISPLAY_ON | DISPLAY_IR_DISPLAY_CONTROL_CURSOR_OFF);
    displayWriteCommand(DISPLAY_IR_CLEAR_DISPLAY);
    displayWriteCommand(DISPLAY_IR_ENTRY_MODE_SET | DISPLAY_IR_ENTRY_MODE_SET_INCREMENT | DISPLAY_IR_ENTRY_MODE_SET_NO_SHIFT);

    initialized = true;
}

// Establece la posición del cursor en el display
void displayCharPositionWrite(uint8_t charPositionX, uint8_t charPositionY) {
    uint8_t address = 0x80 | (charPositionY * 0x40 + charPositionX);
    displayWriteCommand(address);
}

// Escribe una cadena de caracteres en el display
void displayStringWrite(const char *str) {
    while (*str) {
        displayWriteData(*str++);
    }
}

