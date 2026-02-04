#include <iostream>
#include <string>

// attempt numero tres!!!
int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    std::string r;
    std::cin >> r;
    if (r == "0")
    {
        std::cout << 19;
        return 0;
    }

    int len = r.length();
    const int mod = 1000000007;
    // 0-81
    const int cap = 82;
    long long counts[cap] = {0};
    long long counts_b_nonzero[cap] = {0};
    long long counts_ab_nonzero[cap] = {0};

    // for each mult 0-81, how many pairs result in mult?
    for (int digit_a = 0; digit_a <= 9; digit_a++)
    {
        for (int digit_b = 0; digit_b <= 9; digit_b++)
        {
            int multiplied = digit_a * digit_b;
            counts[multiplied]++;
            if (digit_a != 0 && digit_b != 0) counts_ab_nonzero[multiplied]++;
            if (digit_b != 0) counts_b_nonzero[multiplied]++;
        }
    }

    long long dp_singledigit = 1;
    long long dp_twodigit = 0;

    long long result = 0;

    // processes BACKWARDS
    for (int i = len - 1; i >= 0; i--)
    {
        int value = r[i] - '0';

        // current digit alone (for example, 5 in 567)
        // multiplied by ways to do the rest
        long long cur = (counts[value] * dp_singledigit) % mod;

        // two digits together (56 in 567)
        if (i + 1 < len && r[i] != '0')
        {
            // skip leading 0
            int two_digit_val = value * 10 + (r[i + 1] - '0');
            if (two_digit_val <= 81)
            {
                // again, add ways to do this times existing ways
                cur = (cur + counts[two_digit_val] * dp_twodigit) % mod;
            }
        }

        // splits: (5, 67) vs (56, 7) etc
        if (i == 0)
        {
            // leftmost
            // weight: how many pairs total can produce this number? (taken from counts)
            long long weight = (len == 1) ? counts[value] : counts_ab_nonzero[value];
            // contrib: weight * ways to do the rest of the string
            long long contrib = (weight * dp_singledigit) % mod;

            // two digits on left
            if (len >= 2 && r[0] != '0')
            {
                int two_digit_val = value * 10 + (r[1] - '0');
                if (two_digit_val <= 81)
                {
                    contrib = (contrib + counts_ab_nonzero[two_digit_val] * dp_twodigit) % mod;
                }
            }
            result = (result + contrib) % mod;
        }
        else
        {
            // second number starts here
            long long weight = (i == len - 1) ? counts[value] : counts_b_nonzero[value];
            long long contrib = (weight * dp_singledigit) % mod;

            // two digits?
            if (i + 1 < len && r[i] != '0')
            {
                int two_digit_val = value * 10 + (r[i + 1] - '0');
                if (two_digit_val <= 81)
                {
                    contrib = (contrib + counts[two_digit_val] * dp_twodigit) % mod;
                }
            }
            // counts twice
            result = (result + contrib * 2) % mod;
        }
        dp_twodigit = dp_singledigit;
        dp_singledigit = cur;
    }

    std::cout << result;
}
