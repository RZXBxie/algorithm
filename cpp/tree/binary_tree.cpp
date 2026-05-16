#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(const int x) : val(x), left(nullptr), right(nullptr) {}
};

void destroyTree(TreeNode* root) {
    if (!root) return;
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
}

void preorder(const TreeNode* root, vector<int>& ans) {
    if (!root) return;
    ans.push_back(root->val);
    preorder(root->left, ans);
    preorder(root->right, ans);
}

vector<int> preorderStack(const TreeNode* root) {
    if (!root) return {};
    stack<const TreeNode*> s;
    s.push(root);

    vector<int> ans;
    while (!s.empty()) {
        auto* node = s.top();
        s.pop();
        ans.push_back(node->val);
        if (node->right) s.push(node->right);
        if (node->left) s.push(node->left);
    }

    return ans;
}

void inorder(const TreeNode* root, vector<int>& ans) {
    if (!root) return;
    inorder(root->left, ans);
    ans.push_back(root->val);
    inorder(root->right, ans);
}

vector<int> inorderStack(const TreeNode* root) {
    stack<const TreeNode*> s;
    vector<int> ans;
    const TreeNode* cur = root;
    while (!s.empty() || cur) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        cur = s.top();
        s.pop();
        ans.push_back(cur->val);
        cur = cur->right;
    }

    return ans;
}

// Morris 中序遍历，空间 O(1)
// 核心思路：利用二叉树叶子节点的空指针，临时建立到中序前驱/后继的线索，
// 访问完左子树后再沿线索回到当前节点，最后恢复树结构。
//
// 设当前节点为 x：
// 1. x 无左孩子 → 访问 x，x = x->right
// 2. x 有左孩子 → 找到 x 左子树最右节点 pre（中序前驱）：
//    2.1 pre->right == nullptr → 建立线索 pre->right = x，x = x->left（继续向左）
//    2.2 pre->right != nullptr → 左子树已访问完，访问 x，恢复 pre->right = nullptr，x = x->right
vector<int> inorderMorris(TreeNode* root) {
    vector<int> ans;
    TreeNode* cur = root;
    while (cur) {
        if (cur->left) {
            // 寻找x的前驱结点
            TreeNode* pre = cur->left;
            while (pre->right && pre->right != cur) {
                pre = pre->right;
            }
            // pre->right指向x，说明x左子树已经访问完
            if (pre->right) {
                pre->right = nullptr;
                ans.push_back(cur->val);
                cur = cur->right;
            } else {
                pre->right = cur;
                cur = cur->left;
            }
        } else {
            ans.push_back(cur->val);
            cur = cur->right;
        }
    }

    return ans;
}

void postorder(const TreeNode* root, vector<int>& ans) {
    if (!root) return;
    postorder(root->left, ans);
    postorder(root->right, ans);
    ans.push_back(root->val);
}

// 迭代实现的后序遍历
// 核心思路：利用prev指针记录上次访问的节点，从而判断根节点能否被访问
//
// 1. 当cur不为空时，一路向左走，并把沿途节点入栈
// 2. 取出栈顶元素top，判断：
//   2.1 top->right = nullptr || top->right = prev -> 说明右子树已经访问完了，可以访问top，出栈，但是要把cur置为空，强制让下一轮循环跳过向左入栈的过程
//   2.2 top->right != nullptr -> 说明我们是从左子树回来，此时要遍历右子树。下一轮循环会继续cur->lef
vector<int> postorderStack(const TreeNode* root) {
    stack<const TreeNode*> s;
    auto* cur = root;
    const TreeNode* prev = nullptr;
    vector<int> ans;

    while (!s.empty() || cur) {
        while (cur) {
            s.push(cur);
            cur = cur->left;
        }
        cur = s.top();
        if (!cur->right || cur->right == prev) {
            s.pop();
            ans.push_back(cur->val);
            prev = cur;
            cur = nullptr;
        } else {
            cur = cur->right;
        }
    }

    return ans;
}

int main() {
    auto* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);
    vector<int> ans = postorderStack(root);
    for (auto i : ans) {
        cout << i << " ";
    }
    cout << endl;

    destroyTree(root);

    return 0;
}