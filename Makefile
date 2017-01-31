.phony: all

all:
	make -C headers all
	make -C tests all
	./tests/fl-lib-tests
