# vim: syntax=cmake
# ----------------------------------------------------------------------------
# codec h264 built-in unit test case
# ----------------------------------------------------------------------------
# log system unit test
option(H264E_HAL_TEST "Build codec h264e_hal unit test" ON)
if(H264E_HAL_TEST)
    add_executable(h264e_hal_test h264e_hal_test.c)
    target_link_libraries(h264e_hal_test rockchip_mpp utils)
endif()


