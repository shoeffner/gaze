# General settings
# ################

BUILD_DIR := $(abspath build)
PROJECT_NAME := gaze


# OpenCV
# ######

OPENCV_BUILD_DIR := $(BUILD_DIR)/opencv
OPENCV_INSTALL_DIR := $(OPENCV_BUILD_DIR)/install

ENABLED_OPENCV_MODULES = core highgui imgcodecs imgproc videoio objdetect
DISABLED_OPENCV_MODULES = apps calib3d dnn features2d flann imgcodecs ml photo shape stitching superres video videostab
ENABLED_OPENCV_FEATURES =
DISABLED_OPENCV_FEATURES = SHARED_LIBS DOCS PERF_TESTS TESTS EXAMPLES

OPENCV_LIBRARY_FILES = $(addprefix libopencv_,$(addsuffix .a,$(ENABLED_OPENCV_MODULES)))

vpath libopencv_%.a $(OPENCV_INSTALL_DIR)/lib
vpath OpenCVConfig.cmake $(OPENCV_BUILD_DIR)

OPENCV_FLAGS = -DCMAKE_BUILD_TYPE=RELEASE \
 	 			-DCMAKE_INSTALL_PREFIX=$(OPENCV_INSTALL_DIR) \
				$(addsuffix =OFF,$(addprefix -DBUILD_,$(DISABLED_OPENCV_FEATURES)) \
				 			     $(addprefix -DBUILD_opencv_,$(DISABLED_OPENCV_MODULES))) \
				$(addsuffix =ON,$(addprefix -DBUILD_,$(ENABLED_OPENCV_FEATURES)) \
								$(addprefix -DBUILD_opencv_,$(ENABLED_OPENCV_MODULES)))

.PHONY: opencv
opencv: | $(OPENCV_INSTALL_DIR)

$(OPENCV_INSTALL_DIR): OpenCVConfig.cmake
	cd $(OPENCV_BUILD_DIR) && make install

OpenCVConfig.cmake: | $(OPENCV_BUILD_DIR)
	cd $| && cmake $(OPENCV_FLAGS) $(abspath vendor/opencv)


# Project
# #######

PROJECT_BUILD_DIR := $(BUILD_DIR)/$(PROJECT_NAME)
PROJECT_INSTALL_DIR := $(PROJECT_BUILD_DIR)/install

vpath $(PROJECT_NAME)Config.cmake $(PROJECT_BUILD_DIR)

PROJECT_FLAGS = -DPROJECT_NAME=$(PROJECT_NAME) \
				-DOpenCV_DIR=$(OPENCV_BUILD_DIR) \
				-D$(PROJECT_NAME)_INSTALL_PREFIX=$(PROJECT_INSTALL_DIR)

.PHONY: $(PROJECT_NAME)
$(PROJECT_NAME): opencv $(PROJECT_INSTALL_DIR)

$(PROJECT_INSTALL_DIR): $(PROJECT_NAME)Config.cmake
	cd $(PROJECT_BUILD_DIR) && make install

$(PROJECT_NAME)Config.cmake: | $(PROJECT_BUILD_DIR)
	cd $| && cmake $(PROJECT_FLAGS) $(CURDIR)


.DEFAULT_GOAL = run
.PHONY: run
run: $(PROJECT_NAME) $(PROJECT_INSTALL_DIR)/$(PROJECT_NAME)
	cd $(PROJECT_INSTALL_DIR) && ./$<

# Helpers
# #######

$(BUILD_DIR)/%:
	@mkdir -p $(BUILD_DIR)/$*

clean:
	rm $(PROJECT_INSTALL_DIR)/$(PROJECT_NAME)

.SECONDEXPANSION:
clean%:
	rm -rf $(BUILD_DIR)/$*
