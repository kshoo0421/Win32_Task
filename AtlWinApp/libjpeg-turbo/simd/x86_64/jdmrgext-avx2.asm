;
; Merged upsampling/color conversion (64-bit AVX2)
;
; Copyright 2009, 2012 Pierre Ossman <ossman@cendio.se> for Cendio AB
; Copyright (C) 2009, 2012, 2016, 2024-2025, D. R. Commander.
; Copyright (C) 2015, Intel Corporation.
; Copyright (C) 2018, Matthias Räncker.
; Copyright (C) 2023, Aliaksiej Kandracienka.
;
; Based on the x86 SIMD extension for IJG JPEG library
; Copyright (C) 1999-2006, MIYASAKA Masaru.
; For conditions of distribution and use, see copyright notice in jsimdext.inc
;
; This file should be assembled with NASM (Netwide Assembler) or Yasm.

%include "jcolsamp.inc"

; --------------------------------------------------------------------------
;
; Upsample and color convert for the case of 2:1 horizontal and 1:1 vertical.
;
; GLOBAL(void)
; jsimd_h2v1_merged_upsample_avx2(JDIMENSION output_width,
;                                 JSAMPIMAGE input_buf,
;                                 JDIMENSION in_row_group_ctr,
;                                 JSAMPARRAY output_buf)
;
; r10d = JDIMENSION output_width
; r11 = JSAMPIMAGE input_buf
; r12d = JDIMENSION in_row_group_ctr
; r13 = JSAMPARRAY output_buf

%define wk(i)   r15 - (WK_NUM - (i)) * SIZEOF_YMMWORD  ; ymmword wk[WK_NUM]
%define WK_NUM  3

    align       32
    GLOBAL_FUNCTION(jsimd_h2v1_merged_upsample_avx2)

EXTN(jsimd_h2v1_merged_upsample_avx2):
    ENDBR64
    push        rbp
    mov         rbp, rsp
    push        r15
    and         rsp, byte (-SIZEOF_YMMWORD)  ; align to 256 bits
    ; Allocate stack space for wk array.  r15 is used to access it.
    mov         r15, rsp
    sub         rsp, SIZEOF_YMMWORD * WK_NUM
    COLLECT_ARGS 4
    push        rbx

    mov         ecx, r10d               ; col
    test        rcx, rcx
    jz          near .return

    push        rcx

    mov         rdi, r11
    mov         ecx, r12d
    mov         rsip, JSAMPARRAY [rdi + 0 * SIZEOF_JSAMPARRAY]
    mov         rbxp, JSAMPARRAY [rdi + 1 * SIZEOF_JSAMPARRAY]
    mov         rdxp, JSAMPARRAY [rdi + 2 * SIZEOF_JSAMPARRAY]
    mov         rdi, r13
    mov         rsip, JSAMPROW [rsi + rcx * SIZEOF_JSAMPROW]  ; inptr0
    mov         rbxp, JSAMPROW [rbx + rcx * SIZEOF_JSAMPROW]  ; inptr1
    mov         rdxp, JSAMPROW [rdx + rcx * SIZEOF_JSAMPROW]  ; inptr2
    mov         rdip, JSAMPROW [rdi]                          ; outptr

    pop         rcx                     ; col

.columnloop:

    vmovdqu     ymm6, YMMWORD [rbx]
                ; ymm6 = Cb(0123456789abcdefghijklmnopqrstuv)
    vmovdqu     ymm7, YMMWORD [rdx]
                ; ymm7 = Cr(0123456789abcdefghijklmnopqrstuv)

    vpxor       ymm1, ymm1, ymm1  ; ymm1 = (all 0's)
    vpcmpeqw    ymm3, ymm3, ymm3
    vpsllw      ymm3, ymm3, 7     ; ymm3 = { 0xFF80 0xFF80 0xFF80 0xFF80 .. }

    vpermq      ymm6, ymm6, 0xd8  ; ymm6 = Cb(01234567ghijklmn89abcdefopqrstuv)
    vpermq      ymm7, ymm7, 0xd8  ; ymm7 = Cr(01234567ghijklmn89abcdefopqrstuv)
    vpunpcklbw  ymm4, ymm6, ymm1  ; ymm4 = Cb(0123456789abcdef) = CbL
    vpunpckhbw  ymm6, ymm6, ymm1  ; ymm6 = Cb(ghijklmnopqrstuv) = CbH
    vpunpcklbw  ymm0, ymm7, ymm1  ; ymm0 = Cr(0123456789abcdef) = CrL
    vpunpckhbw  ymm7, ymm7, ymm1  ; ymm7 = Cr(ghijklmnopqrstuv) = CrH

    vpaddw      ymm5, ymm6, ymm3
    vpaddw      ymm2, ymm4, ymm3
    vpaddw      ymm1, ymm7, ymm3
    vpaddw      ymm3, ymm0, ymm3

    ; (Original)
    ; R = Y                + 1.40200 * Cr
    ; G = Y - 0.34414 * Cb - 0.71414 * Cr
    ; B = Y + 1.77200 * Cb
    ;
    ; (This implementation)
    ; R = Y                + 0.40200 * Cr + Cr
    ; G = Y - 0.34414 * Cb + 0.28586 * Cr - Cr
    ; B = Y - 0.22800 * Cb + Cb + Cb

    vpaddw      ymm6, ymm5, ymm5        ; ymm6 = 2 * CbH
    vpaddw      ymm4, ymm2, ymm2        ; ymm4 = 2 * CbL
    vpaddw      ymm7, ymm1, ymm1        ; ymm7 = 2 * CrH
    vpaddw      ymm0, ymm3, ymm3        ; ymm0 = 2 * CrL

    vpmulhw     ymm6, ymm6, [rel PW_MF0228]  ; ymm6 = (2 * CbH * -FIX(0.22800))
    vpmulhw     ymm4, ymm4, [rel PW_MF0228]  ; ymm4 = (2 * CbL * -FIX(0.22800))
    vpmulhw     ymm7, ymm7, [rel PW_F0402]   ; ymm7 = (2 * CrH * FIX(0.40200))
    vpmulhw     ymm0, ymm0, [rel PW_F0402]   ; ymm0 = (2 * CrL * FIX(0.40200))

    vpaddw      ymm6, ymm6, [rel PW_ONE]
    vpaddw      ymm4, ymm4, [rel PW_ONE]
    vpsraw      ymm6, ymm6, 1           ; ymm6 = (CbH * -FIX(0.22800))
    vpsraw      ymm4, ymm4, 1           ; ymm4 = (CbL * -FIX(0.22800))
    vpaddw      ymm7, ymm7, [rel PW_ONE]
    vpaddw      ymm0, ymm0, [rel PW_ONE]
    vpsraw      ymm7, ymm7, 1           ; ymm7 = (CrH * FIX(0.40200))
    vpsraw      ymm0, ymm0, 1           ; ymm0 = (CrL * FIX(0.40200))

    vpaddw      ymm6, ymm6, ymm5
    vpaddw      ymm4, ymm4, ymm2
    vpaddw      ymm6, ymm6, ymm5       ; ymm6 = (CbH * FIX(1.77200)) = (B - Y)H
    vpaddw      ymm4, ymm4, ymm2       ; ymm4 = (CbL * FIX(1.77200)) = (B - Y)L
    vpaddw      ymm7, ymm7, ymm1       ; ymm7 = (CrH * FIX(1.40200)) = (R - Y)H
    vpaddw      ymm0, ymm0, ymm3       ; ymm0 = (CrL * FIX(1.40200)) = (R - Y)L

    vmovdqa     YMMWORD [wk(0)], ymm6   ; wk(0) = (B - Y)H
    vmovdqa     YMMWORD [wk(1)], ymm7   ; wk(1) = (R - Y)H

    vpunpckhwd  ymm6, ymm5, ymm1
    vpunpcklwd  ymm5, ymm5, ymm1
    vpmaddwd    ymm5, ymm5, [rel PW_MF0344_F0285]
    vpmaddwd    ymm6, ymm6, [rel PW_MF0344_F0285]
    vpunpckhwd  ymm7, ymm2, ymm3
    vpunpcklwd  ymm2, ymm2, ymm3
    vpmaddwd    ymm2, ymm2, [rel PW_MF0344_F0285]
    vpmaddwd    ymm7, ymm7, [rel PW_MF0344_F0285]

    vpaddd      ymm5, ymm5, [rel PD_ONEHALF]
    vpaddd      ymm6, ymm6, [rel PD_ONEHALF]
    vpsrad      ymm5, ymm5, SCALEBITS
    vpsrad      ymm6, ymm6, SCALEBITS
    vpaddd      ymm2, ymm2, [rel PD_ONEHALF]
    vpaddd      ymm7, ymm7, [rel PD_ONEHALF]
    vpsrad      ymm2, ymm2, SCALEBITS
    vpsrad      ymm7, ymm7, SCALEBITS

    vpackssdw   ymm5, ymm5, ymm6
                ; ymm5 = CbH * -FIX(0.344) + CrH * FIX(0.285)
    vpackssdw   ymm2, ymm2, ymm7
                ; ymm2 = CbL * -FIX(0.344) + CrL * FIX(0.285)
    vpsubw      ymm5, ymm5, ymm1
                ; ymm5 = CbH * -FIX(0.344) + CrH * -FIX(0.714) = (G - Y)H
    vpsubw      ymm2, ymm2, ymm3
                ; ymm2 = CbL * -FIX(0.344) + CrL * -FIX(0.714) = (G - Y)L

    vmovdqa     YMMWORD [wk(2)], ymm5   ; wk(2) = (G - Y)H

    mov         al, 2                   ; Yctr
    jmp         short .Yloop_1st

.Yloop_2nd:
    vmovdqa     ymm0, YMMWORD [wk(1)]   ; ymm0 = (R - Y)H
    vmovdqa     ymm2, YMMWORD [wk(2)]   ; ymm2 = (G - Y)H
    vmovdqa     ymm4, YMMWORD [wk(0)]   ; ymm4 = (B - Y)H

.Yloop_1st:
    vmovdqu     ymm7, YMMWORD [rsi]
                ; ymm7 = Y(0123456789abcdefghijklmnopqrstuv)

    vpcmpeqw    ymm6, ymm6, ymm6
    vpsrlw      ymm6, ymm6, BYTE_BIT    ; ymm6 = { 0xFF 0x00 0xFF 0x00 .. }
    vpand       ymm6, ymm6, ymm7        ; ymm6 = Y(02468acegikmoqsu) = YE
    vpsrlw      ymm7, ymm7, BYTE_BIT    ; ymm7 = Y(13579bdfhjlnprtv) = YO

    vmovdqa     ymm1, ymm0              ; ymm1 = ymm0 = (R - Y)(L / H)
    vmovdqa     ymm3, ymm2              ; ymm3 = ymm2 = (G - Y)(L / H)
    vmovdqa     ymm5, ymm4              ; ymm5 = ymm4 = (B - Y)(L / H)

    vpaddw      ymm0, ymm0, ymm6
                ; ymm0 = ((R - Y) + YE) = RE = R(02468acegikmoqsu)
    vpaddw      ymm1, ymm1, ymm7
                ; ymm1 = ((R - Y) + YO) = RO = R(13579bdfhjlnprtv)
    vpackuswb   ymm0, ymm0, ymm0   ; ymm0 = R(02468ace********gikmoqsu********)
    vpackuswb   ymm1, ymm1, ymm1   ; ymm1 = R(13579bdf********hjlnprtv********)

    vpaddw      ymm2, ymm2, ymm6
                ; ymm2 = ((G - Y) + YE) = GE = G(02468acegikmoqsu)
    vpaddw      ymm3, ymm3, ymm7
                ; ymm3 = ((G - Y) + YO) = GO = G(13579bdfhjlnprtv)
    vpackuswb   ymm2, ymm2, ymm2   ; ymm2 = G(02468ace********gikmoqsu********)
    vpackuswb   ymm3, ymm3, ymm3   ; ymm3 = G(13579bdf********hjlnprtv********)

    vpaddw      ymm4, ymm4, ymm6
                ; ymm4 = ((B - Y) + YE) = BE = B(02468acegikmoqsu)
    vpaddw      ymm5, ymm5, ymm7
                ; ymm5 = ((B - Y) + YO) = BO = B(13579bdfhjlnprtv)
    vpackuswb   ymm4, ymm4, ymm4   ; ymm4 = B(02468ace********gikmoqsu********)
    vpackuswb   ymm5, ymm5, ymm5   ; ymm5 = B(13579bdf********hjlnprtv********)

%if RGB_PIXELSIZE == 3  ; ---------------

    ; NOTE: The values of RGB_RED, RGB_GREEN, and RGB_BLUE determine the
    ; mapping of components A, B, and C to red, green, and blue.
    ;
    ; ymmA = (A0 A2 A4 A6 A8 Aa Ac Ae Ag Ai Ak Am Ao Aq As Au) = AE
    ; ymmB = (A1 A3 A5 A7 A9 Ab Ad Af Ah Aj Al An Ap Ar At Av) = AO
    ; ymmC = (B0 B2 B4 B6 B8 Ba Bc Be Bg Bi Bk Bm Bo Bq Bs Bu) = BE
    ; ymmD = (B1 B3 B5 B7 B9 Bb Bd Bf Bh Bj Bl Bn Bp Br Bt Bv) = BO
    ; ymmE = (C0 C2 C4 C6 C8 Ca Cc Ce Cg Ci Ck Cm Co Cq Cs Cu) = CE
    ; ymmF = (C1 C3 C5 C7 C9 Cb Cd Cf Ch Cj Cl Cn Cp Cr Ct Cv) = CO
    ; ymmG = (** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **)
    ; ymmH = (** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **)

    vpunpcklbw  ymmA, ymmA, ymmC
                ; ymmA = (A0 B0 A2 B2 A4 B4 A6 B6 A8 B8 Aa Ba Ac Bc Ae Be
                ;         Ag Bg Ai Bi Ak Bk Am Bm Ao Bo Aq Bq As Bs Au Bu)
    vpunpcklbw  ymmE, ymmE, ymmB
                ; ymmE = (C0 A1 C2 A3 C4 A5 C6 A7 C8 A9 Ca Ab Cc Ad Ce Af
                ;         Cg Ah Ci Aj Ck Al Cm An Co Ap Cq Ar Cs At Cu Av)
    vpunpcklbw  ymmD, ymmD, ymmF
                ; ymmD = (B1 C1 B3 C3 B5 C5 B7 C7 B9 C9 Bb Cb Bd Cd Bf Cf
                ;         Bh Ch Bj Cj Bl Cl Bn Cn Bp Cp Br Cr Bt Ct Bv Cv)

    vpsrldq     ymmH, ymmA, 2
                ; ymmH = (A2 B2 A4 B4 A6 B6 A8 B8 Aa Ba Ac Bc Ae Be Ag Bg
                ;         Ai Bi Ak Bk Am Bm Ao Bo Aq Bq As Bs Au Bu -- --)
    vpunpckhwd  ymmG, ymmA, ymmE
                ; ymmG = (A8 B8 C8 A9 Aa Ba Ca Ab Ac Bc Cc Ad Ae Be Ce Af
                ;         Ao Bo Co Ap Aq Bq Cq Ar As Bs Cs At Au Bu Cu Av)
    vpunpcklwd  ymmA, ymmA, ymmE
                ; ymmA = (A0 B0 C0 A1 A2 B2 C2 A3 A4 B4 C4 A5 A6 B6 C6 A7
                ;         Ag Bg Cg Ah Ai Bi Ci Aj Ak Bk Ck Al Am Bm Cm An)

    vpsrldq     ymmE, ymmE, 2
                ; ymmE = (C2 A3 C4 A5 C6 A7 C8 A9 Ca Ab Cc Ad Ce Af Cg Ah
                ;         Ci Aj Ck Al Cm An Co Ap Cq Ar Cs At Cu Av -- --)

    vpsrldq     ymmB, ymmD, 2
                ; ymmB = (B3 C3 B5 C5 B7 C7 B9 C9 Bb Cb Bd Cd Bf Cf Bh Ch
                ;         Bj Cj Bl Cl Bn Cn Bp Cp Br Cr Bt Ct Bv Cv -- --)
    vpunpckhwd  ymmC, ymmD, ymmH
                ; ymmC = (B9 C9 Aa Ba Bb Cb Ac Bc Bd Cd Ae Be Bf Cf Ag Bg
                ;         Bp Cp Aq Bq Br Cr As Bs Bt Ct Au Bu Bv Cv -- --)
    vpunpcklwd  ymmD, ymmD, ymmH
                ; ymmD = (B1 C1 A2 B2 B3 C3 A4 B4 B5 C5 A6 B6 B7 C7 A8 B8
                ;         Bh Ch Ai Bi Bj Cj Ak Bk Bl Cl Am Bm Bn Cn Ao Bo)

    vpunpckhwd  ymmF, ymmE, ymmB
                ; ymmF = (Ca Ab Bb Cb Cc Ad Bd Cd Ce Af Bf Cf Cg Ah Bh Ch
                ;         Cq Ar Br Cr Cs At Bt Ct Cu Av Bv Cv -- -- -- --)
    vpunpcklwd  ymmE, ymmE, ymmB
                ; ymmE = (C2 A3 B3 C3 C4 A5 B5 C5 C6 A7 B7 C7 C8 A9 B9 C9
                ;         Ci Aj Bj Cj Ck Al Bl Cl Cm An Bn Cn Co Ap Bp Cp)

    vpshufd     ymmH, ymmA, 0x4E
                ; ymmH = (A4 B4 C4 A5 A6 B6 C6 A7 A0 B0 C0 A1 A2 B2 C2 A3
                ;         Ak Bk Ck Al Am Bm Cm An Ag Bg Cg Ah Ai Bi Ci Aj)
    vpunpckldq  ymmA, ymmA, ymmD
                ; ymmA = (A0 B0 C0 A1 B1 C1 A2 B2 A2 B2 C2 A3 B3 C3 A4 B4
                ;         Ag Bg Cg Ah Bh Ch Ai Bi Ai Bi Ci Aj Bj Cj Ak Bk)
    vpunpckhdq  ymmD, ymmD, ymmE
                ; ymmD = (B5 C5 A6 B6 C6 A7 B7 C7 B7 C7 A8 B8 C8 A9 B9 C9
                ;         Bl Cl Am Bm Cm An Bn Cn Bn Cn Ao Bo Co Ap Bp Cp)
    vpunpckldq  ymmE, ymmE, ymmH
                ; ymmE = (C2 A3 B3 C3 A4 B4 C4 A5 C4 A5 B5 C5 A6 B6 C6 A7
                ;         Ci Aj Bj Cj Ak Bk Ck Al Ck Al Bl Cl Am Bm Cm An)

    vpshufd     ymmH, ymmG, 0x4E
                ; ymmH = (Ac Bc Cc Ad Ae Be Ce Af A8 B8 C8 A9 Aa Ba Ca Ab
                ;         As Bs Cs At Au Bu Cu Av Ao Bo Co Ap Aq Bq Cq Ar)
    vpunpckldq  ymmG, ymmG, ymmC
                ; ymmG = (A8 B8 C8 A9 B9 C9 Aa Ba Aa Ba Ca Ab Bb Cb Ac Bc
                ;         Ao Bo Co Ap Bp Cp Aq Bq Aq Bq Cq Ar Br Cr As Bs)
    vpunpckhdq  ymmC, ymmC, ymmF
                ; ymmC = (Bd Cd Ae Be Ce Af Bf Cf Bf Cf Ag Bg Cg Ah Bh Ch
                ;         Bt Ct Au Bu Cu Av Bv Cv Bv Cv -- -- -- -- -- --)
    vpunpckldq  ymmF, ymmF, ymmH
                ; ymmF = (Ca Ab Bb Cb Ac Bc Cc Ad Cc Ad Bd Cd Ae Be Ce Af
                ;         Cq Ar Br Cr As Bs Cs At Cs At Bt Ct Au Bu Cu Av)

    vpunpcklqdq ymmH, ymmA, ymmE
                ; ymmH = (A0 B0 C0 A1 B1 C1 A2 B2 C2 A3 B3 C3 A4 B4 C4 A5
                ;         Ag Bg Cg Ah Bh Ch Ai Bi Ci Aj Bj Cj Ak Bk Ck Al)
    vpunpcklqdq ymmG, ymmD, ymmG
                ; ymmG = (B5 C5 A6 B6 C6 A7 B7 C7 A8 B8 C8 A9 B9 C9 Aa Ba
                ;         Bl Cl Am Bm Cm An Bn Cn Ao Bo Co Ap Bp Cp Aq Bq)
    vpunpcklqdq ymmC, ymmF, ymmC
                ; ymmC = (Ca Ab Bb Cb Ac Bc Cc Ad Bd Cd Ae Be Ce Af Bf Cf
                ;         Cq Ar Br Cr As Bs Cs At Bt Ct Au Bu Cu Av Bv Cv)

    vperm2i128  ymmA, ymmH, ymmG, 0x20
                ; ymmA = (A0 B0 C0 A1 B1 C1 A2 B2 C2 A3 B3 C3 A4 B4 C4 A5
                ;         B5 C5 A6 B6 C6 A7 B7 C7 A8 B8 C8 A9 B9 C9 Aa Ba)
    vperm2i128  ymmD, ymmC, ymmH, 0x30
                ; ymmD = (Ca Ab Bb Cb Ac Bc Cc Ad Bd Cd Ae Be Ce Af Bf Cf
                ;         Ag Bg Cg Ah Bh Ch Ai Bi Ci Aj Bj Cj Ak Bk Ck Al)
    vperm2i128  ymmF, ymmG, ymmC, 0x31
                ; ymmF = (Bl Cl Am Bm Cm An Bn Cn Ao Bo Co Ap Bp Cp Aq Bq
                ;         Cq Ar Br Cr As Bs Cs At Bt Ct Au Bu Cu Av Bv Cv)

    cmp         rcx, byte SIZEOF_YMMWORD
    jb          short .column_st64

    test        rdi, SIZEOF_YMMWORD - 1
    jnz         short .out1
    ; --(aligned)-------------------
    vmovntdq    YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovntdq    YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    vmovntdq    YMMWORD [rdi + 2 * SIZEOF_YMMWORD], ymmF
    jmp         short .out0
.out1:  ; --(unaligned)-----------------
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovdqu     YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    vmovdqu     YMMWORD [rdi + 2 * SIZEOF_YMMWORD], ymmF
.out0:
    add         rdi, byte RGB_PIXELSIZE * SIZEOF_YMMWORD  ; outptr
    sub         rcx, byte SIZEOF_YMMWORD
    jz          near .endcolumn

    add         rsi, byte SIZEOF_YMMWORD  ; inptr0
    dec         al                        ; Yctr
    jnz         near .Yloop_2nd

    add         rbx, byte SIZEOF_YMMWORD  ; inptr1
    add         rdx, byte SIZEOF_YMMWORD  ; inptr2
    jmp         near .columnloop

.column_st64:
    lea         rcx, [rcx + rcx * 2]          ; imul ecx, RGB_PIXELSIZE
    cmp         rcx, byte 2 * SIZEOF_YMMWORD
    jb          short .column_st32
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovdqu     YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    add         rdi, byte 2 * SIZEOF_YMMWORD  ; outptr
    vmovdqa     ymmA, ymmF
    sub         rcx, byte 2 * SIZEOF_YMMWORD
    jmp         short .column_st31
.column_st32:
    cmp         rcx, byte SIZEOF_YMMWORD
    jb          short .column_st31
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    add         rdi, byte SIZEOF_YMMWORD      ; outptr
    vmovdqa     ymmA, ymmD
    sub         rcx, byte SIZEOF_YMMWORD
    jmp         short .column_st31
.column_st31:
    cmp         rcx, byte SIZEOF_XMMWORD
    jb          short .column_st15
    vmovdqu     XMMWORD [rdi + 0 * SIZEOF_XMMWORD], xmmA
    add         rdi, byte SIZEOF_XMMWORD      ; outptr
    vperm2i128  ymmA, ymmA, ymmA, 1
    sub         rcx, byte SIZEOF_XMMWORD
.column_st15:
    ; Store the lower 8 bytes of xmmA to the output when it has enough
    ; space.
    cmp         rcx, byte SIZEOF_MMWORD
    jb          short .column_st7
    vmovq       XMM_MMWORD [rdi], xmmA
    add         rdi, byte SIZEOF_MMWORD
    sub         rcx, byte SIZEOF_MMWORD
    vpsrldq     xmmA, xmmA, SIZEOF_MMWORD
.column_st7:
    ; Store the lower 4 bytes of xmmA to the output when it has enough
    ; space.
    cmp         rcx, byte SIZEOF_DWORD
    jb          short .column_st3
    vmovd       XMM_DWORD [rdi], xmmA
    add         rdi, byte SIZEOF_DWORD
    sub         rcx, byte SIZEOF_DWORD
    vpsrldq     xmmA, xmmA, SIZEOF_DWORD
.column_st3:
    ; Store the lower 2 bytes of rax to the output when it has enough
    ; space.
    vmovd       eax, xmmA
    cmp         rcx, byte SIZEOF_WORD
    jb          short .column_st1
    mov         word [rdi], ax
    add         rdi, byte SIZEOF_WORD
    sub         rcx, byte SIZEOF_WORD
    shr         rax, 16
.column_st1:
    ; Store the lower 1 byte of rax to the output when it has enough
    ; space.
    test        rcx, rcx
    jz          short .endcolumn
    mov         byte [rdi], al

%else  ; RGB_PIXELSIZE == 4 ; -----------

%ifdef RGBX_FILLER_0XFF
    vpcmpeqb    ymm6, ymm6, ymm6
                ; ymm6 = XE = X(02468ace********gikmoqsu********)
    vpcmpeqb    ymm7, ymm7, ymm7
                ; ymm7 = XO = X(13579bdf********hjlnprtv********)
%else
    vpxor       ymm6, ymm6, ymm6
                ; ymm6 = XE = X(02468ace********gikmoqsu********)
    vpxor       ymm7, ymm7, ymm7
                ; ymm7 = XO = X(13579bdf********hjlnprtv********)
%endif
    ; NOTE: The values of RGB_RED, RGB_GREEN, and RGB_BLUE determine the
    ; mapping of components A, B, C, and D to red, green, and blue.
    ;
    ; ymmA = (A0 A2 A4 A6 A8 Aa Ac Ae Ag Ai Ak Am Ao Aq As Au) = AE
    ; ymmB = (A1 A3 A5 A7 A9 Ab Ad Af Ah Aj Al An Ap Ar At Av) = AO
    ; ymmC = (B0 B2 B4 B6 B8 Ba Bc Be Bg Bi Bk Bm Bo Bq Bs Bu) = BE
    ; ymmD = (B1 B3 B5 B7 B9 Bb Bd Bf Bh Bj Bl Bn Bp Br Bt Bv) = BO
    ; ymmE = (C0 C2 C4 C6 C8 Ca Cc Ce Cg Ci Ck Cm Co Cq Cs Cu) = CE
    ; ymmF = (C1 C3 C5 C7 C9 Cb Cd Cf Ch Cj Cl Cn Cp Cr Ct Cv) = CO
    ; ymmG = (D0 D2 D4 D6 D8 Da Dc De Dg Di Dk Dm Do Dq Ds Du) = DE
    ; ymmH = (D1 D3 D5 D7 D9 Db Dd Df Dh Dj Dl Dn Dp Dr Dt Dv) = DO

    vpunpcklbw  ymmA, ymmA, ymmC
                ; ymmA = (A0 B0 A2 B2 A4 B4 A6 B6 A8 B8 Aa Ba Ac Bc Ae Be
                ;         Ag Bg Ai Bi Ak Bk Am Bm Ao Bo Aq Bq As Bs Au Bu)
    vpunpcklbw  ymmE, ymmE, ymmG
                ; ymmE = (C0 D0 C2 D2 C4 D4 C6 D6 C8 D8 Ca Da Cc Dc Ce De
                ;         Cg Dg Ci Di Ck Dk Cm Dm Co Do Cq Dq Cs Ds Cu Du)
    vpunpcklbw  ymmB, ymmB, ymmD
                ; ymmB = (A1 B1 A3 B3 A5 B5 A7 B7 A9 B9 Ab Bb Ad Bd Af Bf
                ;         Ah Bh Aj Bj Al Bl An Bn Ap Bp Ar Br At Bt Av Bv)
    vpunpcklbw  ymmF, ymmF, ymmH
                ; ymmF = (C1 D1 C3 D3 C5 D5 C7 D7 C9 D9 Cb Db Cd Dd Cf Df
                ;         Ch Dh Cj Dj Cl Dl Cn Dn Cp Dp Cr Dr Ct Dt Cv Dv)

    vpunpckhwd  ymmC, ymmA, ymmE
                ; ymmC = (A8 B8 C8 D8 Aa Ba Ca Da Ac Bc Cc Dc Ae Be Ce De
                ;         Ao Bo Co Do Aq Bq Cq Dq As Bs Cs Ds Au Bu Cu Du)
    vpunpcklwd  ymmA, ymmA, ymmE
                ; ymmA = (A0 B0 C0 D0 A2 B2 C2 D2 A4 B4 C4 D4 A6 B6 C6 D6
                ;         Ag Bg Cg Dg Ai Bi Ci Di Ak Bk Ck Dk Am Bm Cm Dm)
    vpunpckhwd  ymmG, ymmB, ymmF
                ; ymmG = (A9 B9 C9 D9 Ab Bb Cb Db Ad Bd Cd Dd Af Bf Cf Df
                ;         Ap Bp Cp Dp Ar Br Cr Dr At Bt Ct Dt Av Bv Cv Dv)
    vpunpcklwd  ymmB, ymmB, ymmF
                ; ymmB = (A1 B1 C1 D1 A3 B3 C3 D3 A5 B5 C5 D5 A7 B7 C7 D7
                ;         Ah Bh Ch Dh Aj Bj Cj Dj Al Bl Cl Dl An Bn Cn Dn)

    vpunpckhdq  ymmE, ymmA, ymmB
                ; ymmE = (A4 B4 C4 D4 A5 B5 C5 D5 A6 B6 C6 D6 A7 B7 C7 D7
                ;         Ak Bk Ck Dk Al Bl Cl Dl Am Bm Cm Dm An Bn Cn Dn)
    vpunpckldq  ymmB, ymmA, ymmB
                ; ymmB = (A0 B0 C0 D0 A1 B1 C1 D1 A2 B2 C2 D2 A3 B3 C3 D3
                ;         Ag Bg Cg Dg Ah Bh Ch Dh Ai Bi Ci Di Aj Bj Cj Dj)
    vpunpckhdq  ymmF, ymmC, ymmG
                ; ymmF = (Ac Bc Cc Dc Ad Bd Cd Dd Ae Be Ce De Af Bf Cf Df
                ;         As Bs Cs Ds At Bt Ct Dt Au Bu Cu Du Av Bv Cv Dv)
    vpunpckldq  ymmG, ymmC, ymmG
                ; ymmG = (A8 B8 C8 D8 A9 B9 C9 D9 Aa Ba Ca Da Ab Bb Cb Db
                ;         Ao Bo Co Do Ap Bp Cp Dp Aq Bq Cq Dq Ar Br Cr Dr)

    vperm2i128  ymmA, ymmB, ymmE, 0x20
                ; ymmA = (A0 B0 C0 D0 A1 B1 C1 D1 A2 B2 C2 D2 A3 B3 C3 D3
                ;         A4 B4 C4 D4 A5 B5 C5 D5 A6 B6 C6 D6 A7 B7 C7 D7)
    vperm2i128  ymmD, ymmG, ymmF, 0x20
                ; ymmD = (A8 B8 C8 D8 A9 B9 C9 D9 Aa Ba Ca Da Ab Bb Cb Db
                ;         Ac Bc Cc Dc Ad Bd Cd Dd Ae Be Ce De Af Bf Cf Df)
    vperm2i128  ymmC, ymmB, ymmE, 0x31
                ; ymmC = (Ag Bg Cg Dg Ah Bh Ch Dh Ai Bi Ci Di Aj Bj Cj Dj
                ;         Ak Bk Ck Dk Al Bl Cl Dl Am Bm Cm Dm An Bn Cn Dn)
    vperm2i128  ymmH, ymmG, ymmF, 0x31
                ; ymmH = (Ao Bo Co Do Ap Bp Cp Dp Aq Bq Cq Dq Ar Br Cr Dr
                ;         As Bs Cs Ds At Bt Ct Dt Au Bu Cu Du Av Bv Cv Dv)

    cmp         rcx, byte SIZEOF_YMMWORD
    jb          short .column_st64

    test        rdi, SIZEOF_YMMWORD - 1
    jnz         short .out1
    ; --(aligned)-------------------
    vmovntdq    YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovntdq    YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    vmovntdq    YMMWORD [rdi + 2 * SIZEOF_YMMWORD], ymmC
    vmovntdq    YMMWORD [rdi + 3 * SIZEOF_YMMWORD], ymmH
    jmp         short .out0
.out1:  ; --(unaligned)-----------------
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovdqu     YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    vmovdqu     YMMWORD [rdi + 2 * SIZEOF_YMMWORD], ymmC
    vmovdqu     YMMWORD [rdi + 3 * SIZEOF_YMMWORD], ymmH
.out0:
    add         rdi, RGB_PIXELSIZE * SIZEOF_YMMWORD  ; outptr
    sub         rcx, byte SIZEOF_YMMWORD
    jz          near .endcolumn

    add         rsi, byte SIZEOF_YMMWORD  ; inptr0
    dec         al
    jnz         near .Yloop_2nd

    add         rbx, byte SIZEOF_YMMWORD  ; inptr1
    add         rdx, byte SIZEOF_YMMWORD  ; inptr2
    jmp         near .columnloop

.column_st64:
    cmp         rcx, byte SIZEOF_YMMWORD / 2
    jb          short .column_st32
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    vmovdqu     YMMWORD [rdi + 1 * SIZEOF_YMMWORD], ymmD
    add         rdi, byte 2 * SIZEOF_YMMWORD  ; outptr
    vmovdqa     ymmA, ymmC
    vmovdqa     ymmD, ymmH
    sub         rcx, byte SIZEOF_YMMWORD / 2
.column_st32:
    cmp         rcx, byte SIZEOF_YMMWORD / 4
    jb          short .column_st16
    vmovdqu     YMMWORD [rdi + 0 * SIZEOF_YMMWORD], ymmA
    add         rdi, byte SIZEOF_YMMWORD      ; outptr
    vmovdqa     ymmA, ymmD
    sub         rcx, byte SIZEOF_YMMWORD / 4
.column_st16:
    cmp         rcx, byte SIZEOF_YMMWORD / 8
    jb          short .column_st15
    vmovdqu     XMMWORD [rdi + 0 * SIZEOF_XMMWORD], xmmA
    add         rdi, byte SIZEOF_XMMWORD      ; outptr
    vperm2i128  ymmA, ymmA, ymmA, 1
    sub         rcx, byte SIZEOF_YMMWORD / 8
.column_st15:
    ; Store two pixels (8 bytes) of ymmA to the output when it has enough
    ; space.
    cmp         rcx, byte SIZEOF_YMMWORD / 16
    jb          short .column_st7
    vmovq       MMWORD [rdi], xmmA
    add         rdi, byte SIZEOF_YMMWORD / 16 * 4
    sub         rcx, byte SIZEOF_YMMWORD / 16
    vpsrldq     xmmA, SIZEOF_YMMWORD / 16 * 4
.column_st7:
    ; Store one pixel (4 bytes) of ymmA to the output when it has enough
    ; space.
    test        rcx, rcx
    jz          short .endcolumn
    vmovd       XMM_DWORD [rdi], xmmA

%endif  ; RGB_PIXELSIZE ; ---------------

.endcolumn:
    sfence                              ; flush the write buffer

.return:
    pop         rbx
    vzeroupper
    UNCOLLECT_ARGS 4
    lea         rsp, [rbp - 8]
    pop         r15
    pop         rbp
    ret


; --------------------------------------------------------------------------
;
; Upsample and color convert for the case of 2:1 horizontal and 2:1 vertical.
;
; GLOBAL(void)
; jsimd_h2v2_merged_upsample_avx2(JDIMENSION output_width,
;                                 JSAMPIMAGE input_buf,
;                                 JDIMENSION in_row_group_ctr,
;                                 JSAMPARRAY output_buf)
;
; r10d = JDIMENSION output_width
; r11 = JSAMPIMAGE input_buf
; r12d = JDIMENSION in_row_group_ctr
; r13 = JSAMPARRAY output_buf

    align       32
    GLOBAL_FUNCTION(jsimd_h2v2_merged_upsample_avx2)

EXTN(jsimd_h2v2_merged_upsample_avx2):
    ENDBR64
    push        rbp
    mov         rbp, rsp
    COLLECT_ARGS 4
    push        rbx

    mov         eax, r10d

    mov         rdi, r11
    mov         ecx, r12d
    mov         rsip, JSAMPARRAY [rdi + 0 * SIZEOF_JSAMPARRAY]
    mov         rbxp, JSAMPARRAY [rdi + 1 * SIZEOF_JSAMPARRAY]
    mov         rdxp, JSAMPARRAY [rdi + 2 * SIZEOF_JSAMPARRAY]
    mov         rdi, r13
    lea         rsi, [rsi + rcx * SIZEOF_JSAMPROW]

    sub         rsp, SIZEOF_JSAMPARRAY * 4
    mov         JSAMPARRAY [rsp + 0 * SIZEOF_JSAMPARRAY], rsip  ; intpr00
    mov         JSAMPARRAY [rsp + 1 * SIZEOF_JSAMPARRAY], rbxp  ; intpr1
    mov         JSAMPARRAY [rsp + 2 * SIZEOF_JSAMPARRAY], rdxp  ; intpr2
    mov         rbx, rsp

    push        rdi
    push        rcx
    push        rax

    %ifdef WIN64
    mov         r8, rcx
    mov         r9, rdi
    mov         rcx, rax
    mov         rdx, rbx
    %else
    mov         rdx, rcx
    mov         rcx, rdi
    mov         rdi, rax
    mov         rsi, rbx
    %endif

    call        EXTN(jsimd_h2v1_merged_upsample_avx2)

    pop         rax
    pop         rcx
    pop         rdi
    mov         rsip, JSAMPARRAY [rsp + 0 * SIZEOF_JSAMPARRAY]
    mov         rbxp, JSAMPARRAY [rsp + 1 * SIZEOF_JSAMPARRAY]
    mov         rdxp, JSAMPARRAY [rsp + 2 * SIZEOF_JSAMPARRAY]

    add         rdi, byte SIZEOF_JSAMPROW  ; outptr1
    add         rsi, byte SIZEOF_JSAMPROW  ; inptr01

    mov         JSAMPARRAY [rsp + 0 * SIZEOF_JSAMPARRAY], rsip  ; intpr00
    mov         JSAMPARRAY [rsp + 1 * SIZEOF_JSAMPARRAY], rbxp  ; intpr1
    mov         JSAMPARRAY [rsp + 2 * SIZEOF_JSAMPARRAY], rdxp  ; intpr2
    mov         rbx, rsp

    push        rdi
    push        rcx
    push        rax

    %ifdef WIN64
    mov         r8, rcx
    mov         r9, rdi
    mov         rcx, rax
    mov         rdx, rbx
    %else
    mov         rdx, rcx
    mov         rcx, rdi
    mov         rdi, rax
    mov         rsi, rbx
    %endif

    call        EXTN(jsimd_h2v1_merged_upsample_avx2)

    pop         rax
    pop         rcx
    pop         rdi
    mov         rsip, JSAMPARRAY [rsp + 0 * SIZEOF_JSAMPARRAY]
    mov         rbxp, JSAMPARRAY [rsp + 1 * SIZEOF_JSAMPARRAY]
    mov         rdxp, JSAMPARRAY [rsp + 2 * SIZEOF_JSAMPARRAY]
    add         rsp, SIZEOF_JSAMPARRAY * 4

    pop         rbx
    UNCOLLECT_ARGS 4
    pop         rbp
    ret

; For some reason, the OS X linker does not honor the request to align the
; segment unless we do this.
    align       32
