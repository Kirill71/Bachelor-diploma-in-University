.PHONY: all build run clean

QT_BINARIES_PATH := ~/Qt/6.7.2/macos

all: build

build:
	./scripts/build.sh $(QT_BINARIES_PATH)

run:
	./scripts/run.sh

clean:
	./scripts/clean.sh

unit-tests:
	./scripts/run.sh --unit-tests
