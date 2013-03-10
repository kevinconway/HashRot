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

int main(int argc, char *argv[]) {

    register int argument = 0, thread = 0;

    clock_t runtime;

    pthread_t* thread_handles = NULL;

    void* (*encrypt)(void*) = &right;

    Configuration* config = (Configuration*) malloc(sizeof(Configuration));
    ThreadParameters* params = NULL;

    FILE* input = NULL;
    FILE* output = NULL;

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

            encrypt = &left;

        }

        if (    strcmp(argv[argument], "-t") == 0 ||
                strcmp(argv[argument], "--threads") == 0   ) {

            argument = argument + 1;

            config->threads = atoi(argv[argument]);
            thread_handles = (pthread_t*) malloc(
                                    sizeof(pthread_t) * (config->threads - 1)
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

            config->key_file_name = argv[argument];
            config->key_from_file = 1;

        }

        if (    strcmp(argv[argument], "-i") == 0 ||
                strcmp(argv[argument], "--input") == 0   ) {

            argument = argument + 1;

            config->input_file_name = argv[argument];

        }

        if (    strcmp(argv[argument], "-o") == 0 ||
                strcmp(argv[argument], "--output") == 0   ) {

            argument = argument + 1;

            config->output_file_name = argv[argument];

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

    input = fopen(config->input_file_name, "rb");
    if (input == NULL) {

        puts("Error: Could not open input file.");
        return -1;

    }
    fclose(input);

    output = fopen(config->output_file_name, "rb+");
    if (output == NULL) {

        output = fopen(config->output_file_name, "wb");

        if (output == NULL) {

            puts("Error: Could not open output file.");
            return -1;

        }

    }
    fclose(output);

    runtime = clock();

    for (thread = 1; thread < config->threads; thread = thread + 1) {

        params = (ThreadParameters*)malloc(sizeof(ThreadParameters));
        params->thread_id = thread;
        params->config = config;

        pthread_create( &thread_handles[thread - 1],
                        NULL,
                        encrypt,
                        (void*) params
                    );

    }

    params = (ThreadParameters*)malloc(sizeof(ThreadParameters));
    params->thread_id = 0;
    params->config = config;

    encrypt((void*) params);

    runtime = clock() - runtime;

    printf( "File processed in %f seconds using %i threads.\n",
            (float)runtime / CLOCKS_PER_SEC,
            config->threads
        );

}
