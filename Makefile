all: build test

build:
	@cc -o wordcount wordcount.c

test:
	@$$0 ./test.sh

