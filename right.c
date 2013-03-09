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

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "param.c"
#include "key.c"
#include "sha2.h"

DWORD WINAPI right (LPVOID config) {

    ThreadParameters* params = (ThreadParameters*) config;

    unsigned char buffer[64], hash[64];
    register unsigned char temp;
    register unsigned int file_size, chunks, current_chunk, hash_count;
    register short byte;
    unsigned long bytes_read, bytes_written;

    HANDLE input = CreateFile(  params->config->input_file_name,
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL
                            );

    HANDLE output = CreateFile( params->config->output_file_name,
                                GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL
                            );

    if (output == NULL) {
        puts("Could not open output file.");
        return -1;
    }

    file_size = GetFileSize(input, NULL);

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

        SetFilePointer(input, current_chunk * 64, NULL, FILE_BEGIN);
        SetFilePointer(output, current_chunk * 64, NULL, FILE_BEGIN);

        ReadFile(input, buffer, 64, &bytes_read, NULL);

        for (byte = 0; byte < bytes_read; byte = byte + 1) {

            buffer[byte] = (buffer[byte] + hash[byte]) % 256;

        }

        for (byte = 0; byte < bytes_read; byte = byte + 1) {

            temp = buffer[hash[byte] % bytes_read];
            buffer[hash[byte] % bytes_read] = buffer[byte];
            buffer[byte] = temp;

        }

        if (bytes_read > 0) {

            WriteFile(output, buffer, bytes_read, &bytes_written, NULL);

        }

        for (hash_count = 0; hash_count < params->config->threads; hash_count = hash_count + 1) {

            sha512(hash, 64, hash);

        }

    }

    CloseHandle(input);
    CloseHandle(output);

    return 0;

};

#endif
