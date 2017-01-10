/*
 * Copyright 2015 - 2017 Rockchip Electronics Co. LTD
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

#ifndef __HAL_H264E_VPU_COMMON_H__
#define __HAL_H264E_VPU_COMMON_H__

#include "mpp_hal.h"

#define BIT(n)  (1<<(n))

typedef struct h264e_hal_vpu_dump_files_t {
    FILE *fp_mpp_syntax_in;
    FILE *fp_mpp_reg_in;
    FILE *fp_mpp_reg_out;
    FILE *fp_mpp_strm_out;
    FILE *fp_mpp_feedback;
} h264e_hal_vpu_dump_files;

/* transplant from vpu_api.h:EncInputPictureType */
typedef enum {
    H264E_VPU_CSP_YUV420P   = 0,    // YYYY... UUUU... VVVV
    H264E_VPU_CSP_YUV420SP  = 1,    // YYYY... UVUVUV...
    H264E_VPU_CSP_YUYV422   = 2,    // YUYVYUYV...
    H264E_VPU_CSP_UYVY422   = 3,    // UYVYUYVY...
    H264E_VPU_CSP_RGB565    = 4,    // 16-bit RGB
    H264E_VPU_CSP_BGR565    = 5,    // 16-bit RGB
    H264E_VPU_CSP_RGB555    = 6,    // 15-bit RGB
    H264E_VPU_CSP_BGR555    = 7,    // 15-bit RGB
    H264E_VPU_CSP_RGB444    = 8,    // 12-bit RGB
    H264E_VPU_CSP_BGR444    = 9,    // 12-bit RGB
    H264E_VPU_CSP_RGB888    = 10,   // 24-bit RGB
    H264E_VPU_CSP_BGR888    = 11,   // 24-bit RGB
    H264E_VPU_CSP_RGB101010 = 12,   // 30-bit RGB
    H264E_VPU_CSP_BGR101010 = 13,   // 30-bit RGB
    H264E_VPU_CSP_NONE,
    H264E_VPU_CSP_BUTT,
} h264e_hal_vpu_csp;

/* struct for assemble bitstream */
typedef struct h264e_hal_vpu_stream_t {
    RK_U8 *buffer; /* point to first byte of stream */
    RK_U8 *stream; /* Pointer to next byte of stream */
    RK_U32 size;   /* Byte size of stream buffer */
    RK_U32 byte_cnt;    /* Byte counter */
    RK_U32 bit_cnt; /* Bit counter */
    RK_U32 byte_buffer; /* Byte buffer */
    RK_U32 buffered_bits;   /* Amount of bits in byte buffer, [0-7] */
    RK_U32 zero_bytes; /* Amount of consecutive zero bytes */
    RK_S32 overflow;    /* This will signal a buffer overflow */
    RK_U32 emul_cnt; /* Counter for emulation_3_byte, needed in SEI */
} h264e_hal_vpu_stream;

typedef struct h264e_hal_vpu_extra_info_t {
    h264e_hal_vpu_stream sps_stream;
    h264e_hal_vpu_stream pps_stream;
    h264e_hal_sps sps;
    h264e_hal_pps pps;
} h264e_hal_vpu_extra_info;

typedef enum h264e_hal_vpu_buf_grp_t {
    H264E_HAL_VPU_BUF_GRP_REC,
    H264E_HAL_VPU_BUF_GRP_CABAC_TBL,
    H264E_HAL_VPU_BUF_GRP_NALSIZE_TBL,
    H264E_HAL_VPU_BUF_GRP_BUTT,
} h264e_hal_vpu_buf_grp;

typedef struct h264e_hal_vpu_buffers_t {
    MppBufferGroup hw_buf_grp;

    MppBuffer hw_rec_buf[2];
    MppBuffer hw_cabac_table_buf;
    MppBuffer hw_nal_size_table_buf;
} h264e_hal_vpu_buffers;

#endif
