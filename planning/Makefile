
all: clean test

clean:
		@rm -f $(PASS)
		rm -f $(ACT) $(EXE)

IN = $(wildcard tests/*-input.txt)
ACT = $(IN:-input.txt=-actual.txt)
PASS = $(IN:-input.txt=.passed)

test: $(PASS) 
	@echo "All tests passed"


$(PASS): %.passed: %-input.txt %-expected.txt
	@echo "Running test $*..."
	@rm -f $@
	scala main.scala -- $*-input.txt > $*-actual.txt
	diff $*-actual.txt $*-expected.txt -Z
	@touch $@
