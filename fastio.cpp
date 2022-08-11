#include <cstdio>
#include <ctime>
#include "fastio.h"
using namespace fastio;
rstream rfs("fastio.in");
wstream wfs("fastio.out");
int main() {
    int n = rfs.read<int>();
    unsigned m = rfs.read<unsigned>();
    bool f;
    char s[105], c;
    rfs.ignore().getline(s);
    rfs >> c >> f;
    // double d;
    // wfs << d;
    wfs << n << ends << showpos << m << endl;
    wfs << s << endl;
    wfs << c << ends << boolalpha << f << endl;
    wfs << "const char*" << endl;
    wfs << reset << setw(12) << setfill('0');
    rfs.ignore();
    while (rfs >> n) wfs << n << ends;
    // while (rfs >> c) wfs << c << ',' << ends;
    // while (rfs >> s) wfs << '"' << s << '"' << endl;
    // while (rfs.getline(s, 100)) wfs << '"' << s << '"' << endl;
    return 0;
}