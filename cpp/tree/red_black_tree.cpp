#include <iostream>
using namespace std;

enum Color {RED, BLACK};

struct TreeNode {
    int data;
    TreeNode *left, *right, *parent; //红黑树极其依赖父节点指针
    Color color;

    explicit TreeNode(const int val): data(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

class RedBlackTree {
    TreeNode* root = nullptr;

    void leftRotate(TreeNode* node) {
        TreeNode* child = node->right;
        node->right = child->left;
        if (child->left) child->left->parent = node;
        child->parent = node->parent;
        if (!node->parent) root = child;
        else if (node->parent->left == node) node->parent->left = child;
        else node->parent->right = child;
        child->left = node;
        node->parent = child;
    }

    void rightRotate(TreeNode* node) {
        TreeNode* child = node->left;
        node->left = child->right;
        if (child->right) child->right->parent = node;
        child->parent = node->parent;
        if (!node->parent) root = child;
        else if (node->parent->left == node) node->parent->left = child;
        else node->parent->right = child;
        child->right = node;
        node->parent = child;
    }

    void insertFixUp(TreeNode* node) {
        while (node->parent && node->parent->color == RED) {
            TreeNode* grandparent = node->parent->parent;

            // 如果父节点是祖父节点的左孩子
            if (node->parent == grandparent->left) {
                // 情况 1：如果叔叔节点是红色
                if (TreeNode* uncle = grandparent->right; uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    uncle->color = BLACK;
                    node->parent->color = BLACK;

                    // 指针上移，继续检查
                    node = grandparent;
                } else {
                    // 情况 3：叔叔是黑色，且当前节点是右孩子（LR型）
                    if (node == node->parent->right) {
                        node = node->parent;
                        // 掰成LL型
                        leftRotate(node);
                    }
                    // 情况 2： 叔叔是黑色，且当前节点是左孩子（LL型）
                    node->parent->color = BLACK;
                    grandparent->color = RED;
                    rightRotate(grandparent);
                }
            }
            // 如果父节点是祖父节点的右孩子（逻辑对称）
            else {
                if (TreeNode* uncle = grandparent->left; uncle && uncle->color == RED) {
                    grandparent->color = RED;
                    uncle->color = BLACK;
                    node->parent->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rightRotate(node);
                    }

                    node->parent->color = BLACK;
                    grandparent->color = RED;
                    leftRotate(grandparent);
                }
            }
        }

        // 无论如何，根节点必须是黑色
        root->color = BLACK;
    }

    void inorderTraversal(TreeNode* node) {
        if (!node) return;
        inorderTraversal(node->left);
        cout << node->data << "(" << (node->color == RED ? "R" : "B") << ") ";
        inorderTraversal(node->right);
    }

    // 验证性质4（红节点的子节点必须是黑色）和性质5（所有路径黑节点数相同）
    bool verifyProperty4And5(TreeNode* node, int blackCount, int& pathBlackCount) {
        if (!node) {
            // 到达叶子（nullptr视为黑色NIL节点）
            if (pathBlackCount == -1) {
                pathBlackCount = blackCount;
            } else if (blackCount != pathBlackCount) {
                cerr << "违反性质5：路径黑节点数不一致。期望 " << pathBlackCount << "，实际 " << blackCount << endl;
                return false;
            }
            return true;
        }

        // 性质4：红节点不能有红子节点
        if (node->color == RED) {
            if (node->left && node->left->color == RED) {
                cerr << "违反性质4：红节点 " << node->data << " 有红左子节点 " << node->left->data << endl;
                return false;
            }
            if (node->right && node->right->color == RED) {
                cerr << "违反性质4：红节点 " << node->data << " 有红右子节点 " << node->right->data << endl;
                return false;
            }
        }

        // 统计黑节点数
        if (node->color == BLACK) {
            blackCount++;
        }

        return verifyProperty4And5(node->left, blackCount, pathBlackCount) &&
               verifyProperty4And5(node->right, blackCount, pathBlackCount);
    }

public:
    void insert(int key) {
        TreeNode* node = new TreeNode(key);
        TreeNode* x = root;
        TreeNode* y = nullptr;

        while (x) {
            y = x;
            if (key < x->data) x = x->left;
            else x = x->right;
        }
        node->parent = y;

        // node就是根节点
        if (y == nullptr) {
            root = node;
        } else if (key < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        // node是根节点，直接涂黑返回
        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        // 如果祖父节点为空，不需要平衡操作（父节点一定是黑色）
        if (node->parent->parent == nullptr) return;

        // 调用核心修复函数
        insertFixUp(node);
    }

    // 辅助方法：中序遍历（验证BST性质）
    void inorderTraversal() {
        inorderTraversal(root);
        cout << endl;
    }

    // 辅助方法：验证红黑树性质
    bool verifyProperties() {
        if (!root) return true;

        // 性质2：根节点是黑色
        if (root->color != BLACK) {
            cerr << "违反性质2：根节点不是黑色" << endl;
            return false;
        }

        // 验证性质4和性质5
        int pathBlackCount = -1;
        return verifyProperty4And5(root, 0, pathBlackCount);
    }

};

// 测试函数
void testSingleInsert() {
    cout << "=== 测试1：插入单个节点 ===" << endl;
    RedBlackTree tree;
    tree.insert(10);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testNoRotation() {
    cout << "=== 测试2：无需旋转（父节点为黑色）===" << endl;
    RedBlackTree tree;
    tree.insert(10);  // 根节点，黑色
    tree.insert(5);   // 左子节点，红色，父节点黑色无需调整
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testCase1_RedUncle() {
    cout << "=== 测试3：情况1（叔叔为红色）===" << endl;
    RedBlackTree tree;
    // 构造触发情况1的场景：叔叔节点为红色
    // 插入顺序：10, 5, 15, 1（会导致5的父节点10为红色，叔叔15为红色）
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(1);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testCase2_LL() {
    cout << "=== 测试4：情况2（LL型，叔叔为黑色）===" << endl;
    RedBlackTree tree;
    // 构造LL型：插入 10, 5, 1
    tree.insert(10);
    tree.insert(5);
    tree.insert(1);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testCase3_LR() {
    cout << "=== 测试5：情况3（LR型，叔叔为黑色）===" << endl;
    RedBlackTree tree;
    // 构造LR型：插入 10, 5, 7
    tree.insert(10);
    tree.insert(5);
    tree.insert(7);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testRR() {
    cout << "=== 测试6：RR型（对称情况）===" << endl;
    RedBlackTree tree;
    // 构造RR型：插入 10, 15, 20
    tree.insert(10);
    tree.insert(15);
    tree.insert(20);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testRL() {
    cout << "=== 测试7：RL型（对称情况）===" << endl;
    RedBlackTree tree;
    // 构造RL型：插入 10, 15, 12
    tree.insert(10);
    tree.insert(15);
    tree.insert(12);
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testSequentialInsert() {
    cout << "=== 测试8：顺序插入1-10 ===" << endl;
    RedBlackTree tree;
    for (int i = 1; i <= 10; i++) {
        tree.insert(i);
    }
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

void testRandomInsert() {
    cout << "=== 测试9：随机插入 ===" << endl;
    RedBlackTree tree;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    for (int key : keys) {
        tree.insert(key);
    }
    cout << "中序遍历: ";
    tree.inorderTraversal();
    cout << "验证性质: " << (tree.verifyProperties() ? "通过" : "失败") << endl;
}

int main() {
    testSingleInsert();
    cout << endl;
    testNoRotation();
    cout << endl;
    testCase1_RedUncle();
    cout << endl;
    testCase2_LL();
    cout << endl;
    testCase3_LR();
    cout << endl;
    testRR();
    cout << endl;
    testRL();
    cout << endl;
    testSequentialInsert();
    cout << endl;
    testRandomInsert();

    return 0;
}