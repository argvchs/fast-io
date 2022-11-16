#include <cctype>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstring>
#define SIZ 0xfffff
namespace fastio {
namespace syms {
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
    struct setfill { char data; };
    struct setprecision { int data; };
    struct setbase { int data; };
    // clang-format on
}  // namespace syms
namespace interface {
    using namespace syms;
    template <typename T>
    concept integer_t
        = std::integral<T> || std::same_as<T, __int128_t> || std::same_as<T, __uint128_t>;
    template <typename T>
    concept signed_integer_t = std::signed_integral<T> || std::same_as<T, __int128_t>;
    template <typename T>
    concept float_t = std::floating_point<T>;
    class rstream {
        int base = 10;
        bool pre;
        char prech;
        bool isnum(char ch) {
            return isdigit(ch) && ch < '0' + base || isupper(ch) && ch < 'A' - 10 + base
                || islower(ch) && ch < 'a' - 10 + base;
        }
        bool iseof(char ch) { return !~ch; }
        int tonum(char ch) { return ch - (isdigit(ch) ? '0' : isupper(ch) ? 'A' - 10 : 'a' - 10); }

    protected:
        virtual char vget() = 0;
        virtual void vseek() = 0;

    public:
        bool eof = 0, fail = 0;
        rstream() = default;
        char get() {
            char res = pre ? (pre = 0), prech : prech = vget();
            return iseof(res) && (eof = 1), res;
        }
        operator bool() { return !fail; }
        template <integer_t T> rstream& operator>>(T& x) {
            bool t = x = 0, f = signed_integer_t<T>;
            char ch;
            while (!iseof(ch = get()) && !isnum(ch)) t = ch == '-' && f;
            if (eof) return fail = 1, *this;
            while (isnum(ch)) x = x * base + tonum(ch), ch = get();
            pre = 1, t && (x = -x);
            return *this;
        }
        rstream& operator>>(float_t auto& x) {
            bool t = x = 0;
            long double k = 1;
            char ch;
            while (!iseof(ch = get()) && !isdigit(ch)) t = ch == '-';
            if (eof) return fail = 1, *this;
            while (isdigit(ch)) x = x * 10.0 + ch - '0', ch = get();
            if (ch == '.')
                while (isdigit(ch = get())) x += (double)(ch - '0') / (k *= 10);
            pre = 1, t && (x = -x);
            return *this;
        }
        rstream& operator>>(char& ch) {
            while (!iseof(ch = get()) && isspace(ch))
                ;
            if (eof) fail = 1, ch = 0;
            return *this;
        }
        template <int N> rstream& operator>>(char (&s)[N]) {
            int t = 0;
            char ch;
            while (!iseof(ch = get()) && isspace(ch))
                ;
            if (eof) return fail = 1, *this;
            while (isgraph(ch)) t < N - 1 && (s[t++] = ch), ch = get();
            s[t] = '\0', pre = 1;
            return *this;
        }
        rstream& operator>>(bool& f) {
            long long x;
            return *this >> x, f = x, *this;
        }
        rstream& operator>>(const symbol s) {
            if (s == bin) base = 2;
            else if (s == oct) base = 8;
            else if (s == dec || s == reset) base = 10;
            else if (s == hex) base = 16;
            else if (s == skipws) *this >> *new char, pre = 1;
            return *this;
        }
        rstream& operator>>(const setbase sp) {
            base = sp.data, (base < 2 || base > 36) && (base = 10);
            return *this;
        }
        rstream& ignore(int N = INT_MAX, char delim = '\n') {
            char ch;
            while ((N == INT_MAX || N--) && !iseof(ch = get()) && ch != delim)
                ;
            if (eof) return fail = 1, *this;
            return *this;
        }
        rstream& getline(char* s, int N, char delim = '\n') {
            int t = 0, f = 0;
            char ch;
            while (N-- && !iseof(ch = get()) && ch != delim) s[t++] = ch;
            if (!t) return fail = 1, *this;
            if (delim == '\n' && s[t - 1] == '\r') --t;
            s[t] = '\0';
            return *this;
        }
        rstream& seek() { return vseek(), *this; }
        rstream& read(auto... args) { return (*this >> ... >> args); }
        template <typename T> const T read() {
            T x;
            return *this >> x, x;
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
        char tochr(int x) { return x + (x < 10 ? '0' : kase ? 'A' - 10 : 'a' - 10); }
        void fill(int N) { setw = 0, vfill(setfill, N); }

    protected:
        virtual void vflush() = 0;
        virtual void vfill(char, int) = 0;
        virtual void vput(char) = 0;
        virtual void vputs(const char*, int = -1) = 0;

    public:
        wstream() = default;
        wstream& flush() { return vflush(), *this; }
        wstream& put(char ch) { return vput(ch), *this; }
        wstream& operator<<(integer_t auto x) {
            static char buf[205], *end = buf + 200;
            char* p = end;
            bool t = x < 0;
            if (!x) *p-- = '0';
            while (x) *p-- = tochr(t ? -(x % -base) : x % base), x /= base;
            if (showbase && base == 16) *p-- = kase ? 'X' : 'x', *p-- = '0';
            else if (showbase && base == 8) *p-- = '0';
            if (t || showpos) *p-- = t ? '-' : '+';
            fill(setw - (end - p));
            vputs(p + 1, end - p);
            return unitbuf ? flush() : *this;
        }
        template <float_t T> wstream& operator<<(T x) {
            static char buf[1005], BUF[1005], *end = buf + 1000, *END = BUF + 1000;
            char *p = end, *q = END, *d = END;
            bool t = x < 0;
            if (t) x = -x;
            T a = std::floor(x), b = std::round((x - a) * EPS);
            if (b >= EPS) ++a, b = 0;
            if (a < eps) *p-- = '0';
            while (a >= eps) *p-- = (int)std::fmod(a, 10.0) + '0', a = std::floor(a / 10.0);
            while (b >= eps) *q-- = (int)std::fmod(b, 10.0) + '0', b = std::floor(b / 10.0);
            if (q != END || showpoint) *q-- = '.';
            while (*d == '0' && d != q && !showpoint) --d;
            if (t || showpos) *p-- = t ? '-' : '+';
            fill(setw - (end - p) - (d - q));
            vputs(p + 1, end - p), vputs(q + 1, d - q);
            return unitbuf ? flush() : *this;
        }
        wstream& operator<<(char ch) { return fill(setw - 1), put(ch), unitbuf ? flush() : *this; }
        wstream& operator<<(const char* s) {
            int N = strlen(s);
            return fill(setw - N), vputs(s), unitbuf ? flush() : *this;
        }
        wstream& operator<<(bool f) {
            if (boolalpha) vputs(f ? "true" : "false");
            else fill(setw - 1), put(f ? '1' : '0');
            return unitbuf ? flush() : *this;
        }
        wstream& operator<<(const void* p) {
            int b = base, t = showbase;
            base = 16, showbase = 1;
            *this << (size_t)p;
            base = b, showbase = t;
            return *this;
        }
        wstream& operator<<(const symbol s) {
            if (s == endl) put('\n');
            else if (s == ends) put(' ');
            else if (s == syms::flush) flush();
            else if (s == syms::boolalpha) boolalpha = 1;
            else if (s == noboolalpha) boolalpha = 0;
            else if (s == syms::showpos) showpos = 1;
            else if (s == noshowpos) showpos = 0;
            else if (s == syms::showpoint) showpoint = 1;
            else if (s == noshowpoint) showpoint = 0;
            else if (s == syms::showbase) showbase = 1;
            else if (s == noshowbase) showbase = 0;
            else if (s == syms::unitbuf) unitbuf = 1;
            else if (s == nounitbuf) unitbuf = 0;
            else if (s == lowercase) kase = 0;
            else if (s == uppercase) kase = 1;
            else if (s == bin) base = 2;
            else if (s == oct) base = 8;
            else if (s == dec) base = 10;
            else if (s == hex) base = 16;
            else if (s == reset) {
                boolalpha = showpos = showpoint = showbase = kase = unitbuf = 0;
                setfill = ' ', precision = 6, eps = 1e-6, EPS = 1e6, base = 10;
            }
            return *this;
        }
        wstream& operator<<(syms::setw sp) { return setw = sp.data > 0 ? sp.data : 0, *this; }
        wstream& operator<<(const syms::setfill sp) { return setfill = sp.data, *this; }
        wstream& operator<<(const syms::setprecision sp) {
            precision = sp.data > 0 ? sp.data : 0;
            return update(), *this;
        }
        wstream& operator<<(const syms::setbase sp) {
            base = sp.data, (base < 2 || base > 36) && (base = 10);
            return *this;
        }
        wstream& write(auto... args) { return (*this << ... << args); }
    };
}  // namespace interface
class rstream : public interface::rstream {
    char buf[SIZ], *p = buf, *q = buf;
    char vget() {
        if (p != q) return *p++;
        if (feof(file) || ferror(file)) return clearerr(file), EOF;
        return p == q && (q = (p = buf) + fread(buf, 1, SIZ, file), p == q) ? EOF : *p++;
    }
    void vseek() { fseek(file, 0, 0), p = q = buf; }

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
    char buf[SIZ], *p = buf;
    void vflush() { fwrite(buf, p - buf, 1, file), p = buf; }
    void vfill(char ch, int N) {
        if (N < 0) return;
        int use = p - buf;
        while (N + use >= SIZ) {
            memset(buf + use, ch, SIZ - use);
            p = buf + SIZ, vflush(), N -= SIZ - use, use = 0;
        }
        memset(buf + use, ch, N), p = buf + N + use;
    }
    void vput(char ch) { p - buf >= SIZ && (vflush(), 0), *p++ = ch; }
    void vputs(const char* s, int N = -1) {
        if (N < 0) N = strlen(s);
        int use = p - buf, _len = N;
        while (N + use >= SIZ) {
            memcpy(buf + use, s + _len - N, SIZ - use);
            p = buf + SIZ, vflush(), N -= SIZ - use, use = 0;
        }
        memcpy(buf + use, s + _len - N, N), p = buf + N + use;
    }

protected:
    FILE* file = stdout;

public:
    ~wstream() { vflush(), fclose(file); }
};
class wfstream : public wstream {
public:
    wfstream(FILE* f) { file = f; }
    wfstream(const char* dir) { file = fopen(dir, "w"); }
};
class sstream : public interface::rstream, public interface::wstream {
    char *l = nullptr, *r = nullptr, *p = nullptr, *q = nullptr, *t;
    void reserve() {
        int N = r - l;
        if (!N) N = 2;
        t = new char[(N << 1) + 1]{};
        memcpy(t, l, N);
        p = p - l + t, q = q - l + t;
        delete[] l, r = (l = t) + (N << 1);
    }
    void clear() { delete[] l, r = (p = q = l = new char[2]{}) + 1; }
    void uneof() { eof = fail = 0; }
    char vget() { return q == p ? EOF : *q++; }
    void vseek() { q = l; }
    void vflush() {}
    void vfill(char ch, int N) {
        if (N < 0) return;
        while (r - p < N) reserve();
        memset(p, ch, N), p += N, uneof();
    }
    void vput(char ch) {
        if (p == r) reserve();
        *p++ = ch, uneof();
    }
    void vputs(const char* s, int N = -1) {
        if (N < 0) N = strlen(s);
        while (r - p < N) reserve();
        memcpy(p, s, N), p += N, uneof();
    }

public:
    sstream() { clear(); }
    sstream(const char* s) { str(s); }
    ~sstream() { delete[] l; }
    const char* str() { return l; }
    void str(const char* s) {
        int N = strlen(s);
        delete[] l, r = p = (q = l = new char[N]) + N;
        memcpy(l, s, N), uneof();
    }
};
rstream rs;
wstream ws;
};  // namespace fastio
#undef SIZ