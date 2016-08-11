#!/bin/bash
# Run this from within a bash shell
scan-build cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=debug -DCMAKE_RKPLATFORM_ENABLE=ON ../../../  && scan-build ccmake  ../../../
