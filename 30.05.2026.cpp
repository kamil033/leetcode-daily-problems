#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class SegmentTree {
    int n;
    vector<int> tree;

public:
    SegmentTree(int n) {
        this->n = n;
        tree.assign(4 * n, 0);
    }

    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return 0;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = start + (end - start) / 2;
        int p1 = query(2 * node, start, mid, l, r);
        int p2 = query(2 * node + 1, mid + 1, end, l, r);
        return max(p1, p2);
    }
};

class Solution {
public:
    vector<bool> getResults(vector<vector<int>>& queries) {
        // Find the maximum bound for x from constraints and input
        int max_x = 0;
        for (const auto& q : queries) {
            max_x = max(max_x, q[1]);
        }
        
        // Target boundary size
        int M = max(50000, max_x) + 1;
        
        SegmentTree st(M);
        set<int> obstacles;
        
        // Sentinels to prevent boundary checks
        obstacles.insert(0);
        obstacles.insert(M);
        st.update(1, 0, M - 1, M, M); 
        
        vector<bool> ans;
        
        for (const auto& q : queries) {
            int type = q[0];
            if (type == 1) {
                int x = q[1];
                auto it = obstacles.upper_bound(x);
                int next_obs = *it;
                int prev_obs = *prev(it);
                
                // Insert the new obstacle
                obstacles.insert(x);
                
                // Update sizes in Segment Tree
                st.update(1, 0, M - 1, x, x - prev_obs);
                st.update(1, 0, M - 1, next_obs, next_obs - x);
                
            } else if (type == 2) {
                int x = q[1];
                int sz = q[2];
                
                // Find the closest obstacle to the left of or at x
                auto it = obstacles.upper_bound(x);
                int prev_obs = *prev(it);
                
                // 1. Max gap fully within [0, prev_obs]
                int max_gap = st.query(1, 0, M - 1, 0, prev_obs);
                
                // 2. The remaining interval from the last obstacle up to x
                max_gap = max(max_gap, x - prev_obs);
                
                ans.push_back(max_gap >= sz);
            }
        }
        
        return ans;
    }
};
