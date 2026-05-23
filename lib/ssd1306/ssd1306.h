#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <cstdint>
#include <cstring>

// SSD1306 commands
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_EXTERNALVCC 0x01
#define SSD1306_SWITCHCAPVCC 0x02

// Colors
#define SSD1306_BLACK 0
#define SSD1306_WHITE 1
#define SSD1306_INVERSE 2

// Compatibility aliases
#define BLACK SSD1306_BLACK
#define WHITE SSD1306_WHITE
#define INVERSE SSD1306_INVERSE

// Display size
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

class SSD1306 {
public:
    static constexpr uint8_t ADDRESS = 0x3C;
    
    SSD1306(i2c_inst_t* i2c_instance, uint8_t address = ADDRESS);
    ~SSD1306();
    
    bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0, bool reset = true);
    
    void display();
    void clearDisplay();
    void clear();
    void invertDisplay(bool i);
    void dim(bool dim);
    void setContrast(uint8_t contrast);
    
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    
    void setCursor(int16_t x, int16_t y);
    void setTextSize(uint8_t s);
    void setTextColor(uint16_t c);
    void setTextWrap(bool w);
    size_t write(uint8_t);
    void print(const char* str);
    void println(const char* str);
    
    void drawBatteryBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, float percentage);
    void drawBatteryStatus(float voltage, float percentage);
    
    void drawString(uint8_t x, uint8_t y, const char* str);
    
    uint8_t* getBuffer();
    
    int16_t width() const { return WIDTH; }
    int16_t height() const { return HEIGHT; }

private:
    i2c_inst_t* i2c_;
    uint8_t address_;
    uint8_t* buffer_;
    uint8_t vccstate_;
    uint8_t contrast_;
    
    static const int16_t WIDTH = SSD1306_LCDWIDTH;
    static const int16_t HEIGHT = SSD1306_LCDHEIGHT;
    
    int16_t cursor_x;
    int16_t cursor_y;
    uint8_t textsize;
    uint16_t textcolor;
    bool textwrap;
    
    void ssd1306_command(uint8_t c);
    void ssd1306_commandList(const uint8_t* c, uint8_t n);
    void ssd1306_data(uint8_t* data, size_t size);
    
    void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
    
    static const uint8_t font5x7[][5];
};

#endif // SSD1306_H
