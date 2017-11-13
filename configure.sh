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
#
# You may want to override the environment variable BUILD_DIR to use another
# directory than `build` for the build process.

BUILD_DIR=${BUILD_DIR:=$(pwd)/build}

for arg in $*; do
    case "$arg" in
        "--release")
            echo "Configuring release build."
            BUILD_TYPE=Release
            ;;
        "--debug")
            echo "Configuring debug build."
            BUILD_TYPE=Debug
            VERBOSE_MAKEFILE=ON
            ;;
        "--develop")
            echo "Adding development tools. Building basic docs."
            BUILD_CTAGS=ON
            EXPORT_COMPILE_COMMANDS=ON
            DEPENDENCY_GRAPH=1
            ;;
        "--examples")
            echo "Building examples."
            BUILD_EXAMPLES=ON
            ;;
        "--doc" | "--docs" | "--documentation")
            echo "Building documentation."
            BUILD_DOCS=ON
            ;;
        "--tagfiles")
            echo "Linking to external tagfiles."
            DOXYGEN_TAGFILES=ON
            ;;
    esac
done

# Set default arguments for unset parameters
BUILD_CTAGS=${BUILD_CTAGS:=OFF}
BUILD_DOCS=${BUILD_DOCS:=OFF}
BUILD_EXAMPLES=${BUILD_EXAMPLES:=OFF}
CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
DOXYGEN_TAGFILES=${DOXYGEN_TAGFILES:=OFF}
EXPORT_COMPILE_COMMANDS=${EXPORT_COMPILE_COMMANDS:=OFF}
VERBOSE_MAKEFILE=${VERBOSE_MAKEFILE:=OFF}
DEPENDENCY_GRAPH=${DEPENDENCY_GRAPH:=0}

# Make directory if needed and run cmake
mkdir -p ${BUILD_DIR}
(
    PROJECT_DIR=$(pwd)
    cd ${BUILD_DIR}
    cmake -DBUILD_CTAGS=${BUILD_CTAGS} \
          -DBUILD_DOCS=${BUILD_DOCS} \
          -DBUILD_EXAMPLES=${BUILD_EXAMPLES} \
          -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
          `[[ "${BUILD_DOCS}" == "ON" ]] && echo "-DDOXYGEN_TAGFILES=${DOXYGEN_TAGFILES}"` \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=${EXPORT_COMPILE_COMMANDS} \
          -DCMAKE_VERBOSE_MAKEFILE=${VERBOSE_MAKEFILE} \
          `[[ ${DEPENDENCY_GRAPH} -eq 0 ]] || echo --graphviz=dependencies/dependencies.dot` \
          ${PROJECT_DIR}
    echo -e "\n\tYou can cd into ${BUILD_DIR} and call \"make -j8\" there:\n"
    echo -e "\t\tcd ${BUILD_DIR}\n\t\tmake -j8\n"
)
if [[ ${DEPENDENCY_GRAPH} -ne 0 && -f ${BUILD_DIR}/dependencies/dependencies.dot ]]; then
    dot -Tpdf ${BUILD_DIR}/dependencies/dependencies.dot -o ${BUILD_DIR}/dependencies.pdf
fi



