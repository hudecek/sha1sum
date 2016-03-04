#ifndef SHA1_H
#define SHA1_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;


class sha1
{
public:
    sha1(const char* filename);
    ~sha1();
    void generate();
    void update(unsigned long words[16]);
    void print();

private:
    unsigned long long size;
    unsigned long rol(unsigned long number, unsigned by);
    int blocks;
    ifstream file;
    string buffer;
    unsigned long h0, h1, h2, h3, h4;
};

#endif // SHA1_H
