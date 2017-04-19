.phony: all

all:
	make -C headers all
	make -C tests all

clean:
	make -C headers clean
	make -C tests clean
