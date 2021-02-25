ENTRY(_start)

SECTIONS
{
    . = 1M;     /* put kernel section at 1 MiB, conventional in bootloaders */

    /* multiboot header: needs to be added early so bootloader recognizes image */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* read-write data (uninitialized) & stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* Compiler may produce other sections, which will be added in a segment
     * with the same name. Add stuff here as needed */
}
