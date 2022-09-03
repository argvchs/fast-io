#include "fastio.h"
#include <cstdio>
#include <ctime>
using namespace fastio;
rstream rfs("fastio.in");
wstream wfs("fastio.out");
int main() {
    clock_t start = clock();
    int n = rfs.read<int>();
    unsigned m = rfs.read<unsigned>();
    bool f;
    char s[105], c;
    double d;
    rfs.ignore().getline(s);
    rfs >> c >> f;
    (rfs >> d).ignore();
    wfs << n << ends << m << endl;
    wfs << d << ends << endl;
    wfs << s << endl;
    wfs << c << ends << boolalpha << f << endl;
    wfs << "const char*" << endl;
    wfs << reset << setw(20) << setfill('0') << showpos;
    wfs << showpoint << setprecision(6);
    // while (rfs >> n) wfs << n << ends;
    while (rfs >> d) wfs << d << ends;
    // while (rfs >> c) wfs << c << ',' << ends;
    // while (rfs >> s) wfs << '"' << s << '"' << endl;
    // while (rfs.getline(s)) wfs << '"' << s << '"' << endl;
    // while (rfs.getline(s, ';')) wfs << '"' << s << '"' << endl, rfs.ignore();
    ws << (double)(clock() - start) / CLOCKS_PER_SEC;
    return 0;
}