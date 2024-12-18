CC ?= cc
CFLAGS ?= -O2 -march=native -pipe -Wall -Wextra -Wpedantic
override CFLAGS += -std=c99
override CFLAGS += -Iinclude

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

LIBCARP_REQUIREMENTS := ${OBJECT_FILES}

define COMPILE
${CC} -c $(1) ${CFLAGS} -o $(2)

endef
define REMOVE
$(if $(wildcard $(1)),$\
	rm $(1))

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))
endef

all: libcarp.a

libcarp.a: ${LIBCARP_REQUIREMENTS}
	ar rcs $@ ${OBJECT_FILES}

build/%.o: src/%.c
	$(call COMPILE,$<,$@)
%.gch: %
	$(call COMPILE,$<,$@)
%.pch: %
	$(call COMPILE,$<,$@)

clean:
	$(call REMOVE_LIST,${LIBCARP_REQUIREMENTS})
	$(call REMOVE,libcarp.a)

.PHONY: all clean install uninstall
