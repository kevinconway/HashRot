==============================================
HashRot - A SHA512 Based Double-Caesar Cipher
==============================================

Summary
=======

HashRot is a block based "encryption" algorithm that uses two forms of rotation
to obfuscate information. One rotation is "vertical" where each byte of the
block is added, with modulus, to the corresponding byte of the key. The second
rotation is "horizontal" where each byte of the block is swapped with another
byte in the block based on the corresponding byte in the key.

The encryption has two flavours, a "left" shift and a "right" shift, that modify
the source file based on the given key. Each version will undo the other.

In practical terms, I use this project as a way to practice and maintain my
familiarity with C programming. I return to this project from time to time in
order to try new techniques and new ideas. I would not suggest using this
program to secure truly confidential information.

This version of the program is single threaded and makes use of standard C
libraries.

Branches
========

For other versions of this code base check out the other branches.

-   **master**

    The original, serial version of the code.

-   thread

    A version that uses pthreads and POSIX file operations.

Usage
=====

The command line options are detailed in the help menu outputted by the CLI::

    HashRot Help:

    -p or --password:         The password used to encrypt the file.
    -i or --input:            The file to encrypt.
    -o or --output:           Name to use for the new encrypted file.
    -r or --reverse:          Run the encryption in reverse.
    -h or --help:             Display this output.

    Example:
    hashrot -p mysecretpassword -i plain.txt -o encrypted.txt
    hashrot -p mysecretpassword -i encrypted.txt -i decrypted.txt -r

License
=======

The HashRot code is released under the GPLv3.

The attached SHA2 implementation was released under a BSD style license by
Olivier Gay <olivier.gay@a3.epfl.ch>.
