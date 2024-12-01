package main

type BQueue struct {
	stack1 BStack
	stack2 BStack
}

func (q *BQueue) push(elem int) {
	q.stack1.push(elem)
}

func (q *BQueue) pop() int {
	if q.stack2.empty() {
		for !q.stack1.empty() {
			elem := q.stack1.pop()
			q.stack2.push(elem)
		}
	}
	return q.stack2.pop()
}
