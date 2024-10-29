#ifndef __SD1306_DISPLAY_H__
#define __SD1306_DISPLAY_H__

extern struct display_ops_t sd1306_display_ops;

/** SSD1306 LCD driver commands */
enum SSD1606_CMDS
{
    SSD1306_SETLOWCOLUMN     = 0x00,
    SSD1306_SETHIGHCOLUMN    = 0x10,
    SSD1306_MEMORYMODE       = 0x20,
    SSD1306_COLUMNADDR       = 0x21,
    SSD1306_PAGEADDR         = 0x22,
    SSD1306_SETSTARTLINE     = 0x40,
    SSD1306_DEFAULT_ADDRESS  = 0x78,
    SSD1306_SETCONTRAST      = 0x81,
    SSD1306_CHARGEPUMP       = 0x8D,
    SSD1306_SEGREMAP         = 0xA0,
    SSD1306_DISPLAYALLON_RESUME = 0xA4,
    SSD1306_DISPLAYALLON     = 0xA5,
    SSD1306_NORMALDISPLAY    = 0xA6,
    SSD1306_INVERTDISPLAY    = 0xA7,
    SSD1306_SETMULTIPLEX     = 0xA8,
    SSD1306_SET_DISPLAY      = 0xAE,
    SSD1306_SETPAGE          = 0xB0,
    SSD1306_COMSCANINC       = 0xC0,
    SSD1306_COMSCANDEC       = 0xC8,
    SSD1306_SETDISPLAYOFFSET = 0xD3,
    SSD1306_SETDISPLAYCLOCKDIV = 0xD5,
    SSD1306_SETPRECHARGE     = 0xD9,
    SSD1306_SETCOMPINS       = 0xDA,
    SSD1306_SETVCOMDETECT    = 0xDB,

    SSD1306_SWITCHCAPVCC     = 0x02,
    SSD1306_NOP              = 0xE3,
};

#endif