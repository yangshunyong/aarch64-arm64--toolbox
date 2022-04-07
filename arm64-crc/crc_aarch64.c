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

#define CRC32_X(crc, data) __asm__("CRC32X %w[c], %w[c], %x[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32_W(crc, data) __asm__("CRC32W %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32_H(crc, data) __asm__("CRC32H %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32_B(crc, data) __asm__("CRC32B %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32C_X(crc, data) __asm__("CRC32CX %w[c], %w[c], %x[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32C_W(crc, data) __asm__("CRC32CW %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32C_H(crc, data) __asm__("CRC32CH %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))
#define CRC32C_B(crc, data) __asm__("CRC32CB %w[c], %w[c], %w[v]":[c]"+r"(crc):[v]"r"(data))


typedef enum {
    POLYNOMIAL_04C11DB7,
    POLYNOMIAL_1EDC6F41,
} AARCH64_CRC_TYPE;

uint32_t crc32_aarch64_hw(void *data, uint32_t size,
                              AARCH64_CRC_TYPE type)
{
    int i;
    uint32_t crc = 0xFFFFFFFF;
    uint8_t *src_8;
    uint16_t *src_16;
    uint32_t *src_32;
    uint64_t *src_64;

    /* We simplify size and start address check here */
    uint64_t align = ((uint64_t)data) | size;

    if ((align % sizeof(uint64_t)) == 0) {
        uint32_t round = size / sizeof(uint64_t);
        src_64 = (uint64_t *)data;

        switch (type) {
        case POLYNOMIAL_1EDC6F41:
            for (i = 0; i < round; i++) {
                CRC32C_X(crc, *src_64++);
            }
            break;
        case POLYNOMIAL_04C11DB7:
            for (i = 0; i < round; i++) {
                CRC32_X(crc, *src_64++);
            }
            break;
        }
    } else if ((align % sizeof(uint32_t)) == 0) {
        uint32_t round = size / sizeof(uint32_t);
        src_32 = (uint32_t *)data;

        switch (type) {
        case POLYNOMIAL_1EDC6F41:
            for (i = 0; i < round; i++) {
                CRC32C_W(crc, *src_32++);
            }
            break;
        case POLYNOMIAL_04C11DB7:
            for (i = 0; i < round; i++) {
                CRC32_W(crc, *src_32++);
            }
            break;
        }
    } else if ((align % sizeof(uint16_t)) == 0) {
        uint32_t round = size / sizeof(uint16_t);
        src_16 = (uint16_t *)data;

        switch (type) {
        case POLYNOMIAL_1EDC6F41:
            for (i = 0; i < round; i++) {
                CRC32C_H(crc, *src_16++);
            }
            break;
        case POLYNOMIAL_04C11DB7:
            for (i = 0; i < round; i++) {
                CRC32_H(crc, *src_16++);
            }
            break;
        }
    } else {
        uint32_t round = size / sizeof(uint8_t);
        src_8 = (uint8_t *)data;

        switch (type) {
        case POLYNOMIAL_1EDC6F41:
            for (i = 0; i < round; i++) {
                CRC32C_B(crc, *src_8++);
            }
            break;
        case POLYNOMIAL_04C11DB7:
            for (i = 0; i < round; i++) {
                CRC32_B(crc, *src_8++);
            }
            break;
        }
    }

    return ~crc;
}


