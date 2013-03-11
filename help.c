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

#ifndef HASHROT_PRINT_HELP
#define HASHROT_PRINT_HELP

#include "imports.c"

void print_help() {

    puts("HashRot Help:");
    puts("");
    puts("-p or --password:         The password used to encrypt the file.");
    puts("-k or --keyfile:          The file to use as the encryption key.");
    puts("-i or --input:            The file to encrypt.");
    puts("-o or --output:           Name to use for the new encrypted file.");
    puts("-r or --reverse:          Run the encryption in reverse.");
    puts("-t or --threads:          Number of threads to use.");
    puts("-h or --help:             Display this output.");
    puts("");
    puts("Example:");
    puts("hashrot -p mysecretpassword -i plain.txt -o encrypted.txt");
    puts("hashrot -k ./keyfile.txt -i encrypted.txt -i decrypted.txt -r");

    return;
}

#endif
