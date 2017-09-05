# General settings
# ################

BUILD_DIR := $(abspath build)
PROJECT_NAME := gaze

# subst tricks (see https://stackoverflow.com/a/7531247/3004221)
_NULL :=
_SPACE := $(_NULL) #
_COMMA := ,


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

.PHONY: opencv
opencv: | $(OPENCV_INSTALL_DIR)

$(OPENCV_INSTALL_DIR): OpenCVConfig.cmake
	cd $(OPENCV_BUILD_DIR) && make install

OpenCVConfig.cmake: | $(OPENCV_BUILD_DIR)
	cd $| && cmake $(OPENCV_FLAGS) $(OPENCV_DIR)


# Boost
# #####

BOOST_DIR = $(abspath vendor/boost)
BOOST_VERSION = 1.65.0
BOOST_BUILD_DIR = $(BUILD_DIR)/boost
BOOST_INCLUDE_DIR = $(BOOST_BUILD_DIR)/include
BOOST_LIBRARY_DIR = $(BOOST_BUILD_DIR)/lib

# Comma separated list
BOOST_LIBRARIES = program_options
WITH_OR_WITHOUT_BOOST_LIBRARIES = with

BOOST_PROJECT_FLAGS = -DBOOST_ROOT=$(BOOST_BUILD_DIR) \
                      -DBOOST_LIBRARYDIR=$(BOOST_LIBRARY_DIR) \
                      -DBOOST_INCLUDEDIR=$(BOOST_INCLUDE_DIR) \
                      -DBOOST_VERSION=$(BOOST_VERSION) \
                      -DBoost_NO_BOOST_CMAKE=TRUE \
                      -DBoost_SYSTEM_PATHS=TRUE \
                      -DBoost_DEBUG=1 \
                      -DBOOST_COMPONENTS="$(BOOST_LIBRARIES)"

vpath version.hpp $(BOOST_INCLUDE_DIR)/boost

.PHONY: boost
boost: | version.hpp

version.hpp:
	cd $(BOOST_DIR) \
	&& git fetch --tags \
	&& git checkout tags/boost-$(BOOST_VERSION) \
	&& git submodule update --init --recursive \
	&& ./bootstrap.sh --prefix=$(BOOST_BUILD_DIR) \
	                  --libdir=$(BOOST_LIBRARY_DIR) \
	                  --includedir=$(BOOST_INCLUDE_DIR) \
	                  --$(WITH_OR_WITHOUT_BOOST_LIBRARIES)-libraries=$(subst $(_SPACE),$(_COMMA),$(BOOST_LIBRARIES)) \
	&& ./b2 install


# Project
# #######

PROJECT_BUILD_DIR := $(BUILD_DIR)/$(PROJECT_NAME)
PROJECT_INSTALL_DIR := $(PROJECT_BUILD_DIR)/install

vpath $(PROJECT_NAME)Config.cmake $(PROJECT_BUILD_DIR)

PROJECT_FLAGS = -DPROJECT_NAME=$(PROJECT_NAME) \
                $(OPENCV_PROJECT_FLAGS) \
                $(BOOST_PROJECT_FLAGS) \
                -D$(PROJECT_NAME)_INSTALL_PREFIX=$(PROJECT_INSTALL_DIR)

.PHONY: $(PROJECT_NAME)
$(PROJECT_NAME): opencv boost $(PROJECT_INSTALL_DIR)

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
