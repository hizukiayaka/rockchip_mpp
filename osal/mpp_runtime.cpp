/*
 * Copyright 2015 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define MODULE_TAG "mpp_rt"

#include <fcntl.h>
#include <unistd.h>

#include "mpp_log.h"
#include "mpp_runtime.h"

class MppRuntimeService
{
private:
    // avoid any unwanted function
    MppRuntimeService();
    ~MppRuntimeService() {};
    MppRuntimeService(const MppRuntimeService &);
    MppRuntimeService &operator=(const MppRuntimeService &);

    RK_U32  allocator_valid[MPP_BUFFER_TYPE_BUTT];

public:
    static MppRuntimeService *get_instance() {
        static MppRuntimeService instance;
        return &instance;
    }

    RK_U32 get_allocator_valid(MppBufferType type);
};

RK_U32 MppRuntimeService::get_allocator_valid(MppBufferType type)
{
    MppBufferType buffer_type = (MppBufferType)(type & MPP_BUFFER_TYPE_MASK);
    return (buffer_type < MPP_BUFFER_TYPE_BUTT) ? allocator_valid[buffer_type] : (0);
};

MppRuntimeService::MppRuntimeService()
{
    allocator_valid[MPP_BUFFER_TYPE_NORMAL] = 1;

    if (access("/dev/ion", F_OK | R_OK | W_OK)) {
        allocator_valid[MPP_BUFFER_TYPE_ION] = 0;
        mpp_log("NOT found ion allocator\n");
    } else {
        allocator_valid[MPP_BUFFER_TYPE_ION] = 1;
        mpp_log("found ion allocator\n");
    }

    if (access("/dev/dri/card0", F_OK | R_OK | W_OK)) {
        allocator_valid[MPP_BUFFER_TYPE_DRM] = 0;
        mpp_log("NOT found drm allocator\n");
    } else {
        allocator_valid[MPP_BUFFER_TYPE_DRM] = 1;
        mpp_log("found drm allocator\n");
    }
}

RK_U32 mpp_rt_allcator_is_valid(MppBufferType type)
{
    return MppRuntimeService::get_instance()->get_allocator_valid(type);
}
