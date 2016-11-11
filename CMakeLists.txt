cmake_minimum_required(VERSION 2.6)
find_package(PkgConfig)

PROJECT(rockchip_media_process_platform C CXX)
INCLUDE(GNUInstallDirs)

pkg_search_module(PTHREAD pthread)

ADD_DEFINITIONS(-Wmissing-declarations -Wall -Wextra -Wshadow -ffast-math)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")

# putting those into config.h would be better
set(RKPLATFORM "" CACHE BOOL "Enable RK HW CONFIG")
if(RKPLATFORM)
ADD_DEFINITIONS(-DRKPLATFORM)
endif(RKPLATFORM)

# VCS version generation

# ----------------------------------------------------------------------------
#  Start module definition
# ----------------------------------------------------------------------------
# project overall include file
include_directories(inc)
# small utile functions for test case
include_directories(utils)

# ----------------------------------------------------------------------------
#  osal library
# ----------------------------------------------------------------------------
# Operation System Abstract Layer (OSAL) include
include_directories(osal/inc)
# OSAL is needed on all platform, do not need option
add_subdirectory(osal)

# ----------------------------------------------------------------------------
#  utils for test case
# ----------------------------------------------------------------------------
add_subdirectory(utils)

# ----------------------------------------------------------------------------
#  Media Process Platform library
# ----------------------------------------------------------------------------
# Media Process Platform include
include_directories(mpp/inc)
add_subdirectory(mpp)

# ----------------------------------------------------------------------------
#  test / demo
# ----------------------------------------------------------------------------
add_subdirectory(test)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h.cmake"
"${CMAKE_CURRENT_SOURCE_DIR}/inc/config.h")

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/rockchip_mpp.pc.cmake"
"${CMAKE_CURRENT_BINARY_DIR}/rockchip_mpp.pc" @ONLY)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/rockchip_vpu.pc.cmake"
"${CMAKE_CURRENT_BINARY_DIR}/rockchip_vpu.pc" @ONLY)

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/rockchip_mpp.pc"
DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/rockchip_vpu.pc"
DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/inc/"
DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/rockchip")

# End generate the build configure file
