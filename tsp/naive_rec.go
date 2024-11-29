package main

func shortestHamCycle(visited, current, length int, paths [][]int) int {
	if visited == (1<<length)-1 {
		return paths[current][0]
	}

	ret := int(^uint(0) >> 1)

	for i := 0; i < length; i++ {
		if visited&(1<<i) == 0 {
			ret = min(ret, paths[current][i]+shortestHamCycle(visited|(1<<i), i, length, paths))
		}
	}
	return ret
}

func starter(length int, paths [][]int) int {
	ans := int(^uint(0) >> 1)
	for i := 0; i < length; i++ {
		ans = min(ans, shortestHamCycle((1<<i), i, length, paths))
	}
	return ans
}
