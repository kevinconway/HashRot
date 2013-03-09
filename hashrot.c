// Hashrot - A SHA512 based double-caesar cipher
// Copyright (C) 2012  Kevin Conway

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string.h>
#include <stdio.h>
#include "sha2/sha2.h"
#include "help.c"
#include "key.c"
#include "right.c"
#include "left.c"

int main(int argc, char *argv[]) {

    unsigned char hash[64];

    FILE* input = NULL;
    FILE* output = NULL;

    char* input_file_name = NULL;
    char* output_file_name = NULL;

    unsigned long file_size;

    short received_key = 0;
    short arg = 0;

    void (*encrypt)(unsigned char*, FILE*, unsigned long, FILE*) = &right;

    while(arg < argc) {

        if (strcmp(argv[arg], "-r") == 0 || strcmp(argv[arg], "--reverse") == 0) {

            encrypt = &left;

        }

        if (strcmp(argv[arg], "-p") == 0 || strcmp(argv[arg], "--password") == 0) {

            arg = arg + 1;

            sha512((unsigned char*)argv[arg], (unsigned) strlen(argv[arg]), hash);

            received_key = 1;

        }

        if (strcmp(argv[arg], "-k") == 0 || strcmp(argv[arg], "--keyfile") == 0) {

            arg = arg + 1;

            hash_from_keyfile(argv[arg], hash);

            received_key = 1;

        }

        if (strcmp(argv[arg], "-i") == 0 || strcmp(argv[arg], "--input") == 0) {

            arg = arg + 1;
            input_file_name = argv[arg];

        }

        if (strcmp(argv[arg], "-o") == 0 ||strcmp(argv[arg], "--output") == 0) {

            arg = arg + 1;
            output_file_name = argv[arg];

        }

        if (strcmp(argv[arg], "-h") == 0 || strcmp(argv[arg], "--help") == 0) {

            print_help();
            return 0;

        }

        arg = arg + 1;
    }

    if (received_key == 0) {

        puts("Error: No password defined for encryption.");
        return -1;

    }

    if (input_file_name == NULL) {

        puts("Error: No input file defined.");
        return -1;

    }

    if (output_file_name == NULL) {

        puts("Error: No output file defined.");
        return -1;

    }

    input = fopen(input_file_name, "rb");
    output = fopen(output_file_name, "wb");

    fseek(input, 0, SEEK_END);
    file_size = ftell(input);
    rewind(input);

    encrypt(hash, input, file_size, output);

    fclose(input);
    fclose(output);
    return 0;
}
