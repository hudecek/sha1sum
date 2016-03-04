/*
 * program computes a SHA-1 hash of a file supplies
 * created as a part of homework 3 for CSCE 477 course at UNL
 * author: Jan Hudecek
 *
 * references:
 * https://en.wikipedia.org/wiki/SHA-1 - pseudocode
 * http://stackoverflow.com/questions/3844546/how-do-i-check-if-a-directory-is-a-file-or-folder - find out whether path is folder or file
 * pushover.sourceforge.net/repos/head/src/ - the idea of using the buffer to store blocks of data
 * lecture notes
 * linux sha1sum function for debugging
 */

#include <iostream>
#include <fstream>
#include "sha1.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cerr << "Add a name of the file to compute hash of as an argument - ./sha1 filename"<< endl;
        return 1;
    }

    sha1 hash(argv[1]);
    hash.generate();
    hash.print();

    return 0;
}

