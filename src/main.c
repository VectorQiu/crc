/**
 * \file            main.c
 * \brief           CRC (Cyclic Redundancy Check) tests for CRC8, CRC16, and
 *                  CRC32 algorithms.
 * \date            2025-02-14
 */

/*
 * Copyright (c) 2024 Vector Qiu
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of the CRC library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         1.0.0
 */
/* includes ----------------------------------------------------------------- */
#include <stdint.h>
#include <stdio.h>
#include "crc/crc16.h"
#include "crc/crc16_lookup.h"
#include "crc/crc32.h"
#include "crc/crc32_lookup.h"
#include "crc/crc8.h"
#include "crc/crc8_lookup.h"

/* Private configuration ---------------------------------------------------- */

/* Private definitions ------------------------------------------------------ */

/* Private typedefs --------------------------------------------------------- */

/* Private variables -------------------------------------------------------- */
/**
 * \brief       Array of model names for CRC8 algorithms
 */
static char* crc8_model_name[] = {
    "CRC8", "CRC8_ITU", "CRC8_ROHC", "CRC8_MAXIM", "CRC8_CUSTOM",
};

/**
 * \brief       Array of model names for CRC16 algorithms
 */
static char* crc16_model_name[] = {
    "CRC16_IBM",    "CRC16_MAXIM",  "CRC16_USB",
    "CRC16_MODBUS", "CRC16_CCITT",  "CRC16_CCITT_FALSE",
    "CRC16_X25",    "CRC16_XMODEM", "CRC16_DNP",
};

/**
 * \brief       Array of model names for CRC32 algorithms
 */
static char* crc32_model_name[] = {
    "CRC32",
    "CRC32_MPEG2",
};

/* Private function prototypes ---------------------------------------------- */
static void print_table(char* name, uint8_t bits, void* table,
                        uint32_t table_len);
static void generate_table(void);
static void test_crc8(uint8_t data[], uint32_t len, uint8_t expected[]);
static void test_crc16(uint8_t data[], uint32_t len, uint16_t expected[]);
static void test_crc32(uint8_t data[], uint32_t len, uint32_t expected[]);

/* Public variables --------------------------------------------------------- */

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Main function to test CRC8, CRC16, and CRC32 functionality.
 *
 * This function verifies the correctness of CRC8, CRC16, and CRC32
 * implementations by calculating checksums for predefined input data and
 * comparing them with expected values.
 *
 * \param[in]       argc: Number of command-line arguments (unused in this
 *                  example).
 * \param[in]       argv: Array of command-line arguments (unused in this
 *                  example).
 * \return          int: Returns 0 if all tests pass, non-zero otherwise.
 */
int main(int argc, char* argv[]) {
    (void)argc; // Mark argc as unused
    (void)argv; // Mark argv as unused

    printf("generate crc table...\r\n");
    generate_table();

    // Test data : ASCII string "12345"
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"
    uint32_t len = sizeof(data) / sizeof(data[0]);
    printf("test data:{0x31, 0x32, 0x33, 0x34, 0x35}\r\n");
    uint8_t crc8_expected[10] = {0};
    crc8_expected[CRC8_MODEL] = 0xCB;
    crc8_expected[CRC8_ITU_MODEL] = 0x9E;
    crc8_expected[CRC8_ROHC_MODEL] = 0xC4;
    crc8_expected[CRC8_MAXIM_MODEL] = 0xAB;
    test_crc8(data, len, crc8_expected);

    uint16_t crc16_expected[10] = {0};
    crc16_expected[CRC16_IBM_MODEL] = 0xA455;
    crc16_expected[CRC16_MAXIM_MODEL] = 0x5BAA;
    crc16_expected[CRC16_USB_MODEL] = 0x5B8E;
    crc16_expected[CRC16_MODBUS_MODEL] = 0xA471;
    crc16_expected[CRC16_CCITT_MODEL] = 0x7437;
    crc16_expected[CRC16_CCITT_FALSE_MODEL] = 0x4560;
    crc16_expected[CRC16_X25_MODEL] = 0xBB40;
    crc16_expected[CRC16_XMODEM_MODEL] = 0x546C;
    crc16_expected[CRC16_DNP_MODEL] = 0x711C;
    test_crc16(data, len, crc16_expected);

    uint32_t crc32_expected[10] = {0};
    crc32_expected[CRC32_MODEL] = 0xCBF53A1C;
    crc32_expected[CRC32_MPEG2_MODEL] = 0xBD9AB747;
    test_crc32(data, len, crc32_expected);

    return 0;
}

/* Private functions -------------------------------------------------------- */
/**
 * \brief           Prints CRC tables in a format suitable for static table
 *                  generation.
 *
 * This function takes a CRC table and prints it in a format that can be used
 * directly in source code for fast CRC calculation.
 *
 * \param[in]       name: Name of the CRC model.
 * \param[in]       bits: Number of bits used in the CRC (e.g., 8, 16, 32).
 * \param[in]       table: Pointer to the CRC table.
 * \param[in]       table_len: Length of the CRC table.
 */
static void print_table(char* name, uint8_t bits, void* table,
                        uint32_t table_len) {

    printf("static const uint%d_t crc%d_poly_%s_table[%d] = {\r\n", bits, bits,
           name, table_len);
    for (uint32_t i = 0; i < table_len; i++) {
        if (i % 16 == 0) {
            printf("    ");
        }
        if (bits == 8) {
            uint8_t* crc8_table = table;
            printf("0x%02X, ", crc8_table[i]);
        } else if (bits == 16) {
            uint16_t* crc16_table = table;
            printf("0x%04X, ", (uint16_t)crc16_table[i]);
        } else if (bits == 32) {
            uint32_t* crc32_table = table;
            printf("0x%08X, ", (uint32_t)crc32_table[i]);
        } else {
            return;
        }

        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("};\n");
}

/**
 * \brief           Generates CRC tables for various models.
 *
 * This function generates CRC lookup tables for multiple CRC models (CRC8,
 * CRC16, CRC32) based on predefined polynomials and prints the tables using
 * `print_table`.
 */
static void generate_table(void) {
    uint8_t crc8_table[16];
    crc8_generate_table(0x07, crc8_table, 16);
    print_table("0x07", 8, crc8_table, 16);

    crc8_generate_table(0x31, crc8_table, 16);
    print_table("0x31", 8, crc8_table, 16);

    uint16_t crc16_table[16];
    crc16_generate_table(0x8005, crc16_table, 16);
    print_table("0x8005", 16, crc16_table, 16);

    crc16_generate_table(0x1021, crc16_table, 16);
    print_table("0x1021", 16, crc16_table, 16);

    crc16_generate_table(0x3D65, crc16_table, 16);
    print_table("0x3D65", 16, crc16_table, 16);

    uint32_t crc32_table[16];
    crc32_generate_table(0x04C11DB7, crc32_table, 16);
    print_table("0x04C11DB7", 32, crc32_table, 16);
}

/**
 * \brief           Tests CRC8 functionality.
 *
 * This function tests the CRC8 calculation for various models by comparing the
 * calculated CRC values with the expected values.
 *
 * \param[in]       data: The input data for CRC calculation.
 * \param[in]       len: The length of the input data.
 * \param[in]       expected: The expected CRC8 results for each model.
 */
static void test_crc8(uint8_t data[], uint32_t len, uint8_t expected[]) {
    int test_cnt = 0;
    int err_cnt = 0;

    test_cnt = (CRC8_NONE_MODEL - 1) * 2;
    printf("[----------] %d tests form %s\r\n", test_cnt, __FUNCTION__);
    uint8_t crc_result = 0;
    for (int i = 0; i < CRC8_NONE_MODEL; i++) {
        printf("[----------] \r\n");
        printf("[ RUN      ] %s.%s\r\n", __FUNCTION__, crc8_model_name[i]);
        printf("[ RUN      ] expected result: 0x%02X\r\n", expected[i]);

        crc_result = crc8_calculate((crc8_param_model_e)i, data, len);
        printf("[ RUN      ] crc8_calculate, actual result: 0x%02X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc8_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc8_model_name[i]);
        }
        printf("[ -------- ] \r\n");
        crc_result = crc8_lookup_calculate((crc8_lookup_param_model_e)i, data,
                                           len);
        printf("[ RUN      ] crc8_lookup_calculate, actual result: 0x%02X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc8_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc8_model_name[i]);
        }
    }
    printf("[==========] %d tests from %s\r\n", test_cnt, __FUNCTION__);
    if (err_cnt == 0) {
        printf("[  PASSED  ] %d tests.\r\n", test_cnt);
    } else {
        printf("[  FAILED  ] %d tests, fail:%d\r\n", test_cnt, err_cnt);
    }
    printf("\r\n");
}

/**
 * \brief           Tests CRC16 functionality.
 *
 * This function tests the CRC16 calculation for various models by comparing the
 * calculated CRC values with the expected values.
 *
 * \param[in]       data: The input data for CRC calculation.
 * \param[in]       len: The length of the input data.
 * \param[in]       expected: The expected CRC16 results for each model.
 */
static void test_crc16(uint8_t data[], uint32_t len, uint16_t expected[]) {
    int test_cnt = 0;
    int err_cnt = 0;

    test_cnt = (CRC16_NONE_MODEL - 1) * 2;
    printf("[----------] %d tests form %s\r\n", test_cnt, __FUNCTION__);
    uint16_t crc_result = 0;
    for (int i = 0; i < CRC16_NONE_MODEL; i++) {
        printf("[----------] \r\n");
        printf("[ RUN      ] %s.%s\r\n", __FUNCTION__, crc16_model_name[i]);
        printf("[ RUN      ] expected result: 0x%04X\r\n", expected[i]);

        crc_result = crc16_calculate((crc16_param_model_e)i, data, len);
        printf("[ RUN      ] crc16_calculate, actual result: 0x%04X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc16_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc16_model_name[i]);
        }
        printf("[ -------- ] \r\n");
        crc_result = crc16_lookup_calculate((crc16_lookup_param_model_e)i, data,
                                            len);
        printf("[ RUN      ] crc16_lookup_calculate, actual result: 0x%04X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc16_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc16_model_name[i]);
        }
    }
    printf("[==========] %d tests from %s\r\n", test_cnt, __FUNCTION__);
    if (err_cnt == 0) {
        printf("[  PASSED  ] %d tests.\r\n", test_cnt);
    } else {
        printf("[  FAILED  ] %d tests\r\n, fail:%d", test_cnt, err_cnt);
    }
    printf("\r\n");
}

/**
 * \brief           Tests CRC32 functionality.
 *
 * This function tests the CRC32 calculation for various models by comparing the
 * calculated CRC values with the expected values.
 *
 * \param[in]       data: The input data for CRC calculation.
 * \param[in]       len: The length of the input data.
 * \param[in]       expected: The expected CRC32 results for each model.
 */
static void test_crc32(uint8_t data[], uint32_t len, uint32_t expected[]) {
    int test_cnt = 0;
    int err_cnt = 0;

    test_cnt = (CRC8_NONE_MODEL - 1) * 2;
    printf("[----------] %d tests form %s\r\n", test_cnt, __FUNCTION__);
    uint32_t crc_result = 0;
    for (int i = 0; i < CRC32_NONE_MODEL; i++) {
        printf("[----------] \r\n");
        printf("[ RUN      ] %s.%s\r\n", __FUNCTION__, crc32_model_name[i]);
        printf("[ RUN      ] expected result: 0x%08X\r\n", expected[i]);

        crc_result = crc32_calculate((crc32_param_model_e)i, data, len);
        printf("[ RUN      ] crc32_calculate, actual result: 0x%08X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc32_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc32_model_name[i]);
        }

        printf("[ -------- ] \r\n");
        crc_result = crc32_lookup_calculate((crc32_lookup_param_model_e)i, data,
                                            len);
        printf("[ RUN      ] crc32_lookup_calculate, actual result: 0x%08X\r\n",
               crc_result);
        if (crc_result != expected[i]) {
            err_cnt++;
            printf("[     FAIL ] model_name: %s\r\n", crc32_model_name[i]);
        } else {
            printf("[       OK ] model_name: %s\r\n", crc32_model_name[i]);
        }
    }
    printf("[==========] %d tests from %s\r\n", test_cnt, __FUNCTION__);
    if (err_cnt == 0) {
        printf("[  PASSED  ] %d tests.\r\n", test_cnt);
    } else {
        printf("[  FAILED  ] %d tests, fail:%d\r\n", test_cnt, err_cnt);
    }
    printf("\r\n");
}

/* ----------------------------- end of file -------------------------------- */
