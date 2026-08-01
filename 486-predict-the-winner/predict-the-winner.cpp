class Solution {
public:
    vector<vector<int>> dp;

    int solve(vector<int>& nums, int i, int j) {
        if (i > j)
            return 0;

        if (dp[i][j] != INT_MIN)
            return dp[i][j];

        int takeLeft = nums[i] - solve(nums, i + 1, j);
        int takeRight = nums[j] - solve(nums, i, j - 1);

        return dp[i][j] = max(takeLeft, takeRight);
    }

    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        dp.assign(n, vector<int>(n, INT_MIN));

        return solve(nums, 0, n - 1) >= 0;
    }
};