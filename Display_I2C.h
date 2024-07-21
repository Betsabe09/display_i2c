#ifndef DISPLAY_I2C_H
#define DISPLAY_I2C_H

#include "mbed.h"

// Dirección del PCF8574T para Arduino ID 0x27 para mbed 0x4E
#define LCD_ADDR 0x4E

class Display_I2C {
public:
    // Constructor que inicializa el I2C y establece la dirección del LCD
    Display_I2C(PinName sda, PinName scl, uint8_t addr = LCD_ADDR);
    
    // Inicializa el LCD con la configuración básica
    void initialize();
    
    // Imprime una cadena de texto en el LCD
    void print(const char* str);
    
    // Limpia el contenido del LCD
    void clear();
    
    // Establece la posición del cursor en el LCD
    void setCursor(uint8_t col, uint8_t row);
    
    // Establece el estado del backlight (encendido o apagado)
    void setBacklight(bool state);

private:
    I2C _i2c;        // Objeto I2C para comunicación
    uint8_t _addr;   // Dirección del LCD
    uint8_t _backlight; // Estado del backlight
    
    // Envía un byte de datos al LCD
    void send(uint8_t data, bool mode);
    
    // Envía un comando al LCD
    void command(uint8_t cmd);
    
    // Envía un byte de datos al LCD como texto
    void write(uint8_t data);
    
    // Función para esperar un tiempo específico en milisegundos
    void delayMs(int ms);
    
    // Envía un nibble de 4 bits al LCD (parte de la comunicación de 8 bits)
    void sendNibble(uint8_t nibble);
};

#endif // DISPLAY_I2C_H
