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

#ifndef __HAL_H264D_VDPU1_PKT_H__
#define __HAL_H264D_VDPU1_PKT_H__

#include "rk_type.h"
#include "mpp_err.h"
#include "hal_task.h"
#include "h264d_log.h"
#include "hal_h264d_fifo.h"
#include "hal_h264d_global.h"

typedef struct {
    uint32_t SwReg00;

    struct {
        uint32_t sw_dec_en          : 1;
        uint32_t reverse0           : 3;
        uint32_t sw_dec_irq_dis_cfg : 1;
        uint32_t reverse1           : 3;
        uint32_t sw_dec_irq_config  : 1;
        uint32_t reverse2           : 3;
        uint32_t sw_dec_rdy_int     : 1;
        uint32_t sw_dec_bus_int     : 1;
        uint32_t sw_dec_buffer_int  : 1;
        uint32_t sw_dec_aso_int     : 1;
        uint32_t sw_dec_error_int   : 1;
        uint32_t sw_dec_slice_int   : 1;
        uint32_t sw_dec_timeout     : 1;
        uint32_t reverse3           : 5;
        uint32_t sw_dec_pic_inf     : 1;
        uint32_t reverse4           : 7;
    } SwReg01;

    struct {
        uint32_t sw_dec_max_burst   : 5;
        uint32_t sw_dec_scmd_dis    : 1;
        uint32_t sw_dec_adv_pre_dis : 1;
        uint32_t sw_tiled_mode_lsb  : 1;
        uint32_t sw_dec_out_endian  : 1;
        uint32_t sw_dec_in_endian   : 1;
        uint32_t sw_dec_clk_gate_e  : 1;
        uint32_t sw_dec_latency     : 6;
        uint32_t sw_tiled_mode_msb  : 1;
        uint32_t sw_dec_data_disc_e : 1;
        uint32_t sw_dec_outswap32_e : 1;
        uint32_t sw_dec_inswap32_e  : 1;
        uint32_t sw_dec_strendian_e : 1;
        uint32_t sw_dec_strswap32_e : 1;
        uint32_t sw_dec_timeout_e   : 1;
        uint32_t sw_dec_axi_rd_id   : 8;
    } SwReg02;

    struct {
        uint32_t sw_dec_axi_wr_id   : 8;
        uint32_t sw_dec_ahb_hlock_e : 1;
        uint32_t sw_picord_count_e  : 1;
        uint32_t sw_seq_mbaff_e     : 1;
        uint32_t sw_reftopfirst_e   : 1;
        uint32_t sw_write_mvs_e     : 1;
        uint32_t sw_pic_fixed_quant : 1;
        uint32_t sw_filtering_dis   : 1;
        uint32_t sw_dec_out_dis     : 1;
        uint32_t sw_ref_topfield_e  : 1;
        uint32_t sw_sorenson_e      : 1;
        uint32_t sw_fwd_interlace_e : 1;
        uint32_t sw_pic_topfield_e  : 1;
        uint32_t sw_pic_inter_e     : 1;
        uint32_t sw_pic_b_e         : 1;
        uint32_t sw_pic_fieldmode_e : 1;
        uint32_t sw_pic_interlace_e : 1;
        uint32_t sw_pjpeg_e         : 1;
        uint32_t sw_divx3_e         : 1;
        uint32_t sw_skip_mode       : 1;
        uint32_t sw_rlc_mode_e      : 1;
        uint32_t sw_dec_mode        : 4;
    } SwReg03;

    struct {
        uint32_t sw_ref_frames      : 5;
        uint32_t reverse0           : 6;
        uint32_t sw_pic_mb_height_p : 8;
        uint32_t reverse1           : 4;
        uint32_t sw_pic_mb_width    : 9;
    } SwReg04;

    struct {
        uint32_t sw_fieldpic_flag_e : 1;
        uint32_t reverse0           : 13;
        uint32_t sw_ch_qp_offset2   : 5;    /* Cr */
        uint32_t sw_ch_qp_offset    : 5;    /* Cb */
        uint32_t sw_type1_quant_e   : 1;
        uint32_t sw_sync_marker_e   : 1;
        uint32_t sw_strm_start_bit  : 6;
    } SwReg05;

    struct {
        uint32_t sw_stream_len      : 24;
        uint32_t sw_ch_8pix_ileav_e : 1;
        uint32_t sw_init_qp         : 6;
        uint32_t sw_start_code_e    : 1;
    } SwReg06;

    struct {
        uint32_t sw_framenum        : 16;
        uint32_t sw_framenum_len    : 5;
        uint32_t reverse0           : 5;
        uint32_t sw_weight_bipr_idc : 2;
        uint32_t sw_weight_pred_e   : 1;
        uint32_t sw_dir_8x8_infer_e : 1;
        uint32_t sw_blackwhite_e    : 1;
        uint32_t sw_cabac_e         : 1;
    } SwReg07;

    struct {
        uint32_t sw_idr_pic_id      : 16;
        uint32_t sw_idr_pic_e       : 1;
        uint32_t sw_refpic_mk_len   : 11;
        uint32_t sw_8x8trans_flag_e : 1;
        uint32_t sw_rdpic_cnt_pres  : 1;
        uint32_t sw_filt_ctrl_pres  : 1;
        uint32_t sw_const_intra_e   : 1;
    } SwReg08;

    struct {
        uint32_t sw_poc_length      : 8;
        uint32_t reverse0           : 6;
        uint32_t sw_refidx0_active  : 5;
        uint32_t sw_refidx1_active  : 5;
        uint32_t sw_pps_id          : 8;
    } SwReg09;

    struct {
        uint32_t sw_pinit_rlist_f4  : 5;
        uint32_t sw_pinit_rlist_f5  : 5;
        uint32_t sw_pinit_rlist_f6  : 5;
        uint32_t sw_pinit_rlist_f7  : 5;
        uint32_t sw_pinit_rlist_f8  : 5;
        uint32_t sw_pinit_rlist_f9  : 5;
        uint32_t reverse0           : 2;
    } SwReg10;

    struct {
        uint32_t sw_pinit_rlist_f10  : 5;
        uint32_t sw_pinit_rlist_f11  : 5;
        uint32_t sw_pinit_rlist_f12  : 5;
        uint32_t sw_pinit_rlist_f13  : 5;
        uint32_t sw_pinit_rlist_f14  : 5;
        uint32_t sw_pinit_rlist_f15  : 5;
        uint32_t sw_i4x4_or_dc_base  : 2;
    } SwReg11;

    struct {
        uint32_t rlc_vlc_st_adr : 32;
    } SwReg12;

    struct {
        uint32_t dec_out_st_adr : 32;
    } SwReg13;

    /* MPP passes fd of reference frame to kernel
     * with the whole register rather than higher 30-bit.
     * At the same time, the lower 2-bit will be assigned
     * by kernel.
     * */
    struct {
        //uint32_t sw_refer0_topc_e    : 1;
        //uint32_t sw_refer0_field_e   : 1;
        uint32_t sw_refer0_base      : 32;
    } SwReg14;

    struct {
        //uint32_t sw_refer1_topc_e    : 1;
        //uint32_t sw_refer1_field_e   : 1;
        uint32_t sw_refer1_base      : 32;
    } SwReg15;

    struct {
        //uint32_t sw_refer2_topc_e    : 1;
        //uint32_t sw_refer2_field_e   : 1;
        uint32_t sw_refer2_base      : 32;
    } SwReg16;

    struct {
        //  uint32_t sw_refer3_topc_e    : 1;
        //uint32_t sw_refer3_field_e   : 1;
        uint32_t sw_refer3_base      : 32;
    } SwReg17;

    struct {
        //uint32_t sw_refer4_topc_e    : 1;
        //uint32_t sw_refer4_field_e   : 1;
        uint32_t sw_refer4_base      : 32;
    } SwReg18;

    struct {
        //uint32_t sw_refer5_topc_e    : 1;
        //uint32_t sw_refer5_field_e   : 1;
        uint32_t sw_refer5_base      : 32;
    } SwReg19;

    struct {
        //uint32_t sw_refer6_topc_e    : 1;
        //uint32_t sw_refer6_field_e   : 1;
        uint32_t sw_refer6_base      : 32;
    } SwReg20;

    struct {
        //uint32_t sw_refer7_topc_e    : 1;
        //uint32_t sw_refer7_field_e   : 1;
        uint32_t sw_refer7_base      : 32;
    } SwReg21;

    struct {
        //uint32_t sw_refer8_topc_e    : 1;
        //uint32_t sw_refer8_field_e   : 1;
        uint32_t sw_refer8_base      : 32;
    } SwReg22;

    struct {
        //uint32_t sw_refer9_topc_e    : 1;
        //uint32_t sw_refer9_field_e   : 1;
        uint32_t sw_refer9_base      : 32;
    } SwReg23;

    struct {
        //uint32_t sw_refer10_topc_e    : 1;
        //uint32_t sw_refer10_field_e   : 1;
        uint32_t sw_refer10_base      : 32;
    } SwReg24;

    struct {
        //uint32_t sw_refer11_topc_e    : 1;
        //uint32_t sw_refer11_field_e   : 1;
        uint32_t sw_refer11_base      : 32;
    } SwReg25;

    struct {
        //uint32_t sw_refer12_topc_e    : 1;
        //uint32_t sw_refer12_field_e   : 1;
        uint32_t sw_refer12_base      : 32;
    } SwReg26;

    struct {
        //uint32_t sw_refer13_topc_e    : 1;
        //uint32_t sw_refer13_field_e   : 1;
        uint32_t sw_refer13_base      : 32;
    } SwReg27;

    struct {
        //uint32_t sw_refer14_topc_e    : 1;
        //uint32_t sw_refer14_field_e   : 1;
        uint32_t sw_refer14_base      : 32;
    } SwReg28;

    struct {
        //uint32_t sw_refer15_topc_e    : 1;
        //uint32_t sw_refer15_field_e   : 1;
        uint32_t sw_refer15_base      : 32;
    } SwReg29;

    struct {
        uint32_t sw_refer0_nbr      : 16;
        uint32_t sw_refer1_nbr      : 16;
    } SwReg30;

    struct {
        uint32_t sw_refer2_nbr      : 16;
        uint32_t sw_refer3_nbr      : 16;
    } SwReg31;

    struct {
        uint32_t sw_refer4_nbr      : 16;
        uint32_t sw_refer5_nbr      : 16;
    } SwReg32;

    struct {
        uint32_t sw_refer6_nbr      : 16;
        uint32_t sw_refer7_nbr      : 16;
    } SwReg33;

    struct {
        uint32_t sw_refer8_nbr      : 16;
        uint32_t sw_refer9_nbr      : 16;
    } SwReg34;

    struct {
        uint32_t sw_refer10_nbr      : 16;
        uint32_t sw_refer11_nbr      : 16;
    } SwReg35;

    struct {
        uint32_t sw_refer12_nbr      : 16;
        uint32_t sw_refer13_nbr      : 16;
    } SwReg36;

    struct {
        uint32_t sw_refer14_nbr      : 16;
        uint32_t sw_refer15_nbr      : 16;
    } SwReg37;

    struct {
        uint32_t refpic_term_flag : 32;
    } SwReg38;

    struct {
        uint32_t refpic_valid_flag : 32;
    } SwReg39;

    struct {
        uint32_t qtable_st_adr : 32;
    } SwReg40;

    struct {
        uint32_t dmmv_st_adr : 32;
    } SwReg41;

    struct {
        uint32_t sw_binit_rlist_f0 : 5;
        uint32_t sw_binit_rlist_b0 : 5;
        uint32_t sw_binit_rlist_f1 : 5;
        uint32_t sw_binit_rlist_b1 : 5;
        uint32_t sw_binit_rlist_f2 : 5;
        uint32_t sw_binit_rlist_b2 : 5;
        uint32_t reverse0          : 2;
    } SwReg42;

    struct {
        uint32_t sw_binit_rlist_f3 : 5;
        uint32_t sw_binit_rlist_b3 : 5;
        uint32_t sw_binit_rlist_f4 : 5;
        uint32_t sw_binit_rlist_b4 : 5;
        uint32_t sw_binit_rlist_f5 : 5;
        uint32_t sw_binit_rlist_b5 : 5;
        uint32_t reverse0          : 2;
    } SwReg43;

    struct {
        uint32_t sw_binit_rlist_f6 : 5;
        uint32_t sw_binit_rlist_b6 : 5;
        uint32_t sw_binit_rlist_f7 : 5;
        uint32_t sw_binit_rlist_b7 : 5;
        uint32_t sw_binit_rlist_f8 : 5;
        uint32_t sw_binit_rlist_b8 : 5;
        uint32_t reverse0 : 2;
    } SwReg44;

    struct {
        uint32_t sw_binit_rlist_f9  : 5;
        uint32_t sw_binit_rlist_b9  : 5;
        uint32_t sw_binit_rlist_f10 : 5;
        uint32_t sw_binit_rlist_b10 : 5;
        uint32_t sw_binit_rlist_f11 : 5;
        uint32_t sw_binit_rlist_b11 : 5;
        uint32_t reverse0           : 2;
    } SwReg45;

    struct {
        uint32_t sw_binit_rlist_f12 : 5;
        uint32_t sw_binit_rlist_b12 : 5;
        uint32_t sw_binit_rlist_f13 : 5;
        uint32_t sw_binit_rlist_b13 : 5;
        uint32_t sw_binit_rlist_f14 : 5;
        uint32_t sw_binit_rlist_b14 : 5;
        uint32_t reverse0           : 2;
    } SwReg46;

    struct {
        uint32_t sw_binit_rlist_f15 : 5;
        uint32_t sw_binit_rlist_b15 : 5;
        uint32_t sw_pinit_rlist_f0  : 5;
        uint32_t sw_pinit_rlist_f1  : 5;
        uint32_t sw_pinit_rlist_f2  : 5;
        uint32_t sw_pinit_rlist_f3  : 5;
        uint32_t reverse0           : 2;
    } SwReg47;

    struct {
        uint32_t reverse0     : 15;
        uint32_t sw_startmb_y : 8;
        uint32_t sw_startmb_x : 9;
    } SwReg48;

    struct {
        uint32_t reverse0           : 2;
        uint32_t sw_pred_bc_tap_0_2 : 10;
        uint32_t sw_pred_bc_tap_0_1 : 10;
        uint32_t sw_pred_bc_tap_0_0 : 10;
    } SwReg49;

    uint32_t SwReg50_54[5];

    struct {
        uint32_t sw_apf_threshold : 14;
        uint32_t sw_refbu2_picid  : 5;
        uint32_t sw_refbu2_thr    : 12;
        uint32_t sw_refbu2_buf_e  : 1;
    } SwReg55;

    uint32_t SwReg56;

    struct {
        uint32_t sw_stream_len_hi  : 1;
        uint32_t sw_inter_dblspeed : 1;
        uint32_t sw_intra_dblspeed : 1;
        uint32_t sw_intra_dbl3t    : 1;
        uint32_t sw_paral_bus      : 1;
        uint32_t sw_axiwr_sel      : 1;
        uint32_t sw_pref_sigchan   : 1;
        uint32_t sw_cache_en       : 1;
        uint32_t reverse0          : 24;
    } SwReg57;

    uint32_t SwReg58_101[44];
} H264dVdpu1Regs_t;

/* Number registers for the decoder */
#define DEC_VDPU1_REGISTERS        (102)

#define VDPU1_CABAC_TAB_SIZE        (3680)        /* bytes */
#define VDPU1_POC_BUF_SIZE          (34*4)        /* bytes */
#define VDPU1_SCALING_LIST_SIZE     (6*16+2*64)   /* bytes */

typedef struct h264d_vdpu1_dpb_info_t {
    RK_U8     valid;

    RK_S32    slot_index;
    RK_U32    is_long_term;
    RK_S32    TOP_POC;
    RK_S32    BOT_POC;
    RK_U16    frame_num;
    RK_U32    long_term_frame_idx;
    RK_U32    long_term_pic_num;
    RK_U32    top_used;
    RK_U32    bot_used;
    RK_U32    view_id;
    RK_U32    colmv_is_used;
    RK_U32    field_flag;
    RK_U32    is_ilt_flag;
    RK_U32    voidx;

    RK_U8     have_same;
    RK_U32    new_dpb_idx;
} H264dVdpu1DpbInfo_t;

typedef struct h264d_vdpu1_ref_pic_info_t {
    RK_U32    valid;

    RK_S32    dpb_idx;
    RK_S32    bottom_flag;
} H264dVdpu1RefPicInfo_t;

typedef struct h264d_vdpu1_priv_t {
    RK_U32                 layed_id;

    H264dVdpu1DpbInfo_t     old_dpb[2][16];
    H264dVdpu1DpbInfo_t     new_dpb[16];
    H264dVdpu1DpbInfo_t     *ilt_dpb;
    H264dVdpu1RefPicInfo_t  refinfo[3][32]; //!< listP listB0 list1
} H264dVdpu1Priv_t;

#ifdef  __cplusplus
extern "C" {
#endif

extern const RK_U32 H264_VDPU1_Cabac_table[VDPU1_CABAC_TAB_SIZE / 4];

MPP_RET vdpu1_set_pic_regs(H264dHalCtx_t *p_hal, H264dVdpu1Regs_t  *p_regs);
MPP_RET vdpu1_set_vlc_regs(H264dHalCtx_t *p_hal, H264dVdpu1Regs_t  *p_regs);
MPP_RET vdpu1_set_ref_regs(H264dHalCtx_t *p_hal, H264dVdpu1Regs_t  *p_regs);
MPP_RET vdpu1_set_asic_regs(H264dHalCtx_t *p_hal, H264dVdpu1Regs_t *p_regs);

#ifdef  __cplusplus
}
#endif


#endif /* __HAL_H264D_VDPU1_PKT_H__ */



