CC ?= cc
CFLAGS ?= -O2 -march=native -pipe
COMMONFLAGS := -std=c99 $\
							 -Wall -Wextra -Wpedantic $\
							 -I.

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

TEST_REQUIREMENTS := ${OBJECT_FILES}

define COMPILE
${CC} -c $(1) ${CFLAGS} ${COMMONFLAGS} -o $(2)

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

all: test

test: ${TEST_REQUIREMENTS}
	${CC} $< ${CFLAGS} ${COMMONFLAGS} -o $@

build/%.o: src/%.c
	$(call COMPILE,$<,$@)
%.gch: %
	$(call COMPILE,$<,$@)
%.pch: %
	$(call COMPILE,$<,$@)

clean:
	$(call REMOVE_LIST,${TEST_REQUIREMENTS})
	$(call REMOVE,libcarp.a)

.PHONY: all clean install uninstall
