# vim: syntax=cmake
# ----------------------------------------------------------------------------
# add libvpu implement
# ----------------------------------------------------------------------------
add_library(utils STATIC
    utils.c
    iniparser.c
    dictionary.c
    )

target_link_libraries(utils rockchip_mpp)
