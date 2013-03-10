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

#include "imports.c"

void* right (void* config) {

    ThreadParameters* params = (ThreadParameters*) config;

    unsigned char buffer[64], hash[64];
    register unsigned char temp;
    register unsigned int file_size, chunks, current_chunk, hash_count;
    register short byte, bytes_read;

    int input = open(params->config->input_file_name, O_RDWR);
    struct stat input_stat;
    fstat(input, &input_stat);
    int output = open(params->config->output_file_name, O_RDWR);

    if (output < 0) {
        puts("Could not open output file.");
        return NULL;
    }

    file_size = input_stat.st_size;

    chunks = file_size / 64;
    current_chunk = params->thread_id;


    if (params->config->key_from_file == 1) {

        hash_from_keyfile(  params->config->key_file_name,
                            hash
                        );

    } else {

        sha512(params->config->password, (unsigned) strlen((char*) params->config->password), hash);

    }

    hash_count = 0;
    while (hash_count < current_chunk) {

        sha512(hash, 64, hash);

        hash_count = hash_count + 1;
    }

    for (current_chunk = params->thread_id; current_chunk <= chunks; current_chunk = current_chunk + params->config->threads) {

        lseek(input, current_chunk * 64, SEEK_SET);
        lseek(output, current_chunk * 64, SEEK_SET);

        bytes_read = read(input, buffer, 64);

        for (byte = 0; byte < bytes_read; byte = byte + 1) {

            buffer[byte] = (buffer[byte] + hash[byte]) % 256;

        }

        for (byte = 0; byte < bytes_read; byte = byte + 1) {

            temp = buffer[hash[byte] % bytes_read];
            buffer[hash[byte] % bytes_read] = buffer[byte];
            buffer[byte] = temp;

        }

        if (bytes_read > 0) {

            write(output, buffer, bytes_read);

        }

        for (hash_count = 0; hash_count < params->config->threads; hash_count = hash_count + 1) {

            sha512(hash, 64, hash);

        }

    }

    close(input);
    close(output);

    return NULL;

};
