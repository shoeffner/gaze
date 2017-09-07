# General settings
# ################

BUILD_DIR := $(abspath build)
PROJECT_NAME := gaze

# OpenCV
# ######

OPENCV_DIR = $(abspath vendor/opencv)
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

OPENCV_PROJECT_FLAGS = -DOpenCV_DIR=$(OPENCV_BUILD_DIR)

# .PHONY: opencv
# opencv: | $(OPENCV_INSTALL_DIR)

$(OPENCV_INSTALL_DIR): OpenCVConfig.cmake
	cd $(OPENCV_BUILD_DIR) && make install

OpenCVConfig.cmake: | $(OPENCV_BUILD_DIR)
	cd $| && cmake $(OPENCV_FLAGS) $(OPENCV_DIR)


# Project
# #######
.PHONY: $(PROJECT_NAME)
$(PROJECT_NAME): CMakeCache.txt
	cd $(BUILD_DIR) && make install

.DEFAULT_GOAL = run
.PHONY: run
run: $(PROJECT_NAME)
	cd $(BUILD_DIR) && ./$<

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

vpath CMakeCache.txt $(BUILD_DIR)
CMakeCache.txt: | $(BUILD_DIR)
	cd $| && cmake ..

clean:
	cd $(BUILD_DIR) && make clean

clean%:
	rm -rf $(BUILD_DIR)/$*
