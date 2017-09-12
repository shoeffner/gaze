BUILD_DIR ?= $(abspath build)
INSTALL_DIR ?= $(BUILD_DIR)/install

# Pick up variables from Environment for cmake
CMAKE_VARS := -DBoost_VERSION=$(DBoost_VERSION) \
              -DOpenCV_VERSION=$(DOpenCV_VERSION) \
              -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR)

PROJECT_NAME = gaze

vpath CMakeCache.txt $(BUILD_DIR)
vpath $(PROJECT_NAME) $(INSTALL_DIR)

$(PROJECT_NAME): CMakeCache.txt
	@cd $(BUILD_DIR) && make install

.PHONY: run
run: $(PROJECT_NAME)
	cd $(INSTALL_DIR) && ./$(PROJECT_NAME)

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

CMakeCache.txt: | $(BUILD_DIR)
	@cd $| && cmake $(CMAKE_VARS) $(CURDIR)

clean:
	@cd $(BUILD_DIR) && make clean

clean%:
	@rm -rf $(BUILD_DIR)/$*

remove:
	@rm -rf $(BUILD_DIR)
