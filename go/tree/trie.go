package tree

// Trie 前缀树（字典树）
// 用途：高效存储/检索字符串集合，支持前缀匹配、自动补全、词频统计等。
// 本实现只支持小写字母 a-z；如需更大字符集，可把 children 换成 map[rune]*Trie。
//
// 空间复杂度：O(总字符数 * 26)
// 时间复杂度（以 L 为单词长度）：
//   - Insert / Search / StartsWith / Delete 均为 O(L)
type Trie struct {
	children [26]*Trie // 26 个小写字母的子节点
	isEnd    bool      // 标记从根到当前节点的路径是否构成一个完整单词
}

// NewTrie 构造一个空的前缀树
func NewTrie() *Trie {
	return &Trie{}
}

// Insert 插入一个单词
// 沿着字符依次下探，缺失的节点就补上，最后在末尾节点打上 isEnd=true
func (t *Trie) Insert(word string) {
	node := t
	for i := 0; i < len(word); i++ {
		idx := word[i] - 'a'
		if node.children[idx] == nil {
			node.children[idx] = &Trie{}
		}
		node = node.children[idx]
	}
	node.isEnd = true
}

// Search 判断单词是否完整存在于前缀树中
// 仅前缀命中但没有 isEnd=true 的节点，不算存在
func (t *Trie) Search(word string) bool {
	node := t.walk(word)
	return node != nil && node.isEnd
}

// StartsWith 判断是否存在以 prefix 开头的任意单词
// 只要能沿着 prefix 走到底，就说明存在对应分支
func (t *Trie) StartsWith(prefix string) bool {
	return t.walk(prefix) != nil
}

// walk 沿着 s 的字符从根向下走，若中途断开返回 nil，否则返回终点节点
func (t *Trie) walk(s string) *Trie {
	node := t
	for i := 0; i < len(s); i++ {
		idx := s[i] - 'a'
		if node.children[idx] == nil {
			return nil
		}
		node = node.children[idx]
	}
	return node
}

// Delete 删除一个单词；若该单词不存在返回 false
// 思路：递归向下寻找末尾节点，回溯时若当前节点既不是某单词的结尾、
// 又没有任何子节点，则可以把它从父节点上摘掉，释放空间。
func (t *Trie) Delete(word string) bool {
	return deleteHelper(t, word, 0)
}

func deleteHelper(node *Trie, word string, depth int) bool {
	if node == nil {
		return false
	}
	// 已经走到单词末尾，清理isEnd标记
	if depth == len(word) {
		if !node.isEnd {
			return false // 该单词并未被插入过
		}
		node.isEnd = false
		return true
	}

	idx := word[depth] - 'a'
	child := node.children[idx]
	if !deleteHelper(child, word, depth+1) {
		return false
	}

	// 回溯：如果子节点既不是单词结尾，也没有任何孙节点，则摘除
	if !child.isEnd && child.isEmpty() {
		node.children[idx] = nil
	}
	return true
}

// isEmpty 判断节点是否没有任何子节点
func (t *Trie) isEmpty() bool {
	for _, c := range t.children {
		if c != nil {
			return false
		}
	}
	return true
}
