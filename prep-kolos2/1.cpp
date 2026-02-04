#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
    int n; // item count
    int W; // backpack capacity
    std::cin >> n >> W;
    int *dp = new int[W+1]();

    //10 60
    //20 100
    //30 120
    //40 240
    // out: 300 (4, 1)

    for (int i = 0; i < n; i++) {
        int wi, vi; // item weight, item value
        std::cin >> wi >> vi;
        //wi = weights[i];
        //vi = values[i];
        // DP[i][w] = max(DP[i-1][w], vi + DP[i-1][w - wi])
        for (int j = W; j >= wi; j--) if (dp[j-wi] + vi > dp[j]) dp[j] = dp[j-wi] + vi;
    }    
    std::cout << dp[W] << '\n';
    delete[] dp;
}