package main

import "fmt"

func main() {
	stack := newPStack()
	for i := 0; i < 5; i++ {
		stack.push(i, i)
		fmt.Println(stack)
	}
	// rtq := makeRTQueue()
	// for i := 0; i < 100; i++ {
	// 	rtq.push(i)
	// }
	// for i := 0; i < 30; i++ {
	// 	fmt.Println(rtq.pop())
	// }
	// for i := 0; i < 100; i++ {
	// 	rtq.push(i)
	// }
	// for i := 0; i < 30; i++ {
	// 	fmt.Println(rtq.pop())
	// }
	// for i := 0; i < 100; i++ {
	// 	rtq.push(i)
	// }
	// for i := 0; i < 30; i++ {
	// 	fmt.Println(rtq.pop())
	// }
	// for i := 0; i < 30; i++ {
	// 	fmt.Println(rtq.pop())
	// }
}
