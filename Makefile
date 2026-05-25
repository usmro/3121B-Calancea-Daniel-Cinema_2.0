# Wrapper Makefile peste CMake
BUILD_DIR := build
BUILD_QT  ?= OFF

.PHONY: all staff tests clean

all: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --parallel

staff: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target cinema --parallel

tests: $(BUILD_DIR)/Makefile
	cmake --build $(BUILD_DIR) --target cinema_tests --parallel
	cd $(BUILD_DIR) && ctest --output-on-failure

$(BUILD_DIR)/Makefile:
	cmake -S . -B $(BUILD_DIR) -DBUILD_QT_UI=$(BUILD_QT)

clean:
	rm -rf $(BUILD_DIR)
