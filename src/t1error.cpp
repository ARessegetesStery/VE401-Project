#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

using namespace std;

long long factVal[11] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};

struct pairInt {
    int a;
    int b;
    pairInt(int a, int b) : a(a), b(b) {}
};

bool operator<(const pairInt& p, const pairInt& q) { return p.a == q.a ? p.b < q.b : p.a < q.a; }


map<pairInt, long long> binomPool;

float factorial(int n)
{
    if (n < 0)
        return 0;
    else if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

long long binom_s (int a, int b)
{
    return factVal[a] / factVal[b] / factVal[a - b];
}

// long long binom(int a, int b)
// {
//     if (a - b < b)
//         b = a - b;
//     if (b < 0 || a < b)
//         b = 1;
//     if (a == 0)
//         return 1;
//     else if (b == 0)
//         return 1;
//     if (b == 1)
//         return a;
//     else if (a <= 10)
//         return binom_s(a, b);
//     else 
//         return binom(a - 1, b - 1) + binom(a - 1, b);
// }

long long binom(int a, int b)
{
    /*  Pre-processing  */
    if (a - b < b)
        b = a - b;
    if (b < 0 || a < b)
        b = 1;

    /*  Search for cases in binomPool  */
    auto res = binomPool.find(pairInt(a, b));
    if (res != binomPool.end())
        return res->second;

    /*  Special cases  */
    if (a == 0)
        return 1;
    else if (b == 0)
        return 1;
    if (b == 1)
        return a;

    /*  Evaluate  */
    else if (a <= 10)
    {
        long long ans = binom_s(a, b);
        binomPool[pairInt(a, b)] = ans;
        return ans;
    }
    else 
    {
        long long ans = binom(a - 1, b - 1) + binom(a - 1, b);
        binomPool[pairInt(a, b)] = ans;
        return ans;
    }
}

double bHyperGeometricPDF(int n, int n1, int n2, int N, int N1, int N2)
{
    long long bNn = binom(N, n);
    double res = 1.0 / bNn * binom(N1, n1) * binom(N2, n2) * binom(N - N1 - N2, n - n1 - n2);
    return res;
}

int t1Tolerance(int N, int n, ofstream& os)
{
    int b3Tol = 0;
    int b4Tol = 0;
    double cumulP = 0;

    /*  Examine B.3  */
    for (int tol = 0; tol <= n; ++tol)
    {
        cumulP += bHyperGeometricPDF(n, tol, 0, N, floor(0.025*N) + 1, 0);
        cout << cumulP << endl;
        if (cumulP >= 0.95)
        {
            b3Tol = tol;
            break;
        }
        else if (tol == n)
            b3Tol = n;
    }

    /*  Examine B.4  */
    cumulP = 0;
    for (int tol = 0; tol <= floor(0.0863*N) + 1; ++tol)
    {
        cumulP += bHyperGeometricPDF(n, tol, 0, N, floor(0.0863*N) + 1, floor(0.0037*N));
        cout << n << " " << tol << " " << 0 << " " << N << " " << floor(0.0863*N) + 1 << " " << floor(0.0037*N) << endl;
        cout << cumulP << endl;
        if (cumulP >= 0.10)
        {
            b4Tol = tol == 0 ? 0 : tol - 1;
            break;
        }
    }
    cout << b3Tol << " " << b4Tol << endl;
    os << n << "," << N << "," << b3Tol << "," << b4Tol << "," << min(b3Tol, b4Tol) << endl;
    return min(b3Tol, b4Tol);
}

int main()
{
    // int N, n;
    // cin >> N >> n;
    ofstream fout("t1.csv");
    for (int cnt = 20; cnt <= 50; ++cnt)
        cout << t1Tolerance(cnt, 10, fout);
    for (int cnt = 51; cnt <= 64; ++cnt)
        cout << t1Tolerance(cnt, 32, fout);
    fout.close();
}