#! /bin/bash
# This file calls cmake with some sensible default values.
# You can simply overwrite values by providing several arguments:
#   --release    Sets the CMAKE_BUILD_TYPE to Release
#   --debug      Sets the CMAKE_BUILD_TYPE to Debug and generates verbose
#                makefiles
#   --develop    Exports compile commands (e.g. for vim's YCM) and ctags.
#                Builds the dependency graph (build/dependencies.pdf).
#   --examples   Configures the examples to be built.
#   --docs       Configures the documentation to be built.
#   --tagfiles   Configures the documentation to link to external projects.
#   --dry        Only prints the generated CMake command for custom
#                modifications.
#
# You may want to override the environment variable BUILD_DIR to use another
# directory than `build` for the build process.
# Similarly you can override INSTALL_DIR to change the cmake install prefix.
# --develop defaults the INSTALL_DIR to the build dir.

BUILD_DIR=${BUILD_DIR:=build}

for arg in $*; do
    case "$arg" in
        "--release")
            echo "Configuring release build."
            CMAKE_BUILD_TYPE=Release
            ;;
        "--debug")
            echo "Configuring debug build."
            CMAKE_BUILD_TYPE=Debug
            VERBOSE_MAKEFILE=ON
            ;;
        "--develop")
            echo "Adding development tools. Building basic docs."
            DEPENDENCY_GRAPH=1
            EXPORT_COMPILE_COMMANDS=ON
            INSTALL_DIR=${INSTALL_DIR:=${BUILD_DIR}}
            ;;
        "--examples")
            echo "Building examples."
            BUILD_EXAMPLES=ON
            ;;
        "--doc" | "--docs" | "--documentation")
            echo "Building documentation."
            BUILD_DOCS=ON
            EXPORT_COMPILE_COMMANDS=ON
            ;;
        "--tagfiles")
            echo "Linking to external tagfiles."
            DOXYGEN_TAGFILES=ON
            ;;
        "--dry" | "--dryrun" | "--dry-run")
            echo "Just printing command."
            DRYRUN=1
            ;;
        "--caffe")
            echo "Using experimental caffe support."
            WITH_CAFFE=ON
            ;;
    esac
done

# Set default arguments for unset parameters
BUILD_CTAGS=${BUILD_CTAGS:=OFF}
BUILD_DOCS=${BUILD_DOCS:=OFF}
BUILD_EXAMPLES=${BUILD_EXAMPLES:=OFF}
CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
DEPENDENCY_GRAPH=${DEPENDENCY_GRAPH:=0}
DOXYGEN_TAGFILES=${DOXYGEN_TAGFILES:=OFF}
EXPORT_COMPILE_COMMANDS=${EXPORT_COMPILE_COMMANDS:=OFF}
VERBOSE_MAKEFILE=${VERBOSE_MAKEFILE:=OFF}
WITH_CAFFE=${WITH_CAFFE:=OFF}

INSTALL_DIR=${INSTALL_DIR:=/usr/local}

DRYRUN=${DRYRUN:=0}

# Make directory if needed and run cmake
mkdir -p "${BUILD_DIR}"
(
    PROJECT_DIR=$(pwd)
    cd "${BUILD_DIR}"
    command="
    cmake -DBUILD_CTAGS=${BUILD_CTAGS}
          -DBUILD_DOCS=${BUILD_DOCS}
          -DBUILD_EXAMPLES=${BUILD_EXAMPLES}
          -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
          `[[ "${BUILD_DOCS}" == "ON" ]] && echo "-DDOXYGEN_TAGFILES=${DOXYGEN_TAGFILES}"`
          -DCMAKE_EXPORT_COMPILE_COMMANDS=${EXPORT_COMPILE_COMMANDS}
          -DCMAKE_VERBOSE_MAKEFILE=${VERBOSE_MAKEFILE}
          `[[ ${DEPENDENCY_GRAPH} -eq 0 ]] || echo --graphviz=dependencies/dependencies.dot`
          -DWITH_CAFFE=${WITH_CAFFE}
          -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
          ${PROJECT_DIR}
    "
    echo $command
    if [[ ${DRYRUN} -eq 0 ]]; then
        exec $command
        if [[ $? -eq 0 ]]; then
            echo -e "\n\tYou can go into ${BUILD_DIR} and compile and install from there:\n"
            echo -e "\tcd ${BUILD_DIR}\n\tmake -j8\n\tmake install\n"
        else
            echo -e "\n\tFailure!\n"
            return 1
        fi
    fi
)
if [[ ${DEPENDENCY_GRAPH} -ne 0 && -f "${BUILD_DIR}/dependencies/dependencies.dot" && ${DRYRUN} -ne 0 ]]; then
    dot -Tpdf "${BUILD_DIR}/dependencies/dependencies.dot" -o "${BUILD_DIR}/dependencies.pdf"
fi
