cmake_minimum_required( VERSION 2.6.3 )

SET(CMAKE_SYSTEM_NAME Linux)
set(ARMEABI_V7A true)
set(ANDROID_ARCH_NAME "arm")
set(CMAKE_SYSTEM_PROCESSOR "armv7")

add_definitions(-fPIC)

#set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
#set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
#set(CMAKE_CPP_FLAGS "-fPIC ${CMAKE_C_FLAGS}")

add_definitions(-DARM_LINUX)
