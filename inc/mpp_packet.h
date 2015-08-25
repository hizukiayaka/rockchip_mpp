/*
 * Copyright 2010 Rockchip Electronics S.LSI Co. LTD
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

#ifndef __MPP_PACKET_H__
#define __MPP_PACKET_H__

#include "rk_type.h"
#include "mpp_err.h"

typedef void* MppPacket;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MppPacket interface
 */
MPP_RET mpp_packet_init(MppPacket *packet, void *data, size_t size);
MPP_RET mpp_packet_deinit(MppPacket packet);

MPP_RET mpp_packet_set_pts(MppPacket packet, RK_S64 pts);
MPP_RET mpp_packet_set_dts(MppPacket packet, RK_S64 dts);
MPP_RET mpp_packet_set_eos(MppPacket packet);
MPP_RET mpp_packet_set_extra_data(MppPacket packet);

#ifdef __cplusplus
}
#endif

#endif /*__MPP_PACKET_H__*/