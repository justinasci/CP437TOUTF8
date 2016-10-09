#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

#include <cstdio>
#include <cstdlib>

using namespace std;

string int2utf8 (int cp)
{
    if(cp == 0) return " ";

    if(cp <= 0x7F && cp > 0x00) // 1 byte
    {
        string out ("_");
        out[0] = (char) cp;
        return out;
    }
        else if((cp >= 0x80) && (cp <= 0x07FF)) // 2 bytes
    {
         string out ("__");
         char a = (cp & 0x003F) + 0x80;
         char b = ((cp &0x07C0) >> 6) + 0xC0 ;
         out[0] = b;
         out[1] = a;
         return out;
    }

    else if( (cp >= 0x0800) && (cp<= 0xFFFF) ) // 3 bytes
    {
        string out("___");
        char a = ((cp & 0xF000) >> 12) + 0x00E0;
        char b = ((cp & 0xFC0) >> 6) + 0x80;
        char c = ( cp & 0x003F) + 0x80;

        out[0]= a;
        out[1] = b;
        out[2]= c;
        return out;
    }else  if ( cp >= 0x10000 && cp <= 0x10FFFF) // 4 bytes
    {
        string out("____");

        char a = ((cp & 0x3F000) >>  18) + 0xF0;
        char b = ((cp & 0x3F000) >>  12) + 0x80;
        char c = ((cp & 0xFC0) >> 6) + 0x80;
        char d = ( cp & 0x003F) + 0x80;

        out[0] = a;
        out[1] = b;
        out[2] = c;
        out[3] = d;

        return out;
    }

    return "~";
}

void printUtf(int sk)
{
    cout << "U+"<< hex << sk;
}

void convertfile(const char* source,const char* dest, int table[])
{
    ifstream in  (source, ios::binary);
    ofstream out (dest, ios::binary);
    char c;
    while(in.get(c))
    {
        unsigned char o = c;
        int t = (int)o;

        if(t < 127)
            out << c;
        else
        {

            out << int2utf8(table[t-128]);
        }
    }

    in.close();
    out.close();
}


void filltable(int table[],const char* page)
{

    ifstream in (page);
    string t;

    for(int i = 0 ; i < 128;i++)
    {
        in >> t;
        in >> t;
        unsigned int x = strtoul(t.substr(2, 5).c_str(), NULL, 16);
        table[i] = x;
    }

    in.close();
}


int main()
{

    int table[128];
    filltable(table,"cp437.txt");
    convertfile("386intel.txt","output.txt",table);

    return 0;

}
