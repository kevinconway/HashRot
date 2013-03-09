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

void left (unsigned char* hash, FILE* input, unsigned long file_size, FILE* output) {

    unsigned char buffer[64];
    register unsigned int chunks = file_size / 64, current_chunk = 0;
    register unsigned char temp;
    register short byte, bytes_read;

    while (current_chunk <= chunks) {

        bytes_read = fread(buffer, 1, 64, input);

        byte = bytes_read - 1;
        while (byte >= 0) {
            temp = buffer[hash[byte] % bytes_read];
            buffer[hash[byte] % bytes_read] = buffer[byte];
            buffer[byte] = temp;
            byte = byte - 1;
        }

        byte = 0;
        while (byte < bytes_read) {
            buffer[byte] = (buffer[byte] - hash[byte]) % 256;
            byte = byte + 1;
        }

        fwrite(buffer, 1, bytes_read, output);
        sha512(hash, bytes_read, hash);
        current_chunk = current_chunk + 1;
    }

    return;

};
