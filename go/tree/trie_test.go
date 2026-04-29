package tree

import "testing"

func TestTrie_InsertAndSearch(t *testing.T) {
	tr := NewTrie()
	words := []string{"apple", "app", "application", "apply", "banana"}
	for _, w := range words {
		tr.Insert(w)
	}

	tests := []struct {
		word string
		want bool
	}{
		{"apple", true},
		{"app", true},
		{"application", true},
		{"apply", true},
		{"banana", true},
		{"appl", false},   // 仅前缀，不是完整单词
		{"ban", false},    // 仅前缀，不是完整单词
		{"orange", false}, // 未插入
		{"", false},       // 空串未插入
	}
	for _, tt := range tests {
		if got := tr.Search(tt.word); got != tt.want {
			t.Errorf("Search(%q) = %v, want %v", tt.word, got, tt.want)
		}
	}
}

func TestTrie_StartsWith(t *testing.T) {
	tr := NewTrie()
	for _, w := range []string{"apple", "app", "banana"} {
		tr.Insert(w)
	}

	tests := []struct {
		prefix string
		want   bool
	}{
		{"app", true},
		{"appl", true},
		{"apple", true},
		{"b", true},
		{"ban", true},
		{"bananas", false}, // 超出已有最长单词
		{"cat", false},
		{"", true}, // 空前缀永远匹配
	}
	for _, tt := range tests {
		if got := tr.StartsWith(tt.prefix); got != tt.want {
			t.Errorf("StartsWith(%q) = %v, want %v", tt.prefix, got, tt.want)
		}
	}
}

func TestTrie_EmptyString(t *testing.T) {
	tr := NewTrie()
	// 未插入空串时，Search("") 应为 false
	if tr.Search("") {
		t.Errorf(`未插入时 Search("") 应为 false`)
	}
	// 插入空串后，Search("") 应为 true
	tr.Insert("")
	if !tr.Search("") {
		t.Errorf(`插入后 Search("") 应为 true`)
	}
}

func TestTrie_Delete(t *testing.T) {
	tr := NewTrie()
	for _, w := range []string{"apple", "app", "apply"} {
		tr.Insert(w)
	}

	// 删除未插入的单词：返回 false，不影响已有数据
	if tr.Delete("orange") {
		t.Errorf(`Delete("orange") 应为 false`)
	}
	if !tr.Search("apple") {
		t.Errorf(`误删: "apple" 应仍然存在`)
	}

	// 删除 "apple"：apple 本身不存在了，但 app 仍然是单词，apply 也应保留
	if !tr.Delete("apple") {
		t.Errorf(`Delete("apple") 应为 true`)
	}
	if tr.Search("apple") {
		t.Errorf(`"apple" 已被删除，不应存在`)
	}
	if !tr.Search("app") {
		t.Errorf(`删除 "apple" 后 "app" 仍应存在`)
	}
	if !tr.Search("apply") {
		t.Errorf(`删除 "apple" 后 "apply" 仍应存在`)
	}

	// 再删一次 "apple"：已经不存在，应返回 false
	if tr.Delete("apple") {
		t.Errorf(`重复 Delete("apple") 应为 false`)
	}

	// 删除 "app"：仅清除 isEnd 标记，不影响 "apply"
	if !tr.Delete("app") {
		t.Errorf(`Delete("app") 应为 true`)
	}
	if tr.Search("app") {
		t.Errorf(`"app" 已被删除，不应存在`)
	}
	if !tr.Search("apply") {
		t.Errorf(`删除 "app" 后 "apply" 仍应存在`)
	}
	if !tr.StartsWith("app") {
		t.Errorf(`"apply" 还在，StartsWith("app") 应为 true`)
	}

	// 删除最后一个 "apply" 后，以 "app" 为前缀的分支应彻底释放
	if !tr.Delete("apply") {
		t.Errorf(`Delete("apply") 应为 true`)
	}
	if tr.StartsWith("a") {
		t.Errorf(`所有以 a 开头的单词均被删除，StartsWith("a") 应为 false`)
	}
}

func TestTrie_Print(t *testing.T) {
	tr := NewTrie()
	words := []string{"go", "good", "goodbye", "golang", "gopher"}
	for _, w := range words {
		tr.Insert(w)
	}
	t.Logf("插入单词: %v", words)
	for _, w := range []string{"go", "good", "goo", "gone"} {
		t.Logf("  Search(%q)     = %v", w, tr.Search(w))
		t.Logf("  StartsWith(%q) = %v", w, tr.StartsWith(w))
	}
}
