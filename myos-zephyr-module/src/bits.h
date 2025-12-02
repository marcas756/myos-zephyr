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



/**
 * @file bits.h
 *
 * @copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
 * @license    This file is released under the MIT License.\n
 *             https://opensource.org/licenses/MIT
 *
 * @brief Provides preprocessor macros for bit manipulations and bit testing.
 *
 * @details
 * Bit Field:
 *          A bit field is an unsigned integer where single bits or groups of bits are
 *          combined. It represents a compound data type at the bit level, unlike
 *          primitive data types where the entire value is interpreted collectively.
 *          Bit fields are embedded within the data type of the binary number, differing
 *          from bit arrays which are more explicit structures.
 *          More information can be found at [Wikipedia - Bit field](https://de.wikipedia.org/wiki/Bitfeld).
 *
 * Bit Mask:
 *          A bit mask is used to isolate or manipulate subsets of bits within a bit field.
 *          While hardware often supports direct access to individual bits, compiler-level
 *          support typically involves using a bit mask in conjunction with boolean bit
 *          operations like AND, OR, XOR, and NOT. Bit masks are specially crafted bit fields
 *          that don't hold data themselves but facilitate operations on other bit fields.
 *          For further reading, visit [Wikipedia - Bit array](https://de.wikipedia.org/wiki/Bitkette).
 *
 *          These macros provide a convenient and efficient way to perform common bit
 *          manipulations. They are useful in various scenarios, such as setting, clearing,
 *          toggling, and testing the state of flags or configuration options represented as bits.
 *
 *          Bit manipulation is crucial in low-level programming, hardware interfacing, and
 *          situations where performance and memory optimization are essential. These operations
 *          offer fine-grained control over data at the bit level, allowing for compact
 *          representation and efficient manipulation of state and configuration data.
 *
 * Usage:
 *          Include this header file in your project to utilize the provided bit manipulation
 *          and testing macros. These macros are generic and work with any standard C integer types.
 *
 * @note Refer to each macro's documentation for specific usage examples.
 *
 * @see BITS, BITS_INVERT, BITS_TEST, BITS_SET, BITS_CLEAR, BITS_TOGGLE
 *
 * This file was documented in doxygen style format by using ChatGPT 4.
 */

#ifndef BITS_H_
#define BITS_H_

/**
 * @brief Reads out specific bits from a bit field using a bit mask.
 * @details This macro allows for extracting specific bits from a bit field by performing
 *          a logical AND operation with a bit mask. The bit mask should have bits set
 *          (1s) at the positions you want to read and cleared (0s) elsewhere. When the
 *          bit field is AND-ed with this mask, the resulting value contains the bits
 *          from the original bit field that align with the 1s in the mask, with all
 *          other bits set to 0.
 *
 *          This operation is commonly used to isolate a subset of bits within a larger
 *          bit field for examination or further processing.
 *
 * Example Usage:
 *      1-Bit:
 *          01001011 (bit field)
 *      AND 00001000 (bit mask)
 *      ------------
 *          00001000 (result - Bit extracted)
 *
 *      0-Bit:
 *          01001011 (bit field)
 *      AND 00000100 (bit mask)
 *      ------------
 *          00000000 (result - Bit not present)
 *
 * @param x The bit field from which to extract bits.
 * @param m The bit mask specifying which bits to extract.
 * @return The result of the bit field AND-ed with the bit mask.
 */
#define BITS(x,m)           ((x)&(m))

/**
 * @brief Toggles all bits of an integer.
 * @details This macro inverts all bits of the given integer. Each bit that is 0
 *          becomes 1, and each bit that is 1 becomes 0. This is achieved using the
 *          bitwise NOT operation. The inversion of bits is a common operation in
 *          scenarios where you need to flip the bit pattern, for example, when
 *          implementing bitwise algorithms or performing certain kinds of data
 *          transformations.
 *
 * Example Usage:
 *      Original:
 *          01001011 (Original bit pattern of integer)
 *      Invert:
 *          ~01001011
 *      ------------
 *          10110100 (Inverted bit pattern)
 *
 * @param x The integer whose bits are to be toggled.
 * @return The result of inverting all bits of the given integer.
 */
#define BITS_INVERT(x)      (~x)

/**
 * @brief Tests if all bits defined by a bit mask are set in the bit field.
 * @details This macro checks whether all the bits specified in the bit mask are set
 *          (i.e., are 1) in the given bit field. It first performs a logical AND
 *          operation between the bit field and the bit mask, then compares the result
 *          to the bit mask. If the comparison is equal, it means all the bits in the
 *          bit field corresponding to the set bits in the mask are also set.
 *
 *          This operation is useful for checking the state of multiple bits at once
 *          in a bit field, such as in flag checking or feature enabling scenarios.
 *
 * Example Usage:
 *      Check if specific bits are set:
 *          Bit field: 01101101
 *          Bit mask:  00001101
 *      Test:
 *          (01101101 & 00001101) == 00001101
 *      ------------
 *          Result is true (all specified bits are set)
 *
 * @param x The bit field to test.
 * @param m The bit mask defining which bits to check.
 * @return True if all bits defined by the bit mask are set in the bit field, false otherwise.
 */
#define BITS_TEST(x,m)      (((x)&(m))==(m)))

/**
 * @brief Sets specified bits to logical 1 in a bit field.
 * @details This macro sets one or more specific bits of a bit field to 1 by logically
 *          OR-ing the bit field with a bit mask. The bit mask should have bits set
 *          (1s) at the positions you want to set and cleared (0s) elsewhere. When the
 *          bit field is OR-ed with this mask, the specified bits in the bit field
 *          are set to 1, while the rest remain unchanged.
 *
 *          This operation is commonly used for setting flags or enabling features
 *          represented by specific bits in a larger bit field.
 *
 * Example Usage:
 *      Set specific bits to 1:
 *          Bit field: 01001011
 *          Bit mask:  00000100
 *      Set:
 *          01001011 | 00000100
 *      ------------
 *          01001111 (Result with specified bits set to 1)
 *
 * @param x The bit field in which to set bits.
 * @param m The bit mask defining which bits to set to 1.
 */
#define BITS_SET(x,m)       ((x)|=(m))

/**
 * @brief Sets specified bits to logical 0 in a bit field.
 * @details This macro clears (sets to 0) one or more specific bits of a bit field by
 *          logically AND-ing the bit field with the inverted bit mask. The bit mask
 *          should have bits set (1s) at the positions you want to clear and cleared (0s)
 *          elsewhere. When the inverted mask is AND-ed with the bit field, the specified
 *          bits in the bit field are set to 0, while the rest remain unchanged.
 *
 *          This operation is typically used for clearing flags or disabling features
 *          represented by specific bits within a larger bit field.
 *
 * Example Usage:
 *      Clear specific bits to 0:
 *          Bit field: 01001011
 *          Bit mask:  00001000
 *      Clear:
 *          01001011 & ~00001000
 *      ------------
 *          01000011 (Result with specified bits cleared to 0)
 *
 * @param x The bit field in which to clear bits.
 * @param m The bit mask defining which bits to set to 0.
 */
#define BITS_CLEAR(x,m)     ((x)&=~(m))

/**
 * @brief Toggles specified bits in a bit field.
 * @details This macro toggles (flips) one or more specific bits of a bit field by logically
 *          XOR-ing the bit field with a bit mask. The bit mask should have bits set (1s) at
 *          the positions you want to toggle and cleared (0s) elsewhere. When the bit field is
 *          XOR-ed with this mask, the specified bits in the bit field are flipped, i.e., 0
 *          becomes 1 and 1 becomes 0, while the rest of the bits remain unchanged.
 *
 *          This operation is useful for changing the state of flags or toggling features
 *          on and off within a bit field.
 *
 * Example Usage:
 *      Toggle specific bits:
 *          Bit field: 01001011
 *          Bit mask:  00000110
 *      Toggle:
 *          01001011 XOR 00000110
 *      ------------
 *          01001101 (Result with specified bits toggled)
 *
 * @param x The bit field in which to toggle bits.
 * @param m The bit mask defining which bits to flip.
 */
#define BITS_TOGGLE(x,m)    ((x)^=(m))

#endif /* BITS_H_ */
