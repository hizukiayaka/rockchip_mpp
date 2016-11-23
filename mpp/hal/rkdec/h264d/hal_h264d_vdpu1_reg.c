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

#define MODULE_TAG "hal_h264d_vdpu1_reg"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rk_type.h"
#include "mpp_err.h"
#include "mpp_mem.h"
#include "vpu.h"
#include "mpp_time.h"

#include "h264d_log.h"
#include "hal_regdrv.h"
#include "hal_h264d_global.h"
#include "hal_h264d_api.h"
#include "hal_h264d_vdpu1_pkt.h"
#include "hal_h264d_vdpu1_reg.h"

static RK_U32 vdpu1_ver_align(RK_U32 val)
{
    return MPP_ALIGN(val, 16);
}

static RK_U32 vdpu1_hor_align(RK_U32 val)
{
    return MPP_ALIGN(val, 16);
}

static MPP_RET vdpu1_set_device_regs(H264dHalCtx_t *p_hal, H264dVdpu1Regs_t *p_reg)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;

    p_reg->SwReg03.sw_dec_mode = 0; /* set H264 mode */
    p_reg->SwReg02.sw_dec_out_endian = 1;  /* little endian */
    p_reg->SwReg02.sw_dec_in_endian = 0;  /* big endian */
    p_reg->SwReg02.sw_dec_strendian_e = 1; //!< little endian
    p_reg->SwReg02.sw_tiled_mode_msb = 0; /* 0: raster scan  1: tiled */

    /* bus_burst_length = 16, bus burst */
    p_reg->SwReg02.sw_dec_max_burst = 16; /* (0, 4, 8, 16) choice one */
    p_reg->SwReg02.sw_dec_scmd_dis = 0; /* disable */
    p_reg->SwReg02.sw_dec_adv_pre_dis = 0; /* disable */
    p_reg->SwReg55.sw_apf_threshold = 8;
    p_reg->SwReg02.sw_dec_latency = 0; /* compensation for bus latency; values up to 63 */
    p_reg->SwReg02.sw_dec_data_disc_e = 0;
    p_reg->SwReg02.sw_dec_out_endian = 1; /* little endian */
    p_reg->SwReg02.sw_dec_inswap32_e = 1; /* little endian */
    p_reg->SwReg02.sw_dec_outswap32_e = 1;
    p_reg->SwReg02.sw_dec_strswap32_e = 1;
    p_reg->SwReg02.sw_dec_strendian_e = 1; /* little endian */
    p_reg->SwReg02.sw_dec_timeout_e = 1;

    /* clock_gating  0:clock always on, 1: clock gating module control the key(turn off when decoder free) */
    p_reg->SwReg02.sw_dec_clk_gate_e = 1;
    p_reg->SwReg01.sw_dec_irq_dis_cfg = 0;

    //!< set AXI RW IDs
    p_reg->SwReg02.sw_dec_axi_rd_id = (0xFF & 0xFFU); /* 0-255 */
    p_reg->SwReg03.sw_dec_axi_wr_id = (0x00 & 0xFFU); /* 0-255 */

    ///!< Set prediction filter taps
    {
        RK_U32 val = 0;
        p_reg->SwReg49.sw_pred_bc_tap_0_0 = 1;

        val = (RK_U32)(-5);
        p_reg->SwReg49.sw_pred_bc_tap_0_1 = val;
        p_reg->SwReg49.sw_pred_bc_tap_0_2 = 20;
    }

    (void)p_hal;

    H264D_LOG("Exit!");
    return ret = MPP_OK;
}

static MPP_RET vdpu1_get_info_input(H264dHalCtx_t *p_hal, H264dVdpu1Priv_t *priv)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    DXVA_PicParams_H264_MVC  *pp = p_hal->pp;

    memset(priv->new_dpb, 0, sizeof(priv->new_dpb));
    memset(priv->refinfo, 0, sizeof(priv->refinfo));

    //!< change dpb_info syntax
    {
        RK_U32 i = 0;
        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefFrameList); i++) {
            if (pp->RefFrameList[i].bPicEntry != 0xff) {
                priv->new_dpb[i].valid = 1;
                priv->new_dpb[i].is_long_term = pp->RefFrameList[i].AssociatedFlag;
                priv->new_dpb[i].slot_index = pp->RefFrameList[i].Index7Bits;
                priv->new_dpb[i].TOP_POC = pp->FieldOrderCntList[i][0];
                priv->new_dpb[i].BOT_POC = pp->FieldOrderCntList[i][1];
                if (priv->new_dpb[i].is_long_term) {
                    priv->new_dpb[i].long_term_frame_idx = pp->FrameNumList[i];
                } else {
                    priv->new_dpb[i].frame_num = pp->FrameNumList[i];
                }
                priv->new_dpb[i].long_term_pic_num = pp->LongTermPicNumList[i];
                priv->new_dpb[i].top_used = ((pp->UsedForReferenceFlags >> (2 * i + 0)) & 0x1) ? 1 : 0;
                priv->new_dpb[i].bot_used = ((pp->UsedForReferenceFlags >> (2 * i + 1)) & 0x1) ? 1 : 0;
            }
        }
        for (i = 0; i < MPP_ARRAY_ELEMS(pp->ViewIDList); i++) {
            priv->new_dpb[i].view_id = pp->ViewIDList[i];
        }
        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefFrameList); i++) {
            priv->new_dpb[i].colmv_is_used = ((pp->RefPicColmvUsedFlags >> i) & 0x1) ? 1 : 0;
            priv->new_dpb[i].field_flag = ((pp->RefPicFiledFlags >> i) & 0x1) ? 1 : 0;
            priv->new_dpb[i].is_ilt_flag = ((pp->UsedForInTerviewflags >> i) & 0x1) ? 1 : 0;
        }
        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefPicLayerIdList); i++) {
            priv->new_dpb[i].voidx = pp->RefPicLayerIdList[i];
        }
    }

    //!< change ref_pic_info syntax
    {
        RK_U32 i = 0, j = 0;
        DXVA_Slice_H264_Long *p_long = &p_hal->slice_long[0];
        //!< list P B0 B1
        for (j = 0; j < 3; j++) {
            for (i = 0; i < MPP_ARRAY_ELEMS(p_long->RefPicList[j]); i++) {
                if (p_long->RefPicList[j][i].bPicEntry != 0xff) {
                    priv->refinfo[j][i].valid = 1;
                    priv->refinfo[j][i].dpb_idx = p_long->RefPicList[j][i].Index7Bits;
                    priv->refinfo[j][i].bottom_flag = p_long->RefPicList[j][i].AssociatedFlag;
                }
            }
        }
    }
    H264D_LOG("Exit!");
    return ret = MPP_OK;
}

static void fill_picture_entry(DXVA_PicEntry_H264 *pic, RK_U32 index, RK_U32 flag)
{
    ASSERT((index & 0x7f) == index && (flag & 0x01) == flag);
    pic->bPicEntry = index | (flag << 7);
}

static MPP_RET vdpu1_refill_info_input(H264dHalCtx_t *p_hal, H264dVdpu1Priv_t *priv)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    DXVA_PicParams_H264_MVC  *pp = p_hal->pp;
    {
        RK_U32 i = 0;
        H264dVdpu1DpbInfo_t *old_dpb = priv->old_dpb[priv->layed_id];
        //!< re-fill dpb_info
        pp->UsedForReferenceFlags = 0;

        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefFrameList); i++) {
            if (old_dpb[i].valid) {
                fill_picture_entry(&pp->RefFrameList[i], old_dpb[i].slot_index, old_dpb[i].is_long_term);
                pp->FieldOrderCntList[i][0] = old_dpb[i].TOP_POC;
                pp->FieldOrderCntList[i][1] = old_dpb[i].BOT_POC;
                pp->FrameNumList[i] = old_dpb[i].is_long_term ? old_dpb[i].long_term_frame_idx : old_dpb[i].frame_num;
                pp->LongTermPicNumList[i] = old_dpb[i].long_term_pic_num;
                if (old_dpb[i].top_used) { //!< top_field
                    pp->UsedForReferenceFlags |= 1 << (2 * i + 0);
                }
                if (old_dpb[i].bot_used) { //!< bot_field
                    pp->UsedForReferenceFlags |= 1 << (2 * i + 1);
                }
            } else {
                pp->RefFrameList[i].bPicEntry = 0xff;
                pp->FieldOrderCntList[i][0] = 0;
                pp->FieldOrderCntList[i][1] = 0;
                pp->FrameNumList[i] = 0;
                pp->LongTermPicNumList[i] = 0;
            }
        }

        for (i = 0; i < MPP_ARRAY_ELEMS(pp->ViewIDList); i++) {
            pp->ViewIDList[i] = old_dpb[i].view_id;
        }
        pp->RefPicColmvUsedFlags = 0;
        pp->RefPicFiledFlags = 0;
        pp->UsedForInTerviewflags = 0;

        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefFrameList); i++) {
            if (old_dpb[i].colmv_is_used) {
                pp->RefPicColmvUsedFlags |= 1 << i;
            }
            if (old_dpb[i].field_flag) {
                pp->RefPicFiledFlags |= 1 << i;
            }
            if (old_dpb[i].is_ilt_flag) {
                pp->UsedForInTerviewflags |= 1 << i;
            }
        }

        for (i = 0; i < MPP_ARRAY_ELEMS(pp->RefPicLayerIdList); i++) {
            pp->RefPicLayerIdList[i] = old_dpb[i].voidx;
        }
    }

    //!< re-fill ref_info
    {
        RK_U32 i = 0, j = 0;
        DXVA_Slice_H264_Long *p_long = &p_hal->slice_long[0];
        for (j = 0; j < 3; j++) {
            H264dVdpu1RefPicInfo_t *p = priv->refinfo[j];
            for (i = 0; i < MPP_ARRAY_ELEMS(p_long->RefPicList[j]); i++) {
                if (p[i].valid) {
                    fill_picture_entry(&p_long->RefPicList[j][i], p[i].dpb_idx, p[i].bottom_flag);
                } else {
                    p_long->RefPicList[j][i].bPicEntry = 0xff;
                }
            }
        }
    }
    H264D_LOG("Exit!");
    return ret = MPP_OK;
}

static MPP_RET vdpu1_adjust_input(H264dHalCtx_t *p_hal, H264dVdpu1Priv_t *priv)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;

    priv->layed_id = p_hal->pp->curr_layer_id;
    vdpu1_get_info_input(p_hal, priv);

    //!< dpb mapping to new order
    {
        RK_U32 i = 0, j = 0;
        RK_U32 find_flag = 0;
        H264dVdpu1DpbInfo_t *new_dpb = priv->new_dpb;
        H264dVdpu1DpbInfo_t *old_dpb = priv->old_dpb[priv->layed_id];

        //!< delete old dpb
        for (i = 0; i < MPP_ARRAY_ELEMS(priv->old_dpb[priv->layed_id]); i++) {
            find_flag = 0;
            if (old_dpb[i].valid) {
                for (j = 0; j < MPP_ARRAY_ELEMS(priv->new_dpb); j++) {
                    if (new_dpb[j].valid) {
                        find_flag = ((old_dpb[i].frame_num == new_dpb[j].frame_num) ? 1 : 0);
                        find_flag = ((old_dpb[i].slot_index == new_dpb[j].slot_index) ? find_flag : 0);
                        if (new_dpb[j].top_used) {
                            find_flag = ((old_dpb[i].TOP_POC == new_dpb[j].TOP_POC) ? find_flag : 0);
                        }
                        if (new_dpb[j].bot_used) {
                            find_flag  = ((old_dpb[i].BOT_POC == new_dpb[j].BOT_POC) ? find_flag : 0);
                        }
                        if (find_flag) { //!< found
                            new_dpb[j].have_same = 1;
                            new_dpb[j].new_dpb_idx = i;
                            break;
                        }
                    }
                }
            }

            //!< not found
            if (find_flag == 0) {
                memset(&old_dpb[i], 0, sizeof(old_dpb[i]));
            }
        }

        //!< add new dpb
        for (j = 0; j < MPP_ARRAY_ELEMS(priv->new_dpb); j++) {
            if ((new_dpb[j].valid == 0) || new_dpb[j].have_same) {
                continue;
            }
            for (i = 0; i < MPP_ARRAY_ELEMS(priv->old_dpb[priv->layed_id]); i++) {
                if (old_dpb[i].valid == 0) {
                    old_dpb[i] = new_dpb[j];
                    new_dpb[j].new_dpb_idx = i;
                    break;
                }
            }
        }

        //!< inter-layer reference
        priv->ilt_dpb = NULL;
        if (priv->layed_id) {
            for (i = 0; i < MPP_ARRAY_ELEMS(priv->old_dpb[1]); i++) {
                if ((old_dpb[i].valid == 0) && old_dpb[i].is_ilt_flag) {
                    priv->ilt_dpb = &old_dpb[i];
                    break;
                }
            }
        }
    }

    //!< addjust ref_dpb
    {
        RK_U32 i = 0, j = 0;
        H264dVdpu1DpbInfo_t *new_dpb = priv->new_dpb;

        for (j = 0; j < 3; j++) {
            H264dVdpu1RefPicInfo_t *p = priv->refinfo[j];
            for (i = 0; i < MPP_ARRAY_ELEMS(priv->refinfo[j]); i++) {
                if (p[i].valid) {
                    p[i].dpb_idx = new_dpb[p[i].dpb_idx].new_dpb_idx;
                }
            }
        }
    }
    vdpu1_refill_info_input(p_hal, priv);

    H264D_LOG("Exit!");
    return ret = MPP_OK;
}

/*!
***********************************************************************
* \brief
*    init  VDPU granite decoder
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_init(void *hal, MppHalCfg *cfg)
{
    H264D_LOG("Enter!");
    RK_U32 cabac_size = 0;
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t  *p_hal = (H264dHalCtx_t *)hal;
    INP_CHECK(ret, NULL == hal);

    //!< malloc init registers
    MEM_CHECK(ret, p_hal->regs = mpp_calloc_size(void, sizeof(H264dVdpu1Regs_t)));
    MEM_CHECK(ret, p_hal->priv = mpp_calloc_size(void, sizeof(H264dVdpu1Priv_t)));

    //!< malloc cabac+scanlis + packets + poc_buf
    cabac_size = VDPU1_CABAC_TAB_SIZE + VDPU1_SCALING_LIST_SIZE + VDPU1_POC_BUF_SIZE;
    FUN_CHECK(ret = mpp_buffer_get(p_hal->buf_group, &p_hal->cabac_buf, cabac_size));

    //!< copy cabac table bytes
    FUN_CHECK(ret = mpp_buffer_write(p_hal->cabac_buf, 0, (void *)H264_VDPU1_Cabac_table, sizeof(H264_VDPU1_Cabac_table)));
    FUN_CHECK(ret = vdpu1_set_device_regs(p_hal, (H264dVdpu1Regs_t *)p_hal->regs));
    mpp_slots_set_prop(p_hal->frame_slots, SLOTS_HOR_ALIGN, vdpu1_hor_align);
    mpp_slots_set_prop(p_hal->frame_slots, SLOTS_VER_ALIGN, vdpu1_ver_align);
    mpp_slots_set_prop(p_hal->frame_slots, SLOTS_LEN_ALIGN, NULL);
    p_hal->iDecodedNum = 0;

    H264D_LOG("Exit!");
    (void)cfg;
__RETURN:
    return MPP_OK;
__FAILED:
    vdpu1_h264d_deinit(hal);

    return ret;
}

/*!
***********************************************************************
* \brief
*    deinit
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_deinit(void *hal)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t  *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, NULL == p_hal);

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    MPP_FREE(p_hal->regs);
    MPP_FREE(p_hal->priv);
    if (p_hal->cabac_buf) {
        FUN_CHECK(ret = mpp_buffer_put(p_hal->cabac_buf));
    }

    H264D_LOG("Exit!");
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
__FAILED:
    return ret;
}

/*!
***********************************************************************
* \brief
*    generate register
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_gen_regs(void *hal, HalTaskInfo *task)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;

    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;
    INP_CHECK(ret, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);
    p_hal->in_task = &task->dec;
    if (task->dec.flags.had_error)  {
        goto __RETURN;
    }

    LogTrace(p_hal->logctx.parr[RUN_HAL], "[Generate register begin]");
    FUN_CHECK(ret = vdpu1_adjust_input(p_hal, (H264dVdpu1Priv_t *)p_hal->priv));
    FUN_CHECK(ret = vdpu1_set_pic_regs(p_hal, (H264dVdpu1Regs_t *)p_hal->regs));
    FUN_CHECK(ret = vdpu1_set_vlc_regs(p_hal, (H264dVdpu1Regs_t *)p_hal->regs));
    FUN_CHECK(ret = vdpu1_set_ref_regs(p_hal, (H264dVdpu1Regs_t *)p_hal->regs));
    FUN_CHECK(ret = vdpu1_set_asic_regs(p_hal, (H264dVdpu1Regs_t *)p_hal->regs));
    LogTrace(p_hal->logctx.parr[RUN_HAL], "[Generate register end]");
    p_hal->in_task->valid = 0;

    H264D_LOG("Exit!");
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
__FAILED:
    return ret;
}

/*!
***********************************************************************
* \brief h
*    start hard
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_start(void *hal, HalTaskInfo *task)
{
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264D_LOG("Enter!");
    H264dHalCtx_t *p_hal  = (H264dHalCtx_t *)hal;
    H264dVdpu1Regs_t *p_regs = (H264dVdpu1Regs_t *)p_hal->regs;

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);
    if (task->dec.flags.had_error) {
        goto __RETURN;
    }

    p_regs->SwReg57.sw_cache_en = 1;
    p_regs->SwReg57.sw_pref_sigchan = 1;
    p_regs->SwReg57.sw_intra_dbl3t = 1;
    p_regs->SwReg57.sw_inter_dblspeed = 1;
    p_regs->SwReg57.sw_intra_dblspeed = 1;
    p_regs->SwReg57.sw_paral_bus = 1;

#ifdef RKPLATFORM
    if (VPUClientSendReg(p_hal->vpu_socket, (RK_U32 *)p_hal->regs, DEC_VDPU1_REGISTERS)) {
        ret =  MPP_ERR_VPUHW;
        mpp_err_f("H264 VDPU1 FlushRegs fail, pid=%d, hal_frame_no=%d. \n", getpid(), p_hal->iDecodedNum);
    } else {
        H264D_LOG("H264 VDPU1 FlushRegs success, pid=%d, hal_frame_no=%d. \n", getpid(), p_hal->iDecodedNum);
    }
#endif

    H264D_LOG("Exit!");
__RETURN:
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)task;
    return ret = MPP_OK;
}

/*!
***********************************************************************
* \brief
*    wait hard
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_wait(void *hal, HalTaskInfo *task)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t  *p_hal = (H264dHalCtx_t *)hal;
    H264dVdpu1Regs_t *p_regs = (H264dVdpu1Regs_t *)p_hal->regs;

    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    if (task->dec.flags.had_error) {
        goto __SKIP_HARD;
    }
#ifdef RKPLATFORM
    {
        RK_S32 wait_ret = -1;
        RK_S32 ret_len = 0, cur_deat = 0;
        VPU_CMD_TYPE ret_cmd = VPU_CMD_BUTT;
        RK_S64 p_s, p_e;
        p_s = mpp_time();
        wait_ret = VPUClientWaitResult(p_hal->vpu_socket, (RK_U32 *)p_hal->regs, DEC_VDPU1_REGISTERS, &ret_cmd, &ret_len);
        p_e = mpp_time();
        cur_deat = (p_e - p_s) / 1000;
        p_hal->total_time += cur_deat;
        p_hal->iDecodedNum++;
        (void)wait_ret;
    }
#endif

__SKIP_HARD:
    if (p_hal->init_cb.callBack) {
        IOCallbackCtx m_ctx = { 0 };
        m_ctx.device_id = HAL_VDPU;
        if (!p_regs->SwReg01.sw_dec_rdy_int) {
            m_ctx.hard_err = 1;
        }
        m_ctx.task = (void *)&task->dec;
        m_ctx.regs = (RK_U32 *)p_hal->regs;
        p_hal->init_cb.callBack(p_hal->init_cb.opaque, &m_ctx);
    }
    memset(&p_regs->SwReg01, 0, sizeof(RK_U32));
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)task;

    H264D_LOG("Exit!");
    return ret = MPP_OK;
}

/*!
***********************************************************************
* \brief
*    reset
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_reset(void *hal)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    memset(p_hal->priv, 0, sizeof(H264dVdpu1Priv_t));

    H264D_LOG("Exit!");
    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
}

/*!
***********************************************************************
* \brief
*    flush
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_flush(void *hal)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);


    H264D_LOG("Exit!");

    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
__RETURN:
    return ret = MPP_OK;
}

/*!
***********************************************************************
* \brief
*    control
***********************************************************************
*/
//extern "C"
MPP_RET vdpu1_h264d_control(void *hal, RK_S32 cmd_type, void *param)
{
    H264D_LOG("Enter!");
    MPP_RET ret = MPP_ERR_UNKNOW;
    H264dHalCtx_t *p_hal = (H264dHalCtx_t *)hal;

    INP_CHECK(ret, NULL == p_hal);
    FunctionIn(p_hal->logctx.parr[RUN_HAL]);

    FunctionOut(p_hal->logctx.parr[RUN_HAL]);
    (void)hal;
    (void)cmd_type;
    (void)param;
__RETURN:
    return ret = MPP_OK;
}
