package main

import (
	"fmt"
	"math"
)

func prep(length int) ([][]int, [][]int) {
	y := int(math.Pow(2, float64(length)))
	A := make([][]int, length)
	for i := 0; i < length; i++ {
		A[i] = make([]int, y)
		for j := 0; j < y; j++ {
			A[i][j] = 10000
		}
	}
	S := make([][]int, length+1)
	for i := 2; i <= length; i++ {
		S[i] = bitSubsetStarter(length, i)
	}
	A[0][1] = 0
	return S, A
}

func bhk(graph [][]int) {
	length := len(graph)
	S, A := prep(length)
	for i := 2; i < length+1; i++ {
		curS := S[i]
		for j := 0; j < len(curS); j++ {
			curV := curS[j]
			// fmt.Print(curV, " ")
			ans := 10001
			for k := 1; k < length; k++ {
				if curV&(1<<k) == 0 {
					continue
				}
				SmV := curV & (^(1 << k))
				for h := 0; h < length; h++ {
					if (curV&(1<<h) == 0) || h == k {
						continue
					}
					fmt.Println("SmV:", SmV, "graph:", graph[k][h], "k:", k, "h:", h, "A[h][SmV]:", A[h][SmV])
					ans = min(A[h][SmV]+graph[k][h], ans)
				}
				A[k][curV] = ans
				fmt.Println("k:", k, "curV:", curV, "ans:", ans)
				fmt.Println()
			}
		}
	}
	ans := 100000
	l := (1 << length) - 1
	y := int(math.Pow(2, float64(length)))
	for i := 0; i < length; i++ {
		for j := 0; j < y; j++ {
			fmt.Print(A[i][j], " ")
		}
		fmt.Println()
	}
	for i := 2; i < length; i++ {
		ans = min(ans, A[i][l]+graph[i][0])
	}
	fmt.Println(ans)
}
