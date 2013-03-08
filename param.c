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

#ifndef HASHROT_PARAM
#define HASHROT_PARAM

typedef struct Configuration {

    short key_from_file;

    int threads;

    unsigned char* password;
    char* input_file_name;
    char* output_file_name;
    char* key_file_name;
    char* initial_key;

} Configuration;

typedef struct ThreadParameters {

    int thread_id;
    Configuration* config;

} ThreadParameters;

#endif
