#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#include "stb_image.h"

// DO NOT TOUCH THIS FILE
/*
In all other places where you need something from this library, just include the stb_image.h header as usual.

These "implementation" macros are a "trick" used by some library authors to make "installing" their library easy. 
The idea is that when a specific macro (chosen by the library authors) is defined before the header of that library is included, 
some code with the actual implementation will be added. That is why this must be in a source file instead of a header and only in one source file 
(otherwise you get multiple definitions for the same functions).
*/
