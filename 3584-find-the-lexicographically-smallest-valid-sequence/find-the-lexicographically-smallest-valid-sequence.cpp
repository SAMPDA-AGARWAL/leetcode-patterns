class Solution {
public:
    vector<int> validSequence(string word1, string word2) {
        int n = word1.size();
        int m = word2.size();

        // suf[i] = earliest index in word1 where
        // word2[i...] can be matched exactly
        vector<int> suf(m, -1);

        int j = m - 1;

        for (int i = n - 1; i >= 0 && j >= 0; i--) {
            if (word1[i] == word2[j]) {
                suf[j] = i;
                j--;
            }
        }

        // If word2 cannot be matched even with one mismatch,
        // answer will remain empty.
        vector<int> ans;

        int pos = 0;
        bool usedMismatch = false;

        for (int i = 0; i < m; i++) {

            // Try every possible position starting from the earliest.
            while (pos < n) {

                // Case 1: characters match
                if (word1[pos] == word2[i]) {
                    ans.push_back(pos);
                    pos++;
                    break;
                }

                // Case 2: use the one allowed mismatch
                if (!usedMismatch) {

                    // If this is the last character,
                    // we can directly use the mismatch.
                    if (i == m - 1) {
                        ans.push_back(pos);
                        pos++;
                        usedMismatch = true;
                        break;
                    }

                    // Remaining word2[i+1...] must match exactly
                    // after pos.
                    if (suf[i + 1] != -1 &&
                        suf[i + 1] > pos) {

                        ans.push_back(pos);
                        pos++;
                        usedMismatch = true;
                        break;
                    }
                }

                pos++;
            }

            // Couldn't find a valid position
            if (ans.size() != i + 1)
                return {};
        }

        return ans;
    }
};