package main

import (
	"os"
	"strconv"
	"strings"
)

func atoiLine(str []string) (int, int, int) {
	f, _ := strconv.Atoi(str[0])
	g, _ := strconv.Atoi(str[1])
	h, _ := strconv.Atoi(str[2])
	return f, g, h
}

func parseLine(line string, graph *[][]int) {
	args := strings.Split(line, " ")
	from, to, weight := atoiLine(args)
	(*graph)[from][to] = weight
	(*graph)[to][from] = weight
}

func make2dArray(n, m int) [][]int {
	arr := make([][]int, n)
	for i := 0; i < n; i++ {
		arr[i] = make([]int, m)
	}
	return arr
}

func parseTest(path string) [][]int {
	file, _ := os.ReadFile(path)
	strFile := string(file)
	lines := strings.Split(strFile, "\n")

	firstLine := strings.Split(lines[0], " ")
	n, m, edges := atoiLine(firstLine)

	graph := make2dArray(n, m)

	for i := 0; i < edges; i++ {
		parseLine(lines[i+1], &graph)
	}
	return graph
}
