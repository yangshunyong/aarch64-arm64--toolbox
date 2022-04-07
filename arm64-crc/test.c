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

