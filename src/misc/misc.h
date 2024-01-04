/**
 * @file    misc.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-03
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Miscellaneous Functionalities header file.
 *
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2024 Jose Miguel Rios Rubio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************/

/* Include Guard */

#ifndef MISC_H
#define MISC_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>
#include <cstddef>

/*****************************************************************************/

/* Data Types */

enum class t_return_code : int8_t
{
    RC_BAD = -1,
    RC_INVALID_INPUT = -2,

    RC_OK = 0
};

/*****************************************************************************/

/* Misscellaneous Function Declarations */

/**
 * @brief Get the device id.
 * @return char* Pointer to Device ID static string.
 */
extern char* get_device_id();

/**
 * @brief Get a device Unique Universal Identifier string.
 * @param uuid Pointer to string array to store the UUID.
 * @param uuid_size Size of 'uuid' string array to store the UUID in.
 */
extern void get_device_uuid(char* uuid, const uint32_t uuid_size);

// Auxiliary functions for string to number safe conversion
extern t_return_code safe_atoi_u8(const char* in_str, const size_t in_str_len,
    uint8_t* out_int, bool check_null_terminated=true);
extern t_return_code safe_atoi_u32(const char* in_str, const size_t in_str_len,
    uint32_t* out_int, bool check_null_terminated=true);

/*****************************************************************************/

/* Include Guard Close */

#endif /* MISC_H */
