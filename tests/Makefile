ifeq (1,${V})
	SILENT =
else
	SILENT = @
endif

PROJECT = fl-lib-tests

CFLAGS= -I ../ -Wall -std=gnu11
CPPFLAGS= -I ../ -Wall -std=gnu++14

LDFLAGS=

OBJDIR = objs
SRCFILES = \
    main.cpp \
    test_c_wrapper.cpp \
    test_macros.cpp \
    test_ring_fifo.cpp \
    test_linked_list.cpp \
    test_time.cpp \
    test_from_c_code.c

SRCDIRS =

get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
SRCS += $(SRCFILES) $(call get_src_from_dir_list, $(SRCDIRS))

__src_to = $(subst .c,$1, $(subst .cpp,$1,$2))
src_to = $(addprefix $(OBJDIR)/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1)
OBJS = $(call src_to_o,$(SRCS))

DEPS := $(OBJS:.o=.d)
-include $(DEPS)

all: $(PROJECT) run

run:
	./$(PROJECT)

$(PROJECT): buildrepo $(OBJS)
	@echo Linking $(PROJECT)
	$(SILENT)$(CXX) $(OBJS) $(LDFLAGS) -o $@
	@chmod +x $(PROJECT)

$(OBJDIR)/%.o: %.cpp
	@echo Compiling $(notdir $<)
	$(SILENT)$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

$(OBJDIR)/%.o: %.c
	@echo Compiling $(notdir $<)
	$(SILENT)$(CC) $(CFLAGS) -MMD -c $< -o $@

clean:
	rm $(PROJECT) $(OBJDIR) -Rf

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
