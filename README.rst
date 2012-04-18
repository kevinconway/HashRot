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

The command line options are still a little fragile. Only one parameter is
optional. Any missing required parameters at this point will probably cause
a SegFault. When I get a little time I'll add some better error prevention.
Assuming you name the compiled binary `hashrot`.

    hashrot -p <password> -i <input_file> -o <output_file> [-b]

The optional `-b` indicates that you want the to run the reverse encryption.
You would use `-b` to decrypt something encrypted without `-b` and the
other way around.

Road Map
=======

1. Implement enforcement of required command line options.
2. Implement a printable help menu.
3. Add more comments in `forward` and `backward` functions.
4. Add option for multiple passes. (Wait until you see my double ROT13!)
5. Large scale tests. (Handled "War and Peace" just fine, but I want something
   meatier...)

License
=======

The HashRot code is released under the GPLv3. 

The attached SHA2 implementation was released under a BSD style license by
Olivier Gay <olivier.gay@a3.epfl.ch>.
