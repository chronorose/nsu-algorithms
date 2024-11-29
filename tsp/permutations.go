package main

import "fmt"

func swap(arr []int, i, j int) {
	t := arr[i]
	arr[i] = arr[j]
	arr[j] = t
}

func permsAux(res *[][]int, current int, arr []int) {
	if current == len(arr) {
		copyArr := make([]int, len(arr))
		copy(copyArr, arr)
		*res = append(*res, copyArr)
		return
	}

	for i := current; i < len(arr); i++ {
		swap(arr, current, i)
		permsAux(res, current+1, arr)
		swap(arr, current, i)
	}
}

func permutations(arr []int) [][]int {
	res := make([][]int, 0)
	permsAux(&res, 0, arr)
	return res
}

func subsets(arr []int, size int) {
	res := make([][]int, 0)
	current := make([]int, 0)
	subset(&res, arr, current, len(arr), size, 0)
	fmt.Println(res)
}

func subset(res *[][]int, arr, currentArr []int, n, k, current int) {
	if n < k {
		return
	}
	if k == 0 {
		copyArr := make([]int, len(currentArr))
		copy(copyArr, currentArr)
		*res = append(*res, copyArr)
		return
	}
	subset(res, arr, currentArr, n-1, k, current+1)
	currentArr = append(currentArr, arr[current])
	subset(res, arr, currentArr, n-1, k-1, current+1)
	currentArr = currentArr[:len(currentArr)-1]
}

func bitSubsetStarter(length, size int) []int {
	res := make([]int, 0)
	bitSubset(&res, 1, length-1, size-1, 1)
	return res
}

func bitSubset(res *[]int, current, n, k, index int) {
	if n < k {
		return
	}
	if k == 0 {
		*res = append(*res, current)
		return
	}
	bitSubset(res, current, n-1, k, index+1)
	current = current | (1 << index)
	bitSubset(res, current, n-1, k-1, index+1)
}
