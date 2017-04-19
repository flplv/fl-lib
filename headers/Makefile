.phony: all

output_h =../fl-lib.h
output_hpp =../fl-lib.hpp
headers = \
    includes.h \
    macro_typeof.hXX \
    macro_auto_type.hXX \
    macro_offsetof.hXX \
    macro_container_of.hXX \
    macro_build_bug_on_zero.h \
    macro_gnuc_prereq.h \
    macro_sizeof.h \
    macro_array_size.hXX \
    ring_fifo.h \
    linked_list.h \
    time.h \

processed_headers_h = $(subst .hXX,.h,$(foreach f, $(headers), $(f) .tmp_file ))
processed_headers_hpp = $(subst .hXX,.hpp,$(foreach f, $(headers), $(f) .tmp_file ))

all: $(output_h) $(output_hpp)

prepare:
	mkdir -f objs

clean: rm_tmp_file

.tmp_file:
	@echo "" > .tmp_file

rm_tmp_file:
	@rm .tmp_file

$(output_h): $(processed_headers_h)
	@echo "#ifndef _FL_LIB_H_" > $@
	@echo "#define _FL_LIB_H_" >> $@
	@echo "" >> $@
	@cat $(processed_headers_h) >> $@
	@echo "#endif /* _FL_LIB_H_ */" >> $@
	@echo "Generated $@."

$(output_hpp): $(processed_headers_hpp)
	@echo "#ifndef _FL_LIB_HPP_" > $@
	@echo "#define _FL_LIB_HPP_" >> $@
	@echo "" >> $@
	@cat $(processed_headers_hpp) >> $@
	@echo "#endif /* _FL_LIB_HPP_ */" >> $@
	@echo "Generated $@."
