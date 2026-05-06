#include <cassert>
#include <iostream>
#include <unordered_map>

// LRUCache：双向链表 + 哈希表，get/put 均 O(1)。
// 头部为最近使用，尾部为最久未使用，head_/tail_ 为哨兵节点。
class LRUCache {
public:
    struct Node {
        int key, value;
        Node *prev, *next;
    };

    int capacity;
    std::unordered_map<int, Node*> map;
    Node* head;  // 哨兵，head->next 为最近使用
    Node* tail;  // 哨兵，tail->prev 为最久未使用

    LRUCache(int cap) : capacity(cap) {
        head = new Node{};
        tail = new Node{};
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* cur = head;
        while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
    }

    // 查询，命中返回值并刷新为最近使用，否则返回 -1。
    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        move_to_front(it->second);
        return it->second->value;
    }

    // 写入，已存在则更新；容量满时淘汰最久未使用。
    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->value = value;
            move_to_front(it->second);
            return;
        }
        if ((int)map.size() >= capacity) {
            Node* victim = tail->prev;
            detach(victim);
            map.erase(victim->key);
            delete victim;
        }
        Node* node = new Node{key, value};
        attach_front(node);
        map[key] = node;
    }

private:
    void detach(Node* n) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }

    void attach_front(Node* n) {
        n->prev = head;
        n->next = head->next;
        head->next->prev = n;
        head->next = n;
    }

    void move_to_front(Node* n) { detach(n); attach_front(n); }
};

int main() {
    LRUCache cache(2);

    cache.put(1, 100);
    cache.put(2, 200);

    assert(cache.get(1) == 100);   // 1 变为最近使用

    cache.put(3, 300);             // 容量满，淘汰 key=2
    assert(cache.get(2) == -1);    // 2 已被淘汰
    assert(cache.get(3) == 300);
    assert(cache.get(1) == 100);

    cache.put(1, 111);             // 更新已有 key
    assert(cache.get(1) == 111);
    assert((int)cache.map.size() == 2);

    std::cout << "lru passed\n";
}
