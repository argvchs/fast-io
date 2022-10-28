#include <cctype>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstring>
static const int SIZ = 1 << 20;
namespace fastio {
namespace symlib {
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
}  // namespace symlib
using namespace symlib;
namespace interface {
    template <typename T>
    concept integer_t = std::integral<T> || std::same_as<T, __int128_t> || std::same_as<T, __uint128_t>;
    template <typename T>
    concept signed_integer_t = std::signed_integral<T> || std::same_as<T, __int128_t>;
    template <typename T>
    concept float_t = std::floating_point<T>;
    class rstream {
        int base = 10;
        bool isnum(char ch) {
            return isdigit(ch) && ch < 48 + base || isupper(ch) && ch < 55 + base || islower(ch) && ch < 87 + base;
        }
        bool iseof(char ch) { return !~ch; }
        int tonum(char ch) { return ch - (isdigit(ch) ? 48 : isupper(ch) ? 55 : 87); }

    protected:
        bool eof = 0, eofx = 0, pre;
        char prech;
        virtual char get(int) = 0;

    public:
        rstream() = default;
        char get() {
            char res = pre ? (pre = 0), prech : prech = get(NULL);
            return iseof(res) && (eof = 1), res;
        }
        operator bool() { return !eofx; }
        template <integer_t T> rstream& operator>>(T& x) {
            bool t = x = 0, f = signed_integer_t<T>;
            char ch;
            while (!iseof(ch = get()) && !isnum(ch)) t = ch == '-' && f;
            if (eof) return eofx = 1, *this;
            while (isnum(ch)) x = x * base + tonum(ch), ch = get();
            pre = 1, t && (x = -x);
            return *this;
        }
        rstream& operator>>(float_t auto& x) {
            bool t = x = 0;
            long double k = 1;
            char ch;
            while (!iseof(ch = get()) && !isdigit(ch)) t = ch == '-';
            if (eof) return eofx = 1, *this;
            while (isdigit(ch)) x = x * 10.0 + ch - 48, ch = get();
            if (ch == '.')
                while (isdigit(ch = get())) x += (double)(ch - 48) / (k *= 10);
            pre = 1, t && (x = -x);
            return *this;
        }
        rstream& operator>>(char& ch) {
            while (!iseof(ch = get()) && isspace(ch))
                ;
            if (eof) eofx = 1, ch = 0;
            return *this;
        }
        template <size_t N> rstream& operator>>(char (&s)[N]) {
            int t = 0;
            char ch;
            while (!iseof(ch = get()) && isspace(ch))
                ;
            if (eof) return eofx = 1, *this;
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
            char ch;
            while (!iseof(ch = get()) && ch != delim)
                ;
            if (eof) return eofx = 1, *this;
            return *this;
        }
        rstream& getline(char* s, size_t N, char delim = '\n') {
            int t = 0, f = 0;
            char ch;
            while (!iseof(ch = get()) && ch != delim) t < N - 1 && (s[t++] = ch);
            if (!t) return eofx = 1, *this;
            if (delim == '\n' && s[t - 1] == '\r') --t;
            s[t] = '\0';
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
        int setw = 0, precision = 6, base = 10;
        bool boolalpha = 0, showpos = 0, showpoint = 0, showbase = 0, kase = 0, unitbuf = 0;
        char setfill = ' ';
        long double eps = 1e-6, EPS = 1e6;
        void update() {
            long double x = 0.1, y = 10.0;
            int k = precision;
            for (eps = EPS = 1.0; k; k >>= 1, x *= x, y *= y)
                if (k & 1) eps *= x, EPS *= y;
        }
        char tochr(int x) { return x + (x < 10 ? 48 : kase ? 55 : 87); }
        void fill(int len) { setw = 0, fill(setfill, len); }

    protected:
        virtual void flush(int) = 0;
        virtual void fill(char, int) = 0;
        virtual void put(char, int) = 0;
        virtual void puts(const char*, int = -1) = 0;

    public:
        wstream() = default;
        wstream& flush() { return flush(NULL), *this; }
        wstream& put(char ch) { return put(ch, NULL), *this; }
        wstream& operator<<(integer_t auto x) {
            static char buf[205], *end = buf + 200;
            char* p = end;
            bool t = x < 0;
            if (!x) *p-- = 48;
            while (x) *p-- = tochr(t ? -(x % -base) : x % base), x /= base;
            if (showbase)
                if (base == 16) *p-- = kase ? 'X' : 'x', *p-- = 48;
                else if (base == 8) *p-- = 48;
            if (t || showpos) *p-- = t ? '-' : '+';
            fill(setw - (end - p));
            puts(p + 1, end - p);
            return unitbuf ? flush() : *this;
        }
        template <float_t T> wstream& operator<<(T x) {
            static char buf[1005], BUF[1005], *end = buf + 1000, *END = BUF + 1000;
            char *p = end, *q = END, *d = END;
            bool t = x < 0;
            if (t) x = -x;
            T a = std::floor(x), b = std::round((x - a) * EPS);
            if (b >= EPS) ++a, b = 0;
            if (a < eps) *p-- = 48;
            while (a >= eps) *p-- = (int)std::fmod(a, 10.0) + 48, a = std::floor(a / 10.0);
            while (b >= eps) *q-- = (int)std::fmod(b, 10.0) + 48, b = std::floor(b / 10.0);
            if (q != END || showpoint) *q-- = '.';
            while (*d == 48 && d != q && !showpoint) --d;
            if (t || showpos) *p-- = t ? '-' : '+';
            fill(setw - (end - p) - (d - q));
            puts(p + 1, end - p), puts(q + 1, d - q);
            return unitbuf ? flush() : *this;
        }
        wstream& operator<<(char ch) { return fill(setw - 1), put(ch), unitbuf ? flush() : *this; }
        wstream& operator<<(const char* s) {
            int len = strlen(s);
            return fill(setw - len), puts(s), unitbuf ? flush() : *this;
        }
        wstream& operator<<(bool f) {
            if (boolalpha) puts(f ? "true" : "false");
            else fill(setw - 1), put(f ? '1' : 48);
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
            if (s == symlib::flush) flush();
            if (s == symlib::boolalpha) boolalpha = 1;
            if (s == noboolalpha) boolalpha = 0;
            if (s == symlib::showpos) showpos = 1;
            if (s == noshowpos) showpos = 0;
            if (s == symlib::showpoint) showpoint = 1;
            if (s == noshowpoint) showpoint = 0;
            if (s == symlib::showbase) showbase = 1;
            if (s == noshowbase) showbase = 0;
            if (s == symlib::unitbuf) unitbuf = 1;
            if (s == nounitbuf) unitbuf = 0;
            if (s == lowercase) kase = 0;
            if (s == uppercase) kase = 1;
            if (s == bin) base = 2;
            if (s == oct) base = 8;
            if (s == dec) base = 10;
            if (s == hex) base = 16;
            if (s == reset) {
                boolalpha = showpos = showpoint = showbase = kase = unitbuf = 0;
                setfill = ' ', precision = 6, eps = 1e-6, EPS = 1e6, base = 10;
            }
            return *this;
        }
        wstream& operator<<(symlib::setw sp) { return setw = sp.data > 0 ? sp.data : 0, *this; }
        wstream& operator<<(const symlib::setfill sp) { return setfill = sp.data, *this; }
        wstream& operator<<(const symlib::setprecision sp) {
            precision = sp.data > 0 ? sp.data : 0;
            return update(), *this;
        }
        wstream& operator<<(const symlib::setbase sp) {
            base = sp.data, (base < 2 || base > 36) && (base = 10);
            return *this;
        }
        wstream& write(auto x) { return operator<<(x); }
        wstream& write(auto x, auto... args) { return operator<<(x), write(args...); }
    };
}  // namespace interface
class rstream : public interface::rstream {
    static const int SIZ = 1 << 20;
    char buf[SIZ], *p = buf, *q = buf;
    char get(int) { return p == q && (q = (p = buf) + fread(buf, 1, SIZ, file), p == q) ? EOF : *p++; }

protected:
    FILE* file = stdin;

public:
    ~rstream() { fclose(file); }
};
class rfstream : public rstream {
public:
    rfstream(FILE* f) { file = f; }
    rfstream(const char* dir) { file = fopen(dir, "r"); }
};
class wstream : public interface::wstream {
    static const int SIZ = 1 << 20;
    char buf[SIZ], *p = buf;
    void flush(int) { fwrite(buf, p - buf, 1, file), p = buf; }
    void fill(char ch, int len) {
        if (len < 0) return;
        int use = p - buf;
        while (len + use >= SIZ) {
            memset(buf + use, ch, SIZ - use);
            p = buf + SIZ, flush(NULL), len -= SIZ - use, use = 0;
        }
        memset(buf + use, ch, len), p = buf + len + use;
    }
    void put(char ch, int) { p - buf >= SIZ && (flush(NULL), NULL), *p++ = ch; }
    void puts(const char* s, int len = -1) {
        if (len < 0) len = strlen(s);
        int use = p - buf, _len = len;
        while (len + use >= SIZ) {
            memcpy(buf + use, s + _len - len, SIZ - use);
            p = buf + SIZ, flush(NULL), len -= SIZ - use, use = 0;
        }
        memcpy(buf + use, s + _len - len, len), p = buf + len + use;
    }

protected:
    FILE* file = stdout;

public:
    ~wstream() { flush(NULL), fclose(file); }
};
class wfstream : public wstream {
public:
    wfstream(FILE* f) { file = f; }
    wfstream(const char* dir) { file = fopen(dir, "w"); }
};
class sstream : public interface::rstream, public interface::wstream {
    char *l = nullptr, *r = nullptr, *p = nullptr, *q = nullptr;
    void reserve() {
        size_t len = r - l;
        if (!len) len = 2;
        char* t = new char[(len << 1) + 1]{};
        std::copy(l, l + len, t);
        p = p - l + t, q = q - l + t;
        delete[] l, r = (l = t) + (len << 1);
    }
    void clear() { delete[] l, r = (p = q = l = new char[2]{}) + 1; }
    void uneof() { eof = eofx = 0; }
    char get(int) { return q == p ? EOF : *q++; }
    void flush(int) {}
    void fill(char ch, int len) {
        if (len < 0) return;
        while (r - p < len) reserve();
        memset(p, ch, len), p += len, uneof();
    }
    void put(char ch, int) {
        if (p == r) reserve();
        *p++ = ch, uneof();
    }
    void puts(const char* s, int len = -1) {
        if (len < 0) len = strlen(s);
        while (r - p < len) reserve();
        memcpy(p, s, len), p += len, uneof();
    }

public:
    sstream() { clear(); }
    sstream(const char* s) { str(s); }
    ~sstream() { delete[] l; }
    const char* str() { return l; }
    void str(const char* s) {
        size_t len = strlen(s);
        delete[] l, r = p = (q = l = new char[len]) + len;
        memcpy(l, s, len), uneof();
    }
    sstream& seek() { return q = l, *this; }
};
rstream rs;
wstream ws;
};  // namespace fastio