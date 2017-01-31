.phony: all

output =../fl-lib.h
headers = \
    includes.h \
	ring_fifo.h

processed_headers = $(foreach f, $(headers), $(f) .tmp_file )

all: $(output)

clean:
	@rm -f $(output)

.tmp_file:
	@echo "" > .tmp_file

$(output): $(processed_headers)
	@echo "#ifndef _FL_LIB_H_" > $(output)
	@echo "#define _FL_LIB_H_" >> $(output)
	@echo "" >> $(output)
	@cat $(processed_headers) >> $(output)
	@echo "#endif /* _FL_LIB_H_ */" >> $(output)
	@rm .tmp_file
	@echo "Generated $(output)."
