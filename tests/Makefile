.phony: all

output = fl-lib-tests
cpp_sources = \
    main.cpp \
    test_ring_fifo.cpp \

c_sources = \
#    test_c.c

cpp_flags = -I ../ -Wall
c_flags = -I ../ -Wall

cpp_objs = $(patsubst %.cpp,%.o,$(cpp_sources))
c_objs = $(patsubst %.c,%.o,$(c_sources))

all: $(output)

clean:
	rm -f $(output) $(c_objs) $(cpp_objs)

%.o: %.c
	gcc -c $(c_flags) -o $@ $^

%.o: %.cpp
	g++ -c $(c_flags) -o $@ $^

$(output): $(c_objs) $(cpp_objs)
	g++ $(ld_flags) -o $(output) $^
