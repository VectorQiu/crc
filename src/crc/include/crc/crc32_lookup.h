/**
 * \file            crc32_lookup_lookup.h
 * \brief           Cyclic Redundancy Check (CRC8) Look-Up Table
 * \date            2025-02-14
 *
 * This file provides functions for CRC32 calculation using look-up tables,
 * including the generation and use of CRC32 lookup tables for various models.
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
#ifndef __CRC32_LOOKUP_H__
#define __CRC32_LOOKUP_H__

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        crc32_lookup Lookup Manager
 * \brief           Manages CRC32 look-up table calculations.
 * \{
 */

/* Public typedefs ---------------------------------------------------------- */
/**
 * \brief           Enumeration of available CRC32 lookup models.
 *
 * This enum defines the different CRC32 lookup models supported by this
 * library.
 */
typedef enum {
    CRC32_LOOKUP_MODEL = 0,   /*!< Standard CRC32 lookup model */
    CRC32_MPEG2_LOOKUP_MODEL, /*!< MPEG2 CRC32 lookup model */
    CRC32_NONE_LOOKUP_MODEL,  /*!< No specific CRC32 lookup model */
} crc32_lookup_param_model_e;

/**
 * \brief           CRC32 lookup context structure for CRC32 calculation.
 *
 * This structure holds the parameters needed for CRC32 calculation using a
 * lookup table, including the initial value, final XOR value, polynomial,
 * bit reversal options, and the lookup table.
 */
typedef struct {
    uint32_t init;      /*!< Initial value for the CRC32 calculation */
    uint32_t xor_out;   /*!< Final XOR value to apply to the result */
    uint32_t poly;      /*!< Polynomial used in CRC32 calculation */
    bool ref_in;        /*!< Whether to reverse the input data bits */
    bool ref_out;       /*!< Whether to reverse the output data bits */
    uint16_t table_len; /*!< Length of the lookup table */
    uint32_t* table;    /*!< Pointer to the CRC32 lookup table */
} crc32_lookup_ctx_t;

/* Public functions --------------------------------------------------------- */
/**
 * \brief           Initialize the CRC32 lookup context with the specified
 * model.
 *
 * This function initializes the CRC32 context based on the specified CRC32
 * lookup model. It sets the initial CRC value, XOR value, polynomial, and bit
 * reversal options.
 *
 * \param[in,out]   ctx: Pointer to the CRC32 lookup context structure to be
 * initialized
 * \param[in]       model: The CRC32 lookup model to use for initialization
 */
void crc32_lookup_init(crc32_lookup_ctx_t* ctx,
                       crc32_lookup_param_model_e model);

/**
 * \brief           Update the CRC32 calculation with new data.
 *
 * This function processes the input data buffer and updates the CRC32 checksum
 * stored in the context based on the parameters set by `crc32_lookup_init`.
 *
 * \param[in,out]   ctx: Pointer to the CRC32 lookup context structure
 *                  containing the current state
 * \param[in]       buf: Pointer to the input data buffer to process
 * \param[in]       len: Length of the input data buffer in bytes
 */
void crc32_lookup_update(crc32_lookup_ctx_t* ctx, const uint8_t* buf,
                         uint32_t len);

/**
 * \brief           Finalize the CRC32 calculation and return the checksum.
 *
 * This function completes the CRC32 calculation and returns the final checksum
 * based on the current context. It should be called after all data has been
 * processed using `crc32_lookup_update`.
 *
 * \param[in]       ctx: Pointer to the CRC32 lookup context structure
 * \return          The final CRC32 checksum
 */
uint32_t crc32_lookup_final(crc32_lookup_ctx_t* ctx);

/**
 * \brief           Calculate the CRC32 checksum for a given data buffer using a
 *                  specific model.
 *
 * This function calculates the CRC32 checksum for the provided data buffer and
 * CRC32 lookup model, returning the calculated checksum. It does not require
 * prior initialization.
 *
 * \param[in]       model: The CRC32 lookup model to use for calculation
 * \param[in]       buf: Pointer to the data buffer to process
 * \param[in]       len: Length of the data buffer in bytes
 * \return          The calculated CRC32 checksum
 */
uint32_t crc32_lookup_calculate(crc32_lookup_param_model_e model,
                                const uint8_t* buf, uint32_t len);

/**
 * \brief           Pack the CRC32 lookup parameters into a buffer.
 *
 * This function packs the CRC32 lookup model parameters into the provided
 * buffer. It can be used to serialize the CRC32 lookup model configuration.
 *
 * \param[in]       model: The CRC32 lookup model whose parameters are to be
 *                  packed
 * \param[in,out]   buf: Pointer to the buffer where the packed data will be
 *                  stored
 * \param[in]       len: Length of the buffer
 */
void crc32_lookup_pack_buf(crc32_lookup_param_model_e model, uint8_t* buf,
                           uint32_t len);

/**
 * \brief           Verify the CRC32 checksum of a data buffer.
 *
 * This function verifies if the CRC32 checksum of the given buffer matches the
 * expected value for the specified CRC32 lookup model.
 *
 * \param[in]       model: The CRC32 lookup model to use for verification
 * \param[in]       buf: Pointer to the data buffer to verify
 * \param[in]       len: Length of the data buffer in bytes
 * \return          `true` if the checksum is correct, `false` otherwise
 */
bool crc32_lookup_verify_buf(crc32_lookup_param_model_e model,
                             const uint8_t* buf, uint32_t len);

/**
 * \brief           Generate a CRC32 lookup table based on the specified
 *                  polynomial.
 *
 * This function generates a CRC32 lookup table based on the provided
 * polynomial, which can be used for fast CRC32 calculation.
 *
 * \param[in]       polynomial: The polynomial to use for the CRC32 calculation
 * \param[out]      table: Pointer to the lookup table to be generated
 * \param[in]       table_len: Length of the lookup table
 */
void crc32_generate_table(uint32_t polynomial, uint32_t table[],
                          uint32_t table_len);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CRC32_LOOKUP_H__ */

/* ----------------------------- end of file -------------------------------- */
