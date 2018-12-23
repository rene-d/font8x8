// test pour Arduino
// rene-d 2018
//
#include <cstdio>
#include <cstring>
#include "font8x8_arduino.h"

typedef unsigned char byte;

class LedControl
{
  public:
    void setRow(int /*num*/, int /*row*/, byte b)
    {
        static const char on[] = "\033[0;31m●\033[0m";
        static const char off[] = "\033[0;37m●\033[0m";
        printf("\033[0;37m│\033[0m");
        for (int i = 0; i < 8; ++i)
            printf("%s", (b & (1 << (7 - i))) ? on : off);
        printf("\033[0;37m│\033[0m\n");
    }
};

byte pgm_read_byte_near(const void *c)
{
    return *(const char *)c;
}

void delay(int)
{
    printf("\033[0;37m├⎯⎯⎯⎯⎯⎯⎯⎯┤\033[0m\n");
}

size_t strlen_P(const char *str)
{
    return strlen(str);
}

//
// ------------------------- ARDUINO CODE -------------------------
//

static inline byte invbits(byte b)
{
    byte inv = 0;
    for (int j = 0; j < 8; ++j)
    {
        inv <<= 1;
        if (b & 0x1)
            inv |= 1;
        b >>= 1;
    }
    return inv;
}

static inline const byte * get_image(byte ch)
{
    if (ch >= sizeof(font8x8_arduino) / 8)
        ch = 0;
    return font8x8_arduino[ch];
}


void displayChar(LedControl &lc, byte ch)
{
    if (ch > 127 || ch < 32)
        return;

    const byte *image = get_image(ch);
    for (int i = 0; i < 8; i++)
    {
        byte b = pgm_read_byte_near(image + i);
        lc.setRow(0, i, invbits(b));
    }
}


void displayString(LedControl &lc, const char *str, unsigned wait)
{
    unsigned o;
    for (o = 0; o < strlen(str) - 1; ++o)
    {
        const byte *image1 = get_image(str[o]);
        const byte *image2 = get_image(str[o + 1]);

        for (int offset = 0; offset < 8; ++offset)
        {
            for (int i = 0; i < 8; ++i)
            {
                byte b;

                if (offset == 0)
                {
                    b = invbits(pgm_read_byte_near(image1 + i));
                }
                else
                {
                    byte b1 = invbits(pgm_read_byte_near(image1 + i));
                    byte b2 = invbits(pgm_read_byte_near(image2 + i));
                    b = (b1 << offset) | (b2 >> (8 - offset));
                }

                lc.setRow(0, i, b);
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    displayChar(lc, str[o]);
    delay(wait);
}


void displayString_P(LedControl &lc, const char *str, unsigned wait)
{
    unsigned o;
    for (o = 0; o < strlen_P(str) - 1; ++o)
    {
        const byte *image1 = get_image(pgm_read_byte_near(str + o));
        const byte *image2 = get_image(pgm_read_byte_near(str + o + 1));

        for (int offset = 0; offset < 8; ++offset)
        {
            for (int i = 0; i < 8; ++i)
            {
                byte b;

                if (offset == 0)
                {
                    b = invbits(pgm_read_byte_near(image1 + i));
                }
                else
                {
                    byte b1 = invbits(pgm_read_byte_near(image1 + i));
                    byte b2 = invbits(pgm_read_byte_near(image2 + i));
                    b = (b1 << offset) | (b2 >> (8 - offset));
                }

                lc.setRow(0, i, b);
            }
            delay(wait);
        }
    }

    // affiche le dernier caractère entièrement
    displayChar(lc, str[o]);
    delay(wait);
}

//
// ------------------------- ARDUINO CODE -------------------------
//


int main()
{
    LedControl lc;
    delay(0);

    displayString_P(lc, "AB", 0);
    displayChar(lc, '_');
    delay(0);
    displayChar(lc, '\\');
    delay(0);
}
