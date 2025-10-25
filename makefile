SRC_FILES := $(wildcard src/*.c)
BIN_FILES := $(patsubst src/%.c, bin/%, $(SRC_FILES))

build: $(BIN_FILES)

bin/%: src/%.c
	@mkdir -p bin
	cc $< -o $@

clean:
	rm -f bin/*

