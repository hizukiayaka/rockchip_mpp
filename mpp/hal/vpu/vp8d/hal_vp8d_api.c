/*
*
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
#define MODULE_TAG "hal_vp8d_api"

#include <string.h>

#include "mpp_hal.h"
#include "hal_vp8d_base.h"
#include "hal_vp8d_reg.h"
#include "hal_vp8d_vdpu1_reg.h"

RK_U32 vp8h_debug = 0;

MPP_RET hal_vp8d_init (void *hal, MppHalCfg *cfg)
{
	MppHalApi *p_api = NULL;
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	if (self == NULL)
		return MPP_ERR_VALUE;
	memset(self, 0, sizeof(VP8DHalContext_t));

	p_api = &self->hal_api;
	cfg->device_id = mpp_hal_get_vpu_version();

	switch (cfg->device_id) {
	case HAL_VDPU2:
		p_api->init = hal_vp8d_vdpu2_init;
		p_api->deinit = hal_vp8d_vdpu2_deinit;
		p_api->reg_gen = hal_vp8d_vdpu2_gen_regs;
		p_api->start = hal_vp8d_vdpu2_start;
		p_api->wait = hal_vp8d_vdpu2_wait;
		p_api->reset = hal_vp8d_vdpu2_reset;
		p_api->flush = hal_vp8d_vdpu2_flush;
		p_api->control = hal_vp8d_vdpu2_control;
		break;
	case HAL_VDPU1:
		p_api->init = hal_vp8d_vdpu1_init;
		p_api->deinit = hal_vp8d_vdpu1_deinit;
		p_api->reg_gen = hal_vp8d_vdpu1_gen_regs;
		p_api->start = hal_vp8d_vdpu1_start;
		p_api->wait = hal_vp8d_vdpu1_wait;
		p_api->reset = hal_vp8d_vdpu1_reset;
		p_api->flush = hal_vp8d_vdpu1_flush;
		p_api->control = hal_vp8d_vdpu1_control;
		break;
	default:
		return MPP_ERR_INIT;
	}

	return p_api->init(hal, cfg);
}

MPP_RET hal_vp8d_deinit (void *hal)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.deinit(hal);
}

MPP_RET hal_vp8d_gen_regs (void *hal, HalTaskInfo *task)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.reg_gen(hal, task);
}

MPP_RET hal_vp8d_start (void *hal, HalTaskInfo *task)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.start(hal, task);
}

MPP_RET hal_vp8d_wait (void *hal, HalTaskInfo *task)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.wait(hal, task);
}

MPP_RET hal_vp8d_reset (void *hal)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.reset(hal);
}

MPP_RET hal_vp8d_flush (void *hal)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.flush(hal);
}

MPP_RET hal_vp8d_control (void *hal, RK_S32 cmd_type, void *param)
{
	VP8DHalContext_t *self = (VP8DHalContext_t *)hal;

	return self->hal_api.control(hal, cmd_type, param);
}

const MppHalApi hal_api_vp8d = {
    .name = "vp8d_rkdec",
    .type = MPP_CTX_DEC,
    .coding = MPP_VIDEO_CodingVP8,
    .ctx_size = sizeof(VP8DHalContext_t),
    .flag = 0,
    .init = hal_vp8d_init,
    .deinit = hal_vp8d_deinit,
    .reg_gen = hal_vp8d_gen_regs,
    .start = hal_vp8d_start,
    .wait = hal_vp8d_wait,
    .reset = hal_vp8d_reset,
    .flush = hal_vp8d_flush,
    .control = hal_vp8d_control,
};
