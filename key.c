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

#include <stdio.h>

void hash_from_keyfile(char* keyfile_name, unsigned char* hash) {

    FILE* key = fopen(keyfile_name, "rb");
    unsigned char buffer[64];
    register short bytes = 0;
    sha512_ctx ctx;

    sha512_init(&ctx);

    bytes = fread(buffer, 1, 64, key);

    while (bytes > 0) {

        sha512_update(&ctx, buffer, bytes);

        bytes = fread(buffer, 1, 64, key);

    }

    sha512_final(&ctx, hash);

    fclose(key);

}
#endif
