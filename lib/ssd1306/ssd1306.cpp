#include "ssd1306.h"
#include <cstdio>
#include <cstdlib>
#include <algorithm>

// 5x7 font data
const uint8_t SSD1306::font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // space (32)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // ! (33)
    {0x00, 0x07, 0x00, 0x07, 0x00}, // " (34)
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // # (35)
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $ (36)
    {0x23, 0x13, 0x08, 0x64, 0x62}, // % (37)
    {0x36, 0x49, 0x55, 0x22, 0x50}, // & (38)
    {0x00, 0x05, 0x03, 0x00, 0x00}, // ' (39)
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // ( (40)
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // ) (41)
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // * (42)
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // + (43)
    {0x00, 0x00, 0xA0, 0x60, 0x00}, // , (44)
    {0x08, 0x08, 0x08, 0x08, 0x08}, // - (45)
    {0x00, 0x60, 0x60, 0x00, 0x00}, // . (46)
    {0x20, 0x10, 0x08, 0x04, 0x02}, // / (47)
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 (48)
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1 (49)
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2 (50)
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3 (51)
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4 (52)
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5 (53)
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6 (54)
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7 (55)
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8 (56)
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9 (57)
    {0x00, 0x36, 0x36, 0x00, 0x00}, // : (58)
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ; (59)
    {0x08, 0x14, 0x22, 0x41, 0x00}, // < (60)
    {0x14, 0x14, 0x14, 0x14, 0x14}, // = (61)
    {0x00, 0x41, 0x22, 0x14, 0x08}, // > (62)
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ? (63)
    {0x32, 0x49, 0x59, 0x51, 0x3E}, // @ (64)
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // A (65)
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B (66)
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C (67)
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D (68)
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E (69)
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F (70)
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G (71)
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H (72)
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I (73)
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J (74)
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K (75)
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L (76)
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M (77)
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N (78)
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O (79)
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P (80)
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q (81)
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R (82)
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S (83)
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T (84)
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U (85)
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V (86)
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W (87)
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X (88)
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y (89)
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z (90)
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // [ (91)
    {0x02, 0x04, 0x08, 0x10, 0x20}, // \ (92)
    {0x00, 0x00, 0x41, 0x41, 0x7F}, // ] (93)
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^ (94)
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _ (95)
    {0x00, 0x01, 0x02, 0x04, 0x00}, // ` (96)
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a (97)
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b (98)
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c (99)
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d (100)
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e (101)
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f (102)
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g (103)
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h (104)
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i (105)
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j (106)
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k (107)
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l (108)
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // m (109)
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n (110)
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o (111)
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p (112)
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q (113)
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r (114)
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s (115)
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t (116)
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u (117)
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v (118)
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w (119)
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x (120)
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y (121)
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z (122)
    {0x00, 0x08, 0x36, 0x41, 0x00}, // { (123)
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // | (124)
    {0x00, 0x41, 0x36, 0x08, 0x00}, // } (125)
    {0x10, 0x08, 0x08, 0x10, 0x08}, // ~ (126)
};

SSD1306::SSD1306(i2c_inst_t* i2c_instance, uint8_t address)
    : i2c_(i2c_instance), address_(address), buffer_(nullptr), vccstate_(SSD1306_SWITCHCAPVCC),
      contrast_(0x8F), cursor_x(0), cursor_y(0), textsize(1), textcolor(SSD1306_WHITE), textwrap(true) {
}

SSD1306::~SSD1306() {
    if (buffer_) {
        free(buffer_);
        buffer_ = nullptr;
    }
}

bool SSD1306::begin(uint8_t switchvcc, uint8_t i2caddr, bool reset) {
    // Allocate framebuffer
    if (!buffer_ && !(buffer_ = (uint8_t*)malloc(WIDTH * ((HEIGHT + 7) / 8)))) {
        return false;
    }
    
    clearDisplay();
    vccstate_ = switchvcc;
    
    // Override I2C address if provided
    if (i2caddr) {
        address_ = i2caddr;
    }
    
    // Init sequence
    static const uint8_t init1[] = {
        SSD1306_DISPLAYOFF,         // 0xAE
        SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
        0x80,                       // clock divide ratio
        SSD1306_SETMULTIPLEX        // 0xA8
    };
    ssd1306_commandList(init1, sizeof(init1));
    ssd1306_command(HEIGHT - 1);
    
    static const uint8_t init2[] = {
        SSD1306_SETDISPLAYOFFSET,   // 0xD3
        0x0,                        // no offset
        SSD1306_SETSTARTLINE | 0x0, // start line 0
        SSD1306_CHARGEPUMP          // 0x8D
    };
    ssd1306_commandList(init2, sizeof(init2));
    
    ssd1306_command((vccstate_ == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);
    
    static const uint8_t init3[] = {
        SSD1306_MEMORYMODE,         // 0x20
        0x00,                       // horizontal addressing mode
        SSD1306_SEGREMAP | 0x1,
        SSD1306_COMSCANDEC
    };
    ssd1306_commandList(init3, sizeof(init3));
    
    uint8_t comPins = 0x02;
    contrast_ = 0x8F;
    
    if ((WIDTH == 128) && (HEIGHT == 32)) {
        comPins = 0x02;
        contrast_ = 0x8F;
    } else if ((WIDTH == 128) && (HEIGHT == 64)) {
        comPins = 0x12;
        contrast_ = (vccstate_ == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
    } else if ((WIDTH == 96) && (HEIGHT == 16)) {
        comPins = 0x2;
        contrast_ = (vccstate_ == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
    }
    
    ssd1306_command(SSD1306_SETCOMPINS);
    ssd1306_command(comPins);
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(contrast_);
    
    ssd1306_command(SSD1306_SETPRECHARGE);
    ssd1306_command((vccstate_ == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
    
    ssd1306_command(SSD1306_SETVCOMDETECT);
    ssd1306_command(0x40);
    
    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);
    ssd1306_command(SSD1306_NORMALDISPLAY);
    ssd1306_command(SSD1306_DISPLAYON);
    
    return true;
}

void SSD1306::display() {
    static const uint8_t dlist1[] = {
        SSD1306_PAGEADDR,
        0,                   // page start
        0xFF,                // page end (placeholder)
        SSD1306_COLUMNADDR   // column addr cmd
    };
    ssd1306_commandList(dlist1, sizeof(dlist1));
    
    if (WIDTH == 64) {
        ssd1306_command(0x20);             // column start
        ssd1306_command(0x20 + WIDTH - 1); // column end
    } else {
        ssd1306_command(0);           // column start
        ssd1306_command((WIDTH - 1)); // column end
    }
    
    uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
    uint8_t* ptr = buffer_;
    
    // I2C burst write
    uint8_t buffer[count + 1];
    buffer[0] = 0x40; // data mode
    memcpy(&buffer[1], ptr, count);
    
    i2c_write_blocking(i2c_, address_, buffer, count + 1, false);
}

void SSD1306::clearDisplay() {
    memset(buffer_, 0, WIDTH * ((HEIGHT + 7) / 8));
}

void SSD1306::clear() {
    clearDisplay();
}

void SSD1306::invertDisplay(bool i) {
    ssd1306_command(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void SSD1306::dim(bool dim) {
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(dim ? 0 : contrast_);
}

void SSD1306::setContrast(uint8_t contrast) {
    contrast_ = contrast;
    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(contrast);
}

void SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
        switch (color) {
        case SSD1306_WHITE:
            buffer_[x + (y / 8) * WIDTH] |= (1 << (y & 7));
            break;
        case SSD1306_BLACK:
            buffer_[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
            break;
        case SSD1306_INVERSE:
            buffer_[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
            break;
        }
    }
}

void SSD1306::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawFastHLineInternal(x, y, w, color);
}

void SSD1306::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawFastVLineInternal(x, y, h, color);
}

void SSD1306::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if ((y >= 0) && (y < HEIGHT)) {
        if (x < 0) {
            w += x;
            x = 0;
        }
        if ((x + w) > WIDTH) {
            w = (WIDTH - x);
        }
        if (w > 0) {
            uint8_t* pBuf = &buffer_[x + (y / 8) * WIDTH];
            uint8_t mask = 1 << (y & 7);
            switch (color) {
            case SSD1306_WHITE:
                while (w--) {
                    *pBuf++ |= mask;
                }
                break;
            case SSD1306_BLACK:
                mask = ~mask;
                while (w--) {
                    *pBuf++ &= mask;
                }
                break;
            case SSD1306_INVERSE:
                while (w--) {
                    *pBuf++ ^= mask;
                }
                break;
            }
        }
    }
}

void SSD1306::drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if ((x >= 0) && (x < WIDTH)) {
        if (y < 0) {
            h += y;
            y = 0;
        }
        if ((y + h) > HEIGHT) {
            h = (HEIGHT - y);
        }
        if (h > 0) {
            uint8_t* pBuf = &buffer_[x + (y / 8) * WIDTH];
            uint8_t mod = (y & 7);
            uint8_t mask;
            uint8_t step;
            
            if (mod) {
                mod = 8 - mod;
                mask = ~(0xFF >> mod);
                if (h < mod) {
                    mask &= (0xFF >> (mod - h));
                }
                switch (color) {
                case SSD1306_WHITE:
                    *pBuf |= mask;
                    break;
                case SSD1306_BLACK:
                    *pBuf &= ~mask;
                    break;
                case SSD1306_INVERSE:
                    *pBuf ^= mask;
                    break;
                }
                pBuf += WIDTH;
                h -= mod;
            }
            
            if (h >= 8) {
                step = h / 8;
                switch (color) {
                case SSD1306_WHITE:
                    do {
                        *pBuf = 0xFF;
                        pBuf += WIDTH;
                    } while (--step);
                    break;
                case SSD1306_BLACK:
                    do {
                        *pBuf = 0x00;
                        pBuf += WIDTH;
                    } while (--step);
                    break;
                case SSD1306_INVERSE:
                    do {
                        *pBuf = ~*pBuf;
                        pBuf += WIDTH;
                    } while (--step);
                    break;
                }
                h %= 8;
            }
            
            if (h > 0) {
                mod = h & 7;
                mask = (1 << mod) - 1;
                switch (color) {
                case SSD1306_WHITE:
                    *pBuf |= mask;
                    break;
                case SSD1306_BLACK:
                    *pBuf &= ~mask;
                    break;
                case SSD1306_INVERSE:
                    *pBuf ^= mask;
                    break;
                }
            }
        }
    }
}

void SSD1306::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int16_t ystep;
    
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    
    for (; x0 <= x1; x0++) {
        if (steep) {
            drawPixel(y0, x0, color);
        } else {
            drawPixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void SSD1306::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}

void SSD1306::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t i = x; i < x + w; i++) {
        drawFastVLine(i, y, h, color);
    }
}

void SSD1306::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void SSD1306::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void SSD1306::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;
    
    delta++;
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        if (x < (y + 1)) {
            if (corners & 1) drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            if (corners & 2) drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
        }
        if (y != py) {
            if (corners & 1) drawFastVLine(x0 + py, y0 - px, 2 * px + 1 + delta, color);
            if (corners & 2) drawFastVLine(x0 - py, y0 - px, 2 * px + 1 + delta, color);
            py = y;
        }
        px = x;
    }
}

void SSD1306::setCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void SSD1306::setTextSize(uint8_t s) {
    textsize = (s > 0) ? s : 1;
}

void SSD1306::setTextColor(uint16_t c) {
    textcolor = c;
}

void SSD1306::setTextWrap(bool w) {
    textwrap = w;
}

size_t SSD1306::write(uint8_t c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y += textsize * 8;
        return 1;
    } else if (c == '\r') {
        cursor_x = 0;
        return 1;
    }
    
    if ((c < 32) || (c > 126)) return 0;
    
    // Bounds-check glyph index
    uint8_t index = c - 32;
    if (index >= 95) return 0;  // font holds 95 glyphs
    
    uint8_t line;
    for (int8_t i = 0; i < 5; i++) {
        line = font5x7[index][i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                if (textsize == 1) {
                    drawPixel(cursor_x + i, cursor_y + j, textcolor);
                } else {
                    fillRect(cursor_x + i * textsize, cursor_y + j * textsize, textsize, textsize, textcolor);
                }
            }
        }
    }
    
    cursor_x += textsize * 6;
    if (textwrap && (cursor_x > (WIDTH - textsize * 6))) {
        cursor_x = 0;
        cursor_y += textsize * 8;
    }
    
    return 1;
}

void SSD1306::print(const char* str) {
    while (*str) {
        write(*str++);
    }
}

void SSD1306::println(const char* str) {
    print(str);
    write('\n');
}

void SSD1306::drawBatteryBar(uint8_t x, uint8_t y, uint8_t width, uint8_t height, float percentage) {
    // Battery outline
    drawRect(x, y, width, height, WHITE);
    
    // Positive terminal
    fillRect(x + width, y + 2, 2, height - 4, WHITE);
    
    // Fill level
    uint8_t bar_width = (uint8_t)((width - 2) * percentage / 100.0f);
    
    if (bar_width > 0) {
        fillRect(x + 1, y + 1, bar_width, height - 2, WHITE);
    }
}

void SSD1306::drawBatteryStatus(float voltage, float percentage) {
    clearDisplay();
    
    // Voltage text
    char voltage_str[16];
    snprintf(voltage_str, sizeof(voltage_str), "%.2fV", voltage);
    setCursor(0, 0);
    print(voltage_str);
    
    // Percentage text
    char percent_str[16];
    snprintf(percent_str, sizeof(percent_str), "%.0f%%", percentage);
    setCursor(0, 10);
    print(percent_str);
    
    // Battery icon
    drawBatteryBar(0, 20, 120, 10, percentage);
}

void SSD1306::drawString(uint8_t x, uint8_t y, const char* str) {
    setCursor(x, y);
    print(str);
}

uint8_t* SSD1306::getBuffer() {
    return buffer_;
}

void SSD1306::ssd1306_command(uint8_t c) {
    uint8_t buffer[2];
    buffer[0] = 0x00; // command mode
    buffer[1] = c;
    i2c_write_blocking(i2c_, address_, buffer, 2, false);
}

void SSD1306::ssd1306_commandList(const uint8_t* c, uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        ssd1306_command(c[i]);
    }
}

void SSD1306::ssd1306_data(uint8_t* data, size_t size) {
    uint8_t* buffer = (uint8_t*)malloc(size + 1);
    buffer[0] = 0x40; // data mode
    memcpy(&buffer[1], data, size);
    i2c_write_blocking(i2c_, address_, buffer, size + 1, false);
    free(buffer);
} 