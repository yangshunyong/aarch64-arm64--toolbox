# AARCH64 CRC32
## Introduction
AARCH64 supports two crc instructions, "CRC32" and "CRC32C". The polynomial are, <br>

| Algorithm | Polynomial |
|-----------|------------|
| CRC32     | 0x04C11DB7 |
| CRC32C    | 0x1EDC6F41 | <br>

Both instructions has variants according data width, such as CRCX (64bit), CRCW (32bit), CRCH (16bit), CRCB (8bit), etc. <br>

We handle this by checking the input data address and total data size. Then select the right instruction, please check the implementation of crc32_aarch64_hw(). <br>

A software implementation is also included, both with/without CRC table version supported. They can be used to evaluatate perfromance between SW and HW implementabtion. <br>

## Performance Test
Following is a 128MB test on a Cortex A53 CPU of Polynomial 0x04C11DB7. SW performance is not changed as it always use 8bit calculation. The HW highest performance achieved when both data address and size are all 64bit aligned.<br>
| Address    | Size      | SW (ms) | HW (ms) |
|------------|-----------|---------|---------|
| 0x22000030 | 0x8000000 | 1471    | 128     |
| 0x22000031 | 0x7ffffff | 1473    | 658     | <br>

So, in order to achieve best performance, please make address and size 64bit aligned.
