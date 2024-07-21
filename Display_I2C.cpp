#include "Display_I2C.h"

// Constructor que inicializa el I2C y establece la dirección del LCD
Display_I2C::Display_I2C(PinName sda, PinName scl, uint8_t addr) : _i2c(sda, scl), _addr(addr), _backlight(0) {
    _i2c.frequency(100000); // Configura la frecuencia I2C a 100 kHz
}

// Inicializa el LCD con la configuración básica
void Display_I2C::initialize() {
    command(0x33); // Inicializa el LCD en modo de 4 bits
    command(0x32); // Configura el LCD en modo de 4 bits
    command(0x28); // Configura el LCD en modo de 4 bits, 2 líneas, 5x8 puntos
    command(0x0C); // Enciende el cursor
    command(0x06); // Configura el cursor para incrementar automáticamente
    command(0x01); // Limpia el LCD
    delayMs(5);    // Espera para permitir que el LCD procese el comando

    // Configura el backlight al iniciar
    setBacklight(true);
}

// Imprime una cadena de texto en el LCD
void Display_I2C::print(const char* str) {
    while (*str) {
        write(*str++); // Escribe cada carácter en el LCD
    }
}

// Limpia el contenido del LCD
void Display_I2C::clear() {
    command(0x01); // Envía el comando de limpieza
    delayMs(5);    // Espera para permitir que el LCD procese el comando
}

// Establece la posición del cursor en el LCD
void Display_I2C::setCursor(uint8_t col, uint8_t row) {
    uint8_t address;
    if (row == 0) {
        address = 0x80 + col; // Primera línea
    } else {
        address = 0xC0 + col; // Segunda línea
    }
    command(address); // Envía el comando para mover el cursor
}

// Establece el estado del backlight (encendido o apagado)
void Display_I2C::setBacklight(bool state) {
    _backlight = state ? 0x08 : 0x00; // Ajusta el bit del backlight
    // Reenvía todos los datos para incluir el estado del backlight
    command(0x00); // Puede requerir ajustar el comando para tus necesidades
}

// Envía un byte de datos al LCD
void Display_I2C::send(uint8_t data, bool mode) {
    uint8_t highNibble = (data & 0xF0) | (mode ? 0x01 : 0x00) | _backlight;
    uint8_t lowNibble = ((data << 4) & 0xF0) | (mode ? 0x01 : 0x00) | _backlight;

    // Enviar el nibble alto
    _i2c.write(_addr, reinterpret_cast<char*>(&highNibble), 1, false);
    delayMs(1); // Espera para permitir que el LCD procese el nibble

    // Enviar el nibble alto con pin de habilitación
    uint8_t highNibbleEnable = highNibble | 0x04;
    _i2c.write(_addr, reinterpret_cast<char*>(&highNibbleEnable), 1, false);
    delayMs(1); // Espera para permitir que el LCD procese el nibble

    // Enviar el nibble bajo
    _i2c.write(_addr, reinterpret_cast<char*>(&lowNibble), 1, false);
    delayMs(1); // Espera para permitir que el LCD procese el nibble

    // Enviar el nibble bajo con pin de habilitación
    uint8_t lowNibbleEnable = lowNibble | 0x04;
    _i2c.write(_addr, reinterpret_cast<char*>(&lowNibbleEnable), 1, false);
    delayMs(1); // Espera para permitir que el LCD procese el nibble
}

// Envía un comando al LCD
void Display_I2C::command(uint8_t cmd) {
    send(cmd, false); // Envía el comando (sin modo de datos)
}

// Envía un byte de datos al LCD como texto
void Display_I2C::write(uint8_t data) {
    send(data, true); // Envía el dato (modo de datos activado)
}

// Función para esperar un tiempo específico en milisegundos
void Display_I2C::delayMs(int ms) {
    ThisThread::sleep_for(std::chrono::milliseconds(ms)); // Usa std::chrono para especificar la duración
}
