#include <cctype>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstring>
namespace fastio {
const size_t SIZ = 1 << 24;
template <typename T>
concept integer_t = std::integral<T> || std::same_as<T, __int128_t> || std::same_as<T, __uint128_t>;
template <typename T>
concept unsigned_integer_t = std::unsigned_integral<T> || std::same_as<T, __uint128_t>;
template <typename T>
concept string_t = std::same_as<T, char*> || std::same_as<T, const char*>;
template <typename T>
concept float_t = std::floating_point<T>;
enum symbol {
    endl,
    ends,
    flush,
    boolalpha,
    noboolalpha,
    showpos,
    noshowpos,
    showpoint,
    noshowpoint,
    unitbuf,
    nounitbuf,
    _setw,
    _setfill,
    _setprecision,
    reset
};
template <typename T> struct sympack {
    symbol s;
    T data;
    sympack(symbol _s, T _data) : s(_s), data(_data) {}
    bool operator==(symbol rhs) { return s == rhs; }
};
const sympack<int> setw(int x) { return sympack(_setw, x); }
const sympack<char> setfill(char c) { return sympack(_setfill, c); }
const sympack<int> setprecision(int x) { return sympack(_setprecision, x); }
class rstream {
    char buf[SIZ], *p1 = buf, *p2 = buf, prec;
    bool eof = 0, flag = 0, ispre = 0;
    FILE* file = stdin;
    bool iseof(char c) { return ~c ? 0 : flag = 1; }
    char _get() { return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, SIZ, file), p1 == p2) ? EOF : *p1++; }

public:
    rstream() {}
    rstream(const char* dir) : file(fopen(dir, "r")) {}
    rstream(char* dir) : file(fopen(dir, "r")) {}
    char get() { return ispre ? (ispre = 0), prec : prec = _get(); }
    operator bool() { return !eof; }
    template <integer_t T> rstream& operator>>(T& x) {
        bool t(x = 0);
        char ch;
        while (!iseof(ch = get()) && !isdigit(ch))
            if (ch == '-' && !unsigned_integer_t<T>) t = 1;
        while (isdigit(ch)) x = x * 10 + ch - 48, ch = get();
        ispre = 1, t && (x = -x), flag && (eof = 1);
        return *this;
    }
    template <float_t T> rstream& operator>>(T& x) {
        bool t(x = 0);
        size_t k = 1;
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
        size_t t(0);
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
    rstream& ignore(char delim = '\n') {
        char ch;
        while (!iseof(ch = get()) && ch != delim)
            ;
        return *this;
    }
    rstream& getline(char* s, char delim = '\n') {
        if (flag) return (eof = 1), *this;
        size_t t(0);
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
    bool boolalpha = 0, showpos = 0, showpoint = 0, unitbuf = 0;
    size_t setw = 0, precision = 3;
    long double eps = 1e3;
    FILE* file = stdout;
    void fill(int cnt) {
        setw = 0;
        while (cnt-- > 0) put(setfill);
    }

public:
    wstream() {}
    wstream(const char* dir) : file(fopen(dir, "w")) {}
    wstream(char* dir) : file(fopen(dir, "w")) {}
    ~wstream() { flush(); }
    wstream& flush() { return (fwrite(buf, p - buf, 1, file), p = buf), *this; }
    wstream& put(char ch) { return (p - buf >= SIZ && (flush(), NULL), *p++ = ch), *this; }
    template <integer_t T> wstream& operator<<(T x) {
        static char cbuf[45];
        size_t len(0);
        bool t(x < 0);
        if (!x) cbuf[len++] = '0';
        while (x) cbuf[len++] = (t ? -(x % -10) : x % 10) + 48, x /= 10;
        if (t || showpos) cbuf[len++] = t ? '-' : '+';
        fill(setw - len);
        while (len) put(cbuf[--len]);
        return unitbuf ? flush() : *this;
    }
    template <float_t T> wstream& operator<<(T x) {
        static char ibuf[5005], dbuf[5005];
        size_t ilen(0), dlen(0), pos(1), k = precision;
        bool t(x < 0), f(showpoint);
        if (t) x = -x;
        T p = floor(x), q = round((x - p) * eps);
        if (q >= eps) ++p, q = 0;
        if (!p) ibuf[ilen++] = '0';
        while (p) ibuf[ilen++] = fmod(p, 10) + 48, p = floor(p / 10.0);
        while (k--) {
            dbuf[dlen++] = fmod(q, 10) + 48, q = floor(q / 10.0);
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
    template <string_t T> wstream& operator<<(T s) {
        size_t len(strlen(s));
        fill(setw - len);
        for (int i(0); i < len; ++i) put(s[i]);
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(bool f) {
        if (boolalpha) operator<<(f ? "true" : "false");
        else fill(setw - 1), put(f ? '1' : '0');
        return unitbuf ? flush() : *this;
    }
    wstream& operator<<(symbol s) {
        if (s == symbol::endl) put('\n');
        if (s == symbol::ends) put(' ');
        if (s == symbol::flush) flush();
        if (s == symbol::boolalpha) boolalpha = 1;
        if (s == symbol::noboolalpha) boolalpha = 0;
        if (s == symbol::showpos) showpos = 1;
        if (s == symbol::noshowpos) showpos = 0;
        if (s == symbol::showpoint) showpoint = 1;
        if (s == symbol::noshowpoint) showpoint = 0;
        if (s == symbol::unitbuf) unitbuf = 1;
        if (s == symbol::nounitbuf) unitbuf = 0;
        if (s == symbol::reset) {
            boolalpha = showpos = unitbuf = 0;
            setfill = ' ';
            precision = 3, eps = 1e3;
        }
        return *this;
    }
    template <typename T> wstream& operator<<(sympack<T> sp) {
        if (sp == symbol::_setw) setw = sp.data;
        if (sp == symbol::_setfill) setfill = sp.data;
        if (sp == symbol::_setprecision) {
            precision = sp.data, eps = 1.0;
            while (sp.data--) eps *= 10.0;
        }
        return *this;
    }
    template <typename T> wstream& write(T x) { return operator<<(x); }
    template <typename T, typename... Args> wstream& write(T x, Args... args) { return operator<<(x), write(args...); }
};
rstream rs;
wstream ws;
};  // namespace fastio