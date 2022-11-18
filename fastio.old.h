#include <cctype>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstring>
namespace fastio {
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
    reset
};
// clang-format off
struct setw { int data; };
struct setfill { int data; };
struct setprecision { char data; };
struct setbase { int data; };
// clang-format on
class rstream {
    static const int SIZ = 1 << 20;
    int base = 10;
    bool eof = 0, eofx = 0, pre = 0;
    char buf[SIZ], *p = buf, *q = buf, prech;
    FILE* file = stdin;
    bool isnum(char ch) {
        return isdigit(ch) && ch < 48 + base || isupper(ch) && ch < 55 + base
            || islower(ch) && ch < 87 + base;
    }
    bool iseof(char ch) { return !~ch; }
    int tonum(char ch) { return ch - (isdigit(ch) ? 48 : isupper(ch) ? 55 : 87); }
    char fget() {
        return p == q && (q = (p = buf) + fread(buf, 1, SIZ, file), p == q) ? EOF : *p++;
    }

  public:
    rstream() {}
    rstream(const char* dir) : file(fopen(dir, "r")) {}
    char get() {
        char res = pre ? (pre = 0), prech : prech = fget();
        return iseof(res) && (eof = 1), res;
    }
    operator bool() { return !eofx; }
    template <integer_t T> rstream& operator>>(T& x) {
        bool t = x = 0, f = !unsigned_integer_t<T>;
        if (eof) return (eofx = 1), *this;
        char ch;
        while (!iseof(ch = get()) && !isnum(ch)) t = ch == '-' && f;
        while (isnum(ch)) x = x * base + tonum(ch), ch = get();
        pre = 1, t && (x = -x);
        return *this;
    }
    rstream& operator>>(float_t auto& x) {
        bool t = x = 0;
        if (eof) return (eofx = 1), *this;
        long double k = 1;
        char ch;
        while (!iseof(ch = get()) && !isdigit(ch)) t = ch == '-';
        while (isdigit(ch)) x = x * 10.0 + ch - 48, ch = get();
        if (ch == '.')
            while (isdigit(ch = get())) x += (double)(ch - 48) / (k *= 10);
        pre = 1, t && (x = -x);
        return *this;
    }
    rstream& operator>>(char& ch) {
        if (eof) return (eofx = 1), *this;
        while (!iseof(ch = get()) && isspace(ch))
            ;
        return *this;
    }
    template <size_t N> rstream& operator>>(char (&s)[N]) {
        if (eof) return (eofx = 1), *this;
        int t = 0;
        char ch;
        while (!iseof(ch = get()) && isspace(ch))
            ;
        while (isgraph(ch)) t < N - 1 && (s[t++] = ch), ch = get();
        s[t] = '\0', pre = 1;
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
        if (s == skipws) operator>>(*new char), pre = 1;
        return *this;
    }
    rstream& operator>>(const setbase sp) {
        base = sp.data, (base < 2 || base > 36) && (base = 10);
        return *this;
    }
    rstream& ignore(char delim = '\n') {
        if (eof) return (eofx = 1), *this;
        char ch;
        while (!iseof(ch = get()) && ch != delim)
            ;
        return *this;
    }
    template <size_t N> rstream& getline(char (&s)[N], char delim = '\n') {
        if (eof) return (eofx = 1), *this;
        int t = 0, f = 0;
        char ch;
        while (!iseof(ch = get()) && ch != delim && !(f = delim == '\n' && ch == '\r'))
            t < N - 1 && (s[t++] = ch);
        s[t] = '\0', f && get();
        return *this;
    }
    rstream& read(auto x) { return operator>>(x); }
    rstream& read(auto x, auto... args) { return operator>>(x), read(args...); }
    template <typename T> const T read() {
        T x;
        return operator>>(x), x;
    }
};
class wstream {
    static const int SIZ = 1 << 20;
    int setw = 0, precision = 6, base = 10;
    bool boolalpha = 0, showpos = 0, showpoint = 0, showbase = 0, ccase = 0, unitbuf = 0;
    char buf[SIZ], *p = buf, setfill = ' ';
    long double eps = 1e6;
    FILE* file = stdout;
    long double pow10(int k) {
        long double res = 1, x = 10;
        for (; k; k >>= 1, x *= x)
            if (k & 1) res *= x;
        return res;
    }
    char tochr(int x) { return x + (x < 10 ? 48 : ccase ? 55 : 87); }
    void fill(int len) {
        setw = 0;
        if (len < 0) return;
        int use = p - buf;
        while (len + use >= SIZ) {
            memset(buf + use, setfill, SIZ - use);
            p = buf + SIZ, flush(), len -= SIZ - use, use = 0;
        }
        memset(buf + use, setfill, len), p = buf + len + use;
    }

  public:
    wstream() {}
    wstream(const char* dir) : file(fopen(dir, "w")) {}
    ~wstream() { flush(); }
    wstream& flush() { return fwrite(buf, p - buf, 1, file), p = buf, *this; }
    wstream& put(char ch) { return p - buf >= SIZ && (flush(), NULL), *p++ = ch, *this; }
    wstream& puts(const char* s, int len = -1) {
        if (len < 0) len = strlen(s);
        int use = p - buf, _len = len;
        while (len + use >= SIZ) {
            memcpy(buf + use, s + _len - len, SIZ - use);
            p = buf + SIZ, flush(), len -= SIZ - use, use = 0;
        }
        memcpy(buf + use, s + _len - len, len), p = buf + len + use;
        return *this;
    }
    wstream& operator<<(integer_t auto x) {
        static char cbuf[205];
        static char* const end = cbuf + 200;
        char* p = end;
        bool t = x < 0;
        if (!x) *p-- = '0';
        while (x) *p-- = tochr(t ? -(x % -base) : x % base), x /= base;
        if (showbase)
            if (base == 16) *p-- = ccase ? 'X' : 'x', *p-- = '0';
            else if (base == 8) *p-- = '0';
        if (t || showpos) *p-- = t ? '-' : '+';
        fill(setw - (end - p));
        puts(p + 1, end - p);
        return unitbuf ? flush() : *this;
    }
    // TODO: fix write of float_t
    template <float_t T> wstream& operator<<(T x) {
        static char ibuf[5005], dbuf[5005];
        int ilen = 0, dlen = 0, pos = 1, k = precision;
        bool t = x < 0, f = showpoint;
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
    wstream& operator<<(char ch) { return fill(setw - 1), put(ch), unitbuf ? flush() : *this; }
    wstream& operator<<(const char* s) {
        int len = strlen(s);
        return fill(setw - len), puts(s), unitbuf ? flush() : *this;
    }
    wstream& operator<<(bool f) {
        if (boolalpha) puts(f ? "true" : "false");
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
        if (s == fastio::flush) flush();
        if (s == fastio::boolalpha) boolalpha = 1;
        if (s == noboolalpha) boolalpha = 0;
        if (s == fastio::showpos) showpos = 1;
        if (s == noshowpos) showpos = 0;
        if (s == fastio::showpoint) showpoint = 1;
        if (s == noshowpoint) showpoint = 0;
        if (s == fastio::showbase) showbase = 1;
        if (s == noshowbase) showbase = 0;
        if (s == fastio::unitbuf) unitbuf = 1;
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
    wstream& operator<<(fastio::setw sp) { return setw = sp.data > 0 ? sp.data : 0, *this; }
    wstream& operator<<(const fastio::setfill sp) { return setfill = sp.data, *this; }
    wstream& operator<<(const fastio::setprecision sp) {
        eps = pow10(precision = sp.data > 0 ? sp.data : 0);
        return *this;
    }
    wstream& operator<<(const fastio::setbase sp) {
        base = sp.data, (base < 2 || base > 36) && (base = 10);
        return *this;
    }
    wstream& write(auto x) { return operator<<(x); }
    wstream& write(auto x, auto... args) { return operator<<(x), write(args...); }
};
rstream rs;
wstream ws;
};  // namespace fastio