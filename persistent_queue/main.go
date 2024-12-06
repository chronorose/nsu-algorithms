package main

import "fmt"

func main() {
	pqueue := newPQueue()
	index := 0
	for i := 0; i < 100; i++ {
		index = pqueue.push(i, i)
	}
	for i := 0; i < 30; i++ {
		el, new_index := pqueue.pop(index)
		index = new_index
		fmt.Println(el)
	}
	for i := 0; i < 100; i++ {
		index = pqueue.push(index, i)
	}
	for i := 0; i < 30; i++ {
		el, new_index := pqueue.pop(index)
		index = new_index
		fmt.Println(el)
	}
}
