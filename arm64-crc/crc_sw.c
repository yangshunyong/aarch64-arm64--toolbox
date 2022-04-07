/*
 * MIT License
 *
 * Copyright (c) 2022 Shunyong Yang (yang.shunyong@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */




#include <stdio.h>
#include <stdint.h>

#define CRC_WITH_TABLE
#define CRC32_TABLE_SIZE (256)

#define REVERSE_POLY(poly_in, poly_out) __asm__("rbit %w[c], %w[v]":[c]"+r"(poly_out):[v]"r"(poly_in))

static uint32_t crc32_table_reverse[CRC32_TABLE_SIZE];

void build_crc32_reverse_table(uint32_t *table, uint32_t poly_reverse)
{
    uint32_t i, j;

    for (i = 0; i < CRC32_TABLE_SIZE; i++) {
        uint32_t data = i;

        for (j = 0; j < 8; j++) {
            if (data & 0x01)
                data = (data >> 1) ^ poly_reverse;
            else
                data = data >> 1;
        }

        table[i] = data;
    }
}

uint32_t crc32_poly_reverse_without_table(uint8_t *src, uint32_t size, uint32_t poly_reverse)
{
    uint32_t crc = 0xFFFFFFFF;

    for (int i = 0; i < size; i++) {
        crc = crc ^ *src++;

        for (int j = 0; j < 8; j++) {
            if (crc & 0x01)
                crc =  (crc >> 1) ^ poly_reverse;
            else
                crc = crc >> 1;
        }
    }

    return ~crc;
}

uint32_t crc32_poly_reverse_with_table(uint8_t *src, uint32_t size, uint32_t * table)
{
    uint32_t crc = 0xFFFFFFFF;

    for (int i = 0; i < size; i++) {
        crc = crc32_table_reverse[(crc ^ *src++) & 0xff] ^ (crc >> 8);
    }

    return ~crc;
}

uint32_t crc32_sw(uint8_t *src, uint32_t size, uint32_t polynomial)
{
    uint8_t *pdata = (uint8_t *)src;
    uint32_t poly_reverse;

    REVERSE_POLY(polynomial, poly_reverse);
#ifdef CRC_WITH_TABLE
    build_crc32_reverse_table(crc32_table_reverse, poly_reverse);
    return crc32_poly_reverse_with_table(src, size, crc32_table_reverse);
#else
    return crc32_poly_reverse_without_table(src, size, poly_reverse);
#endif
}


