.DEFAULT_GOAL := all

.PHONY: all credential-bundle

all: transform
ifeq ($(shell uname -s),Darwin)
all: credential-bundle
endif

transform: transform.c
	$(CC) -std=c11 -O2 -o $@ $<

credential-bundle:
	@$(MAKE) --no-print-directory -C credential-retrieval
