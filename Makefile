.PHONY: all configure build clean run

BUILD_DIR = build

all: build


resources:
	mkdir -p $(BUILD_DIR)


configure: resources
	cmake -B $(BUILD_DIR)


build: configure
	cmake --build $(BUILD_DIR)


clean:
	rm -rf $(BUILD_DIR)


consumer: build
	./$(BUILD_DIR)/consumer_app --pipe my_pipe


producer: build
	./$(BUILD_DIR)/producer_app --pipe my_pipe --size 8192


mypipe:
	mkfifo my_pipe
