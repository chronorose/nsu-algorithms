package main

import "errors"

type PSNode struct {
	version int
	value   int
	prev    int
	size    int
	none    bool // ideally you would keep separate bitset because of alignment.
}

type PStack struct {
	stack []PSNode
}

func newPStack() PStack {
	psn := PSNode{
		0,
		0,
		0,
		0,
		true,
	}
	stack := append(make([]PSNode, 0), psn)
	return PStack{stack}
}

func (stack *PStack) push(elem, version int) {
	prev_node := stack.stack[version]
	node := PSNode{
		len(stack.stack),
		elem,
		version,
		prev_node.size + 1,
		false,
	}
	stack.stack = append(stack.stack, node)
}

func (stack *PStack) pop(version int) (int, error) {
	node := stack.stack[version]
	if node.none {
		return -1, errors.New("empty stack")
	}
	prev := stack.stack[node.prev]
	new_head := PSNode{
		len(stack.stack),
		prev.value,
		prev.prev,
		prev.size,
		prev.none,
	}
	stack.stack = append(stack.stack, new_head)
	return node.value, nil
}

func (stack *PStack) size(version int) int {
	return stack.stack[version].size
}

func (stack *PStack) empty(version int) bool {
	return stack.stack[version].none
}
