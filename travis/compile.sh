#!/usr/bin/env bash

set -e

BUILD_DIR=${HOME}/BUILD_${PLATFORM}
INSTALL_DIR=${BUILD_DIR}/INSTALL

TEST=0
case ${PLATFORM} in
    "native_static")
         MESON_OPTION="--default-library=static"
         TEST=1
         ;;
    "native_dyn")
         MESON_OPTION="--default-library=shared"
         TEST=1
         ;;
    "win32_static")
         MESON_OPTION="--default-library=static --cross-file ${BUILD_DIR}/meson_cross_file.txt"
         ;;
    "win32_dyn")
         MESON_OPTION="--default-library=shared --cross-file ${BUILD_DIR}/meson_cross_file.txt"
         ;;
    "android_arm")
         MESON_OPTION="-Dandroid=true --default-library=shared --cross-file ${BUILD_DIR}/meson_cross_file.txt"
         ;;
    "android_arm64")
         MESON_OPTION="-Dandroid=true --default-library=shared --cross-file ${BUILD_DIR}/meson_cross_file.txt"
         ;;

esac

cd ${TRAVIS_BUILD_DIR}
if [[ "${TRAVIS_OS_NAME}" == "osx" ]]
then
  export PKG_CONFIG_PATH=${INSTALL_DIR}/lib/pkgconfig
else
  export PKG_CONFIG_PATH=${INSTALL_DIR}/lib/x86_64-linux-gnu/pkgconfig
fi
export CPPFLAGS="-I${INSTALL_DIR}/include"
meson . build ${MESON_OPTION}
cd build
ninja
if [[ "$TEST" == "1" ]]
then
  echo "Running test"
  export LD_LIBRARY_PATH=${INSTALL_DIR}/lib:${INSTALL_DIR}/lib64:${INSTALL_DIR}/lib/x86_64-linux-gnu
  ninja test
fi
