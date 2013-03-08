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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "sha2/sha2.h"
#include <stdio.h>

void hash_from_keyfile(char* keyfile_name, unsigned char* hash) {

    int key = open(keyfile_name, O_RDONLY);
    unsigned char buffer[64];
    register short bytes = 0;
    sha512_ctx ctx;

    sha512_init(&ctx);

    bytes = read(key, buffer, 64);

    while (bytes > 0) {

        sha512_update(&ctx, buffer, bytes);

        bytes = read(key, buffer, 64);

    }

    sha512_final(&ctx, hash);

    close(key);

}
#endif
