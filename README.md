# FastIO

一个快读/快写库，支持基本类型（除浮点类型）与 `__int128_t` 读写，和 `cin/cout` 用法类似

虽然相对于原版函数式 `fread` 快读会有点慢，但还是比 `getchar/putchar` 要快的

编译不过？FastIO 用了 C++20 Concepts，用以下编译参数即可
最新版 MinGW 编译器在[这里](https://github.com/niXman/mingw-builds-binaries/releases)下载

```bash
g++ <file>.cpp -o <file> -std=c++20 -O3 -Ofast
```

## 目录解释

```text
fastio.cpp         # 测试文件
fastio.h           # FastIO 库源文件
fastio.noconcept.h # FastIO 库源文件（不使用 C++20 Concept）
fastio.in          # 输入测试文件
fastio.spd.in      # 速度测试文件
README.md          # README
```

## 使用

`rs >> x;`

读取 x

`rs >> x >> y >> z;`

读取 x, y, z

`c = rs.get();`

读取一个字符到 c

`rs.read(x, y, z);`

读取 x, y, z

`x = rs.read<int>();`

读取一个 `int` 类型的数据到 x

`x = rs.read<T>();`

读取一个 `T` 类型的数据到 x

`rs.getline(s);`

读取一行到 s

`rs.getline(s, delim);`

读取字符到 s，一直读到 delim 停止

`rs.ignore();`

忽略一行

`rs.ignore(delim);`

忽略字符一直读到 delim 停止

`ws << x;`

写入 x

`ws << x << y << z;`

写入 x, y, z

`ws.put(c);`

写入一个字符 c

`ws.write(x, y, z);`

写入 x, y, z

`ws.flush();` 或 `ws << flush;`

刷新流

`ws << endl;`

写入换行

`ws << ends;`

写入空格

`ws << boolalpha;`

设置写入 `bool` 时用 `true/false`

`ws << noboolalpha;`

设置写入 `bool` 时用 `0/1`

`ws << showpos;`

设置写入正整数（不包括 `bool`）时前面加 `+` 号

`ws << noshowpos;`

设置写入正整数（不包括 `bool`）时前面不加 `+` 号

`ws << unitbuf;`

设置每写入一个数据就刷新流

`ws << nounitbuf;`

设置超过最大限制 SIZ 时才刷新流（SIZ 为一个 `const` 变量）

`ws << setfill(c);`

设置 `setw` 填补的字符，默认为空格

`ws << setw(width);`

设置下一次写入宽度若小于 `width`，就填补字符

`ws << reset;`

取消前面的所有设置（`boolalpha, showpos, unitbuf`）

`rstream new_rs;`

创建新读流 new_rs（已经有默认流 rs 了，可能会出错）

`wstream new_ws;`

创建新写流 new_ws（已经有默认流 ws 了，可能会出错）

`rstream file_rs(dir);`

创建新读流，从 dir 路径文件读入

`wstream file_ws(dir);`

创建新写流，写入到 dir 路径的文件

## Code:

```cpp
#include <cctype>
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
enum symbol {
    endl,
    ends,
    flush,
    boolalpha,
    noboolalpha,
    showpos,
    noshowpos,
    unitbuf,
    nounitbuf,
    _setw,
    _setfill,
    reset
};
template <typename T>
struct sympack {
    symbol s;
    T data;
    sympack(symbol _s, T _data) : s(_s), data(_data) {}
    bool operator==(symbol rhs) { return s == rhs; }
};
const sympack<int> setw(int w) { return sympack(_setw, w); }
const sympack<char> setfill(char c) { return sympack(_setfill, c); }
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
    template <integer_t T>
    rstream& operator>>(T& x) {
        x = 0;
        bool t(0);
        char ch;
        while (!iseof(ch = get()) && !isdigit(ch))
            if (ch == '-' && !unsigned_integer_t<T>) t = 1;
        while (isdigit(ch)) x = x * 10 + (t ? -ch + 48 : ch - 48), ch = get();
        ispre = 1;
        if (flag) eof = 1;
        return *this;
    }
    rstream& operator>>(char& c) {
        while (!iseof(c = get()) && isspace(c))
            ;
        if (flag) eof = 1;
        return *this;
    }
    rstream& operator>>(char* s) {
        size_t t(0);
        char ch;
        while (!iseof(ch = get()) && isspace(ch))
            ;
        while (isgraph(ch)) s[t++] = ch, ch = get();
        s[t] = '\0', ispre = 1;
        if (flag) eof = 1;
        return *this;
    }
    rstream& operator>>(bool& f) {
        int x;
        operator>>(x);
        f = x;
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
        size_t t(0);
        char ch;
        while (!iseof(ch = get()) && ch != delim) s[t++] = ch;
        s[t] = '\0';
        if (delim == '\n' && s[t - 1] == '\r') s[t - 1] = '\0';
        return *this;
    }
    template <typename T>
    rstream& read(T x) {
        return operator>>(x);
    }
    template <typename T, typename... Args>
    rstream& read(T x, Args... args) {
        operator>>(x);
        return read(args...);
    }
    template <typename T>
    const T read() {
        T x;
        operator>>(x);
        return x;
    }
};
class wstream {
    char buf[SIZ], *p = buf, setfill = ' ';
    bool boolalpha = 0, showpos = 0, unitbuf = 0;
    size_t setw = 0;
    FILE* file = stdout;
    void fill(int cnt) {
        setw = 0;
        if (cnt < 0) return;
        while (cnt--) put(setfill);
    }

   public:
    wstream() {}
    wstream(const char* dir) : file(fopen(dir, "w")) {}
    wstream(char* dir) : file(fopen(dir, "w")) {}
    ~wstream() { flush(); }
    wstream& flush() { return (fwrite(buf, p - buf, 1, file), p = buf), *this; }
    wstream& put(char ch) { return (p - buf >= SIZ && (flush(), NULL), *p++ = ch), *this; }
    template <integer_t T>
    wstream& operator<<(T x) {
        static char cbuf[45];
        size_t len(0);
        bool t(0);
        if (!x) {
            fill(setw - showpos);
            if (showpos) put('+');
            return put('0'), *this;
        }
        if (x < 0) t = 1;
        while (x) cbuf[len++] = (t ? -(x % -10) : x % 10) + 48, x /= 10;
        if (t || showpos) cbuf[len++] = t ? '-' : '+';
        fill(setw - len);
        while (len) put(cbuf[--len]);
        if (unitbuf) flush();
        return *this;
    }
    wstream& operator<<(char c) {
        fill(setw - 1);
        put(c);
        if (unitbuf) flush();
        return *this;
    }
    template <string_t T>
    wstream& operator<<(T s)  {
        size_t len(strlen(s));
        fill(setw - len);
        for (int i(0); i < len; ++i) put(*(s + i));
        if (unitbuf) flush();
        return *this;
    }
    wstream& operator<<(bool f) {
        if (boolalpha)
            operator<<(f ? "true" : "false");
        else
            fill(setw - 1), put(f ? '1' : '0');
        if (unitbuf) flush();
        return *this;
    }
    wstream& operator<<(symbol s) {
        if (s == symbol::endl) put('\n');
        if (s == symbol::ends) put(' ');
        if (s == symbol::flush) flush();
        if (s == symbol::boolalpha) boolalpha = 1;
        if (s == symbol::noboolalpha) boolalpha = 0;
        if (s == symbol::showpos) showpos = 1;
        if (s == symbol::noshowpos) showpos = 0;
        if (s == symbol::unitbuf) unitbuf = 1;
        if (s == symbol::nounitbuf) unitbuf = 0;
        if (s == symbol::reset) boolalpha = showpos = unitbuf = 0, setfill = ' ';
        return *this;
    }
    template <typename T>
    wstream& operator<<(sympack<T> sp) {
        if (sp == symbol::_setw) setw = sp.data;
        if (sp == symbol::_setfill) setfill = sp.data;
        return *this;
    }
    template <typename T>
    wstream& write(T x) {
        return operator<<(x);
    }
    template <typename T, typename... Args>
    wstream& write(T x, Args... args) {
        operator<<(x);
        return write(args...);
    }
};
rstream rs;
wstream ws;
};  // namespace fastio
```
