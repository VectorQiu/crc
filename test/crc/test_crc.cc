/**
 * \file            test_crc.cc
 * \brief           description
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
 * This file is part of the crc library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         v0.0.1
 */
/* includes ----------------------------------------------------------------- */
#include <cstdint>
#include <cstring>
#include <gtest/gtest.h>

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

/* Private function prototypes ---------------------------------------------- */

/* Public variables --------------------------------------------------------- */

/* Public functions --------------------------------------------------------- */
// Test cases for CRC8
TEST(CRC8Test, Calculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"

    uint8_t crc8_expected[10] = {0};
    crc8_expected[CRC8_MODEL] = 0xCB;
    crc8_expected[CRC8_ITU_MODEL] = 0x9E;
    crc8_expected[CRC8_ROHC_MODEL] = 0xC4;
    crc8_expected[CRC8_MAXIM_MODEL] = 0xAB;

    for (int i = 0; i < CRC8_NONE_MODEL; i++) {
        uint8_t result = crc8_calculate((crc8_param_model_e)i, data,
                                        sizeof(data));
        EXPECT_EQ(result, crc8_expected[i]);
    }
}

TEST(CRC8Test, PackAndVerify) {
    uint8_t data[6] = {0x31, 0x32, 0x33,
                       0x34, 0x35, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC8_NONE_MODEL; i++) {
        // Pack the buffer with CRC
        crc8_pack_buf((crc8_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc8_verify_buf((crc8_param_model_e)i, data, len);
        EXPECT_TRUE(is_valid);
    }
}

TEST(CRC8LookupTest, LookupCalculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"
    uint8_t crc8_expected[10] = {0};
    crc8_expected[CRC8_LOOKUP_MODEL] = 0xCB;
    crc8_expected[CRC8_ITU_LOOKUP_MODEL] = 0x9E;
    crc8_expected[CRC8_ROHC_LOOKUP_MODEL] = 0xC4;
    crc8_expected[CRC8_MAXIM_LOOKUP_MODEL] = 0xAB;
    for (int i = 0; i < CRC8_NONE_LOOKUP_MODEL; i++) {
        uint8_t result = crc8_lookup_calculate((crc8_lookup_param_model_e)i,
                                               data, sizeof(data));
        EXPECT_EQ(result, crc8_expected[i]);
    }
}

TEST(CRC8LookupTest, LookupPackAndVerify) {
    uint8_t data[6] = {0x31, 0x32, 0x33,
                       0x34, 0x35, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC8_NONE_LOOKUP_MODEL; i++) {
        // Pack the buffer with CRC
        crc8_lookup_pack_buf((crc8_lookup_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc8_lookup_verify_buf((crc8_lookup_param_model_e)i,
                                               data, len);
        EXPECT_TRUE(is_valid);
    }
}

// Test cases for CRC16
TEST(CRC16Test, Calculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"
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
    for (int i = 0; i < CRC16_NONE_MODEL; i++) {
        uint16_t result = crc16_calculate((crc16_param_model_e)i, data,
                                          sizeof(data));
        EXPECT_EQ(result, crc16_expected[i]);
    }
}

TEST(CRC16Test, PackAndVerify) {
    uint8_t data[7] = {0x31, 0x32, 0x33, 0x34,
                       0x35, 0x00, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC16_NONE_MODEL; i++) {
        // Pack the buffer with CRC
        crc16_pack_buf((crc16_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc16_verify_buf((crc16_param_model_e)i, data, len);
        EXPECT_TRUE(is_valid);
    }
}

TEST(CRC16Test, LargeData) {
    uint8_t data[256]; // Fill with 0x00 to 0xFF
    for (int i = 0; i < 256; i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    uint16_t expected_crc =
        0xD841; // Expected CRC16 value for large data (CRC16_CCITT_MODEL)
    uint16_t result = crc16_calculate(CRC16_CCITT_MODEL, data, sizeof(data));
    EXPECT_EQ(result, expected_crc);
}

TEST(CRC16Test, LargeData_PackAndVerify) {
    uint8_t data[258] = {0}; // Fill with 0x00 to 0xFF
    for (int i = 0; i < 256; i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    uint32_t len = sizeof(data);
    for (int i = 0; i < CRC16_NONE_MODEL; i++) {
        // Pack the buffer with CRC
        crc16_pack_buf((crc16_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc16_verify_buf((crc16_param_model_e)i, data, len);
        EXPECT_TRUE(is_valid);
    }
}

TEST(CRC16LookupTest, LookupCalculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"

    uint16_t crc16_expected[10] = {0};
    crc16_expected[CRC16_IBM_LOOKUP_MODEL] = 0xA455;
    crc16_expected[CRC16_MAXIM_LOOKUP_MODEL] = 0x5BAA;
    crc16_expected[CRC16_USB_LOOKUP_MODEL] = 0x5B8E;
    crc16_expected[CRC16_MODBUS_LOOKUP_MODEL] = 0xA471;
    crc16_expected[CRC16_CCITT_LOOKUP_MODEL] = 0x7437;
    crc16_expected[CRC16_CCITT_FALSE_LOOKUP_MODEL] = 0x4560;
    crc16_expected[CRC16_X25_LOOKUP_MODEL] = 0xBB40;
    crc16_expected[CRC16_XMODEM_LOOKUP_MODEL] = 0x546C;
    crc16_expected[CRC16_DNP_LOOKUP_MODEL] = 0x711C;
    for (int i = 0; i < CRC16_NONE_LOOKUP_MODEL; i++) {
        uint16_t result = crc16_lookup_calculate((crc16_lookup_param_model_e)i,
                                                 data, sizeof(data));
        EXPECT_EQ(result, crc16_expected[i]);
    }
}

TEST(CRC16LookupTest, LookupPackAndVerify) {
    uint8_t data[6] = {0x31, 0x32, 0x33,
                       0x34, 0x35, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC16_NONE_LOOKUP_MODEL; i++) {
        // Pack the buffer with CRC
        crc16_lookup_pack_buf((crc16_lookup_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc16_lookup_verify_buf((crc16_lookup_param_model_e)i,
                                                data, len);
        EXPECT_TRUE(is_valid);
    }
}

// Test cases for CRC32
TEST(CRC32Test, Calculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"

    uint32_t crc32_expected[10] = {0};
    crc32_expected[CRC32_MODEL] = 0xCBF53A1C;
    crc32_expected[CRC32_MPEG2_MODEL] = 0xBD9AB747;

    for (int i = 0; i < CRC32_NONE_MODEL; i++) {
        uint32_t result = crc32_calculate((crc32_param_model_e)i, data,
                                          sizeof(data));
        EXPECT_EQ(result, crc32_expected[i]);
    }
}

TEST(CRC32Test, CRC32_PackAndVerify_Model) {
    uint8_t data[9] = {0x31, 0x32, 0x33, 0x34, 0x35,
                       0x00, 0x00, 0x00, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC32_NONE_MODEL; i++) {
        // Pack the buffer with CRC
        crc32_pack_buf((crc32_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc32_verify_buf((crc32_param_model_e)i, data, len);
        EXPECT_TRUE(is_valid);
    }
}

TEST(CRC32Test, LargeData) {
    uint8_t data[256]; // Fill with 0x00 to 0xFF
    for (int i = 0; i < 256; i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    uint32_t expected_crc =
        0x29058C73; // Expected CRC32 value for large data (CRC32_MODEL)
    uint32_t result = crc32_calculate(CRC32_MODEL, data, sizeof(data));
    EXPECT_EQ(result, expected_crc);
}

TEST(CRC32LookupTest, LookupCalculate) {
    uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35}; // "12345"

    uint32_t crc32_expected[10] = {0};
    crc32_expected[CRC32_LOOKUP_MODEL] = 0xCBF53A1C;
    crc32_expected[CRC32_MPEG2_LOOKUP_MODEL] = 0xBD9AB747;
    for (int i = 0; i < CRC32_NONE_LOOKUP_MODEL; i++) {
        uint32_t result = crc32_lookup_calculate((crc32_lookup_param_model_e)i,
                                                 data, sizeof(data));
        EXPECT_EQ(result, crc32_expected[i]);
    }
}

TEST(CRC32LookupTest, LookupPackAndVerify) {
    uint8_t data[6] = {0x31, 0x32, 0x33,
                       0x34, 0x35, 0x00}; // Reserve space for CRC
    uint32_t len = sizeof(data);

    for (int i = 0; i < CRC32_NONE_LOOKUP_MODEL; i++) {
        // Pack the buffer with CRC
        crc32_lookup_pack_buf((crc32_lookup_param_model_e)i, data, len);

        // Verify the packed buffer
        bool is_valid = crc32_lookup_verify_buf((crc32_lookup_param_model_e)i,
                                                data, len);
        EXPECT_TRUE(is_valid);
    }
}

/* Private functions -------------------------------------------------------- */

/* ----------------------------- end of file -------------------------------- */
