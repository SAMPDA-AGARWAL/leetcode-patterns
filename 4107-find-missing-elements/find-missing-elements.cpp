class Solution {
public:
    vector<int> findMissingElements(vector<int>& nums) {
        int mn = nums[0];
        int mx = nums[0];

        unordered_set<int> st;

        // Find minimum, maximum and store all elements
        for (int num : nums) {
            mn = min(mn, num);
            mx = max(mx, num);
            st.insert(num);
        }

        vector<int> ans;

        // Check every number between min and max
        for (int i = mn + 1; i < mx; i++) {
            if (st.find(i) == st.end()) {
                ans.push_back(i);
            }
        }

        return ans;
    }
};