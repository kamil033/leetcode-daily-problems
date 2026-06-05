#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

class Solution {
    // dp[pos][tight][is_started][last_digit][second_last_digit]
    // Stores: {count_of_valid_numbers, sum_of_waviness_for_those_numbers}
    pair<long long, long long> dp[17][2][2][11][11];
    string num_str;

    pair<long long, long long> dfs(int pos, bool tight, bool is_started, int last, int second_last) {
        // Base case: all digits placed
        if (pos == num_str.length()) {
            return {1LL, 0LL}; // Found 1 valid number, its trailing contribution to waviness is 0
        }

        // Return cached result if available
        if (dp[pos][tight][is_started][last][second_last].first != -1) {
            return dp[pos][tight][is_started][last][second_last];
        }

        int limit = tight ? (num_str[pos] - '0') : 9;
        long long total_count = 0;
        long long total_waviness = 0;

        for (int digit = 0; digit <= limit; ++digit) {
            bool next_tight = tight && (digit == limit);
            bool next_started = is_started || (digit > 0);

            // Calculate waviness contribution of the 'last' digit
            int waviness_contrib = 0;
            if (is_started && next_started && last != 10 && second_last != 10) {
                if (last > second_last && last > digit) {
                    waviness_contrib = 1; // Peak detected
                } else if (last < second_last && last < digit) {
                    waviness_contrib = 1; // Valley detected
                }
            }

            // Transition to the next digit position
            int next_last = next_started ? digit : 10;
            int next_second_last = next_started ? last : 10;

            auto [sub_count, sub_waviness] = dfs(pos + 1, next_tight, next_started, next_last, next_second_last);

            total_count += sub_count;
            // Total waviness = waviness from deeper branches + (waviness introduced here * number of paths down those branches)
            total_waviness += sub_waviness + (waviness_contrib * sub_count);
        }

        return dp[pos][tight][is_started][last][second_last] = {total_count, total_waviness};
    }

    long long solve(long long n) {
        if (n < 100) return 0; // Any number with fewer than 3 digits has 0 waviness
        num_str = to_string(n);
        
        // Reset memoization table with -1
        memset(dp, -1, sizeof(dp));
        
        // State initializations use '10' dummy value representing "no digit placed yet"
        return dfs(0, true, false, 10, 10).second;
    }

public:
    long long totalWaviness(long long num1, long long num2) {
        return solve(num2) - solve(num1 - 1);
    }
};
