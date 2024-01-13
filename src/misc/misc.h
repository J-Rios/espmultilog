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

// Global Data
#include "../global/global.h"

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
 * @return char* Pointer to Device UUID static string.
 */
extern char* get_device_uuid();

/**
 * @brief Merge all the strings from an array of strings into one single
 * string.
 * @param argc Number of arguments in the array of strings.
 * @param array_str Array of strings to merge.
 * @param str_out Pointer to output single string.
 * @param str_out_len Size of output single string.
 * @return true Something was appended.
 * @return false Nothing was appended.
 */
extern bool single_str_from_array_of_str(int argc, char* array_str[],
        char* str_out, const size_t str_out_len);
/**
 * @brief Parse a string into a "s_str_cmd_args" structure to handle it
 * as a command+arguments.
 * @param str_in Input string to parse.
 * @param cmd_args "s_str_cmd_args" structure to store the parsed data.
 */
extern void str_parse_cmd_args(char* str_in,
    ns_misc::s_str_cmd_args* cmd_args);

/**
 * @brief  Count the number of words inside a string.
 * @param  str_in Input string from where to count words.
 * @return The number of words in the string.
 */
extern uint32_t str_count_words(const char* str_in);

/**
 * @brief  Get substring from array until a specific character or end
 * of string.
 * @param  str Input string from where to get the substring.
 * @param  str_len Number of characters in "str".
 * @param  until_c Get substring until this character.
 * @param  str_read Buffer to store the read substring.
 * @param  str_read_size Max size of read buffer.
 * @return If character "until_c" was found (true/false).
 */
extern bool str_read_until_char(char* str, const size_t str_len,
    const char until_c, char* str_read, const size_t str_read_size);

// Auxiliary functions for string to number safe conversion
extern t_return_code safe_atoi_u8(const char* in_str, const size_t in_str_len,
    uint8_t* out_int, bool check_null_terminated=true);
extern t_return_code safe_atoi_u32(const char* in_str, const size_t in_str_len,
    uint32_t* out_int, bool check_null_terminated=true);

/*****************************************************************************/

/* Include Guard Close */

#endif /* MISC_H */
