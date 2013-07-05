/*
 *  arm_decode.c
 *  SDMSymbolTable
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDMARMDECODE_C_
#define _SDMARMDECODE_C_

#include "arm_decode.h"
#include <math.h>

#pragma mark -
#pragma mark ARM
#pragma mark -
#pragma mark Condition Code
#define ARM_CONDITION_CODE(data) ((data & 0xF0000000) >> 28)

#pragma mark OP Instrunction
#define ARM_OP(data) ((data & 0x0F000000) >> 24)
#define ARM_OP_ALT(data) (((data & 0x000000F0) >> 4) & 1)
#define ARM_OP_0(data) (data >> 3)
#define ARM_OP_1(data) ((data >> 2) & 1)
#define ARM_OP_2(data) ((data >> 1) & 1)
#define ARM_OP_3(data) (data & 1)

#pragma mark Data-Processing
#define ARM_DP_OP0(data) (((data & 0x0F000000) >> 25) & 1)
#define ARM_DP_OP1(data) (((data & 0x0FF00000) >> 20) & 0x1F)
#define ARM_DP_OP2(data) ((data & 0x000000F0) >> 4)

#pragma mark Load/Store
#define ARM_LS_A(data) (((data & 0x0F000000) >> 25) & 1)
#define ARM_LS_OP1(data) (((data & 0x0FF00000) >> 20) & 0x1F)
#define ARM_LS_B(data) (((data & 0x000000F0) >> 4) & 1)
#define ARM_LS_Rn(data) ((data & 0x000F0000) >> 16)

#pragma mark Media
#define ARM_MEDIA_OP1(data) (((data & 0x0FF00000) >> 20) & 0x1F)
#define ARM_MEDIA_OP2(data) (((data & 0x000000F0) >> 4) & 0x7)
#define ARM_MEDIA_Rd(data) ((data & 0x0000F000) >> 12)
#define ARM_MEDIA_Rn(data) ((data & 0x0000000F) >> 20)

#pragma mark Branch
#define ARM_BRANCH_OP1(data) (((data & 0x0FF00000) >> 20) & 0x3F)
#define ARM_BRANCH_Rn(data) ((data & 0x000F0000) >> 16)
#define ARM_BRANCH_R(data) (((data & 0x0000F000) >> 15) & 1)

#pragma mark Coprocessor
#define ARM_CO_OP1(data) (((data & 0x0FF00000) >> 20) & 0x3F)
#define ARM_CO_Rn(data) ((data & 0x000F0000) >> 16)
#define ARM_CO_CoProc(data) ((data & 0x00000F00) >> 8)
#define ARM_CO_OP2(data) (((data & 0x0000000F) >> 3) & 1)

#pragma mark Unconditional
#define UC_OP1(data) ((data & 0x0FF00000) >> 20)
#define UC_Rn(data) ((data & 0x000F0000) >> 16)
#define CO_OP2(data) (((data & 0x0000000F) >> 3) & 1)

#pragma mark Instruction Classes
#define ARM_IsConditional(data) (ARM_CONDITION_CODE(data) != 0xF)
#define ARM_IsDataProcessing(data) (ARM_OP_0(ARM_OP(data)) == 0 && ARM_OP_1(ARM_OP(data)) == 0)
#define ARM_IsLoadStore(data) (ARM_OP_0(ARM_OP(data)) == 0 && ARM_OP_1(ARM_OP(data)) == 1 && (ARM_OP_2(ARM_OP(data)) == 0 || (ARM_OP_2(ARM_OP(data)) == 1 && ARM_OP_ALT(data) == 0)))
#define ARM_IsMedia(data) (ARM_OP_0(ARM_OP(data)) == 0 && ARM_OP_1(ARM_OP(data)) == 1 && ARM_OP_2(ARM_OP(data)) == 1 && ARM_OP_ALT(data) == 1)
#define ARM_IsBranch(data) (ARM_OP_0(ARM_OP(data)) == 1 && ARM_OP_1(ARM_OP(data)) == 0)
#define ARM_IsCoProc(data) (ARM_OP_0(ARM_OP(data)) == 1 && ARM_OP_1(ARM_OP(data)) == 1)

#define ARM_IsSIMDDataProcessing(data) (data)

#define ARM_IsFloatingDataProcessing(data) (data)


#pragma mark -
#pragma mark THUMB
#pragma mark -

#pragma mark Op Code
#define THUMB_OPCode(data) (((data & 0xFF00) >> 10) & 0x3F)
#define THUMB_OP0(data) ((THUMB_OPCode(data) >> 5) & 1)
#define THUMB_OP1(data) ((THUMB_OPCode(data) >> 4) & 1)
#define THUMB_OP2(data) ((THUMB_OPCode(data) >> 3) & 1)
#define THUMB_OP3(data) ((THUMB_OPCode(data) >> 2) & 1)
#define THUMB_OP4(data) ((THUMB_OPCode(data) >> 1) & 1)
#define THUMB_OP5(data) ((THUMB_OPCode(data)) & 1)

#define THUMB_16_BL_OPCode(data) (((data & 0xFF00) >> 9) & 0x1F)
#define THUMB_16_BL_OP0(data) ((THUMB_16_BL_OPCode(data) >> 4) & 1)
#define THUMB_16_BL_OP1(data) ((THUMB_16_BL_OPCode(data) >> 3) & 1)
#define THUMB_16_BL_OP2(data) ((THUMB_16_BL_OPCode(data) >> 2) & 1)
#define THUMB_16_BL_OP3(data) ((THUMB_16_BL_OPCode(data) >> 1) & 1)
#define THUMB_16_BL_OP4(data) ((THUMB_16_BL_OPCode(data)) & 1)

#define THUMB_16_BL_MOVCHECK(data) (((data & 0x00F0) >> 6) & 0x7)

#define THUMB_16_DP_OPCode(data) (((data & 0x0FF0) >> 5) & 0xF)
#define THUMB_16_DP_OP0(data) ((THUMB_16_DP_OPCode(data) >> 3) & 1)
#define THUMB_16_DP_OP1(data) ((THUMB_16_DP_OPCode(data) >> 2) & 1)
#define THUMB_16_DP_OP2(data) ((THUMB_16_DP_OPCode(data) >> 1) & 1)
#define THUMB_16_DP_OP3(data) ((THUMB_16_DP_OPCode(data)) & 1)

#define THUMB_16_SD_OPCode(data) (((data & 0x0FF0) >> 5) & 0xF)
#define THUMB_16_SD_OP0(data) ((THUMB_16_DP_OPCode(data) >> 3) & 1)
#define THUMB_16_SD_OP1(data) ((THUMB_16_DP_OPCode(data) >> 2) & 1)
#define THUMB_16_SD_OP2(data) ((THUMB_16_DP_OPCode(data) >> 1) & 1)
#define THUMB_16_SD_OP3(data) ((THUMB_16_DP_OPCode(data)) & 1)

#define THUMB_16_LS_OPACode(data) (((data & 0xF000) >> 12) & 0xF)
#define THUMB_16_LS_OPA0(data) ((data >> 3) & 1)
#define THUMB_16_LS_OPA1(data) ((data >> 2) & 1)
#define THUMB_16_LS_OPA2(data) ((data >> 1) & 1)
#define THUMB_16_LS_OPA3(data) ((data) & 1)
#define THUMB_16_LS_OPBCode(data) (((data & 0x0F00) >> 9) & 0x7)
#define THUMB_16_LS_OPB0(data) ((data >> 2) & 1)
#define THUMB_16_LS_OPB1(data) ((data >> 1) & 1)
#define THUMB_16_LS_OPB2(data) ((data) & 1)

#define THUMB_16_MS_OPCode(data) (((data & 0x0FF0) >> 5) & 0x7F)
#define THUMB_16_MS_OP0(data) ((THUMB_16_MS_OPCode(data) >> 6) & 1)
#define THUMB_16_MS_OP1(data) ((THUMB_16_MS_OPCode(data) >> 5) & 1)
#define THUMB_16_MS_OP2(data) ((THUMB_16_MS_OPCode(data) >> 4) & 1)
#define THUMB_16_MS_OP3(data) ((THUMB_16_MS_OPCode(data) >> 3) & 1)
#define THUMB_16_MS_OP4(data) ((THUMB_16_MS_OPCode(data) >> 2) & 1)
#define THUMB_16_MS_OP5(data) ((THUMB_16_MS_OPCode(data) >> 1) & 1)
#define THUMB_16_MS_OP6(data) ((THUMB_16_MS_OPCode(data)) & 1)

#pragma mark Type
#define THUMB_Is32Bit(data) (THUMB_OPCode(data) == 0x1F || THUMB_OPCode(data) == 0x1E || THUMB_OPCode(data) == 0x1D)
#define THUMB_Is16Bit(data) (!THUMB_Is32Bit(data))

#pragma mark 16Bit Classes
#define THUMB_16_BasicLogic(data) (THUMB_OP0(data) == 0 && THUMB_OP1(data) == 0)
#define THUMB_16_DataProcessing(data) (THUMB_OP0(data) == 0 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 0 && THUMB_OP5(data) == 0)
#define THUMB_16_SpecialData(data) (THUMB_OP0(data) == 0 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 0 && THUMB_OP5(data) == 1)
#define THUMB_16_LoadLiteral(data) (THUMB_OP0(data) == 0 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 1)
#define THUMB_16_LoadStore(data) ((THUMB_OP0(data) == 0 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 1) || (THUMB_OP0(data) == 0 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 1) || (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 0 && THUMB_OP2(data) == 0))
#define THUMB_16_PCAddress(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 0 && THUMB_OP2(data) == 1 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 0)
#define THUMB_16_SPAddress(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 0 && THUMB_OP2(data) == 1 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 1)
#define THUMB_16_MiscInstructions(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 0 && THUMB_OP2(data) == 1 && THUMB_OP3(data) == 1)
#define THUMB_16_StoreMulti(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 0)
#define THUMB_16_LoadMulti(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 1)
#define THUMB_16_ConditionalBranch(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 0 && THUMB_OP3(data) == 1)
#define THUMB_16_UnconditionalBranch(data) (THUMB_OP0(data) == 1 && THUMB_OP1(data) == 1 && THUMB_OP2(data) == 1 && THUMB_OP3(data) == 0 && THUMB_OP4(data) == 0)

#define THUMB_16_TableResolve(data) ((THUMB_16_UnconditionalBranch(data) << 11) + (THUMB_16_ConditionalBranch(data) << 10) + (THUMB_16_LoadMulti(data) << 9) + (THUMB_16_StoreMulti(data) << 8) + (THUMB_16_MiscInstructions(data) << 7) + (THUMB_16_SPAddress(data) << 6) + (THUMB_16_PCAddress(data) << 5) + (THUMB_16_LoadStore(data) << 4) + (THUMB_16_LoadLiteral(data) << 3) + (THUMB_16_SpecialData(data) << 2) + (THUMB_16_DataProcessing(data) << 1) + THUMB_16_BasicLogic(data))
#pragma mark -
#define THUMB_16_BL_LShiftLeft(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 0 && THUMB_16_BL_OP2(data) == 0) && THUMB_16_BL_MOVCHECK(data) != 0)
#define THUMB_16_BL_LShiftRight(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 0 && THUMB_16_BL_OP2(data) == 1))
#define THUMB_16_BL_AShiftRight(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 0))
#define THUMB_16_BL_Add(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 1 && THUMB_16_BL_OP3(data) == 0 && THUMB_16_BL_OP4(data) == 0))
#define THUMB_16_BL_Sub(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 1 && THUMB_16_BL_OP3(data) == 0 && THUMB_16_BL_OP4(data) == 1))
#define THUMB_16_BL_AddThreeBit(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 1 && THUMB_16_BL_OP3(data) == 1 && THUMB_16_BL_OP4(data) == 0))
#define THUMB_16_BL_SubThreeBit(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 1 && THUMB_16_BL_OP3(data) == 1 && THUMB_16_BL_OP4(data) == 1))
#define THUMB_16_BL_Mov(data) (THUMB_16_BasicLogic(data) && ((THUMB_16_BL_OP0(data) == 1 && THUMB_16_BL_OP1(data) == 0 && THUMB_16_BL_OP2(data) == 0) || ((THUMB_16_BL_OP0(data) == 0 && THUMB_16_BL_OP1(data) == 0 && THUMB_16_BL_OP2(data) == 0 && THUMB_16_BL_OP3(data) == 0 && THUMB_16_BL_OP4(data) == 0) && THUMB_16_BL_MOVCHECK(data) == 0)))
#define THUMB_16_BL_Cmp(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 1 && THUMB_16_BL_OP1(data) == 0 && THUMB_16_BL_OP2(data) == 1))
#define THUMB_16_BL_AddEightBit(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 1 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 0))
#define THUMB_16_BL_SubEightBit(data) (THUMB_16_BasicLogic(data) && (THUMB_16_BL_OP0(data) == 1 && THUMB_16_BL_OP1(data) == 1 && THUMB_16_BL_OP2(data) == 1))

#define THUMB_16_BL_Resolve(data) ((THUMB_16_BL_SubEightBit(data) << 10) + (THUMB_16_BL_AddEightBit(data) << 9) + (THUMB_16_BL_Cmp(data) << 8) + (THUMB_16_BL_Mov(data) << 7) + (THUMB_16_BL_SubThreeBit(data) << 6) + (THUMB_16_BL_AddThreeBit(data) << 5) + (THUMB_16_BL_Sub(data) << 4) + (THUMB_16_BL_Add(data) << 3) + (THUMB_16_BL_AShiftRight(data) << 2) + (THUMB_16_BL_LShiftRight(data) << 1) + (THUMB_16_BL_LShiftLeft(data)))

#pragma mark -
#define THUMB_16_DP_And(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_Eor(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_LShiftLeft(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_LShiftRight(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_AShiftRight(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_AddCarry(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_SubCarry(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_RotateRight(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 0 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_Test(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_Cmp(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_CmpNeg(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 0 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_BitOr(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_Mul(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 0 && THUMB_16_DP_OP3(data) == 1))
#define THUMB_16_DP_BitClear(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 0))
#define THUMB_16_DP_BitNot(data) (THUMB_16_DataProcessing(data) && (THUMB_16_DP_OP0(data) == 1 && THUMB_16_DP_OP1(data) == 1 && THUMB_16_DP_OP2(data) == 1 && THUMB_16_DP_OP3(data) == 1))

#define THUMB_16_DP_Resolve(data) ((THUMB_16_DP_BitNot(data) << 14) + (THUMB_16_DP_BitClear(data) << 13) + (THUMB_16_DP_Mul(data) << 12) + (THUMB_16_DP_BitOr(data) << 11) + (THUMB_16_DP_CmpNeg(data) << 10) + (THUMB_16_DP_Cmp(data) << 9) + (THUMB_16_DP_Test(data) << 8) + (THUMB_16_DP_RotateRight(data) << 7) + (THUMB_16_DP_SubCarry(data) << 6) + (THUMB_16_DP_AddCarry(data) << 5) + (THUMB_16_DP_AShiftRight(data) << 4) + (THUMB_16_DP_LShiftRight(data) << 3) + (THUMB_16_DP_LShiftLeft(data) << 2) + (THUMB_16_DP_Eor(data) << 1) + (THUMB_16_DP_And(data)))
#pragma mark -
#define THUMB_16_SD_AddLow(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 0 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 0 && THUMB_16_SD_OP3(data) == 0))
#define THUMB_16_SD_AddHigh(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 0 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 0 && THUMB_16_SD_OP3(data) == 1) || (THUMB_16_SD_OP0(data) == 0 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 1))
#define THUMB_16_SD_CmpHigh(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 0 && THUMB_16_SD_OP1(data) == 1))
#define THUMB_16_SD_MovLow(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 1 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 0 && THUMB_16_SD_OP3(data) == 0))
#define THUMB_16_SD_MovHigh(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 1 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 0 && THUMB_16_SD_OP3(data) == 1) || (THUMB_16_SD_OP0(data) == 1 && THUMB_16_SD_OP1(data) == 0 && THUMB_16_SD_OP2(data) == 1))
#define THUMB_16_SD_BranchExchange(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 1 && THUMB_16_SD_OP1(data) == 1 && THUMB_16_SD_OP2(data) == 0))
#define THUMB_16_SD_BranchLink(data) (THUMB_16_SpecialData(data) && (THUMB_16_SD_OP0(data) == 1 && THUMB_16_SD_OP1(data) == 1 && THUMB_16_SD_OP2(data) == 1))

#define THUMB_16_SD_Resolve(data) ((THUMB_16_SD_BranchLink(data) << 6) + (THUMB_16_SD_BranchExchange(data) << 5) + (THUMB_16_SD_MovHigh(data) << 4) + (THUMB_16_SD_MovLow(data) << 3) + (THUMB_16_SD_CmpHigh(data) << 2) + (THUMB_16_SD_AddHigh(data) << 1) + (THUMB_16_SD_AddLow(data)))
#pragma mark -
#define THUMB_16_LL_Resolve(data) (THUMB_16_LoadLiteral(data))
#pragma mark -
#define THUMB_16_LS_Str(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 0)
#define THUMB_16_LS_StrHalf(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 1)
#define THUMB_16_LS_StrByte(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 2)
#define THUMB_16_LS_StrSByte(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 3)
#define THUMB_16_LS_Load(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 4)
#define THUMB_16_LS_LoadHalf(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 5)
#define THUMB_16_LS_LoadByte(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 6)
#define THUMB_16_LS_LoadSHalf(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 5) && THUMB_16_LS_OPBCode(data) == 7)
#define THUMB_16_LS_StrIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 6) && THUMB_16_LS_OPB0(data) == 0)
#define THUMB_16_LS_LoadIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 6) && THUMB_16_LS_OPB0(data) == 1)
#define THUMB_16_LS_StrByteIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 7) && THUMB_16_LS_OPB0(data) == 0)
#define THUMB_16_LS_LoadByteIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 7) && THUMB_16_LS_OPB0(data) == 1)
#define THUMB_16_LS_StrHalfIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 8) && THUMB_16_LS_OPB0(data) == 0)
#define THUMB_16_LS_LoadHalfIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 8) && THUMB_16_LS_OPB0(data) == 1)
#define THUMB_16_LS_StrSPIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 9) && THUMB_16_LS_OPB0(data) == 0)
#define THUMB_16_LS_LoadSPIT(data) ((THUMB_16_LoadStore(data) && THUMB_16_LS_OPACode(data) == 9) && THUMB_16_LS_OPB0(data) == 1)

#define THUMB_16_LS_Resolve(data) ((THUMB_16_LS_LoadSPIT(data) << 15) + (THUMB_16_LS_StrSPIT(data) << 14) + (THUMB_16_LS_LoadHalfIT(data) << 13) + (THUMB_16_LS_StrHalfIT(data) << 12) + (THUMB_16_LS_LoadByteIT(data) << 11) + (THUMB_16_LS_StrByteIT(data) << 10) + (THUMB_16_LS_LoadIT(data) << 9) + (THUMB_16_LS_StrIT(data) << 8) + (THUMB_16_LS_LoadSHalf(data) << 7) + (THUMB_16_LS_LoadByte(data) << 6) + (THUMB_16_LS_LoadHalf(data) << 5) + (THUMB_16_LS_Load(data) << 4) + (THUMB_16_LS_StrSByte(data) << 3) + (THUMB_16_LS_StrByte(data) << 2) + (THUMB_16_LS_StrHalf(data) << 1) + (THUMB_16_LS_Str(data)))
#pragma mark -
#define THUMB_16_PC_Resolve(data) (THUMB_16_PCAddress(data))
#pragma mark -
#define THUMB_16_SP_Resolve(data) (THUMB_16_SPAddress(data))
#pragma mark -
#define THUMB_16_MS_Add(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 0 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 0))
#define THUMB_16_MS_Sub(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 0 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 1))
#define THUMB_16_MS_CmpZ0(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 0 && THUMB_16_MS_OP3(data) == 1))
#define THUMB_16_MS_ExtenSHalf(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 0 && THUMB_16_MS_OP5(data) == 0))
#define THUMB_16_MS_ExtenSByte(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 0 && THUMB_16_MS_OP5(data) == 1))
#define THUMB_16_MS_ExtenHalf(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 1 && THUMB_16_MS_OP5(data) == 0))
#define THUMB_16_MS_ExtenByte(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 1 && THUMB_16_MS_OP5(data) == 1))
#define THUMB_16_MS_CmpZ1(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 1))
#define THUMB_16_MS_PushMulti(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 0 && THUMB_16_MS_OP1(data) == 1 && THUMB_16_MS_OP2(data) == 0))
#define THUMB_16_MS_SetEndian(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OPCode(data) == 50))
#define THUMB_16_MS_ProcState(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OPCode(data) == 51))
#define THUMB_16_MS_CmpNZ0(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 0 && THUMB_16_MS_OP3(data) == 1))
#define THUMB_16_MS_ReverseWord(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 0 && THUMB_16_MS_OP5(data) == 0))
#define THUMB_16_MS_ReversePHalf(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 0 && THUMB_16_MS_OP5(data) == 1))
#define THUMB_16_MS_ReverseSHalf(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0 && THUMB_16_MS_OP4(data) == 1 && THUMB_16_MS_OP5(data) == 1))
#define THUMB_16_MS_CmpNZ1(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 0 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 1))
#define THUMB_16_MS_PopMulti(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 1 && THUMB_16_MS_OP2(data) == 0))
#define THUMB_16_MS_Break(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 1 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 0))
#define THUMB_16_MS_IfThen(data) (THUMB_16_MiscInstructions(data) && (THUMB_16_MS_OP0(data) == 1 && THUMB_16_MS_OP1(data) == 1 && THUMB_16_MS_OP2(data) == 1 && THUMB_16_MS_OP3(data) == 1))

#define THUMB_16_MS_Resolve(data) ((THUMB_16_MS_IfThen(data) << 18) + (THUMB_16_MS_Break(data) << 17) + (THUMB_16_MS_PopMulti(data) << 16) + (THUMB_16_MS_CmpNZ1(data) << 15) + (THUMB_16_MS_ReverseSHalf(data) << 14) + (THUMB_16_MS_ReversePHalf(data) << 13) + (THUMB_16_MS_ReverseWord(data) << 12) + (THUMB_16_MS_CmpNZ0(data) << 11) + (THUMB_16_MS_ProcState(data) << 10) + (THUMB_16_MS_SetEndian(data) << 9) + (THUMB_16_MS_PushMulti(data) << 8) + (THUMB_16_MS_CmpZ1(data) << 7) + (THUMB_16_MS_ExtenByte(data) << 6) + (THUMB_16_MS_ExtenHalf(data) << 5) + (THUMB_16_MS_ExtenSByte(data) << 4) + (THUMB_16_MS_ExtenSHalf(data) << 3) + (THUMB_16_MS_CmpZ0(data) << 2) + (THUMB_16_MS_Sub(data) << 1) + (THUMB_16_MS_Add(data)))
#pragma mark -

#define THUMB_16_SM_Resolve(data) (THUMB_16_StoreMulti(data))
#pragma mark -
#define THUMB_16_LM_Resolve(data) (THUMB_16_LoadMulti(data))
#pragma mark -
#define THUMB_16_CB_Resolve(data) (data)
#pragma mark -
#define THUMB_16_UB_Resolve(data) (data)
#pragma mark -
#pragma mark Translated Instrunctions
#pragma mark -

typedef enum THUMB_16_Table {
	BasicLogic = 1,
	DataProcessing = 2,
	SpecialData = 4,
	LoadLiteral = 8,
	LoadStore = 16,
	PCAddress = 32,
	SPAddress = 64,
	Misc = 128,
	StoreMulti = 256,
	LoadMulti = 512,
	ConditionalBranch = 1024,
	UnconditionalBranch = 2048
} THUMB_16_Table;

typedef struct THUMB_16_OpCode {
	uint32_t opcode;
	char *instruction;
} __attribute__ ((packed)) THUMB_16_OpCode;

typedef enum TableNames {BL=0,DP=1,SD=2,LL=3,LS=4,PC=5,SP=6,MS=7,SM=8,LM=9,CB=10,UB=11} TableNames;
#define THUMB_16_TableNumberResolve(data) (uint32_t)log2l(THUMB_16_TableResolve(data))

static THUMB_16_OpCode THUMB_16_BasicLogicTable[11] = {
	{0,"lsl"},
	{4,"lsr"},
	{8,"asr"},
	{12,"add"},
	{13,"sub"},
	{14,"add"},
	{15,"sub"},
	{16,"mov"},
	{20,"cmp"},
	{24,"add"},
	{28,"sub"}
};

static THUMB_16_OpCode THUMB_16_DataProcessingTable[16] = {
	{0,"and"},
	{1,"eor"},
	{2,"lsl"},
	{3,"lsr"},
	{4,"asr"},
	{5,"adc"},
	{6,"sbc"},
	{7,"ror"},
	{8,"tst"},
	{9,"rsb"},
	{10,"cmp"},
	{11,"cmn"},
	{12,"orr"},
	{13,"mul"},
	{14,"bic"},
	{15,"mvn"}
};

static THUMB_16_OpCode THUMB_16_SpeciaDataTable[7] = {
	{0,"add"},
	{1,"add"},
	{4,"cmp"},
	{8,"mov"},
	{10,"mov"},
	{12,"bx"},
	{14,"blx"}
};

static THUMB_16_OpCode THUMB_16_LoadLiteralTable[1] = {
	{0,"ldr"}
};

static THUMB_16_OpCode THUMB_16_LoadStoreTable[16] = {
	{0,"str"},
	{1,"strh"},
	{2,"strb"},
	{3,"ldrsb"},
	{4,"ldr"},
	{5,"ldrh"},
	{6,"ldrb"},
	{7,"ldrsh"},
	{3,"str"},
	{4,"ldr"},
	{3,"strb"},
	{4,"ldrb"},
	{3,"strh"},
	{4,"ldrh"},
	{3,"str"},
	{4,"ldr"}
};

static THUMB_16_OpCode THUMB_16_PCAddressTable[1] = {
	{0,"adr"}
};

static THUMB_16_OpCode THUMB_16_SPAddressTable[1] = {
	{0,"add"}
};

static THUMB_16_OpCode THUMB_16_MiscTable[19] = {
	{3,"add"},
	{4,"sub"},
	{8,"cbz"},
	{16,"sxth"},
	{18,"sxtb"},
	{20,"uxth"},
	{22,"uxtb"},
	{24,"cbz"},
	{32,"push"},
	{50,"setend"},
	{51,"cps"},
	{72,"cbnz"},
	{80,"rev"},
	{82,"rev16"},
	{86,"revsh"},
	{88,"cbnz"},
	{96,"pop"},
	{112,"bkpt"},
	{120,"ifthen"}
};

static THUMB_16_OpCode THUMB_16_StoreMultiTable[3] = {
	{0,"stm"},
	{1,"stmia"},
	{2,"stmea"}
};

static THUMB_16_OpCode THUMB_16_LoadMultiTable[3] = {
	{0,"ldm"},
	{1,"ldmia"},
	{2,"ldmfd"}
};

static THUMB_16_OpCode *THUMB_16_MasterTable[7] = {
	THUMB_16_BasicLogicTable,
	THUMB_16_DataProcessingTable,
	THUMB_16_SpeciaDataTable,
	THUMB_16_LoadLiteralTable,
	THUMB_16_LoadStoreTable,
	THUMB_16_PCAddressTable,
	THUMB_16_SPAddressTable,
	THUMB_16_MiscTable,
	THUMB_16_StoreMultiTable,
	THUMB_16_LoadMultiTable
};

THUMB_16_OpCode arm_decode_opcode(uint32_t data) {
	if (THUMB_Is16Bit(data)) {
		THUMB_16_Table tableNum = THUMB_16_TableNumberResolve(data);
		uint32_t opcode;
		switch (tableNum) {
			case BL: {opcode = (uint32_t)log2l(THUMB_16_BL_Resolve(data)); break;};
			case DP: {opcode = (uint32_t)log2l(THUMB_16_DP_Resolve(data)); break;};
			case SD: {opcode = (uint32_t)log2l(THUMB_16_SD_Resolve(data)); break;};
			case LL: {opcode = (uint32_t)log2l(THUMB_16_LL_Resolve(data)); break;};
			case LS: {opcode = (uint32_t)log2l(THUMB_16_LS_Resolve(data)); break;};
			case PC: {opcode = (uint32_t)log2l(THUMB_16_PC_Resolve(data)); break;};
			case SP: {opcode = (uint32_t)log2l(THUMB_16_SP_Resolve(data)); break;};
			case MS: {opcode = (uint32_t)log2l(THUMB_16_MS_Resolve(data)); break;};
			case SM: {opcode = (uint32_t)log2l(THUMB_16_SM_Resolve(data)); break;};
			case LM: {opcode = (uint32_t)log2l(THUMB_16_LM_Resolve(data)); break;};
			case CB: {opcode = (uint32_t)log2l(THUMB_16_CB_Resolve(data)); break;};
			case UB: {opcode = (uint32_t)log2l(THUMB_16_UB_Resolve(data)); break;};
			default: break;
		}
		return THUMB_16_MasterTable[tableNum][opcode];
	} else if (THUMB_Is32Bit(data)) {
		return;
	}
}

char* arm_decode_thumb_16_registers(THUMB_16_OpCode code, uint32_t data) {
	char *decode = calloc(1, strlen(code.instruction)+1);
	strncpy(decode ,code.instruction, strlen(code.instruction));
	switch (code.opcode) {
		
	}
	return decode;
}

#endif