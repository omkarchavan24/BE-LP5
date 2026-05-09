#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

// ================= TREE NODE =================
class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    TreeNode(int v) {
        val = v;
        left = right = NULL;
    }
};

// ================= BINARY TREE =================
class BinaryTree {
public:
    TreeNode* root;

    BinaryTree() {
        root = NULL;
    }

    // ================= CREATE LARGE TREE =================
    void createRandomTree(int n) {

        if (n <= 0)
            return;

        vector<TreeNode*> nodes;

        root = new TreeNode(1);

        nodes.push_back(root);

        int value = 2;

        // Create complete binary tree
        for (int i = 0; i < nodes.size() && value <= n; i++) {

            TreeNode* current = nodes[i];

            // Left child
            if (value <= n) {
                current->left = new TreeNode(value++);
                nodes.push_back(current->left);
            }

            // Right child
            if (value <= n) {
                current->right = new TreeNode(value++);
                nodes.push_back(current->right);
            }
        }
    }

    // ================= SEQUENTIAL BFS =================
    void sequential_bfs() {

        if (!root)
            return;

        queue<TreeNode*> q;

        q.push(root);

        long long count = 0;

        while (!q.empty()) {

            TreeNode* node = q.front();
            q.pop();

            count++;

            if (node->left)
                q.push(node->left);

            if (node->right)
                q.push(node->right);
        }

        cout << "Sequential BFS Visited Nodes: "
             << count << endl;
    }

    // ================= PARALLEL BFS =================
    void parallel_bfs() {

        if (!root)
            return;

        queue<TreeNode*> q;

        q.push(root);

        long long totalCount = 0;

        while (!q.empty()) {

            int size = q.size();

            vector<TreeNode*> current_level;

            // Extract current level
            for (int i = 0; i < size; i++) {
                current_level.push_back(q.front());
                q.pop();
            }

            long long levelCount = 0;

            // Process level in parallel
            #pragma omp parallel for reduction(+:levelCount)
            for (int i = 0; i < current_level.size(); i++) {

                TreeNode* node = current_level[i];

                levelCount++;

                // Safe queue insertion
                #pragma omp critical
                {
                    if (node->left)
                        q.push(node->left);

                    if (node->right)
                        q.push(node->right);
                }
            }

            totalCount += levelCount;
        }

        cout << "Parallel BFS Visited Nodes: "
             << totalCount << endl;
    }

    // ================= SEQUENTIAL DFS =================
    void sequential_dfs(TreeNode* node, long long &count) {

        if (!node)
            return;

        count++;

        sequential_dfs(node->left, count);

        sequential_dfs(node->right, count);
    }

    // ================= DFS TASK =================
    void dfs_task(TreeNode* node, long long &count) {

        if (!node)
            return;

        // Safe increment
        #pragma omp atomic
        count++;

        // Left subtree task
        #pragma omp task
        dfs_task(node->left, count);

        // Right subtree task
        #pragma omp task
        dfs_task(node->right, count);

        // Wait for tasks
        #pragma omp taskwait
    }

    // ================= PARALLEL DFS =================
    void parallel_dfs() {

        if (!root)
            return;

        long long count = 0;

        #pragma omp parallel
        {
            #pragma omp single
            {
                dfs_task(root, count);
            }
        }

        cout << "Parallel DFS Visited Nodes: "
             << count << endl;
    }
};

// ================= MAIN =================
int main() {

    BinaryTree tree;

    // Create large tree
    tree.createRandomTree(1000000);

    double start, end;

    // ================= SEQUENTIAL BFS =================
    start = omp_get_wtime();

    tree.sequential_bfs();

    end = omp_get_wtime();

    cout << "Sequential BFS Time: "
         << end - start << " seconds\n\n";

    // ================= PARALLEL BFS =================
    start = omp_get_wtime();

    tree.parallel_bfs();

    end = omp_get_wtime();

    cout << "Parallel BFS Time: "
         << end - start << " seconds\n\n";

    // ================= SEQUENTIAL DFS =================
    long long dfsCount = 0;

    start = omp_get_wtime();

    tree.sequential_dfs(tree.root, dfsCount);

    end = omp_get_wtime();

    cout << "Sequential DFS Visited Nodes: "
         << dfsCount << endl;

    cout << "Sequential DFS Time: "
         << end - start << " seconds\n\n";

    // ================= PARALLEL DFS =================
    start = omp_get_wtime();

    tree.parallel_dfs();

    end = omp_get_wtime();

    cout << "Parallel DFS Time: "
         << end - start << " seconds\n";

    return 0;
}