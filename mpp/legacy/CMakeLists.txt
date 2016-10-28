# vim: syntax=cmake
include_directories(.)

# ----------------------------------------------------------------------------
# add mpp implement
# ----------------------------------------------------------------------------
set (MPP_LEGACY_SRC
    vpu.c
    vpu_api.cpp
    vpu_api_legacy.cpp
    vpu_mem_legacy.c
    rk_list.cpp
    ppOp.cpp
    ../mpp_info.cpp
    )

add_library(mpp_legacy STATIC ${MPP_LEGACY_SRC})
set_target_properties(mpp_legacy PROPERTIES FOLDER "mpp/legacy")
target_link_libraries(mpp_legacy rockchip_mpp)

add_library(mpp_legacy_shared SHARED ${MPP_LEGACY_SRC})
target_link_libraries(mpp_legacy_shared dl rockchip_mpp)
