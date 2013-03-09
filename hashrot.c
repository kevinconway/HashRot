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
#include <Windows.h>
#include <time.h>
#include "sha2.h"
#include "help.c"
#include "param.c"
#include "right.c"
#include "left.c"

int main(int argc, char *argv[]) {

    register int argument = 0, buffer_size = 0, thread = 0, reveresed = 0;

    clock_t runtime;

    HANDLE* thread_handles = NULL;

    Configuration* config = (Configuration*) malloc(sizeof(Configuration));
    ThreadParameters* params = NULL;

    HANDLE input = NULL;
    HANDLE output = NULL;

    if (argc < 4) {

        print_help();
        return 0;

    }

    config->key_from_file = 0;
    config->threads = 1;
    config->password = NULL;
    config->input_file_name = NULL;
    config->output_file_name = NULL;
    config->key_file_name = NULL;
    config->initial_key = NULL;

    while (argument < argc) {

        if (    strcmp(argv[argument], "-h") == 0 ||
                strcmp(argv[argument], "--help") == 0   ) {

            print_help();
            return 0;

        }

        if (    strcmp(argv[argument], "-r") == 0 ||
                strcmp(argv[argument], "--reverse") == 0   ) {

            reveresed = 1;

        }

        if (    strcmp(argv[argument], "-t") == 0 ||
                strcmp(argv[argument], "--threads") == 0   ) {

            argument = argument + 1;

            config->threads = atoi(argv[argument]);
            thread_handles = (HANDLE*) malloc(
                                    sizeof(HANDLE) * (config->threads - 1)
                                );

        }

        if (    strcmp(argv[argument], "-p") == 0 ||
                strcmp(argv[argument], "--password") == 0   ) {

            argument = argument + 1;

            config->password = (unsigned char*)argv[argument];

        }

        if (    strcmp(argv[argument], "-k") == 0 ||
                strcmp(argv[argument], "--keyfile") == 0   ) {

            argument = argument + 1;

            buffer_size = MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, NULL, 0);

            config->key_file_name = (wchar_t*) malloc(sizeof(wchar_t) * buffer_size);

            MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, config->key_file_name, buffer_size);

            config->key_from_file = 1;

        }

        if (    strcmp(argv[argument], "-i") == 0 ||
                strcmp(argv[argument], "--input") == 0   ) {

            argument = argument + 1;

            buffer_size = MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, NULL, 0);

            config->input_file_name = (wchar_t*) malloc(sizeof(wchar_t) * buffer_size);

            MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, config->input_file_name, buffer_size);

        }

        if (    strcmp(argv[argument], "-o") == 0 ||
                strcmp(argv[argument], "--output") == 0   ) {

            argument = argument + 1;

            buffer_size = MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, NULL, 0);

            config->output_file_name = (wchar_t*) malloc(sizeof(wchar_t) * buffer_size);

            MultiByteToWideChar(CP_ACP, NULL, argv[argument], -1, config->output_file_name, buffer_size);

        }

        argument = argument + 1;

    }

    if (config->key_from_file == 0 && config->password == NULL) {

        puts("Error: No password given.");
        return -1;

    }

    if (config->input_file_name == NULL) {

        puts("Error: No input file given.");
        return -1;

    }

    if (config->output_file_name == NULL) {

        puts("Error: No output file given.");
        return -1;

    }

    input = CreateFile(     config->input_file_name,
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL
                        );
    if (input == NULL) {

        puts("Error: Could not open input file.");
        return -1;

    }
    CloseHandle(input);

    output = CreateFile(    config->output_file_name,
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL
                        );
    if (output == NULL) {

        puts("Error: Could not open output file.");
        return -1;

    }
    CloseHandle(output);

    runtime = clock();

    for (thread = 1; thread < config->threads; thread = thread + 1) {

        params = (ThreadParameters*)malloc(sizeof(ThreadParameters));
        params->thread_id = thread;
        params->config = config;

        thread_handles[thread - 1] = CreateThread(  NULL,
                                                    0,
                                                    reveresed == 0 ?
                                                        right : left,
                                                    (void*) params,
                                                    0,
                                                    NULL
                                                );

    }

    params = (ThreadParameters*)malloc(sizeof(ThreadParameters));
    params->thread_id = 0;
    params->config = config;

    if (reveresed == 0) {

        right((void*) params);

    } else {

        left((void*) params);

    }

    runtime = clock() - runtime;

    printf( "File processed in %f seconds using %i threads.\n",
            (float)runtime / CLOCKS_PER_SEC,
            config->threads
        );

}
