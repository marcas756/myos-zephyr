#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>
#include <stddef.h>

/*!
    \brief      Accumulates a single byte into an ongoing SDBM hash calculation.

    \details    This macro is a core part of the SDBM hash function. It takes a
                current hash value and a single byte, then applies a specific
                calculation to accumulate the byte into the hash. The calculation
                involves bitwise operations and arithmetic that are characteristic
                of the SDBM algorithm, known for its simplicity and effectiveness
                in distributing hash values.

                This macro is typically used within a loop that processes each byte
                of a data block, contributing to the final hash value.

    \note       Since this is a macro, care must be taken with the arguments to
                ensure they are not expressions with side effects, as they will be
                evaluated multiple times.

    \example    Usage:
                \code{.c}
                uint32_t hash = 0;
                const char *data = "Example data";
                for (size_t i = 0; data[i] != '\0'; ++i) {
                    hash = hash_sdbm_acc(hash, data[i]);
                }
                \endcode

    \param hash     The current hash value.
    \param byte     The byte to be accumulated into the hash.

    \return         The updated hash value after accumulating the byte.
*/
#define hash_sdbm_acc(hash,byte) \
	(byte + (hash << 6) + (hash << 16) - hash)

/*!
    \brief      Calculates a hash value using the SDBM algorithm.

    \details    This function computes a hash value by processing each byte in
                the provided data block using the SDBM hashing algorithm.
                It starts with an initial seed value and iterates over each byte
                of data, applying a specific hash accumulation operation defined
                in `hash_sdbm_acc` macro. The function is designed for efficiency
                and simplicity, making it suitable for various hashing needs.

                The SDBM algorithm is known for its simplicity and good
                distribution properties, making it a popular choice in situations
                where a non-cryptographic hash is sufficient.

    \note       The function assumes that the data pointer is valid and points
                to a memory block of at least `size` bytes. Passing a null pointer
                or an incorrect size might result in undefined behavior.

    \example    Usage:
                \code{.c}
                uint32_t seed = 0;
                char data[] = "Example data";
                size_t size = sizeof(data) - 1; // excluding the null terminator
                uint32_t hash = hash_sdbm(seed, data, size);
                printf("Hash: %u\n", hash);
                \endcode

    \param seed     The initial seed value for the hash computation.
    \param data     Pointer to the data block to be hashed.
    \param size     Size of the data block in bytes.

    \return         The computed hash value as an unsigned 32-bit integer.
*/
uint32_t hash_sdbm(uint32_t seed, void *data, size_t size);




#endif /* HASH_H_ */
