package dp

import "testing"

func TestMinEditDistance(t *testing.T) {
	tests := []struct {
		name   string
		s1, s2 string
		want   int
	}{
		{"both empty", "", "", 0},
		{"s1 empty", "", "abc", 3},
		{"s2 empty", "abc", "", 3},
		{"equal", "hello", "hello", 0},
		{"classic horse/ros", "horse", "ros", 3},
		{"classic intention/execution", "intention", "execution", 5},
		{"single replace", "a", "b", 1},
		{"insert at end", "ab", "abc", 1},
		{"delete at end", "abc", "ab", 1},
		{"unicode", "你好世界", "你好地球", 2},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got := MinEditDistance(tt.s1, tt.s2)
			if got != tt.want {
				t.Errorf("MinEditDistance(%q, %q) = %d, want %d", tt.s1, tt.s2, got, tt.want)
			}
		})
	}
}
