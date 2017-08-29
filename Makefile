# General settings
# ################

BUILD_DIR := $(abspath build)
PROGRAM_NAME := gaze


# OpenCV
# ######

OPENCV_BUILD_DIR := $(BUILD_DIR)/opencv
OPENCV_INSTALL_DIR := $(OPENCV_BUILD_DIR)/install

ENABLED_OPENCV_MODULES = core highgui imgcodecs imgproc videoio
DISABLED_OPENCV_MODULES = apps calib3d dnn features2d flann imgcodecs ml objdetect photo shape stitching superres video videostab
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
	cd $(abspath $|) && cmake $(OPENCV_FLAGS) $(abspath vendor/opencv)


# Program
# #######

LIBRARY_DIRS = $(OPENCV_INSTALL_DIR)/lib
INCLUDE_DIRS = $(OPENCV_INSTALL_DIR)/include
LIBRARY_FILES = $(addprefix opencv_,$(ENABLED_OPENCV_MODULES))

vpath $(PROGRAM_NAME) $(BUILD_DIR)

.DEFAULT_GOAL = run
.PHONY: run
run: $(PROGRAM_NAME)
	$(BUILD_DIR)/$^

$(PROGRAM_NAME): opencv
	g++ $(PROGRAM_NAME).cpp \
		$(addprefix -l,$(LIBRARY_FILES)) \
		$(addprefix -I ,$(INCLUDE_DIRS)) \
		$(addprefix -L ,$(LIBRARY_DIRS)) \
		-o $(BUILD_DIR)/$@


# Helpers
# #######

$(BUILD_DIR)/%:
	@mkdir -p $(BUILD_DIR)/$*

