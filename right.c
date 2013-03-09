/*
    Hashrot - A SHA512 based double-caesar cipher
    Copyright (C) 2013  Kevin Conway

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include "key.c"
#include "sha2/sha2.h"

void right (unsigned char* hash, FILE* input, unsigned long file_size, FILE* output) {

    unsigned char buffer[64];
    register unsigned int chunks = file_size / 64, current_chunk = 0;
    register unsigned char temp;
    register short byte, bytes_read;

    while (current_chunk <= chunks) {

        bytes_read = fread(buffer, 1, 64, input);

        // Iterate through bytes and apply modulus addition using the
        // same byte in the SHA hash.
        // Just to be clear, 255 + 1 == 0 using the modulus addition.
        int byte = 0;
        while (byte < bytes_read) {
            buffer[byte] = (buffer[byte] + hash[byte]) % 256;
            byte = byte + 1;
        }

        // This section performs the spatial rotation of each byte in reference
        // to the key. The rotation is based on the value of the current
        // position in the hash. The blocks are 64 bytes in length so the value
        // of the current position in the hash must be mod 64 to keep the
        // rotation within the bounds of the block. As a small scale
        // illustration, take a block of length 5 with a key of length 5 using
        // a base 5 number system:
        //
        // Hash to use for encryption:  [*3*, 4, 3, 2, 1]
        // Block to be encrypted:       [*2*, 3, 0, *1*, 4]
        // After first rotation:        [*1*, 3, 0, *2*, 4]
        //
        // The byte at offset 0 was swapped with the byte at offset 3. The idea
        // here being that each byte in the sequence is swapped with the byte
        // that has a matching offset to the same byte in the encryption key.
        byte = 0;
        while (byte < bytes_read) {
            temp = buffer[hash[byte] % bytes_read];
            buffer[hash[byte] % bytes_read] = buffer[byte];
            buffer[byte] = temp;
            byte = byte + 1;
        }

        // Write the newly encrypted block to the output file and generate a new
        // hash to use as the key. The first hash used by the encryption is a
        // direct SHA512 of the password given. The following hashes are SHA512s
        // of the hash that came before. This way each block is encrypted using
        // a different key, but all keys can be generated from the initial
        // password input.
        fwrite(buffer, 1, bytes_read, output);
        sha512(hash, 64, hash);
        current_chunk = current_chunk + 1;
    }

    return;

};
