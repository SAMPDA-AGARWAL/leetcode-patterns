#include <bits/stdc++.h>
using namespace std;

class Solution {
    static constexpr int INF = 1e9;

    // Factor contribution of each digit for primes: 2, 3, 5, 7
    array<int, 4> factorOfDigit(int d) {
        array<int, 4> f{0, 0, 0, 0};
        while (d % 2 == 0) f[0]++, d /= 2;
        while (d % 3 == 0) f[1]++, d /= 3;
        while (d % 5 == 0) f[2]++, d /= 5;
        while (d % 7 == 0) f[3]++, d /= 7;
        return f;
    }

public:
    string smallestNumber(string num, long long t) {
        // Only 2,3,5,7 can appear in a product of digits 1..9.
        array<int, 4> need{0, 0, 0, 0};
        int primes[4] = {2, 3, 5, 7};

        for (int p = 0; p < 4; ++p) {
            while (t % primes[p] == 0) {
                need[p]++;
                t /= primes[p];
            }
        }
        if (t != 1) return "-1";

        array<array<int, 4>, 10> digitFactor{};
        for (int d = 1; d <= 9; ++d)
            digitFactor[d] = factorOfDigit(d);

        int n = (int)num.size();

        // Prefix factor counts. pref[i] = factors in num[0...i-1].
        vector<array<int, 4>> pref(n + 1);
        pref[0] = {0, 0, 0, 0};

        bool hasZero = false;
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i];
            if (num[i] == '0') {
                hasZero = true;
            } else {
                int d = num[i] - '0';
                for (int p = 0; p < 4; ++p)
                    pref[i + 1][p] += digitFactor[d][p];
            }
        }

        // num itself is valid.
        if (!hasZero) {
            bool ok = true;
            for (int p = 0; p < 4; ++p)
                if (pref[n][p] < need[p]) ok = false;
            if (ok) return num;
        }

        /*
         * solve(low, a, b, c, d):
         * Minimum number of non-1 digits needed to provide at least
         * 2^a * 3^b * 5^c * 7^d, using digits >= low.
         *
         * Keeping digits nondecreasing lets reconstruction produce the
         * lexicographically smallest suffix.
         */
        int A = need[0], B = need[1], C = need[2], D = need[3];

        int statesPerLow = (A + 1) * (B + 1) * (C + 1) * (D + 1);
        vector<int> memo(10 * statesPerLow, -1);

        auto id = [&](int low, int a, int b, int c, int d) {
            int x = a;
            x = x * (B + 1) + b;
            x = x * (C + 1) + c;
            x = x * (D + 1) + d;
            return low * statesPerLow + x;
        };

        function<int(int, int, int, int, int)> solve =
            [&](int low, int a, int b, int c, int d) -> int {
                if (a == 0 && b == 0 && c == 0 && d == 0) return 0;

                int key = id(low, a, b, c, d);
                int &res = memo[key];
                if (res != -1) return res;

                res = INF;
                for (int digit = low; digit <= 9; ++digit) {
                    auto f = digitFactor[digit];

                    int na = max(0, a - f[0]);
                    int nb = max(0, b - f[1]);
                    int nc = max(0, c - f[2]);
                    int nd = max(0, d - f[3]);

                    int nxt = solve(digit, na, nb, nc, nd);
                    if (nxt != INF)
                        res = min(res, 1 + nxt);
                }
                return res;
            };

        // Smallest suffix of exactly len digits satisfying residual factors.
        auto buildSuffix = [&](array<int, 4> rem, int len) -> string {
            int nonOneCount = solve(2, rem[0], rem[1], rem[2], rem[3]);
            if (nonOneCount > len) return "";

            string result(len - nonOneCount, '1');

            int low = 2;
            int best = nonOneCount;

            while (best > 0) {
                for (int digit = low; digit <= 9; ++digit) {
                    auto f = digitFactor[digit];

                    array<int, 4> nxt{
                        max(0, rem[0] - f[0]),
                        max(0, rem[1] - f[1]),
                        max(0, rem[2] - f[2]),
                        max(0, rem[3] - f[3])
                    };

                    if (1 + solve(digit, nxt[0], nxt[1], nxt[2], nxt[3]) == best) {
                        result += char('0' + digit);
                        rem = nxt;
                        low = digit;
                        best--;
                        break;
                    }
                }
            }
            return result;
        };

        /*
         * Try increasing a position as far right as possible.
         * This gives the smallest number with the same length.
         *
         * We cannot keep any prefix that contains zero.
         */
        int firstZero = n;
        for (int i = 0; i < n; ++i) {
            if (num[i] == '0') {
                firstZero = i;
                break;
            }
        }

        for (int i = min(n - 1, firstZero); i >= 0; --i) {
            int current = num[i] - '0';

            for (int digit = current + 1; digit <= 9; ++digit) {
                array<int, 4> rem;

                for (int p = 0; p < 4; ++p) {
                    int have = pref[i][p] + digitFactor[digit][p];
                    rem[p] = max(0, need[p] - have);
                }

                string suffix = buildSuffix(rem, n - i - 1);
                if (!suffix.empty() || n - i - 1 == 0) {
                    // Empty suffix is valid only if no factors are still needed.
                    bool done = true;
                    for (int p = 0; p < 4; ++p)
                        if (rem[p] != 0) done = false;

                    if (!suffix.empty() || done) {
                        return num.substr(0, i) + char('0' + digit) + suffix;
                    }
                }
            }
        }

        // No valid answer with n digits. Any n+1 digit zero-free number is larger.
        int minimumDigits = solve(2, need[0], need[1], need[2], need[3]);

if (minimumDigits == INF) return "-1";

// It must have more digits than num, but may need even more digits
// to fit all required prime factors.
int answerLength = max(n + 1, minimumDigits);

return buildSuffix(need, answerLength);
    }
};