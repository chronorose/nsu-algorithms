package main

import (
	"fmt"
)

func main() {
	fmt.Println("Work in progress.")
	graph := parseTest("kek")
	fmt.Println(naiveTSP(graph))
	bhk(graph)
}
