#include "fastio.h"
#include <cstdio>
#include <ctime>
using namespace fastio;
rstream rfs("fastio.in");
wstream wfs("fastio.out");
int main() {
    clock_t start = clock();
    rfs >> hex;
    int n = rfs.read<int>();
    rfs >> dec;
    unsigned m = rfs.read<unsigned>();
    bool f;
    char s[105], c;
    double d;
    rfs.ignore().getline(s);
    rfs >> c >> f;
    (rfs >> d).ignore();
    wfs.write(n, hex, uppercase, '(', n, ')', dec, endl);
    wfs << m << endl;
    wfs << s << endl;
    wfs << c << ends << boolalpha << f << endl;
    wfs << d << endl;
    wfs << "const char*" << endl;
    wfs << reset << setw(20) << setfill('0') << showpos;
    // wfs << showpoint << setprecision(6);
    while (rfs >> n) wfs << n << ends;
    // while (rfs >> d) wfs << d << ends;
    // while (rfs >> c) wfs << c << ',' << ends;
    // while (rfs >> s) wfs << s << ')' << endl;
    // while (rfs.getline(s)) wfs << '(' << s << ')' << endl;
    // while (rfs.getline(s, ';')) wfs << '(' << s << ')' << endl, rfs.ignore();
    double k = (double)(clock() - start) / CLOCKS_PER_SEC;
    ws << k;
    return 0;
}