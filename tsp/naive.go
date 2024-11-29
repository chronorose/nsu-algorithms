package main

func sumPerm(arr []int, graph [][]int) int {
	ret := 0
	for i := 0; i < len(arr)-1; i++ {
		ret += graph[arr[i]][arr[i+1]]
	}
	ret += graph[arr[len(arr)-1]][arr[0]]
	return ret
}

func naiveTSP(graph [][]int) int {
	length := len(graph)
	forPerms := make([]int, length)
	for i := 0; i < length; i++ {
		forPerms[i] = i
	}
	perms := permutations(forPerms)
	ans := int(^uint(0) >> 1)
	for i := 0; i < len(perms); i++ {
		ans = min(ans, sumPerm(perms[i], graph))
	}
	return ans
}
