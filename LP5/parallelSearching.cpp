#include <bits/stdc++.h>
#include <omp.h>
using namespace std;


class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    TreeNode(int v) {
        val = v;
        left = right = NULL;
    }
};


class BinaryTree {
public:
    TreeNode *root;

    BinaryTree(int rootVal) {
        root = new TreeNode(rootVal);
    }

    //parallel bfs
    void parallel_bfs() {
        if (!root) return;

        cout << "Parallel BFS of Tree:\n";

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();

            vector<TreeNode*> current_level;

            // Step 1: Extract current level nodes (sequential)
            for (int i = 0; i < size; i++) {
                current_level.push_back(q.front());
                q.pop();
            }

            // Step 2: Process nodes in parallel
            #pragma omp parallel for
            for (int i = 0; i < current_level.size(); i++) {
                TreeNode* node = current_level[i];

                // Print node safely
                #pragma omp critical
                cout << node->val << " ";

                // Push children safely into queue
                #pragma omp critical
                {
                    if (node->left) q.push(node->left);
                    if (node->right) q.push(node->right);
                }
            }
        }
        cout << endl;
    }

   
    void dfs_task(TreeNode* node) {
        if (!node) return;

       
        #pragma omp critical
        cout << node->val << " ";

       
        #pragma omp task
        dfs_task(node->left);

        #pragma omp task
        dfs_task(node->right);
    }

   
    void parallel_dfs() {
        if (!root) return;

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

    // Constructing tree
    tree.root->left = new TreeNode(2);
    tree.root->right = new TreeNode(3);
    tree.root->left->left = new TreeNode(4);
    tree.root->left->right = new TreeNode(5);
    tree.root->right->left = new TreeNode(6);
    tree.root->right->right = new TreeNode(7);

    tree.parallel_bfs();
    tree.parallel_dfs();

    return 0;
}