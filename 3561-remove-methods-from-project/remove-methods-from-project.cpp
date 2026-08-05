class Solution {
public:
    void dfs(int node, vector<vector<int>> &adj, vector<bool> &suspicious) {
        suspicious[node] = true;

        for (int next : adj[node]) {
            if (!suspicious[next]) {
                dfs(next, adj, suspicious);
            }
        }
    }

    vector<int> remainingMethods(int n, int k, vector<vector<int>>& invocations) {

        vector<vector<int>> adj(n);

        for (auto &edge : invocations) {
            adj[edge[0]].push_back(edge[1]);
        }

        // Step 1: Mark all suspicious methods
        vector<bool> suspicious(n, false);
        dfs(k, adj, suspicious);

        // Step 2: Check if any outside method calls a suspicious one
        for (auto &edge : invocations) {
            int u = edge[0];
            int v = edge[1];

            if (!suspicious[u] && suspicious[v]) {
                vector<int> ans;
                for (int i = 0; i < n; i++)
                    ans.push_back(i);
                return ans;
            }
        }

        // Step 3: Remove suspicious methods
        vector<int> ans;
        for (int i = 0; i < n; i++) {
            if (!suspicious[i])
                ans.push_back(i);
        }

        return ans;
    }
};