/*
   Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
   
   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use,
   copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following
   conditions:
   
   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.
   
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
*/


/*!
 * @file bitarray.h
 *
 *
 * @copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
 * @license    This file is released under the MIT License.\n
 *             https://opensource.org/licenses/MIT
 *
 * @brief Bit Array Library for Embedded Systems
 *
 * This file defines a set of macros and types for manipulating bit arrays in
 * memory-constrained embedded systems. Bit arrays, or bitmaps, are an efficient
 * way to store and manipulate collections of bits. They are especially useful
 * in situations where memory is limited, and each bit can represent a state, flag,
 * or other binary information. The library provides macros for creating bit arrays,
 * and for setting, resetting, toggling, and reading individual bits.
 *
 * The key features include:
 * - Efficient memory usage by packing bits into bytes.
 * - Macros for easy manipulation of individual bits.
 * - Inline functions for common operations like setting, resetting, and toggling bits.
 *
 * Usage Examples:
 * - Defining a bit array:
 *   @code
 *   BITARRAY(status_flags, 10);
 *   @endcode
 * - Setting a bit:
 *   @code
 *   BITARRAY_SET(status_flags, 3);
 *   @endcode
 * - Resetting a bit:
 *   @code
 *   BITARRAY_RESET(status_flags, 3);
 *   @endcode
 * - Reading a bit's value:
 *   @code
 *   int bit_val = BITARRAY_GET(status_flags, 3);
 *   @endcode
 *
 * The library is designed to be portable and efficient, making it well-suited for
 * various embedded system applications where direct hardware manipulation and
 * memory efficiency are critical.
 */

#ifndef BITARRAY_H_
#define BITARRAY_H_
#include <stdint.h> /* uint8_t */
#include <string.h> /* memset */


/*!
 * @brief Macro representing the reset state (0) for bit array operations.
 * @details This macro defines the reset state value used in various bit array
 *          operations. It represents a bit value of 0, typically used to reset
 *          or clear bits in a bit array.
 */
#define BITARRAY_RESET_STATE    0

/*!
 * @brief Macro representing the set state (1) for bit array operations.
 * @details This macro defines the set state value used in bit array
 *          operations. It represents a bit value of 1, typically used to set
 *          bits in a bit array.
 */
#define BITARRAY_SET_STATE      1

/*!
 * @brief Type definition for bit array storage.
 * @details Defines 'bitarray_t' as an alias for 'uint8_t', a standard 8-bit
 *          unsigned integer. This type is used as the fundamental unit of
 *          storage in bit arrays, ensuring each element of the array can
 *          store 8 bits.
 */
typedef uint8_t bitarray_t;


/*!
 * @brief Defines a bit array.
 * @details This macro is used to define a bit array in memory-constrained embedded systems. It efficiently allocates memory for storing bits, rounded up to the nearest byte. This is crucial for systems where memory efficiency is paramount. The macro creates a uniquely named bit array instance with a specified minimum bit capacity, enabling efficient bit-level operations.
 * @param[in] name The identifier for the bit array. This becomes a part of the variable name for the bit array.
 * @param[in] size The minimum number of bits required for the bit array. The actual size is computed to fit this minimum, adjusted to byte boundaries.
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10) // Creates a bit array 'status_flags_bitarray' to store at least 10 bits
 *     // Now, 'status_flags_bitarray' can be used for bit-level operations
 * @endcode
 */
#define BITARRAY(name,size) \
    bitarray_t name##_bitarray[((size)+7)>>3]


/*!
 * @brief Initializes a bit array to zero.
 * @details This macro is used for initializing all bits of a previously defined bit array to zero. It's an essential operation in embedded systems where bit arrays are frequently used for flag and state management. Initializing the bit array ensures a known state at the start, preventing any undefined behavior due to residual data. The macro leverages 'memset' to efficiently set the entire array to zero.
 * @param[in] name The name of the bit array to be initialized. This should match the name used in the BITARRAY macro.
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array named 'status_flags'
 *     BITARRAY_INIT(status_flags); // Initialize all bits of 'status_flags' to zero
 *     // 'status_flags_bitarray' is now ready for use with all bits set to zero
 * @endcode
 */
#define BITARRAY_INIT(name) \
    memset(name##_bitarray,0x00,sizeof(name##_bitarray))

/*!
 * @brief Calculates the size of a bit array in bits.
 * @details This macro computes the total number of bits that a defined bit array can hold.
 *          The size is determined based on the memory allocated for the array, considering that
 *          each unit in the array stores a byte (8 bits). This macro is useful for obtaining the
 *          actual capacity of a bit array, which may be slightly larger than the originally requested
 *          size due to rounding up to the nearest byte. It's particularly useful in scenarios where
 *          precise knowledge of the bit array's capacity is critical.
 *
 * @param[in] name The name of the bit array for which the size is to be calculated.
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array with at least 10 bits
 *     unsigned int size = BITARRAY_SIZE(status_flags); // size will be 16 (as it rounds up to the nearest byte)
 * @endcode
 */
#define BITARRAY_SIZE(name) \
    ((sizeof(name##_bitarray)/sizeof(name##_bitarray[0]))<<3)


/*!
 * @brief Retrieves a specific bit's value from a bit array.
 * @details Accesses a bit's value within a defined bit array, essential for
 *          bit-level data manipulation. Extracts the relevant byte and isolates
 *          the bit using bitwise operations, returning a boolean-like value.
 * @param[in] name Name of the bit array.
 * @param[in] bit Index of the bit (0-based).
 * @return Value of the specified bit (1 if set, 0 otherwise).
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array
 *     if (BITARRAY_GET(status_flags, 3)) {
 *         // Actions for 4th bit set
 *     }
 * @endcode
 */
#define BITARRAY_GET(name,bit) \
    ((unsigned)((name##_bitarray[((unsigned)(bit))>>3] & ((1<<(((unsigned)(bit))&0x07)))) > 0))


/*!
 * @brief Sets a specific bit in a bit array to 1.
 * @details This macro is a fundamental tool for manipulating bit arrays in
 *          embedded systems. It allows for setting a specific bit within a
 *          defined bit array to 1 (true). This operation is commonly used
 *          for flag setting, status marking, or enabling specific features
 *          or functions. The macro calculates the appropriate byte and bit
 *          position and sets the bit using bitwise OR operations.
 * @param[in] name Name of the bit array where the bit will be set.
 * @param[in] bit Index of the bit to set (0-based).
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array
 *     BITARRAY_SET(status_flags, 3); // Set the 4th bit (index 3) to 1
 * @endcode
 */
#define BITARRAY_SET(name,bit) \
    name##_bitarray[((unsigned)(bit))>>3] |= (1<<(((unsigned)(bit))&0x07))

/*!
    \brief      Reset a specific bit to 0

    \param      name        Name of the bit array
    \param      bit         Specific bit index
*/
#define BITARRAY_RESET(name,bit) \
    name##_bitarray[((unsigned)(bit))>>3] &= ~(1<<(((unsigned)(bit))&0x07))

/*!
 * @brief Toggles a specific bit in a bit array.
 * @details This macro is essential for changing the state of a specific bit in
 *          a bit array, flipping it from 0 to 1 or from 1 to 0. This functionality
 *          is crucial in situations where a bit represents a toggleable state or
 *          condition. The macro identifies the correct byte containing the target
 *          bit and toggles its state using bitwise XOR operations.
 * @param[in] name The name of the bit array containing the bit to be toggled.
 * @param[in] bit The index of the bit to toggle (0-based).
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array
 *     BITARRAY_TOGGLE(status_flags, 3); // Toggle the state of the 4th bit (index 3)
 * @endcode
 */
#define BITARRAY_TOGGLE(name,bit) \
     name##_bitarray[((unsigned)(bit))>>3] ^= (1<<(((unsigned)(bit))&0x07))

/*!
 * @brief Sets or resets a specific bit in a bit array based on the given value.
 * @details This macro is a versatile tool for bit manipulation in bit arrays.
 *          It allows the user to either set or reset a specified bit in the
 *          bit array based on the provided value. If the value is 0, the bit
 *          is reset (cleared to 0), and if the value is non-zero, the bit is
 *          set (set to 1). This macro is useful for dynamically changing the
 *          state of bits based on conditions or inputs.
 * @param[in] name The name of the bit array.
 * @param[in] bit The index of the bit to be modified (0-based).
 * @param[in] value The value to set the bit to (0 or 1).
 *
 * Usage Example:
 * @code
 *     BITARRAY(status_flags, 10); // Define a bit array
 *     int condition = check_some_condition(); // Determine the condition
 *     BITARRAY_SET_VALUE(status_flags, 3, condition); // Set or reset the 4th bit based on 'condition'
 * @endcode
 */
#define BITARRAY_SET_VALUE(name,bit,value) \
    do{if((value) == 0){BITARRAY_RESET(name,bit);}else{BITARRAY_SET(name,bit);}}while(0)


#endif /* BITARRAY_H_ */
