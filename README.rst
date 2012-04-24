==============================================
Hashrot - A SHA512 Based Double-Caesar Cipher
==============================================

Summary
=======

This is a project I used to teach myself a little C++ a few years back that I
decided to port to C. When I originally wrote this, I didn't know it was 
called a Double-Caesar cipher, it just seemed like a fun way to pick up
the file API and get some time in working with bytes instead of strings all
the time. I don't expect that anyone will find this terribly useful, but it
was actually fun to make.

Usage
=====

The command line options are detailed in the help menu outputted by the CLI::

    HashRot Help:
    
    -p or --password:         The password used to encrypt the file.
    -i or --input:            The file to encrypt.
    -p or --output:           Name to use for the new encrypted file.
    -b or --backward:         Toggle to run the algorithm backwards.
    -b or --help:             Display this output.

    Example:
    hashrot -p mysecretpassword -i plain.txt -o encrypted.txt
    hashrot -p mysecretpassword -i encrypted.txt -i decrypted.txt -b

The script should now check for missing parameters and notify with a message.

Road Map
=======

1. Implement enforcement of required command line options.
    
    Check.

2. Implement a printable help menu.

    Check.

3. Add more comments in `forward` and `backward` functions.

    Check.

4. Add option for multiple passes. (Wait until you see my double ROT13!)

5. Large scale tests. (Handled "War and Peace" just fine, but I want something
   meatier...)

   Ran the Ubuntu ISO through. Something like 600 megs so not that big. Runs
   alright.

6. Python wrappers

License
=======

The HashRot code is released under the GPLv3. 

The attached SHA2 implementation was released under a BSD style license by
Olivier Gay <olivier.gay@a3.epfl.ch>.
