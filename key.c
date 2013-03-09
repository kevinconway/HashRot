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

#ifndef HASHROT_KEY
#define HASHROT_KEY

#include <Windows.h>
#include <stdio.h>
#include "sha2.h"

void hash_from_keyfile(wchar_t* keyfile_name, unsigned char* hash) {

    HANDLE key = CreateFile(    keyfile_name,
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL
                            );

    unsigned char buffer[64];
    unsigned long bytes = 0;
    sha512_ctx ctx;

    sha512_init(&ctx);

    ReadFile(key, buffer, 64, &bytes, NULL);

    while (bytes > 0) {

        sha512_update(&ctx, buffer, bytes);

        ReadFile(key, buffer, 64, &bytes, NULL);

    }

    sha512_final(&ctx, hash);

    CloseHandle(key);

}

#endif
