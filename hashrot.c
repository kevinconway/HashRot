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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "sha2/sha2.h"

void forward(unsigned char* hash, FILE* infile, unsigned long file_size, FILE* ofile);
void backward(unsigned char* hash, FILE* infile, unsigned long file_size, FILE* ofile);
void print_help();

int main(int argc, char *argv[]) {

    unsigned char* password = NULL;
    char* infile_name = NULL;
    FILE* infile = NULL;
    FILE* ofile = NULL;
    unsigned long file_size;
    char* ofile_name = NULL;
    unsigned char hash[64];
    bool forwerd = true;

    int x = 0;
    while(x < argc) {
        if (strcmp(argv[x], "-b") == 0) {
            forwerd = false;
        }

        if (strcmp(argv[x], "-p") == 0) {
            x = x + 1;
            password = argv[x];
        }
        
        if (strcmp(argv[x], "-i") == 0) {
            x = x + 1;
            infile_name = argv[x];
        }

        if (strcmp(argv[x], "-o") == 0) {
            x = x + 1;
            ofile_name = argv[x];
        }

        if (strcmp(argv[x], "-h") == 0 || strcmp(argv[x], "--help") == 0) {
            print_help();
            return 0;
        }
        
        x = x + 1;
    }

    if (password == NULL) {
        puts("Error: No password defined for encryption.");
        return -1;
    }

    if (infile_name == NULL) {
        puts("Error: No input file defined.");
        return -1;
    }

    if (ofile_name == NULL) {
        puts("Error: No output file defined.");
        return -1;
    }

    infile = fopen(infile_name, "rb");
    ofile = fopen(ofile_name, "wb");

    fseek(infile, 0, SEEK_END);
    file_size = ftell(infile);
    rewind(infile);

    sha512(password, (unsigned) strlen(password), hash);

    if(forwerd == true) {
        forward(hash, infile, file_size, ofile);
        fclose(infile);
        fclose(ofile);
        return 0;
    }

    backward(hash, infile, file_size, ofile);
    fclose(infile);
    fclose(ofile);
    return 0;
}

void forward(unsigned char* hash, FILE* infile, unsigned long file_size, FILE* ofile) {

    unsigned char buffer[64];
    unsigned int chunks = file_size / 64;
    unsigned int left_overs = file_size % 64;

    int current_chunk = 0;
    while (current_chunk < chunks) {

        fread(buffer, 1, 64, infile);

        // Iterate through bytes and apply modulus addition using the
        // same byte in the SHA hash.
        int position = 0;
        while (position < 64) {
            buffer[position] = (buffer[position] + hash[position]) % 256;
            position = position + 1;
        }

        // Iterate through bytes and swap with the byte in
        // the position that matches the value of the hash
        // at the given index.
        position = 0;
        while (position < 64) {
            unsigned char temp = buffer[hash[position] % 64];
            buffer[hash[position] % 64] = buffer[position];
            buffer[position] = temp;
            position = position + 1;
        }

        // Write to file and reroll a new hash
        fwrite(buffer, 1, 64, ofile);
        sha512(hash, 64, hash);
        current_chunk = current_chunk + 1;
    }

    if (left_overs > 0) {

        fread(buffer, 1, left_overs, infile);

        int position = 0;
        while (position < left_overs) {

            buffer[position] = (buffer[position] + hash[position]) % 256;
            position = position + 1;
        }

        position = 0;
        while (position < left_overs) {
            unsigned char temp = buffer[hash[position] % left_overs];
            buffer[hash[position] % left_overs] = buffer[position];
            buffer[position] = temp;
            position = position + 1;
        }

        fwrite(buffer, 1, left_overs, ofile);
    }
    
    return;
}

void backward(unsigned char* hash, FILE* infile, unsigned long file_size, FILE* ofile) {

    unsigned char buffer[64];
    unsigned int chunks = file_size / 64;
    unsigned int left_overs = file_size % 64;

    unsigned int current_chunk = 0;
    while (current_chunk < chunks) {

        fread(buffer, 1, 64, infile);

        int position = 63;
        while (position >= 0) {
            unsigned char temp = buffer[hash[position] % 64];
            buffer[hash[position] % 64] = buffer[position];
            buffer[position] = temp;
            position = position - 1;
        }

        position = 0;
        while (position < 64) {
            buffer[position] = (buffer[position] - hash[position]) % 256;
            position = position + 1;
        }

        fwrite(buffer, 1, 64, ofile);
        sha512(hash, 64, hash);
        current_chunk = current_chunk + 1;
    }

    if (left_overs > 0) {

        fread(buffer, 1, left_overs, infile);

        int position = left_overs - 1;
        while (position >= 0) {
            unsigned char temp = buffer[hash[position] % left_overs];
            buffer[hash[position] % left_overs] = buffer[position];
            buffer[position] = temp;
            position = position - 1;
        }
        
        position = 0;
        while (position < left_overs) {

            buffer[position] = (buffer[position] - hash[position]) % 256;
            position = position + 1;
        }

        fwrite(buffer, 1, left_overs, ofile);
    }
    
    return;
}

void print_help() {

    puts("HashRot Help:");
    puts("");
    puts("-p or --password:         The password used to encrypt the file.");
    puts("-i or --input:            The file to encrypt.");
    puts("-p or --output:           Name to use for the new encrypted file.");
    puts("-b or --backward:         Toggle to run the algorithm backwards.");
    puts("-b or --help:             Display this output.");
    puts("");
    puts("Example:");
    puts("hashrot -p mysecretpassword -i plain.txt -o encrypted.txt");
    puts("hashrot -p mysecretpassword -i encrypted.txt -i decrypted.txt -b");

    return;
}
