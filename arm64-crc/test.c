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
#include <stdlib.h>


#define TEST_SIZE (1024 * 1024)


typedef enum {
    POLYNOMIAL_04C11DB7,
    POLYNOMIAL_1EDC6F41,
} AARCH64_CRC_TYPE;


extern uint32_t crc32_sw(uint8_t *src, uint32_t size, uint32_t polynomial);
uint32_t crc32_aarch64_hw(void *data, uint32_t size,
                              AARCH64_CRC_TYPE type);

void test_main()
{
    int i, j, k;
    uint8_t *test_data;
    uint32_t polynomials[] = {0x04C11DB7, 0x1EDC6F41};
    uint32_t crc_sw, crc_hw;

    test_data = malloc(TEST_SIZE);

    for (i = 0; i < TEST_SIZE; i++) {
        test_data[i] = i + 1 + 0x30;
    }

    for (i = 0;  i < 4; i++) {
        for (k = 0; k < 5; k++) {
            for (j = 0; j < sizeof(polynomials) / sizeof(polynomials[0]); j++) {
                crc_sw = crc32_sw(test_data + i, TEST_SIZE - i - k, polynomials[j]);
                crc_hw = crc32_aarch64_hw(test_data + i, TEST_SIZE - i -k, j);
                if (crc_sw == crc_hw) {
                    continue;
                } else {
                    printf("crc_sw:%x crc_hw:%x mismatch !!!!!!!!!!!!!\n", crc_sw, crc_hw);
                    return;
                }
            }
        }
    }

    printf("CRC32 test passed!\n", crc_sw, crc_hw);
    free(test_data);
}

