#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "font8x8_basic.h"
#include "font8x8_box.h"
#include "font8x8_block.h"
#include "font8x8_ext_latin.h"
#include "font8x8_greek.h"
#include "font8x8_hiragana.h"
#include "font8x8_misc.h"
#include "font8x8_sga.h"

typedef uint8_t font_t[8];

void usage(const char *exec)
{
    printf("Usage: %s <char_code>\n", exec);
    printf("       <char_code> Decimal character code between 0 and 127\n");
}

void render(const font_t *bitmap)
{
    int x, y;
    int set;

    for (x = 0; x < 8; x++)
    {
        for (y = 0; y < 8; y++)
        {
            set = ((const uint8_t *)bitmap)[x] & 1 << y;
            printf("%c", set ? 'X' : ' ');
        }
        printf("\n");
    }
}

void render_all(const font_t *font, int count, int cols)
{
    for (int ord = 0; ord < count; ord += cols)
    {
        int max = ord + cols;
        if (max > count)
            max = count;

        char line[8][10 * cols];

        memset(line, 0, sizeof(line));

        if (ord == 0)
        {
            for (int i = ord; i < max; ++i)
            {
                printf("+--------");
            }
            printf("+\n");
        }
        printf("|");
        for (int i = ord; i < max; ++i)
        {
            printf("  %3d   |", i);
        }
        printf("\n");
        for (int i = ord; i < max; ++i)
        {
            printf("+--------");
        }
        printf("+\n");

        for (int i = ord; i < max; ++i)
        {
            for (int y = 0; y < 8; y++)
            {
                const uint8_t *bitmap = ((const uint8_t *)(font + i));

                for (int x = 0; x < 8; ++x)
                {
                    int set = bitmap[y] & 1 << x;
                    line[y][x + (i - ord) * 9] = set ? 'X' : ' ';
                }
                line[y][8 + (i - ord) * 9] = '|';
            }
        }
        for (int y = 0; y < 8; y++)
        {
            printf("|%s\n", line[y]);
        }
        for (int i = ord; i < max; ++i)
        {
            printf("+--------");
        }
        printf("+\n");
    }
}

int main(int argc, char **argv)
{
    int ord;
    int c;
    const font_t *font = (const font_t *)font8x8_basic;
    int count = sizeof(font8x8_basic) / 8;
    bool all = false;
    int cols = 16;

    while ((c = getopt(argc, argv, "f:hac:")) != -1)
    {
        switch (c)
        {
        case 'h':
            usage(argv[0]);
            exit(0);
            break;
        case 'c':
            cols = atoi(optarg);
            if (cols < 1) cols = 1;
            else if (cols > 128) cols = 128;
            break;
        case 'a':
            all = true;
            break;
        case 'f':
            if (strcmp(optarg, "basic") == 0)
            {
                font = (const font_t *)font8x8_basic;
                count = sizeof(font8x8_basic) / 8;
            }
            else if (strcmp(optarg, "block") == 0)
            {
                font = (const font_t *)font8x8_block;
                count = sizeof(font8x8_block) / 8;
            }
            else if (strcmp(optarg, "box") == 0)
            {
                font = (const font_t *)font8x8_box;
                count = sizeof(font8x8_box) / 8;
            }
            else if (strcmp(optarg, "ext_latin") == 0)
            {
                font = (const font_t *)font8x8_ext_latin;
                count = sizeof(font8x8_ext_latin) / 8;
            }
            else if (strcmp(optarg, "greek") == 0)
            {
                font = (const font_t *)font8x8_greek;
                count = sizeof(font8x8_greek) / 8;
            }
            else if (strcmp(optarg, "hiragana") == 0)
            {
                font = (const font_t *)font8x8_hiragana;
                count = sizeof(font8x8_hiragana) / 8;
            }
            else if (strcmp(optarg, "misc") == 0)
            {
                font = (const font_t *)font8x8_misc;
                count = sizeof(font8x8_misc) / 8;
            }
            else if (strcmp(optarg, "sga") == 0)
            {
                font = (const font_t *)font8x8_sga;
                count = sizeof(font8x8_sga) / 8;
            }
            else
            {
                fprintf(stderr, "unknown font %s\n", optarg);
                exit(1);
            }
            break;
        case '?':
            if (isprint(optopt))
                fprintf(stderr, "unknown/bad option -%c\n", optopt);
            else
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            break;
        default:
            abort();
        }
    }

    if (all)
    {
        printf("\nFONT %s\n", "basic");
        render_all(font8x8_basic, sizeof(font8x8_basic) / 8, cols);

        printf("\nFONT %s\n", "block");
        render_all(font8x8_block, sizeof(font8x8_block) / 8, cols);

        printf("\nFONT %s\n", "box");
        render_all(font8x8_box, sizeof(font8x8_box) / 8, cols);

        printf("\nFONT %s\n", "ext_latin");
        render_all(font8x8_ext_latin, sizeof(font8x8_ext_latin) / 8, cols);

        printf("\nFONT %s\n", "greek");
        render_all(font8x8_greek, sizeof(font8x8_greek) / 8, cols);

        printf("\nFONT %s\n", "hiragana");
        render_all(font8x8_hiragana, sizeof(font8x8_hiragana) / 8, cols);

        printf("\nFONT %s\n", "misc");
        render_all(font8x8_misc, sizeof(font8x8_misc) / 8, cols);

        printf("\nFONT %s\n", "sga");
        render_all(font8x8_sga, sizeof(font8x8_sga) / 8, cols);

        return 0;
    }

    if (optind == argc)
    {
        render_all(font, count, cols);
    }
    else
    {
        for (int index = optind; index < argc; index++)
        {
            ord = atoi(argv[index]);
            if (ord < count)
            {
                printf("%d\n", ord);
                render(font + ord);
            }
        }
    }

    return 0;
}
