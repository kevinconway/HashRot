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
#ifndef HASHROT_RIGHT
#define HASHROT_RIGHT

#include "imports.c"

void* right(void* config) {

    ThreadParameters* params = (ThreadParameters*) config;

    unsigned char hash[64];
    unsigned char* buffer = malloc(sizeof(unsigned char) * BUFFER_SIZE);
    unsigned char* buffer_partition;
    register unsigned char temp;
    register unsigned int block, byte_block, hash_count, bytes_read;
    register short byte, partition_bytes;

    int input = open(params->config->input_file_name, O_RDWR);
    int output = open(params->config->output_file_name, O_RDWR);

    if (output < 0) {
        puts("Could not open output file.");
        return NULL;
    }

    memcpy(hash, params->config->initial_key, 64);

    for (   hash_count = 0;
            hash_count < params->thread_id * (BUFFER_SIZE / 64);
            hash_count = hash_count + 1) {

            sha512(hash, 64, hash);

        }

    block = params->thread_id;
    do {

        lseek(input, block * BUFFER_SIZE, SEEK_SET);
        lseek(output, block * BUFFER_SIZE, SEEK_SET);

        bytes_read = read(input, buffer, BUFFER_SIZE);

        for (byte_block = 0; byte_block < (bytes_read / 64) + 1; byte_block = byte_block + 1) {

            buffer_partition = &buffer[byte_block * 64];
            partition_bytes = byte_block < bytes_read / 64 ?
                                64 :
                                bytes_read % 64;

            for (byte = 0; byte < partition_bytes; byte = byte + 1) {

                buffer_partition[byte] = (buffer_partition[byte] + hash[byte]) % 256;

            }

            for (byte = 0; byte < partition_bytes; byte = byte + 1) {

                temp = buffer_partition[hash[byte] % partition_bytes];
                buffer_partition[hash[byte] % partition_bytes] = buffer_partition[byte];
                buffer_partition[byte] = temp;

            }

        }

        if (bytes_read > 0) {

            write(output, buffer, bytes_read);

        }

        for (   hash_count = 0;
                hash_count < params->config->threads * (BUFFER_SIZE / 64);
                hash_count = hash_count + 1) {

            sha512(hash, 64, hash);

        }


        block = block + params->config->threads;

    } while (bytes_read > 0);

    close(input);
    close(output);

    return NULL;

};

#endif
