# Fast-IO

一个快读/快写库，支持基本类型与 `__int128_t` 读写，指针地址写入，和 `cin/cout` 用法类似

虽然相对于原版函数式 `fread` 快读会有点慢，但还是比 `getchar/putchar` 要快的

编译不过？Fast-IO 用了 C++20 Concepts，用以下编译参数即可
最新版 MinGW 编译器在[这里](https://github.com/niXman/mingw-builds-binaries/releases)下载

```bash
g++ <file>.cpp -o <file> -std=c++20 -O3 -Ofast
```

## 目录解释

```text
fastio.cpp         # 测试文件
fastio.h           # Fast-IO 库源文件
fastio.in          # 输入测试文件
fastio.spd.in      # 速度测试文件
README.md          # README
```

## 使用

-   `using namespace fastio;`

    使用 Fast-IO

-   `using namespace fastio::syms;`

    使用输入输出符号，如 `endl`

-   `rs >> x;`

    读取 x

-   `rs >> x >> y >> z;`

    读取 x, y, z

-   `c = rs.get();`

    读取一个字符到 c

-   `rs.read(x, y, z);`

    读取 x, y, z

-   `x = rs.read<int>();`

    读取一个 `int` 类型的数据到 x

-   `x = rs.read<T>();`

    读取一个 `T` 类型的数据到 x

-   `x = rs.read<char*>(n);` `x = rs.read<const char*>(n);`

    读取一个最长为 n 的字符串到 s，**s 只能为字符指针，不能为数组**

-   `rs >> s;`

    读取一个字符串 s，**s 只能为字符数组，不能为指针**

-   `rs.getline(s, n);`

    读取一行到 s，最多读取 n 个字符

-   `rs.getline(s, n, c);`

    读取字符到 s，最多读取 n 个字符，一直读到 c 停止

-   `rs.ignore();`

    忽略一行

-   `rs.ignore(n);`

    忽略一行，最多忽略 n 个字符（若 n 为 $2147483647$ 则等于 `rs.ignore();`）

-   `rs.ignore(n, c);`

    忽略字符到 c 停止，最多忽略 n 个字符

-   `rs.seek();`

    移动读取指针到开始

-   `rs.eof;`

    rs 是否读到末尾

-   `rs.fail;`

    rs 是否发生错误（读到末尾后，继续读取）

-   `!rs;` `(bool)rs;`

    分别等于 `rs.fail;` `!rs.fail;`

-   `while (rs >> ...);`

    一直读取直到末尾

-   `rs >> bin;` `rs >> oct;` `rs >> dec;` `rs >> hex;`

    按 2 8 10 16 进制读取整数

-   `rs >> skipws;`

    忽略前导空格

-   `rs.setbase(base);`

    按 base 进制读取整数，超出范围默认 10 进制 $(2 \le base \le 36)$

-   `rs >> reset;`

    取消前面的所有设置

-   `ws << x;`

    写入 x

-   `ws << x << y << z;`

    写入 x, y, z

-   `ws.put(c);`

    写入一个字符 c

-   `ws.write(x, y, z);`

    写入 x, y, z

-   `ws.flush();` 或 `ws << flush;`

    刷新流

-   `ws << endl;`

    写入换行

-   `ws << ends;`

    写入空格

-   `ws << boolalpha;`

    设置写入 `bool` 时用 `true/false`

-   `ws << noboolalpha;`

    设置写入 `bool` 时用 `0/1`

-   `ws << showpos;`

    设置写入非负数（不包括 `bool`）时前面加 `+` 号

-   `ws << noshowpos;`

    设置写入非负数（不包括 `bool`）时前面不加 `+` 号

-   `ws << showpoint;`

    设置写入浮点数时严格保留 `setprecision` 时设置的位数

-   `ws << noshowpoint;`

    设置写入浮点数时不严格保留 `setprecision` 时设置的位数

-   `ws << bin;` `ws << oct;` `ws << dec;` `ws << hex;`

    按 2/8/10/16 进制写入整数

-   `ws << lowercase;`

    按大于 10 的进制写入整数时，字母大写（默认小写）

-   `ws << uppercase;`

    按大于 10 的进制写入整数时，字母小写（默认小写）

-   `ws << showbase;`

    写入 8/16 进制的整数时，在前面显示 `0`/`0x`

-   `ws << noshowbase;`

    写入 8/16 进制的整数时，不在前面显示 `0`/`0x`

-   `ws << setbase(base);`

    按 base 进制写入整数，超出范围默认 10 进制 $(2 \le base \le 36)$

-   `ws << unitbuf;`

    设置每写入一个数据就刷新流

-   `ws << nounitbuf;`

    设置超过最大限制 SIZ 时才刷新流（SIZ 为一个 `const` 变量）

-   `ws << setw(width);`

    设置下一次写入宽度若小于 `width`，就填补字符（下一次写入重置）

-   `ws << setfill(c);`

    设置 `setw` 填补的字符，默认为空格

-   `ws << setprecision(precision);`

    设置浮点数保留位数，默认保留 3 位

-   `ws << reset;`

    取消前面的所有设置

-   `rstream rs;`

    创建读流（已经有默认流 rs 了，会发生错误）

-   `wstream ws;`

    创建写流（已经有默认流 ws 了，会发生错误）

-   `rfstream rfs(dir);`

    创建文件读流，从 dir 路径文件读取（也可以是 `FILE*` 文件指针），和普通读流用法相同

-   `wfstream wfs(dir);`

    创建文件写流，写入 dir 路径的文件（也可以是 `FILE*` 文件指针），和普通写流用法相同

-   `sstream ss;`

    创建字符串流，可读写，读取后不会删除数据，会移动读取指针

-   `ss.str();`

    获取字符串流的数据

-   `ss.str(s);`

    将字符串流的数据替换为 s 并移动读取指针到开始

## 接口

-   重载运算符

    **注意要用 `fastio::interface::rstream` `fastio::interface::wstream` 来重载**

    以下是一些示例程序

    ```cpp
    // overload std::string read/write
    auto &operator<<(fastio::interface::rstream &rs, string &s) {
        static char buf[1005];
        return rs >> buf, s = buf, rs;
    }
    auto &operator<<(fastio::interface::wstream &ws, const string s) { return ws << s.c_str(); }

    // overload (,) (;) (.) (?) (!) write
    enum punctuation { comma, simicolon, fullstop, question, exclamatory };
    auto &operator<<(fastio::interface::wstream& ws, const punctuation s) {
        if (s == comma) ws.put(',');
        else if (s == simicolon) ws.put(';');
        else if (s == fullstop) ws.put('.');
        else if (s == question) ws.put('?');
        else if (s == exclamatory) ws.put('!');
        return rs;
    }

    // overload std::vector<T> read/write
    template<class T> auto& operator<<(fastio::interface::rstream& rs, const vector<T> v) {
        v.clear();
        size_t n = rs.read<size_t>();
        for (T x; n--;) {
            rs >> x;
            v.push_back(x);
        }
        return ws;
    }
    template<class T> auto& operator<<(fastio::interface::wstream& ws, const vector<T> v) {
        for (auto p = v.begin(); p != v.end(); ++p) {
            ws << *p;
            if (p != v.end() - 1) ws.put(',');
        }
        return ws;
    }
    ```

## Code

```cpp
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
    template <typename T>
    concept string_t = std::same_as<T, char*> || std::same_as<T, const char*>;
    template <typename T>
    concept notstring_t = (!string_t<T>);
    class noncopyable {
      private:
        noncopyable(const noncopyable&) = delete;
        void operator=(const noncopyable&) = delete;

      protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
    class rstream : public noncopyable {
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
        template <size_t N> rstream& operator>>(char (&s)[N]) {
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
        template <notstring_t T> const T read() {
            T x;
            return *this >> x, x;
        }
        template <string_t T> char* read(int N) {
            char* s = new char[N]{};
            return *this >> s, s;
        }
    };
    class wstream : public noncopyable {
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
```
