
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
