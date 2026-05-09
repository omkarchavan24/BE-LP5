#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

// Tree Node
class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    TreeNode(int v) {
        val = v;
        left = right = NULL;
    }
};

// Binary Tree Class
class BinaryTree {
public:
    TreeNode *root;

    BinaryTree(int rootVal) {
        root = new TreeNode(rootVal);
    }

    // ================= SEQUENTIAL BFS =================
    void sequential_bfs() {

        if (!root)
            return;

        cout << "Sequential BFS of Tree:\n";

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            TreeNode* node = q.front();
            q.pop();

            cout << node->val << " ";

            if (node->left)
                q.push(node->left);

            if (node->right)
                q.push(node->right);
        }

        cout << endl;
    }

    // ================= PARALLEL BFS =================
    void parallel_bfs() {

        if (!root)
            return;

        cout << "Parallel BFS of Tree:\n";

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            int size = q.size();

            vector<TreeNode*> current_level;

            // Extract current level nodes sequentially
            for (int i = 0; i < size; i++) {
                current_level.push_back(q.front());
                q.pop();
            }

            // Process current level in parallel
            #pragma omp parallel for
            for (int i = 0; i < current_level.size(); i++) {

                TreeNode* node = current_level[i];

                // Safe printing
                #pragma omp critical
                cout << node->val << " ";

                // Safe queue insertion
                #pragma omp critical
                {
                    if (node->left)
                        q.push(node->left);

                    if (node->right)
                        q.push(node->right);
                }
            }
        }

        cout << endl;
    }

    // ================= SEQUENTIAL DFS =================
    void sequential_dfs(TreeNode* node) {

        if (!node)
            return;

        cout << node->val << " ";

        sequential_dfs(node->left);
        sequential_dfs(node->right);
    }

    // ================= PARALLEL DFS TASK =================
    void dfs_task(TreeNode* node) {

        if (!node)
            return;

        // Safe printing
        #pragma omp critical
        cout << node->val << " ";

        // Create task for left subtree
        #pragma omp task
        dfs_task(node->left);

        // Create task for right subtree
        #pragma omp task
        dfs_task(node->right);

        // Wait for child tasks
        #pragma omp taskwait
    }

    // ================= PARALLEL DFS =================
    void parallel_dfs() {

        if (!root)
            return;

        cout << "Parallel DFS of Tree:\n";

        #pragma omp parallel
        {
            #pragma omp single
            {
                dfs_task(root);
            }
        }

        cout << endl;
    }
};

int main() {

    BinaryTree tree(1);

    // Constructing Tree
    tree.root->left = new TreeNode(2);
    tree.root->right = new TreeNode(3);

    tree.root->left->left = new TreeNode(4);
    tree.root->left->right = new TreeNode(5);

    tree.root->right->left = new TreeNode(6);
    tree.root->right->right = new TreeNode(7);

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
    cout << "Sequential DFS of Tree:\n";

    start = omp_get_wtime();

    tree.sequential_dfs(tree.root);

    end = omp_get_wtime();

    cout << endl;

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