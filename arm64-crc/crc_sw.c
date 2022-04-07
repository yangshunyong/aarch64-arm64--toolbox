/*
 * Copyright Shunyong Yang (yang.shunyong@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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


