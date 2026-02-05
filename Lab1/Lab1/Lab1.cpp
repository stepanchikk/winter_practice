#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Класи Винятків
class ErrorRange {
public:
    double x;
    ErrorRange(double dx) : x(dx) {}
    void Message() {
        cout << "ErrorRange: x = " << x << " (allowed -10 < x < 10)\n";
    }
};

class ErrorNoFile {
    string name;
public:
    ErrorNoFile(string n) : name(n) {}
    void Message() {
        cout << "ErrorNoFile: cannot open " << name << endl;
    }
};

class ErrorAlg {
protected:
    double v;
public:
    ErrorAlg(double dv) : v(dv) {}
    virtual void Message() {
        cout << "ErrorAlg: invalid value " << v << endl;
    }
    virtual double fix() { return v + 1; }
};

class ErrorDAlg : public ErrorAlg {
    double w;
public:
    ErrorDAlg(double dv, double dw) : ErrorAlg(dv), w(dw) {}
    void Message() override {
        cout << "ErrorDAlg: v=" << v << " w=" << w << endl;
    }
    double fix() override {
        return floor(v + w);
    }
};

// Таблична функція
double Tbl(double x, const string& fname)
throw (ErrorRange, ErrorNoFile)
{
    if (x <= -10 || x >= 10) throw ErrorRange(x);

    ifstream f(fname);
    if (!f) throw ErrorNoFile(fname);

    double x1, y1, x2, y2;
    f >> x1 >> y1;
    if (x1 == x) return y1;

    while (!f.eof()) {
        x2 = x1; y2 = y1;
        f >> x1 >> y1;
        if (x2 < x && x < x1)
            return y2 + (y1 - y2) * (x - x2) / (x1 - x2);
        if (x1 == x) return y1;
    }
    return y1;
}

// Алгоритм 3
double U(double x) throw (ErrorRange, ErrorNoFile) {
    return Tbl(x, "dat1.dat");
}

double T(double x) throw (ErrorRange, ErrorNoFile) {
    return Tbl(x, "dat2.dat");
}

// Алгоритм 2
double Qnr(double x, double y) {
    if (y == 0) throw ErrorAlg(y);
    return (x > y) ? x * x * (10 * y - x)
        : pow(x, 4);
}

double Qnk(double x, double y) {
    return 10 * Qnr(2 * x, y) - 4 * Qnr(x, 2 * y);
}

// Алгоритм 1
double Rnk(double x, double y, double z)
throw (ErrorAlg, ErrorDAlg)
{
    if (z == 0) throw ErrorAlg(z);
    if (x == 0 && y == 0) throw ErrorDAlg(x, y);
    return x * Qnk(x, y) + y * Qnk(y, z) + z * Qnk(z, x);
}

// Алгоритм 4
double Gnk(double x, double y, double z)
throw (ErrorRange, ErrorNoFile)
{
    double r = x * x + y * y + z * z;
    if (r < 0.001) return 0;

    try {
        return x * T(x / r) + y * U(y / r) + z * T(z / r);
    }
    catch (ErrorAlg& e) {
        e.Message();
        return e.fix();
    }
}

// Фінальна функція
double func(double u, double v, vector<string>& text)
throw (ErrorRange, ErrorNoFile)
{
    double res;
    try {
        res = Rnk(u, v, u + v);
    }
    catch (ErrorAlg& e) {
        e.Message();
        res = e.fix();
    }

    if (fabs(res) <= 0.5)
        return Gnk(u, v, res);

    for (string s : text)
        if (s == "key")
            return res + u * u + v * v;

    return res - u * u - v * v;
}

// Основна функція
int main() {
    double u, v;
    cout << "Input u v: ";
    cin >> u >> v;

    vector<string> text;
    ifstream f("dat3.dat");
    if (f) {
        string s;
        while (f >> s) text.push_back(s);
    }

    double result;
    try {
        result = func(u, v, text);
    }
    catch (ErrorRange& e) {
        e.Message();
        result = e.x / 10;
    }
    catch (ErrorNoFile& e) {
        e.Message();
        result = 1.1 * u + 2.2 * v;
    }
    catch (...) {
        cout << "Unknown error\n";
        result = 0;
    }

    cout << "Result = " << result << endl;
    return 0;
}