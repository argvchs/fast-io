#include <cctype>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstring>
namespace fastio {
const int SIZ = 1 << 24;
template <typename T>
concept integer_t = std::integral<T> || std::same_as<T, __int128_t> || std::same_as<T, __uint128_t>;
template <typename T>
concept unsigned_integer_t = std::unsigned_integral<T> || std::same_as<T, __uint128_t>;
template <typename T>
concept float_t = std::floating_point<T>;
enum symbol {
    endl,
    ends,
    flush,
    skipws,
    boolalpha,
    noboolalpha,
    showpos,
    noshowpos,
    showpoint,
    noshowpoint,
    showbase,
    noshowbase,
    bin,
    oct,
    dec,
    hex,
    uppercase,
    lowercase,
    unitbuf,
    nounitbuf,
    _setw,
    _setfill,
    _setprecision,
    _setbase,
    reset
};
template <typename T> struct sympack {
    symbol type;
    T data;
    sympack(symbol s, T _data) : type(s), data(_data) {}
};
const sympack<int> setw(int x) { return sympack(_setw, x); }
const sympack<char> setfill(char c) { return sympack(_setfill, c); }
const sympack<int> setprecision(int x) { return sympack(_setprecision, x); }
const sympack<int> setbase(int x) { return sympack(_setbase, x); }
class rstream {
    char buf[SIZ], *p1 = buf, *p2 = buf, prec;
    bool eof = 0, flag = 0, ispre = 0;
    int base = 10;
    FILE* file = stdin;
    bool iseof(char c) { return ~c ? 0 : flag = 1; }
    char _get() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, SIZ, file), p1 == p2) ? EOF : *p1++; }
    bool isnum(char c) {
        return isdigit(c) && c < 48 + base || isupper(c) && c < 55 + base || islower(c) && c < 87 + base;
    }
    int tonum(char c) { return c - (isdigit(c) ? 48 : isupper(c) ? 55 : 87); }

public:
    rstream() {}
    rstream(const char* dir) : file(fopen(dir, "r")) {}
    char get() { return ispre ? (ispre = 0), prec : prec = _get(); }
    operator bool() { return !eof; }
    template <integer_t T> rstream& operator>>(T& x) {
        bool t(x = 0);
        char ch;
        while (!iseof(ch = get()) && !isnum(ch))
            if (ch == '-' && !unsigned_integer_t<T>) t = 1;
        while (isnum(ch)) x = x * base + tonum(ch), ch = get();
        ispre = 1, t && (x = -x), flag && (eof = 1);
        return *this;
    }
    template <float_t T> rstream& operator>>(T& x) {
        bool t(x = 0);
        long double k = 1;
        char ch;
        while (!iseof(ch = get()) && !isdigit(ch)) t = ch == '-';
        while (isdigit(ch)) x = x * 10.0 + ch - 48, ch = get();
        if (ch == '.')
            while (isdigit(ch = get())) x += (double)(ch - 48) / (k *= 10);
        ispre = 1, t && (x = -x), flag && (eof = 1);
        return *this;
    }
    rstream& operator>>(char& c) {
        while (!iseof(c = get()) && isspace(c))
            ;
        return flag && (eof = 1), *this;
    }
    rstream& operator>>(char* s) {
        int t(0);
        char ch;
        while (!iseof(ch = get()) && isspace(ch))
            ;
        while (isgraph(ch)) s[t++] = ch, ch = get();
        s[t] = '\0', ispre = 1, flag && (eof = 1);
        return *this;
    }
    rstream& operator>>(bool& f) {
        long long x;
        return operator>>(x), f = x, *this;
    }
    rstream& operator>>(const symbol s) {
        if (s == bin) base = 2;
        if (s == oct) base = 8;
        if (s == dec || s == reset) base = 10;
        if (s == hex) base = 16;
        if (s == skipws) operator>>(*new char), ispre = 1;
        return *this;
    }
    template <class T> rstream& operator>>(const sympack<T> sp) {
        if (sp.type == _setbase) {
            base = sp.data;
            if (base < 2 || base > 36) base = 10;
        }
        return *this;
    }
    rstream& ignore(char delim = '\n') {
        char ch;
        while (!iseof(ch = get()) && ch != delim)
            ;
        return *this;
    }
    rstream& getline(char* s, char delim = '\n') {
        if (flag) return (eof = 1), *this;
        int t(0);
        char ch;
        while (!iseof(ch = get()) && ch != delim) s[t++] = ch;
        s[t] = '\0';
        if (delim == '\n' && s[t - 1] == '\r') (s[t - 1] = '\0');
        return *this;
    }
    template <typename T> rstream& read(T x) { return operator>>(x); }
    template <typename T, typename... Args> rstream& read(T x, Args... args) { return operator>>(x), read(args...); }
    template <typename T> const T read() {
        T x;
        return operator>>(x), x;
    }
};
class wstream {
    char buf[SIZ], *p = buf, setfill = ' ';
    bool boolalpha = 0, showpos = 0, showpoint = 0, showbase = 0, ccase = 0, unitbuf = 0;
    int setw = 0, precision = 6, base = 10;
    long double eps = 1e6;
    FILE* file = stdout;
    void fill(int cnt) {
        setw = 0;
        while (cnt-- > 0) put(setfill);
    }
    long double pow10(int k) {
        long double res = 1, x = 10;
        for (; k; k >>= 1, x *= x)
            if (k & 1) res *= x;
        return res;
    }
    char tochr(int x) { return x + (x < 10 ? 48 : ccase ? 55 : 87); }

public:
    wstream() {}
    wstream(const char* dir) : file(fopen(dir, "w")) {}
    ~wstream() { flush(); }
    wstream& flush() { return (fwrite(buf, p - buf, 1, file), p = buf), *this; }
    wstream& put(char ch) { return (p - buf >= SIZ && (flush(), NULL), *p++ = ch), *this; }
    template <integer_t T> wstream& operator<<(T x) {
        static char cbuf[45];
        int len(0);
        bool t(x < 0);
        if (!x) cbuf[len++] = '0';
        while (x) cbuf[len++] = tochr(t ? -(x % -base) : x % base), x /= base;
        if (showbase)
            if (base == 16) cbuf[len++] = ccase ? 'X' : 'x', cbuf[len++] = '0';
            else if (base == 8) cbuf[len++] = '0';
        if (t || showpos) cbuf[len++] = t ? '-' : '+';
        fill(setw - len);
        while (len) put(cbuf[--len]);
        return unitbuf ? flush() : *this;
    }
    template <float_t T> wstream& operator<<(T x) {
        static char ibuf[5005], dbuf[5005];
        int ilen(0), dlen(0), pos(1), k(precision);
        bool t(x < 0), f(showpoint);
        if (t) x = -x;
        T p = std::floor(x), q = std::round((x - p) * eps);
        if (q >= eps) ++p, q = 0;
        if (!p) ibuf[ilen++] = '0';
        while (p) ibuf[ilen++] = (int)std::fmod(p, 10.0) + 48, p = std::floor(p / 10.0);
        while (k--) {
            dbuf[dlen++] = (int)std::fmod(q, 10.0) + 48, q = std::floor(q / 10.0);
            if (dbuf[dlen - 1] != '0' && !f) f = 1, pos = dlen;
        }
        if (!f) dlen = 0;
        if (precision != k && f) dbuf[dlen++] = '.';
        if (t || showpos) ibuf[ilen++] = t ? '-' : '+';
        fill(setw - ilen - dlen);
        while (ilen) put(ibuf[--ilen]);
        while (dlen >= pos) put(dbuf[--dlen]);
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(char c) {
        fill(setw - 1), put(c);
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(const char* s) {
        int len(strlen(s));
        fill(setw - len);
        for (int i(0); i < len; ++i) put(s[i]);
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(bool f) {
        if (boolalpha) operator<<(f ? "true" : "false");
        else fill(setw - 1), put(f ? '1' : '0');
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(const void* p) {
        int b = base, t = showbase;
        base = 16, showbase = 1;
        operator<<((size_t)p);
        base = b, showbase = t;
        return *this;
    }
    wstream& operator<<(const symbol s) {
        if (s == endl) put('\n');
        if (s == ends) put(' ');
        if (s == symbol::flush) flush();
        if (s == symbol::boolalpha) boolalpha = 1;
        if (s == noboolalpha) boolalpha = 0;
        if (s == symbol::showpos) showpos = 1;
        if (s == noshowpos) showpos = 0;
        if (s == symbol::showpoint) showpoint = 1;
        if (s == noshowpoint) showpoint = 0;
        if (s == symbol::showbase) showbase = 1;
        if (s == noshowbase) showbase = 0;
        if (s == symbol::unitbuf) unitbuf = 1;
        if (s == nounitbuf) unitbuf = 0;
        if (s == lowercase) ccase = 0;
        if (s == uppercase) ccase = 1;
        if (s == bin) base = 2;
        if (s == oct) base = 8;
        if (s == dec) base = 10;
        if (s == hex) base = 16;
        if (s == reset) {
            boolalpha = showpos = showpoint = showbase = ccase = unitbuf = 0;
            setfill = ' ', precision = 6, eps = 1e6, base = 10;
        }
        return *this;
    }
    template <typename T> wstream& operator<<(const sympack<T> sp) {
        if (sp.type == _setw) setw = sp.data > 0 ? sp.data : 0;
        if (sp.type == _setfill) setfill = sp.data;
        if (sp.type == _setprecision) eps = pow10(precision = sp.data > 0 ? sp.data : 0);
        if (sp.type == _setbase) {
            base = sp.data;
            if (base < 2 || base > 36) base = 10;
        }
        return *this;
    }
    template <typename T> wstream& write(T x) { return operator<<(x); }
    template <typename T, typename... Args> wstream& write(T x, Args... args) { return operator<<(x), write(args...); }
};
rstream rs;
wstream ws;
};  // namespace fastio