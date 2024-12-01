package main

type BStack struct {
	elems []int
}

func (stack *BStack) push(elem int) {
	stack.elems = append(stack.elems, elem)
}

func (stack *BStack) pop() int {
	ret := stack.elems[len(stack.elems)-1]
	stack.elems = stack.elems[:len(stack.elems)-1]
	return ret
}

func (stack *BStack) empty() bool {
	if len(stack.elems) == 0 {
		return true
	}
	return false
}

func newBStack() BStack {
	return BStack{make([]int, 0)}
}
