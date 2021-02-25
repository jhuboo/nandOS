/*
 * kernel.c - A simple kernel in C, using VGA text mode as output device
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN   = 14,
    VGA_COLOR_WHITE         = 15,
};


/* vga_entry_color */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}


/* vga_entry */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
} 


/* strlen: return length of str */
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}


static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_bufp;


/* terminal_init: initialize terminal */
void terminal_init(void)
{
    terminal_row = 0, terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_bufp = (uint16_t *) 0xB8000;   /* Buffer location */

    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t idx = y * VGA_WIDTH + x;
            terminal_bufp[idx] = vga_entry(' ', terminal_color);
        }
}


/* terminal_setcolor */
void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}


/* terminal_putentryat */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t idx = y + VGA_WIDTH + x;
    terminal_bufp[idx] = vga_entry(c, color);
}


/* terminal_putchar */
void terminal_putchar(char c)
{
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH) {   /* reached max width */
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)   /* reached max height */
            terminal_row = 0;
    }
} 


/* terminal_write */
void terminal_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}


/* terminal_writestring */
void terminal_writestring(const char *data)
{
    terminal_write(data, strlen(data));
}


/* kernel_main */
void kernel_main(void)
{
    terminal_init();    /* Initialize terminal interface */
    terminal_writestring("Hello A, Welcome to nandOS's barebones kernel");
}

