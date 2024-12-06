package main

type PQueueVersion struct {
	recopy bool
	copied bool
	toCopy int
	R      int
	S      int
	// the fact that it has multiple return values BUT NO TUPLE TYPE holy shit
	L     [2]int
	Rc    [2]int
	LSwap bool
	RSwap bool
}

type PQueue struct {
	Versions []PQueueVersion
	L        PStack
	Lp       PStack
	R        PStack
	S        PStack
	Rc       PStack
	Rcp      PStack
}

func newPQueue() PQueue {
	versions := make([]PQueueVersion, 0)
	versions = append(versions, newPQVersion())
	return PQueue{
		versions,
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
		newPStack(),
	}
}

func newPQVersion() PQueueVersion {
	return PQueueVersion{
		false,
		false,
		0,
		0,
		0,
		[2]int{0, 0},
		[2]int{0, 0},
		true,
		true,
	}
}

// it is highly unobvious semantic(at least to me) that it actually copies it
// UNLESS there is anything with references inside. quite shite and not intuitive(from user perspective) tbh
// and it seems [2]int IS NOT a reference
func copyPQVersion(old PQueueVersion) PQueueVersion {
	return old
}

func (pq *PQueue) getPQVersion(version int) PQueueVersion {
	return pq.Versions[version]
}

func getTupleElem(tuple [2]int, which_one bool) int {
	if which_one {
		return tuple[0]
	}
	return tuple[1]
}

func setTupleElem(tuple [2]int, which_one bool, element int) [2]int {
	if which_one {
		tuple[0] = element
	} else {
		tuple[1] = element
	}
	return tuple
}

func (pq *PQueue) getL(is_l bool) PStack {
	if is_l {
		return pq.L
	}
	return pq.Lp
}

func (pq *PQueue) setL(is_l bool, new_l PStack) {
	if is_l {
		pq.L = new_l
	} else {
		pq.Lp = new_l
	}
}

func (pq *PQueue) getRc(is_rc bool) PStack {
	if is_rc {
		return pq.Rc
	}
	return pq.Rcp
}

func (pq *PQueue) setRc(is_rc bool, new_rc PStack) {
	if is_rc {
		pq.Rc = new_rc
	} else {
		pq.Rcp = new_rc
	}
}

func (pq *PQueue) push(version, element int) int {
	old_version := pq.getPQVersion(version)
	new_version := copyPQVersion(old_version)
	if !new_version.recopy {

		Lstack := pq.getL(new_version.LSwap)
		Lversion := getTupleElem(new_version.L, new_version.LSwap)
		Lversion = Lstack.push(element, Lversion)
		new_version.L = setTupleElem(new_version.L, new_version.LSwap, Lversion)
		pq.setL(new_version.LSwap, Lstack)

		curRcp := pq.getRc(!new_version.RSwap)
		RcpVersion := getTupleElem(new_version.Rc, !new_version.RSwap)
		if !curRcp.empty(RcpVersion) {
			_, newRcpVersion, _ := curRcp.pop(RcpVersion)
			new_version.Rc = setTupleElem(new_version.Rc, !new_version.RSwap, newRcpVersion)
		}
		pq.setRc(!new_version.RSwap, curRcp)
		new_version = pq.checkRecopy(new_version)
	} else {
		LpStack := pq.getL(!new_version.LSwap)
		LpVersion := getTupleElem(new_version.L, !new_version.LSwap)
		LpVersion = LpStack.push(element, LpVersion)
		new_version.L = setTupleElem(new_version.L, !new_version.LSwap, LpVersion)
		pq.setL(!new_version.LSwap, LpStack)
		new_version = pq.checkNormal(new_version)
	}
	pq.Versions = append(pq.Versions, new_version)
	return len(pq.Versions) - 1
}

func (pq *PQueue) pop(version int) (int, int) {
	old_version := pq.getPQVersion(version)
	new_version := copyPQVersion(old_version)
	if !new_version.recopy {
		ret, newRVersion, _ := pq.R.pop(new_version.R)
		new_version.R = newRVersion

		curRc := pq.getRc(new_version.RSwap)
		RcVersion := getTupleElem(new_version.Rc, new_version.RSwap)
		_, RcVersion, _ = curRc.pop(RcVersion)
		new_version.Rc = setTupleElem(new_version.Rc, new_version.RSwap, RcVersion)
		pq.setRc(new_version.RSwap, curRc)

		curRcp := pq.getRc(!new_version.RSwap)
		RcpVersion := getTupleElem(new_version.Rc, !new_version.RSwap)

		if !curRcp.empty(RcpVersion) {
			_, RcpVersion, _ = curRcp.pop(RcpVersion)
		}

		new_version.Rc = setTupleElem(new_version.Rc, !new_version.RSwap, RcpVersion)
		pq.setRc(!new_version.RSwap, curRcp)

		new_version = pq.checkRecopy(new_version)
		pq.Versions = append(pq.Versions, new_version)
		return ret, len(pq.Versions) - 1
	}

	curRc := pq.getRc(new_version.RSwap)
	RcVersion := getTupleElem(new_version.Rc, new_version.RSwap)
	ret, newRcVersion, _ := curRc.pop(RcVersion)
	RcVersion = newRcVersion
	new_version.Rc = setTupleElem(new_version.Rc, new_version.RSwap, RcVersion)
	pq.setRc(new_version.RSwap, curRc)

	if new_version.toCopy > 0 {
		new_version.toCopy--
	} else {
		_, new_version.R, _ = pq.R.pop(new_version.R)

		curRcp := pq.getRc(!new_version.RSwap)
		RcpVersion := getTupleElem(new_version.Rc, !new_version.RSwap)
		_, RcpVersion, _ = curRcp.pop(RcpVersion)
		new_version.Rc = setTupleElem(new_version.Rc, !new_version.RSwap, RcpVersion)
		pq.setRc(!new_version.RSwap, curRcp)
	}
	new_version = pq.checkNormal(new_version)

	pq.Versions = append(pq.Versions, new_version)
	return ret, len(pq.Versions) - 1
}

func (pq *PQueue) checkRecopy(current_version PQueueVersion) PQueueVersion {
	curL := pq.getL(current_version.LSwap)
	curLVersion := getTupleElem(current_version.L, current_version.LSwap)
	LLen := curL.size(curLVersion)

	RLen := pq.R.size(current_version.R)
	current_version.recopy = LLen > RLen

	if current_version.recopy {
		current_version.toCopy = RLen
		current_version.copied = false
		current_version = pq.checkNormal(current_version)
	}

	return current_version
}

func (pq *PQueue) checkNormal(current_version PQueueVersion) PQueueVersion {
	current_version = pq.pqOps(current_version)
	current_version.recopy = !pq.S.empty(current_version.S)
	return current_version
}

func (pq *PQueue) pqOps(current_version PQueueVersion) PQueueVersion {
	toDo := 3
	for !current_version.copied && toDo > 0 && !pq.R.empty(current_version.R) {
		el, newRVersion, _ := pq.R.pop(current_version.R)
		current_version.R = newRVersion

		current_version.S = pq.S.push(el, current_version.S)
		toDo--
	}

	curL := pq.getL(current_version.LSwap)
	LVersion := getTupleElem(current_version.L, current_version.LSwap)

	curRcp := pq.getRc(!current_version.RSwap)
	RcpVersion := getTupleElem(current_version.Rc, !current_version.RSwap)
	for toDo > 0 && !curL.empty(LVersion) {
		current_version.copied = true

		elem, newLVersion, _ := curL.pop(LVersion)
		LVersion = newLVersion

		current_version.R = pq.R.push(elem, current_version.R)

		RcpVersion = curRcp.push(elem, RcpVersion)
	}

	current_version.L = setTupleElem(current_version.L, current_version.LSwap, LVersion)
	pq.setL(current_version.LSwap, curL)

	for toDo > 0 && !pq.S.empty(current_version.S) {
		elem, newSVersion, _ := pq.S.pop(current_version.S)
		current_version.S = newSVersion
		if current_version.toCopy > 0 {
			current_version.R = pq.R.push(elem, current_version.R)
			RcpVersion = curRcp.push(elem, RcpVersion)
			current_version.toCopy--
		}
		toDo--
	}
	current_version.Rc = setTupleElem(current_version.Rc, !current_version.RSwap, RcpVersion)
	pq.setRc(!current_version.RSwap, curRcp)

	if pq.S.empty(current_version.S) {
		current_version.RSwap = !current_version.RSwap
		current_version.LSwap = !current_version.LSwap
	}
	return current_version
}
