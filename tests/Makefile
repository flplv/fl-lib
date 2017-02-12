.phony: all

output = fl-lib-tests
cpp_sources = \
    main.cpp \
    test_c_wrapper.cpp \
    test_macros.cpp \
    test_ring_fifo.cpp \
    test_linked_list.cpp \
    test_time.cpp \

c_sources = \
    test_from_c_code.c \

cpp_flags = -I ../ -Wall -std=gnu++14
c_flags = -I ../ -Wall -std=gnu11

cpp_objs = $(patsubst %.cpp,%.o,$(cpp_sources))
c_objs = $(patsubst %.c,%.o,$(c_sources))

all: $(output)

clean:
	rm -f $(output) $(c_objs) $(cpp_objs)

%.o: %.c
	gcc -c $(c_flags) -o $@ $^

%.o: %.cpp
	g++ -c $(cpp_flags) -o $@ $^

$(output): $(c_objs) $(cpp_objs)
	g++ $(ld_flags) -o $(output) $^
