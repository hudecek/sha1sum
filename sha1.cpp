#include "sha1.h"
#include <cstdlib>
#include <sstream>
#include <dirent.h>

unsigned long sha1::rol(unsigned long number, unsigned by) {
    unsigned long n = (number) << by;
    n = n | ((number & 0xFFFFFFFF) >> (32-by));
    return n;
}

sha1::sha1(const char* filename)
{
    //setting constants

    h0 = 0x67452301;
    h1 = 0xEFCDAB89;
    h2 = 0x98BADCFE;
    h3 = 0x10325476;
    h4 = 0xC3D2E1F0;

    buffer = "";

    //checking whether supplied filename is a valid file
    DIR* dir = opendir(filename);
    if(dir != NULL) {
        cerr << "Error: '" << filename << "' is a directory!" << endl;
        closedir(dir);
        exit(1);
    }

    file.open(filename, std::ifstream::ate | std::ifstream::binary);
    if(!file.is_open()) {
        cerr << "No file found: " << filename << endl;
        exit(2);
    }

    //setting size parameter that is used while padding and blocks count (informative)
    size =  file.tellg();
    blocks = size/64;
    file.seekg(0);
    file.clear();
}

sha1::~sha1() {
    file.close();
}

//main function
void sha1::generate() {
    for(int i = 0; i<64; i++) {
        if(!file.eof())
            buffer += file.get();
    }

    unsigned long words[80];

    while(!file.eof()) {

        //unsigned long can fit 4 chars
        for(int i = 0; i<16; i++) {
            //big endian
            words[i] = ((buffer[4*i]  & 0xff)<<24) + ((buffer[4*i+1] & 0xff) << 16) + ((buffer[4*i + 2] & 0xff) << 8) + (buffer[4*i + 3] & 0xff);
        }

        this->update(words);
        buffer = "";

        for(int i = 0; i<64; i++) {
            if(!file.eof())
                buffer += file.get();
        }
    }

    buffer.resize(buffer.size() - 1);

    buffer += char(0x80);   //start of padding

    if(buffer.size() > 56) {

        //pad with 0s, update the hash
        while(buffer.size() < 64) {
            buffer += char(0x00);
        }

        for(int i = 0; i<16; i++) {
            words[i] = ((buffer[4*i]  & 0xff)<<24) + ((buffer[4*i+1] & 0xff) << 16) + ((buffer[4*i + 2] & 0xff) << 8) + (buffer[4*i + 3] & 0xff);
        }
        this->update(words);
        buffer = "";
    }

    //pad with 0s till the last 64b (8B)
    while(buffer.size() <= 56) {
        buffer += char(0x00);
    }

    //first 56B
    for(int i = 0; i<14; i++) {
        words[i] = ((buffer[4*i]  & 0xff)<<24) + ((buffer[4*i+1] & 0xff) << 16) + ((buffer[4*i + 2] & 0xff) << 8) + (buffer[4*i + 3] & 0xff);
    }

    //last 8B - size (bigendian)
    words[15] = (size*8);
    words[14] = (size*8) >> 32;
    this->update(words);
}

//transformation function
void sha1::update(unsigned long words[16]) {
    unsigned long a = h0, b = h1, c = h2, d = h3, e = h4, f, k, temp;

    for(int i = 16; i<80; i++) {
        words[i] = rol((words[i-3] ^ words[i-8] ^ words[i-14] ^ words[i-16]), 1);
    }

    for(int i = 0; i<80; i++) {

        if (0 <= i  && i <= 19) {
            f = (b & (c ^ d)) ^ d;
            k = 0x5A827999;
        } else if (20 <= i && i<= 39) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;

        } else if (40 <= i && i <= 59) {
            f = ((b | c) & d)|(b & c);
            k = 0x8F1BBCDC;
        } else if (60 <= i && i <= 79) {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        temp = (rol(a,5)) + f + e + k + words[i];
        e = d;
        d = c;
        c = rol(b, 30);
        b = a;
        a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}

//prints results
void sha1::print() {
    cout << hex << (h0 & 0xffffffff) << (h1 & 0xffffffff) << (h2 & 0xffffffff) << (h3 & 0xffffffff) << (h4 & 0xffffffff) << endl;
}

