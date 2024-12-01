package main

type PQueue struct {
	recopy bool
	copied bool
	toCopy int
	L      PStack
	Lp     PStack
	R      PStack
	S      PStack
	Rc     PStack
	Rcp    PStack
}

func makePQueue() PQueue {
	return PQueue{
		false,
		false,
		0,
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
	}
}

func (pq *PQueue) empty() bool {
	return !pq.recopy && pq.R.empty(0) // 0 is holder
}

func (rtq *RTQueue) push(elem int) {
	if !rtq.recopy {
		rtq.L.push(elem)
		if !rtq.Rcp.empty() {
			rtq.Rcp.pop()
		}
		rtq.checkRecopy()
	} else {
		rtq.Lp.push(elem)
		rtq.checkNormal()
	}
}

func (rtq *RTQueue) pop() int {
	if !rtq.recopy {
		t := rtq.R.pop()
		rtq.Rc.pop()
		if !rtq.Rcp.empty() {
			rtq.Rcp.pop()
		}
		rtq.checkRecopy()
		return t
	}
	t := rtq.Rc.pop()
	if rtq.toCopy > 0 {
		rtq.toCopy--
	} else {
		rtq.R.pop()
		rtq.Rcp.pop()
	}
	rtq.checkNormal()
	return t
}

func (rtq *RTQueue) checkRecopy() {
	rtq.recopy = len(rtq.L.elems) > len(rtq.R.elems)
	if rtq.recopy {
		rtq.toCopy = len(rtq.R.elems)
		rtq.copied = false
		rtq.checkNormal()
	}
}

func (rtq *RTQueue) checkNormal() {
	rtq.rtOps()
	rtq.recopy = !rtq.S.empty()
}

func (rtq *RTQueue) rtOps() {
	toDo := 3
	for !rtq.copied && toDo > 0 && !rtq.R.empty() {
		rtq.S.push(rtq.R.pop())
		toDo--
	}
	for toDo > 0 && !rtq.L.empty() {
		rtq.copied = true
		elem := rtq.L.pop()
		rtq.R.push(elem)
		rtq.Rcp.push(elem)
		toDo--
	}
	for toDo > 0 && !rtq.S.empty() {
		elem := rtq.S.pop()
		if rtq.toCopy > 0 {
			rtq.R.push(elem)
			rtq.Rcp.push(elem)
			rtq.toCopy--
		}
		toDo--
	}
	if rtq.S.empty() {
		temp := rtq.L
		rtq.L = rtq.Lp
		rtq.Lp = temp
		temp = rtq.Rc
		rtq.Rc = rtq.Rcp
		rtq.Rcp = temp
	}
}
