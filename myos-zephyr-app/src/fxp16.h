/*! \copyright
    Copyright (c) 2017-2022, marco@bacchi.at
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. The name of the author may not be used to endorse or promote
       products derived from this software without specific prior
       written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*!
    \file   fp.h

    \brief  Signed fixed point implementation for small embedded project

    \details
*/

#ifndef _FXP16_H_
#define _FXP16_H_

#define FXP16_VERSION_MAJOR 0
#define FXP16_VERSION_MINOR 0
#define FXP16_VERSION_PATCH 1

#warning "This lib is still in development"

#include <stdint.h>
#include <limits.h>

#define FXP16_RADIX 2


// ---- generische Hilfs-Makros für 16-bit Fixed-Point (signed) ----
#define FXP16_SCALE(q)  (1u << (q))                                         /* 2^q */
#define FXP16_PREC(q)   (1.0 / (float)FXP16_SCALE(q))                       /* 1 / 2^q */
#define FXP16_MAX(q)    (((float)((1u << 15) - 1)) / FXP16_SCALE(q))        /*  (2^15 - 1) / 2^q */
#define FXP16_MIN(q)    ((-(float)(1u << 15)) / FXP16_FROM(q))              /* -(2^15) / 2^q */

// --------------------------------------------------------------------------
#define FXP16_Q0                    (0)
#define FXP16_Q0_PREC               (1.0)
#define FXP16_Q0_MAX                (32767.0)
#define FXP16_Q0_MIN                (-32768.0)
// --------------------------------------------------------------------------
#define FXP16_Q0_M_E                (3)                 // 3.0
#define FXP16_Q0_M_LOG2E            (1)                 // 1.0
#define FXP16_Q0_M_LOG10E           (0)                 // 0.0
#define FXP16_Q0_M_LN2              (1)                 // 1.0
#define FXP16_Q0_M_PI               (3)                 // 3.0
#define FXP16_Q0_M_PI_2             (2)                 // 2.0
#define FXP16_Q0_M_PI_4             (1)                 // 1.0
#define FXP16_Q0_M_1_PI             (0)                 // 0.0
#define FXP16_Q0_M_2_PI             (1)                 // 1.0
#define FXP16_Q0_M_2_SQRTPI         (1)                 // 1.0
#define FXP16_Q0_M_SQRT2            (1)                 // 1.0
#define FXP16_Q0_M_SQRT1_2          (1)                 // 1.0
#define FXP16_Q0_M_TWOPI            (6)                 // 6.0
#define FXP16_Q0_M_3PI_4            (2)                 // 2.0
#define FXP16_Q0_M_SQRTPI           (2)                 // 2.0
#define FXP16_Q0_M_SQRT3            (2)                 // 2.0
#define FXP16_Q0_M_IVLN10           (0)                 // 0.0
#define FXP16_Q0_M_LOG2_E           (1)                 // 1.0
#define FXP16_Q0_M_INVLN2           (1)                 // 1.0
#define FXP16_Q0_ONE_EIGHTH         (0)                 // 0.0
#define FXP16_Q0_ONE_QUARTER        (0)                 // 0.0
#define FXP16_Q0_THREE_EIGHTHS      (0)                 // 0.0
#define FXP16_Q0_ONE_HALF           (0)                 // 0.0
#define FXP16_Q0_FIVE_EIGHTHS       (1)                 // 1.0
#define FXP16_Q0_THREE_QUARTERS     (1)                 // 1.0
#define FXP16_Q0_SEVEN_EIGHTHS      (1)                 // 1.0
#define FXP16_Q0_ONE_THIRD          (0)                 // 0.0
#define FXP16_Q0_TWO_THIRDS         (1)                 // 1.0
#define FXP16_Q0_ONE                (1)                 // 1.0
#define FXP16_Q0_MINUS_ONE          (-1)                // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q1                    (1)
#define FXP16_Q1_PREC               (0.5)
#define FXP16_Q1_MAX                (16383.5)
#define FXP16_Q1_MIN                (-16384.0)
// --------------------------------------------------------------------------
#define FXP16_Q1_M_E                (5)                 // 2.5
#define FXP16_Q1_M_LOG2E            (3)                 // 1.5
#define FXP16_Q1_M_LOG10E           (1)                 // 0.5
#define FXP16_Q1_M_LN2              (1)                 // 0.5
#define FXP16_Q1_M_PI               (6)                 // 3.0
#define FXP16_Q1_M_PI_2             (3)                 // 1.5
#define FXP16_Q1_M_PI_4             (2)                 // 1.0
#define FXP16_Q1_M_1_PI             (1)                 // 0.5
#define FXP16_Q1_M_2_PI             (1)                 // 0.5
#define FXP16_Q1_M_2_SQRTPI         (2)                 // 1.0
#define FXP16_Q1_M_SQRT2            (3)                 // 1.5
#define FXP16_Q1_M_SQRT1_2          (1)                 // 0.5
#define FXP16_Q1_M_TWOPI            (13)                // 6.5
#define FXP16_Q1_M_3PI_4            (5)                 // 2.5
#define FXP16_Q1_M_SQRTPI           (4)                 // 2.0
#define FXP16_Q1_M_SQRT3            (3)                 // 1.5
#define FXP16_Q1_M_IVLN10           (1)                 // 0.5
#define FXP16_Q1_M_LOG2_E           (3)                 // 1.5
#define FXP16_Q1_M_INVLN2           (3)                 // 1.5
#define FXP16_Q1_ONE_EIGHTH         (0)                 // 0.0
#define FXP16_Q1_ONE_QUARTER        (0)                 // 0.0
#define FXP16_Q1_THREE_EIGHTHS      (1)                 // 0.5
#define FXP16_Q1_ONE_HALF           (1)                 // 0.5
#define FXP16_Q1_FIVE_EIGHTHS       (1)                 // 0.5
#define FXP16_Q1_THREE_QUARTERS     (2)                 // 1.0
#define FXP16_Q1_SEVEN_EIGHTHS      (2)                 // 1.0
#define FXP16_Q1_ONE_THIRD          (1)                 // 0.5
#define FXP16_Q1_TWO_THIRDS         (1)                 // 0.5
#define FXP16_Q1_ONE                (2)                 // 1.0
#define FXP16_Q1_MINUS_ONE          (-2)                // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q2                    (2)
#define FXP16_Q2_PREC               (0.25)
#define FXP16_Q2_MAX                (8191.75)
#define FXP16_Q2_MIN                (-8192.0)
// --------------------------------------------------------------------------
#define FXP16_Q2_M_E                (11)                // 2.75
#define FXP16_Q2_M_LOG2E            (6)                 // 1.5
#define FXP16_Q2_M_LOG10E           (2)                 // 0.5
#define FXP16_Q2_M_LN2              (3)                 // 0.75
#define FXP16_Q2_M_PI               (13)                // 3.25
#define FXP16_Q2_M_PI_2             (6)                 // 1.5
#define FXP16_Q2_M_PI_4             (3)                 // 0.75
#define FXP16_Q2_M_1_PI             (1)                 // 0.25
#define FXP16_Q2_M_2_PI             (3)                 // 0.75
#define FXP16_Q2_M_2_SQRTPI         (5)                 // 1.25
#define FXP16_Q2_M_SQRT2            (6)                 // 1.5
#define FXP16_Q2_M_SQRT1_2          (3)                 // 0.75
#define FXP16_Q2_M_TWOPI            (25)                // 6.25
#define FXP16_Q2_M_3PI_4            (9)                 // 2.25
#define FXP16_Q2_M_SQRTPI           (7)                 // 1.75
#define FXP16_Q2_M_SQRT3            (7)                 // 1.75
#define FXP16_Q2_M_IVLN10           (2)                 // 0.5
#define FXP16_Q2_M_LOG2_E           (6)                 // 1.5
#define FXP16_Q2_M_INVLN2           (6)                 // 1.5
#define FXP16_Q2_ONE_EIGHTH         (0)                 // 0.0
#define FXP16_Q2_ONE_QUARTER        (1)                 // 0.25
#define FXP16_Q2_THREE_EIGHTHS      (2)                 // 0.5
#define FXP16_Q2_ONE_HALF           (2)                 // 0.5
#define FXP16_Q2_FIVE_EIGHTHS       (2)                 // 0.5
#define FXP16_Q2_THREE_QUARTERS     (3)                 // 0.75
#define FXP16_Q2_SEVEN_EIGHTHS      (4)                 // 1.0
#define FXP16_Q2_ONE_THIRD          (1)                 // 0.25
#define FXP16_Q2_TWO_THIRDS         (3)                 // 0.75
#define FXP16_Q2_ONE                (4)                 // 1.0
#define FXP16_Q2_MINUS_ONE          (-4)                // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q3                    (3)
#define FXP16_Q3_PREC               (0.125)
#define FXP16_Q3_MAX                (4095.875)
#define FXP16_Q3_MIN                (-4096.0)
// --------------------------------------------------------------------------
#define FXP16_Q3_M_E                (22)                // 2.75
#define FXP16_Q3_M_LOG2E            (12)                // 1.5
#define FXP16_Q3_M_LOG10E           (3)                 // 0.375
#define FXP16_Q3_M_LN2              (6)                 // 0.75
#define FXP16_Q3_M_PI               (25)                // 3.125
#define FXP16_Q3_M_PI_2             (13)                // 1.625
#define FXP16_Q3_M_PI_4             (6)                 // 0.75
#define FXP16_Q3_M_1_PI             (3)                 // 0.375
#define FXP16_Q3_M_2_PI             (5)                 // 0.625
#define FXP16_Q3_M_2_SQRTPI         (9)                 // 1.125
#define FXP16_Q3_M_SQRT2            (11)                // 1.375
#define FXP16_Q3_M_SQRT1_2          (6)                 // 0.75
#define FXP16_Q3_M_TWOPI            (50)                // 6.25
#define FXP16_Q3_M_3PI_4            (19)                // 2.375
#define FXP16_Q3_M_SQRTPI           (14)                // 1.75
#define FXP16_Q3_M_SQRT3            (14)                // 1.75
#define FXP16_Q3_M_IVLN10           (3)                 // 0.375
#define FXP16_Q3_M_LOG2_E           (12)                // 1.5
#define FXP16_Q3_M_INVLN2           (12)                // 1.5
#define FXP16_Q3_ONE_EIGHTH         (1)                 // 0.125
#define FXP16_Q3_ONE_QUARTER        (2)                 // 0.25
#define FXP16_Q3_THREE_EIGHTHS      (3)                 // 0.375
#define FXP16_Q3_ONE_HALF           (4)                 // 0.5
#define FXP16_Q3_FIVE_EIGHTHS       (5)                 // 0.625
#define FXP16_Q3_THREE_QUARTERS     (6)                 // 0.75
#define FXP16_Q3_SEVEN_EIGHTHS      (7)                 // 0.875
#define FXP16_Q3_ONE_THIRD          (3)                 // 0.375
#define FXP16_Q3_TWO_THIRDS         (5)                 // 0.625
#define FXP16_Q3_ONE                (8)                 // 1.0
#define FXP16_Q3_MINUS_ONE          (-8)                // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q4                    (4)
#define FXP16_Q4_PREC               (0.0625)
#define FXP16_Q4_MAX                (2047.9375)
#define FXP16_Q4_MIN                (-2048.0)
// --------------------------------------------------------------------------
#define FXP16_Q4_M_E                (43)                // 2.6875
#define FXP16_Q4_M_LOG2E            (23)                // 1.4375
#define FXP16_Q4_M_LOG10E           (7)                 // 0.4375
#define FXP16_Q4_M_LN2              (11)                // 0.6875
#define FXP16_Q4_M_PI               (50)                // 3.125
#define FXP16_Q4_M_PI_2             (25)                // 1.5625
#define FXP16_Q4_M_PI_4             (13)                // 0.8125
#define FXP16_Q4_M_1_PI             (5)                 // 0.3125
#define FXP16_Q4_M_2_PI             (10)                // 0.625
#define FXP16_Q4_M_2_SQRTPI         (18)                // 1.125
#define FXP16_Q4_M_SQRT2            (23)                // 1.4375
#define FXP16_Q4_M_SQRT1_2          (11)                // 0.6875
#define FXP16_Q4_M_TWOPI            (101)               // 6.3125
#define FXP16_Q4_M_3PI_4            (38)                // 2.375
#define FXP16_Q4_M_SQRTPI           (28)                // 1.75
#define FXP16_Q4_M_SQRT3            (28)                // 1.75
#define FXP16_Q4_M_IVLN10           (7)                 // 0.4375
#define FXP16_Q4_M_LOG2_E           (23)                // 1.4375
#define FXP16_Q4_M_INVLN2           (23)                // 1.4375
#define FXP16_Q4_ONE_EIGHTH         (2)                 // 0.125
#define FXP16_Q4_ONE_QUARTER        (4)                 // 0.25
#define FXP16_Q4_THREE_EIGHTHS      (6)                 // 0.375
#define FXP16_Q4_ONE_HALF           (8)                 // 0.5
#define FXP16_Q4_FIVE_EIGHTHS       (10)                // 0.625
#define FXP16_Q4_THREE_QUARTERS     (12)                // 0.75
#define FXP16_Q4_SEVEN_EIGHTHS      (14)                // 0.875
#define FXP16_Q4_ONE_THIRD          (5)                 // 0.3125
#define FXP16_Q4_TWO_THIRDS         (11)                // 0.6875
#define FXP16_Q4_ONE                (16)                // 1.0
#define FXP16_Q4_MINUS_ONE          (-16)               // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q5                    (5)
#define FXP16_Q5_PREC               (0.03125)
#define FXP16_Q5_MAX                (1023.96875)
#define FXP16_Q5_MIN                (-1024.0)
// --------------------------------------------------------------------------
#define FXP16_Q5_M_E                (87)                // 2.71875
#define FXP16_Q5_M_LOG2E            (46)                // 1.4375
#define FXP16_Q5_M_LOG10E           (14)                // 0.4375
#define FXP16_Q5_M_LN2              (22)                // 0.6875
#define FXP16_Q5_M_PI               (101)               // 3.15625
#define FXP16_Q5_M_PI_2             (50)                // 1.5625
#define FXP16_Q5_M_PI_4             (25)                // 0.78125
#define FXP16_Q5_M_1_PI             (10)                // 0.3125
#define FXP16_Q5_M_2_PI             (20)                // 0.625
#define FXP16_Q5_M_2_SQRTPI         (36)                // 1.125
#define FXP16_Q5_M_SQRT2            (45)                // 1.40625
#define FXP16_Q5_M_SQRT1_2          (23)                // 0.71875
#define FXP16_Q5_M_TWOPI            (201)               // 6.28125
#define FXP16_Q5_M_3PI_4            (75)                // 2.34375
#define FXP16_Q5_M_SQRTPI           (57)                // 1.78125
#define FXP16_Q5_M_SQRT3            (55)                // 1.71875
#define FXP16_Q5_M_IVLN10           (14)                // 0.4375
#define FXP16_Q5_M_LOG2_E           (46)                // 1.4375
#define FXP16_Q5_M_INVLN2           (46)                // 1.4375
#define FXP16_Q5_ONE_EIGHTH         (4)                 // 0.125
#define FXP16_Q5_ONE_QUARTER        (8)                 // 0.25
#define FXP16_Q5_THREE_EIGHTHS      (12)                // 0.375
#define FXP16_Q5_ONE_HALF           (16)                // 0.5
#define FXP16_Q5_FIVE_EIGHTHS       (20)                // 0.625
#define FXP16_Q5_THREE_QUARTERS     (24)                // 0.75
#define FXP16_Q5_SEVEN_EIGHTHS      (28)                // 0.875
#define FXP16_Q5_ONE_THIRD          (11)                // 0.34375
#define FXP16_Q5_TWO_THIRDS         (21)                // 0.65625
#define FXP16_Q5_ONE                (32)                // 1.0
#define FXP16_Q5_MINUS_ONE          (-32)               // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q6                    (6)
#define FXP16_Q6_PREC               (0.015625)
#define FXP16_Q6_MAX                (511.984375)
#define FXP16_Q6_MIN                (-512.0)
// --------------------------------------------------------------------------
#define FXP16_Q6_M_E                (174)               // 2.71875
#define FXP16_Q6_M_LOG2E            (92)                // 1.4375
#define FXP16_Q6_M_LOG10E           (28)                // 0.4375
#define FXP16_Q6_M_LN2              (44)                // 0.6875
#define FXP16_Q6_M_PI               (201)               // 3.140625
#define FXP16_Q6_M_PI_2             (101)               // 1.578125
#define FXP16_Q6_M_PI_4             (50)                // 0.78125
#define FXP16_Q6_M_1_PI             (20)                // 0.3125
#define FXP16_Q6_M_2_PI             (41)                // 0.640625
#define FXP16_Q6_M_2_SQRTPI         (72)                // 1.125
#define FXP16_Q6_M_SQRT2            (91)                // 1.421875
#define FXP16_Q6_M_SQRT1_2          (45)                // 0.703125
#define FXP16_Q6_M_TWOPI            (402)               // 6.28125
#define FXP16_Q6_M_3PI_4            (151)               // 2.359375
#define FXP16_Q6_M_SQRTPI           (113)               // 1.765625
#define FXP16_Q6_M_SQRT3            (111)               // 1.734375
#define FXP16_Q6_M_IVLN10           (28)                // 0.4375
#define FXP16_Q6_M_LOG2_E           (92)                // 1.4375
#define FXP16_Q6_M_INVLN2           (92)                // 1.4375
#define FXP16_Q6_ONE_EIGHTH         (8)                 // 0.125
#define FXP16_Q6_ONE_QUARTER        (16)                // 0.25
#define FXP16_Q6_THREE_EIGHTHS      (24)                // 0.375
#define FXP16_Q6_ONE_HALF           (32)                // 0.5
#define FXP16_Q6_FIVE_EIGHTHS       (40)                // 0.625
#define FXP16_Q6_THREE_QUARTERS     (48)                // 0.75
#define FXP16_Q6_SEVEN_EIGHTHS      (56)                // 0.875
#define FXP16_Q6_ONE_THIRD          (21)                // 0.328125
#define FXP16_Q6_TWO_THIRDS         (43)                // 0.671875
#define FXP16_Q6_ONE                (64)                // 1.0
#define FXP16_Q6_MINUS_ONE          (-64)               // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q7                    (7)
#define FXP16_Q7_PREC               (0.0078125)
#define FXP16_Q7_MAX                (255.9921875)
#define FXP16_Q7_MIN                (-256.0)
// --------------------------------------------------------------------------
#define FXP16_Q7_M_E                (348)               // 2.71875
#define FXP16_Q7_M_LOG2E            (185)               // 1.4453125
#define FXP16_Q7_M_LOG10E           (56)                // 0.4375
#define FXP16_Q7_M_LN2              (89)                // 0.6953125
#define FXP16_Q7_M_PI               (402)               // 3.140625
#define FXP16_Q7_M_PI_2             (201)               // 1.5703125
#define FXP16_Q7_M_PI_4             (101)               // 0.7890625
#define FXP16_Q7_M_1_PI             (41)                // 0.3203125
#define FXP16_Q7_M_2_PI             (81)                // 0.6328125
#define FXP16_Q7_M_2_SQRTPI         (144)               // 1.125
#define FXP16_Q7_M_SQRT2            (181)               // 1.4140625
#define FXP16_Q7_M_SQRT1_2          (91)                // 0.7109375
#define FXP16_Q7_M_TWOPI            (804)               // 6.28125
#define FXP16_Q7_M_3PI_4            (302)               // 2.359375
#define FXP16_Q7_M_SQRTPI           (227)               // 1.7734375
#define FXP16_Q7_M_SQRT3            (222)               // 1.734375
#define FXP16_Q7_M_IVLN10           (56)                // 0.4375
#define FXP16_Q7_M_LOG2_E           (185)               // 1.4453125
#define FXP16_Q7_M_INVLN2           (185)               // 1.4453125
#define FXP16_Q7_ONE_EIGHTH         (16)                // 0.125
#define FXP16_Q7_ONE_QUARTER        (32)                // 0.25
#define FXP16_Q7_THREE_EIGHTHS      (48)                // 0.375
#define FXP16_Q7_ONE_HALF           (64)                // 0.5
#define FXP16_Q7_FIVE_EIGHTHS       (80)                // 0.625
#define FXP16_Q7_THREE_QUARTERS     (96)                // 0.75
#define FXP16_Q7_SEVEN_EIGHTHS      (112)               // 0.875
#define FXP16_Q7_ONE_THIRD          (43)                // 0.3359375
#define FXP16_Q7_TWO_THIRDS         (85)                // 0.6640625
#define FXP16_Q7_ONE                (128)               // 1.0
#define FXP16_Q7_MINUS_ONE          (-128)              // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q8                    (8)
#define FXP16_Q8_PREC               (0.00390625)
#define FXP16_Q8_MAX                (127.99609375)
#define FXP16_Q8_MIN                (-128.0)
// --------------------------------------------------------------------------
#define FXP16_Q8_M_E                (696)               // 2.71875
#define FXP16_Q8_M_LOG2E            (369)               // 1.44140625
#define FXP16_Q8_M_LOG10E           (111)               // 0.43359375
#define FXP16_Q8_M_LN2              (177)               // 0.69140625
#define FXP16_Q8_M_PI               (804)               // 3.140625
#define FXP16_Q8_M_PI_2             (402)               // 1.5703125
#define FXP16_Q8_M_PI_4             (201)               // 0.78515625
#define FXP16_Q8_M_1_PI             (81)                // 0.31640625
#define FXP16_Q8_M_2_PI             (163)               // 0.63671875
#define FXP16_Q8_M_2_SQRTPI         (289)               // 1.12890625
#define FXP16_Q8_M_SQRT2            (362)               // 1.4140625
#define FXP16_Q8_M_SQRT1_2          (181)               // 0.70703125
#define FXP16_Q8_M_TWOPI            (1608)              // 6.28125
#define FXP16_Q8_M_3PI_4            (603)               // 2.35546875
#define FXP16_Q8_M_SQRTPI           (454)               // 1.7734375
#define FXP16_Q8_M_SQRT3            (443)               // 1.73046875
#define FXP16_Q8_M_IVLN10           (111)               // 0.43359375
#define FXP16_Q8_M_LOG2_E           (369)               // 1.44140625
#define FXP16_Q8_M_INVLN2           (369)               // 1.44140625
#define FXP16_Q8_ONE_EIGHTH         (32)                // 0.125
#define FXP16_Q8_ONE_QUARTER        (64)                // 0.25
#define FXP16_Q8_THREE_EIGHTHS      (96)                // 0.375
#define FXP16_Q8_ONE_HALF           (128)               // 0.5
#define FXP16_Q8_FIVE_EIGHTHS       (160)               // 0.625
#define FXP16_Q8_THREE_QUARTERS     (192)               // 0.75
#define FXP16_Q8_SEVEN_EIGHTHS      (224)               // 0.875
#define FXP16_Q8_ONE_THIRD          (85)                // 0.33203125
#define FXP16_Q8_TWO_THIRDS         (171)               // 0.66796875
#define FXP16_Q8_ONE                (256)               // 1.0
#define FXP16_Q8_MINUS_ONE          (-256)              // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q9                    (9)
#define FXP16_Q9_PREC               (0.001953125)
#define FXP16_Q9_MAX                (63.998046875)
#define FXP16_Q9_MIN                (-64.0)
// --------------------------------------------------------------------------
#define FXP16_Q9_M_E                (1392)              // 2.71875
#define FXP16_Q9_M_LOG2E            (739)               // 1.443359375
#define FXP16_Q9_M_LOG10E           (222)               // 0.43359375
#define FXP16_Q9_M_LN2              (355)               // 0.693359375
#define FXP16_Q9_M_PI               (1608)              // 3.140625
#define FXP16_Q9_M_PI_2             (804)               // 1.5703125
#define FXP16_Q9_M_PI_4             (402)               // 0.78515625
#define FXP16_Q9_M_1_PI             (163)               // 0.318359375
#define FXP16_Q9_M_2_PI             (326)               // 0.63671875
#define FXP16_Q9_M_2_SQRTPI         (578)               // 1.12890625
#define FXP16_Q9_M_SQRT2            (724)               // 1.4140625
#define FXP16_Q9_M_SQRT1_2          (362)               // 0.70703125
#define FXP16_Q9_M_TWOPI            (3217)              // 6.283203125
#define FXP16_Q9_M_3PI_4            (1206)              // 2.35546875
#define FXP16_Q9_M_SQRTPI           (907)               // 1.771484375
#define FXP16_Q9_M_SQRT3            (887)               // 1.732421875
#define FXP16_Q9_M_IVLN10           (222)               // 0.43359375
#define FXP16_Q9_M_LOG2_E           (739)               // 1.443359375
#define FXP16_Q9_M_INVLN2           (739)               // 1.443359375
#define FXP16_Q9_ONE_EIGHTH         (64)                // 0.125
#define FXP16_Q9_ONE_QUARTER        (128)               // 0.25
#define FXP16_Q9_THREE_EIGHTHS      (192)               // 0.375
#define FXP16_Q9_ONE_HALF           (256)               // 0.5
#define FXP16_Q9_FIVE_EIGHTHS       (320)               // 0.625
#define FXP16_Q9_THREE_QUARTERS     (384)               // 0.75
#define FXP16_Q9_SEVEN_EIGHTHS      (448)               // 0.875
#define FXP16_Q9_ONE_THIRD          (171)               // 0.333984375
#define FXP16_Q9_TWO_THIRDS         (341)               // 0.666015625
#define FXP16_Q9_ONE                (512)               // 1.0
#define FXP16_Q9_MINUS_ONE          (-512)              // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q10                   (10)
#define FXP16_Q10_PREC              (0.0009765625)
#define FXP16_Q10_MAX               (31.9990234375)
#define FXP16_Q10_MIN               (-32.0)
// --------------------------------------------------------------------------
#define FXP16_Q10_M_E               (2784)              // 2.71875
#define FXP16_Q10_M_LOG2E           (1477)              // 1.4423828125
#define FXP16_Q10_M_LOG10E          (445)               // 0.4345703125
#define FXP16_Q10_M_LN2             (710)               // 0.693359375
#define FXP16_Q10_M_PI              (3217)              // 3.1416015625
#define FXP16_Q10_M_PI_2            (1608)              // 1.5703125
#define FXP16_Q10_M_PI_4            (804)               // 0.78515625
#define FXP16_Q10_M_1_PI            (326)               // 0.318359375
#define FXP16_Q10_M_2_PI            (652)               // 0.63671875
#define FXP16_Q10_M_2_SQRTPI        (1155)              // 1.1279296875
#define FXP16_Q10_M_SQRT2           (1448)              // 1.4140625
#define FXP16_Q10_M_SQRT1_2         (724)               // 0.70703125
#define FXP16_Q10_M_TWOPI           (6434)              // 6.283203125
#define FXP16_Q10_M_3PI_4           (2413)              // 2.3564453125
#define FXP16_Q10_M_SQRTPI          (1815)              // 1.7724609375
#define FXP16_Q10_M_SQRT3           (1774)              // 1.732421875
#define FXP16_Q10_M_IVLN10          (445)               // 0.4345703125
#define FXP16_Q10_M_LOG2_E          (1477)              // 1.4423828125
#define FXP16_Q10_M_INVLN2          (1477)              // 1.4423828125
#define FXP16_Q10_ONE_EIGHTH        (128)               // 0.125
#define FXP16_Q10_ONE_QUARTER       (256)               // 0.25
#define FXP16_Q10_THREE_EIGHTHS     (384)               // 0.375
#define FXP16_Q10_ONE_HALF          (512)               // 0.5
#define FXP16_Q10_FIVE_EIGHTHS      (640)               // 0.625
#define FXP16_Q10_THREE_QUARTERS    (768)               // 0.75
#define FXP16_Q10_SEVEN_EIGHTHS     (896)               // 0.875
#define FXP16_Q10_ONE_THIRD         (341)               // 0.3330078125
#define FXP16_Q10_TWO_THIRDS        (683)               // 0.6669921875
#define FXP16_Q10_ONE               (1024)              // 1.0
#define FXP16_Q10_MINUS_ONE         (-1024)             // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q11                   (11)
#define FXP16_Q11_PREC              (0.00048828125)
#define FXP16_Q11_MAX               (15.99951171875)
#define FXP16_Q11_MIN               (-16.0)
// --------------------------------------------------------------------------
#define FXP16_Q11_M_E               (5567)              // 2.71826171875
#define FXP16_Q11_M_LOG2E           (2955)              // 1.44287109375
#define FXP16_Q11_M_LOG10E          (889)               // 0.43408203125
#define FXP16_Q11_M_LN2             (1420)              // 0.693359375
#define FXP16_Q11_M_PI              (6434)              // 3.1416015625
#define FXP16_Q11_M_PI_2            (3217)              // 1.57080078125
#define FXP16_Q11_M_PI_4            (1608)              // 0.78515625
#define FXP16_Q11_M_1_PI            (652)               // 0.318359375
#define FXP16_Q11_M_2_PI            (1304)              // 0.63671875
#define FXP16_Q11_M_2_SQRTPI        (2311)              // 1.12841796875
#define FXP16_Q11_M_SQRT2           (2896)              // 1.4140625
#define FXP16_Q11_M_SQRT1_2         (1448)              // 0.70703125
#define FXP16_Q11_M_TWOPI           (12868)             // 6.283203125
#define FXP16_Q11_M_3PI_4           (4825)              // 2.35595703125
#define FXP16_Q11_M_SQRTPI          (3630)              // 1.7724609375
#define FXP16_Q11_M_SQRT3           (3547)              // 1.73193359375
#define FXP16_Q11_M_IVLN10          (889)               // 0.43408203125
#define FXP16_Q11_M_LOG2_E          (2955)              // 1.44287109375
#define FXP16_Q11_M_INVLN2          (2955)              // 1.44287109375
#define FXP16_Q11_ONE_EIGHTH        (256)               // 0.125
#define FXP16_Q11_ONE_QUARTER       (512)               // 0.25
#define FXP16_Q11_THREE_EIGHTHS     (768)               // 0.375
#define FXP16_Q11_ONE_HALF          (1024)              // 0.5
#define FXP16_Q11_FIVE_EIGHTHS      (1280)              // 0.625
#define FXP16_Q11_THREE_QUARTERS    (1536)              // 0.75
#define FXP16_Q11_SEVEN_EIGHTHS     (1792)              // 0.875
#define FXP16_Q11_ONE_THIRD         (683)               // 0.33349609375
#define FXP16_Q11_TWO_THIRDS        (1365)              // 0.66650390625
#define FXP16_Q11_ONE               (2048)              // 1.0
#define FXP16_Q11_MINUS_ONE         (-2048)             // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q12                   (12)
#define FXP16_Q12_PREC              (0.000244140625)
#define FXP16_Q12_MAX               (7.999755859375)
#define FXP16_Q12_MIN               (-8.0)
// --------------------------------------------------------------------------
#define FXP16_Q12_M_E               (11134)             // 2.71826171875
#define FXP16_Q12_M_LOG2E           (5909)              // 1.442626953125
#define FXP16_Q12_M_LOG10E          (1779)              // 0.434326171875
#define FXP16_Q12_M_LN2             (2839)              // 0.693115234375
#define FXP16_Q12_M_PI              (12868)             // 3.1416015625
#define FXP16_Q12_M_PI_2            (6434)              // 1.57080078125
#define FXP16_Q12_M_PI_4            (3217)              // 0.785400390625
#define FXP16_Q12_M_1_PI            (1304)              // 0.318359375
#define FXP16_Q12_M_2_PI            (2608)              // 0.63671875
#define FXP16_Q12_M_2_SQRTPI        (4622)              // 1.12841796875
#define FXP16_Q12_M_SQRT2           (5793)              // 1.414306640625
#define FXP16_Q12_M_SQRT1_2         (2896)              // 0.70703125
#define FXP16_Q12_M_TWOPI           (25736)             // 6.283203125
#define FXP16_Q12_M_3PI_4           (9651)              // 2.356201171875
#define FXP16_Q12_M_SQRTPI          (7260)              // 1.7724609375
#define FXP16_Q12_M_SQRT3           (7094)              // 1.73193359375
#define FXP16_Q12_M_IVLN10          (1779)              // 0.434326171875
#define FXP16_Q12_M_LOG2_E          (5909)              // 1.442626953125
#define FXP16_Q12_M_INVLN2          (5909)              // 1.442626953125
#define FXP16_Q12_ONE_EIGHTH        (512)               // 0.125
#define FXP16_Q12_ONE_QUARTER       (1024)              // 0.25
#define FXP16_Q12_THREE_EIGHTHS     (1536)              // 0.375
#define FXP16_Q12_ONE_HALF          (2048)              // 0.5
#define FXP16_Q12_FIVE_EIGHTHS      (2560)              // 0.625
#define FXP16_Q12_THREE_QUARTERS    (3072)              // 0.75
#define FXP16_Q12_SEVEN_EIGHTHS     (3584)              // 0.875
#define FXP16_Q12_ONE_THIRD         (1365)              // 0.333251953125
#define FXP16_Q12_TWO_THIRDS        (2731)              // 0.666748046875
#define FXP16_Q12_ONE               (4096)              // 1.0
#define FXP16_Q12_MINUS_ONE         (-4096)             // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q13                   (13)
#define FXP16_Q13_PREC              (0.0001220703125)
#define FXP16_Q13_MAX               (3.9998779296875)
#define FXP16_Q13_MIN               (-4.0)
// --------------------------------------------------------------------------
#define FXP16_Q13_M_E               (22268)             // 2.71826171875
#define FXP16_Q13_M_LOG2E           (11819)             // 1.4427490234375
#define FXP16_Q13_M_LOG10E          (3558)              // 0.434326171875
#define FXP16_Q13_M_LN2             (5678)              // 0.693115234375
#define FXP16_Q13_M_PI              (25736)             // 3.1416015625
#define FXP16_Q13_M_PI_2            (12868)             // 1.57080078125
#define FXP16_Q13_M_PI_4            (6434)              // 0.785400390625
#define FXP16_Q13_M_1_PI            (2608)              // 0.318359375
#define FXP16_Q13_M_2_PI            (5215)              // 0.6365966796875
#define FXP16_Q13_M_2_SQRTPI        (9244)              // 1.12841796875
#define FXP16_Q13_M_SQRT2           (11585)             // 1.4141845703125
#define FXP16_Q13_M_SQRT1_2         (5793)              // 0.7071533203125
#define FXP32_Q13_M_TWOPI           (51472)             // 6.283203125
#define FXP16_Q13_M_3PI_4           (19302)             // 2.356201171875
#define FXP16_Q13_M_SQRTPI          (14520)             // 1.7724609375
#define FXP16_Q13_M_SQRT3           (14189)             // 1.7320556640625
#define FXP16_Q13_M_IVLN10          (3558)              // 0.434326171875
#define FXP16_Q13_M_LOG2_E          (11819)             // 1.4427490234375
#define FXP16_Q13_M_INVLN2          (11819)             // 1.4427490234375
#define FXP16_Q13_ONE_EIGHTH        (1024)              // 0.125
#define FXP16_Q13_ONE_QUARTER       (2048)              // 0.25
#define FXP16_Q13_THREE_EIGHTHS     (3072)              // 0.375
#define FXP16_Q13_ONE_HALF          (4096)              // 0.5
#define FXP16_Q13_FIVE_EIGHTHS      (5120)              // 0.625
#define FXP16_Q13_THREE_QUARTERS    (6144)              // 0.75
#define FXP16_Q13_SEVEN_EIGHTHS     (7168)              // 0.875
#define FXP16_Q13_ONE_THIRD         (2731)              // 0.3333740234375
#define FXP16_Q13_TWO_THIRDS        (5461)              // 0.6666259765625
#define FXP16_Q13_ONE               (8192)              // 1.0
#define FXP16_Q13_MINUS_ONE         (-8192)             // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q14                   (14)
#define FXP16_Q14_PREC              (6.103515625e-05)
#define FXP16_Q14_MAX               (1.99993896484375)
#define FXP16_Q14_MIN               (-2.0)
// --------------------------------------------------------------------------
#define FXP32_Q14_M_E               (44536)             // 2.71826171875
#define FXP16_Q14_M_LOG2E           (23637)             // 1.44268798828125
#define FXP16_Q14_M_LOG10E          (7115)              // 0.43426513671875
#define FXP16_Q14_M_LN2             (11357)             // 0.69317626953125
#define FXP32_Q14_M_PI              (51472)             // 3.1416015625
#define FXP16_Q14_M_PI_2            (25736)             // 1.57080078125
#define FXP16_Q14_M_PI_4            (12868)             // 0.785400390625
#define FXP16_Q14_M_1_PI            (5215)              // 0.31829833984375
#define FXP16_Q14_M_2_PI            (10430)             // 0.6365966796875
#define FXP16_Q14_M_2_SQRTPI        (18487)             // 1.12835693359375
#define FXP16_Q14_M_SQRT2           (23170)             // 1.4141845703125
#define FXP16_Q14_M_SQRT1_2         (11585)             // 0.70709228515625
#define FXP32_Q14_M_TWOPI           (102944)            // 6.283203125
#define FXP32_Q14_M_3PI_4           (38604)             // 2.356201171875
#define FXP16_Q14_M_SQRTPI          (29040)             // 1.7724609375
#define FXP16_Q14_M_SQRT3           (28378)             // 1.7320556640625
#define FXP16_Q14_M_IVLN10          (7115)              // 0.43426513671875
#define FXP16_Q14_M_LOG2_E          (23637)             // 1.44268798828125
#define FXP16_Q14_M_INVLN2          (23637)             // 1.44268798828125
#define FXP16_Q14_ONE_EIGHTH        (2048)              // 0.125
#define FXP16_Q14_ONE_QUARTER       (4096)              // 0.25
#define FXP16_Q14_THREE_EIGHTHS     (6144)              // 0.375
#define FXP16_Q14_ONE_HALF          (8192)              // 0.5
#define FXP16_Q14_FIVE_EIGHTHS      (10240)             // 0.625
#define FXP16_Q14_THREE_QUARTERS    (12288)             // 0.75
#define FXP16_Q14_SEVEN_EIGHTHS     (14336)             // 0.875
#define FXP16_Q14_ONE_THIRD         (5461)              // 0.33331298828125
#define FXP16_Q14_TWO_THIRDS        (10923)             // 0.66668701171875
#define FXP16_Q14_ONE               (16384)             // 1.0
#define FXP16_Q14_MINUS_ONE         (-16384)            // -1.0


// --------------------------------------------------------------------------
#define FXP16_Q15                   (15)
#define FXP16_Q15_PREC              (3.0517578125e-05)
#define FXP16_Q15_MAX               (0.999969482421875)
#define FXP16_Q15_MIN               (-1.0)
// --------------------------------------------------------------------------
#define FXP32_Q15_M_E               (89073)             // 2.718292236328125
#define FXP32_Q15_M_LOG2E           (47274)             // 1.44268798828125
#define FXP16_Q15_M_LOG10E          (14231)             // 0.434295654296875
#define FXP16_Q15_M_LN2             (22713)             // 0.693145751953125
#define FXP32_Q15_M_PI              (102944)            // 3.1416015625
#define FXP32_Q15_M_PI_2            (51472)             // 1.57080078125
#define FXP16_Q15_M_PI_4            (25736)             // 0.785400390625
#define FXP16_Q15_M_1_PI            (10430)             // 0.31829833984375
#define FXP16_Q15_M_2_PI            (20861)             // 0.636627197265625
#define FXP32_Q15_M_2_SQRTPI        (36975)             // 1.128387451171875
#define FXP32_Q15_M_SQRT2           (46341)             // 1.414215087890625
#define FXP16_Q15_M_SQRT1_2         (23170)             // 0.70709228515625
#define FXP32_Q15_M_TWOPI           (205887)            // 6.283172607421875
#define FXP32_Q15_M_3PI_4           (77208)             // 2.356201171875
#define FXP32_Q15_M_SQRTPI          (58080)             // 1.7724609375
#define FXP32_Q15_M_SQRT3           (56756)             // 1.7320556640625
#define FXP16_Q15_M_IVLN10          (14231)             // 0.434295654296875
#define FXP32_Q15_M_LOG2_E          (47274)             // 1.44268798828125
#define FXP32_Q15_M_INVLN2          (47274)             // 1.44268798828125
#define FXP16_Q15_ONE_EIGHTH        (4096)              // 0.125
#define FXP16_Q15_ONE_QUARTER       (8192)              // 0.25
#define FXP16_Q15_THREE_EIGHTHS     (12288)             // 0.375
#define FXP16_Q15_ONE_HALF          (16384)             // 0.5
#define FXP16_Q15_FIVE_EIGHTHS      (20480)             // 0.625
#define FXP16_Q15_THREE_QUARTERS    (24576)             // 0.75
#define FXP16_Q15_SEVEN_EIGHTHS     (28672)             // 0.875
#define FXP16_Q15_ONE_THIRD         (10923)             // 0.333343505859375
#define FXP16_Q15_TWO_THIRDS        (21845)             // 0.666656494140625
#define FXP32_Q15_ONE               (32768)             // 1.0
#define FXP16_Q15_ALMOST_ONE        (32767)             // ~1.0
#define FXP16_Q15_MINUS_ONE         (-32768)            // -1.0



#define FXP16_Q14_ASIN_A     (25735)          /* 1.57073974609375 */
#define FXP16_Q14_ASIN_B     (-3479)          /* -0.21234130859375 */
#define FXP16_Q14_ASIN_C     (1241)           /* 0.07574462890625 */
#define FXP16_Q14_ASIN_D     (-331)           /* -0.02020263671875 */

#define FXP16_EXP_ITERATIONS         13
#define FXP16_LOG_ITERATIONS         4
#define FXP16_SQRT_ITERATIONS        11
#define FXP16_HYPOT_ITERATIONS        18
#define FXP16_CBRT_ITERATIONS        17

typedef int16_t fxp16_t;
typedef int32_t fxp32_t;


/*!
    \brief      Macro that saturates a result to min and max
    \details    This macro limits a variable to a minimum and maximum value
    \param   var     Variable to be limited. Has to be able to hold minimum or maximum value.
*/
#define fpxx_sat_m(var,min,max) \
    do{var=(var<min)?(min):((var>max)?(max):(var));}while(0)


/*!
    \brief      Macro that saturates a result to fixed point 16 bit limits
    \details    This macro checks if the int32_t result of a previously performed calculation
                results in a fixed point overflow. If so the result is saturated to the
                extends of the fixed point type.
    \param   var     Fixed point int32_t result to be saturated
*/
#define fxp16_sat_m(var) \
        fpxx_sat_m(var,INT16_MIN,INT16_MAX)

fxp16_t fxp16_sat(fxp32_t fxp32);


/*!
    \brief      Macro that saturates a result to fixed point 32 bits limits
    \details    This macro checks if the int64_t result of a previously performed calculation
                results in a fixed point overflow. If so the result is saturated to the
                extends of the fixed point type.
    \param[in]  var     Fixed point int64_t result to be saturated
*/
#define fxp32_sat_m(var) \
        fpxx_sat_m(var,INT32_MIN,INT32_MAX)


/*!
  \brief        Macro to arithmetically right shift fixed point number
  \details      Macro to arithmetically right shift fixed point number

                When shifting left, there is no difference between arithmetic and logical shift.

                When shifting right, the type of shift depends on the type of the value being shifted.
                A "logical" right shift by 1 bit shifts all the bits to the right and fills in the
                leftmost bit with a 0.

                An "arithmetic" shift leaves the original value in the leftmost bit.
                The difference becomes important when dealing with negative numbers.)

                When shifting an unsigned value, the >> operator in C is a logical shift.
                When shifting a signed value, the >> operator is an arithmetic shift.

  \param        result      Fixed point number to right shift (int32_t format)
  \param        shift       Number of bits to right shift
*/

#define FXP16CONF_ARSHIFT_W_ROUNDING 1

#if FXP16CONF_ARSHIFT_W_ROUNDING

    #define fpxx_arshift_m(result,rshift)                                           \
        do{                                                                         \
            if (rshift > 0){                                                       \
            result = (result)>>(rshift-1);                                          \
            result = fxp16_signbit(result)?(result>>1):((result>>1) + (result&1));   \
            }}while(0)

#else
    #define fpxx_arshift_m(result,rshift) do{result>>=rshift;}while(0)
#endif


#if FXP16CONF_ARSHIFT_W_ROUNDING
    fxp32_t fxp32_arshift(fxp32_t var,uint8_t rshift);
#else
    #define fxp32_arshift(var,rshift)    ((var)>>rshift)
#endif



#if FXP16CONF_ARSHIFT_W_ROUNDING
    fxp16_t fxp16_arshift(fxp16_t fp, uint8_t shift);
#else
    #define fxp16_arshift(var,rshift)    ((var)>>rshift)
#endif


/*!
  \brief      Macro to left shift fixed point number
  \details    Macro to left shift fixed point number
  \param      result   Fixed point number to left shift (int32_t format)
  \param      shift    Number of bits to left shift

*/
#define fpxx_alshift_m(result,shift) do{ result<<=shift; }while(0)


/*!
  \brief      Macro to shift fixed point number in any direction
  \details    Macro to shift fixed point number in any direction
  \param      result   Fixed point number to left shift (int32_t format)
  \param      shift    Number of bits to shift (+ = rshift, - = lshift)
*/
#define fpxx_ashift_m(result,shift)               \
 do {                                             \
    if ( (shift) > 0 )                              \
    {                                             \
        fpxx_arshift_m(result,(shift));             \
    }                                             \
    else if( shift < 0 )                          \
    {                                             \
       fpxx_alshift_m(result,-(shift));             \
    }                                             \
 }while(0)


/*!
   \brief      Macro to perform an integer cast on a fixed point number
   \details    Macro to perform an integer cast on a fixed point number
   \param      result   Fixed point number to left cast to int
   \param      frac     Number of fractional bits
*/
#define fxp16_intcast_m(result,frac)               \
 do {                                             \
   if ( fxp16_signbit(result) )                    \
   {                                              \
      result = -((-result)&~((1<<frac)-1));       \
   }                                              \
   else                                           \
   {                                              \
      result&=~((1<<frac)-1);                     \
   }                                              \
 }while(0)


/* fixed point conversions */
fxp16_t fxp16_flt2fp(float var, uint8_t frac);
float fxp16_fp2flt(fxp16_t var, uint8_t frac);
fxp16_t fxp16_int2fp(int16_t intpart, uint8_t frac);
fxp16_t fxp16_fp2fp(fxp16_t fp, uint8_t fracold, uint8_t fracnew);
fxp16_t fxp16_rshift(fxp16_t fp, uint8_t shift);
fxp16_t fxp16_lshift(fxp16_t fp, uint8_t shift);


/*!
    \brief      Compute square root
    \details    Returns the square root of x.
                If a domain error occurs, the global variable errno is set to EDOM.
    \param      fp  Fixed point variable to be taken square root of
    \param      frac     Number of fracional bits

    \returns     Returns the square root of x, with the same number of fractional bits as x.
*/
fxp16_t fxp16_sqrt(fxp16_t  x, uint8_t frac_bits);


/*!
    \brief      Compute cubic root
    \details    Returns the cubic root of fp.

    \param      fp      Value whose cubic root is computed.
    \param      frac    Number of fracional bits

    \returns     Returns the cubic root of fp, with the same number of fractional bits as fp.
*/
fxp16_t fxp16_cbrt(fxp16_t s, uint8_t sfrac);




/*!
    \brief      Converts a fixed point type to integer
    \details    Converts a fixed point variable to an integer variable.
    \param[in]  fp  Fixed point variable to be converted
    \param[in]  frac     Number of fracional bits

    \returns Integer interpretation of provided fixed point number
*/
#define fxp16_fp2int(fp,frac) fxp16_lround(fp,frac)

/* Basic math operations */
fxp16_t fxp16_add(fxp16_t summand1, fxp16_t summand2);
fxp16_t fxp16_sub(fxp16_t minuend, fxp16_t subtrahend);
fxp16_t fxp16_mult(fxp16_t mult1, uint8_t frac1, fxp16_t mult2, uint8_t frac2);
fxp16_t fxp16_div(fxp16_t divident, uint8_t frac1, fxp16_t divisor, uint8_t frac2);


/*!
    \defgroup   fxp16_trig Trigonometric functions (π-normalized fixed-point)
    \brief      CORDIC-based trig for Q1.15 with π-normalized angles.

    ## Angle format (input & output for angle-returning APIs)
    - **Representation:** Q1.15 scaled by π
    - **Domain:** [-1.0, +1.0)  <->  [-π, +π)
    - **Examples:** 0 <-> 0 rad, +0.5 <-> +π/2, −0.5 <-> −π/2

    ## Value formats (function results)
    | Function           | Result format           | Range / Notes                          |
    |--------------------|-------------------------|----------------------------------------|
    | fxp16_sin, fxp16_cos | Q1.15                   | [-1.0, +1.0]                           |
    | fxp16_tan           | Q(?, frac)              | `frac` fractional bits (caller sets)   |
    | fxp16_atan2         | Q1.15 (angle, π-norm)   | (−1.0, +1.0] <-> (−π, +π]              |
    | fxp16_atan          | Q1.15 (angle, π-norm)   | [−0.5, +0.5] <-> [−π/2, +π/2]          |
    | fxp16_asin          | Q1.15 (angle, π-norm)   | [−0.5, +0.5]                           |
    | fxp16_acos          | Q1.15 (angle, π-norm)   | [0.0, +1.0] <-> [0, π]                 |


    ## Implementation
    - 14 CORDIC iterations (sufficient for Q1.15 angle resolution).
    - Pre-scaled CORDIC gain: `K ≈ 0.607252935` (Q1.15 `0x4DBA`).
    - Lookup: `atan(2^-i)/π` table in Q1.15 for i=0..13.

@{ */

/*!
    \brief      Computes sine of a provided angle
    \details    Computes sine of a provided angle. The angle must be fixed point number in Q1.15 format.
                The allowed range is from -1.0 to 0.999969482421875, representing an angle from
                -PI to +PI in radians minus the fraction caused by the last LSB up to +PI.

                The result returned is of fixed point type Q14 [-1.0,+1.0]

    \param[in]  fp        The angle must be fixed point number in Q1.15 format [-1.0,+1.0-LSB]

    \returns The result returned is of fixed point type Q2.14 [-1.0,+1.0]
*/
fxp16_t fxp16_sin(fxp16_t fp);
/*!
    \brief      Computes cosine of a provided angle
    \details    Computes cosine of a provided angle. The angle must be fixed point number in Q1.15 format.
                The allowed range is from -1.0 to 0.999969482421875, representing an angle from
                -PI to +PI in radians minus the fraction caused by the last LSB up to +PI.



    \param[in]  fp        The angle must be fixed point number in Q15 format [-1.0,+1.0-LSB]

    \returns The result returned is of fixed point type Q14 [-1.0,+1.0]
*/
fxp16_t fxp16_cos(fxp16_t fp);
/*!
    \brief      Computes tangens of a provided angle
    \details    Computes tanges of a provided angle. The angle must be fixed point number in Q1.15 format.
                The allowed range is from -1.0 to 0.999969482421875, representing an angle from
                -PI to +PI in radians minus the fraction caused by the last LSB up to +PI.

                If a domain error occurs, the global variable errno is set to EDOM.



    \param    fp        The angle must be fixed point number in Q15 format [-1.0,+1.0-LSB]
    \param    fract     Fractional bits of the result

    \returns The result returned is of fixed point type with fractional bits as defined by frac parameter
*/
fxp16_t fxp16_tan(fxp16_t fp, uint8_t frac);
/*!
    \brief      Compute arc tangent with two arguments
    \details    Returns the principal value of \c atan2(y, x), i.e., the angle of the vector (x, y).
                Unlike \c atan, this function determines the correct quadrant from the signs of both
                arguments. Angles are returned in π-normalized fixed-point Q1.15:
                [-1.0, +1.0] <-> [-π, +π]. By convention, atan2(0, 0) returns 0.

    \param    y         Y component (numerator) in Q1.15.
    \param    x         X component (denominator) in Q1.15.

    \returns    Angle in Q1.15, π-normalized. Principal value in (−1.0, +1.0] corresponding to (−π, +π].
*/
fxp16_t fxp16_atan2 (fxp16_t y, fxp16_t x);
/*!
    \brief      Compute arc tangent
    \details    Returns the principal value of the arc tangent of x, expressed in radians.
                In trigonometrics, arc tangent is the inverse operation of tangent.
                Notice that because of the sign ambiguity, the function cannot determine with certainty
                in which quadrant the angle falls only by its tangent value. See atan2 for an alternative
                that takes a fractional argument instead.

    \param    fp        Value whose arc tangent is computed.
    \param    frac      Fixed point format of provided input parameter fp

    \returns The result returned is of fixed point type Q14 format.
             Principal arc tangent of x, in the interval [-π/2,+π/2] radians.
             One radian is equivalent to 180/π degrees.

*/
fxp16_t fxp16_atan(fxp16_t y, uint8_t frac);
/*!
    \brief      Compute arc sine
    \details    Returns the principal value of the arc sine of x, expressed in radians.
                In trigonometrics, arc sine is the inverse operation of sine.

                If a domain error occurs, the global variable errno is set to EDOM.



    \param    fp        The angle must be fixed point number in Q14 format.
                        Value whose arc sine is computed, in the interval [-1,+1].
                        If the argument is out of this interval, a domain error occurs.


    \returns The result returned is of fixed point type Q14 format. Principal arc sine of x, in the interval [-pi/2,+pi/2] radians. One radian is equivalent to 180/PI degrees.
*/
fxp16_t fxp16_asin(fxp16_t fp);
/*!
    \brief      Compute arc cosine
    \details    Returns the principal value of the arc cosine of x, expressed in radians.
                In trigonometrics, arc cosine is the inverse operation of cosine.

                If a domain error occurs, the global variable errno is set to EDOM.

                The result returned is of fixed point type specified by parameter frac

    \param    fp        The angle must be fixed point number in Q14 format.
                        Value whose arc cosine is computed, in the interval [-1,+1].
                        If the argument is out of this interval, a domain error occurs.


    \returns The result returned is of fixed point type Q13 format. Principal arc cosine of x, in the interval [0,pi] radians. One radian is equivalent to 180/PI degrees.
*/
fxp16_t fxp16_acos(fxp16_t x);

/*! @} */





/*!
    \brief      fxp16 sinh with format conversion, CORDIC core, and saturation
    \details    Computes \p sinh(x) where \p x is an fxp16 fixed-point value with
                \p x_frac fractional bits. Internally, \p x is promoted to 32-bit,
                rescaled to Q15, evaluated by fxp32_cordic_cosh_sinh_q15, and the
                sinh component is then rescaled to the target format with \p y_frac
                fractional bits and saturated to the fxp16 range.

    \param[in]  y_frac   Fractional-bit count of the result format (fxp16 Qy_frac).
    \param[in]  x        fxp16 input value.
    \param[in]  x_frac   Fractional-bit count of the input format (fxp16 Qx_frac).

    \returns    \p sinh(x) as fxp16 in Qy_frac, saturated to the fxp16 limits.
*/
fxp16_t fxp16_sinh(uint8_t y_frac, fxp16_t x, uint8_t  x_frac);

/*!
    \brief      fxp16 cosh with format conversion, CORDIC core, and saturation
    \details    Computes \p cosh(x) where \p x is an fxp16 fixed-point value with
                \p x_frac fractional bits. The value is promoted to 32-bit, rescaled
                to Q15, evaluated by fxp32_cordic_cosh_sinh_q15, then the cosh component
                is rescaled to the target fxp16 format with \p y_frac fractional bits
                and saturated to the fxp16 range.

    \param[in]  y_frac   Fractional-bit count of the result format (fxp16 Qy_frac).
    \param[in]  x        fxp16 input value.
    \param[in]  x_frac   Fractional-bit count of the input format (fxp16 Qx_frac).

    \returns    \p cosh(x) as fxp16 in Qy_frac, saturated to fxp16 limits.
*/
fxp16_t fxp16_cosh(uint8_t y_frac, fxp16_t x, uint8_t  x_frac);

/*!
    \brief      fxp16 tanh with format conversion and saturation
    \details    Computes \p tanh(x) where \p x is an fxp16 fixed-point value with
                \p x_frac fractional bits. The value is promoted to 32-bit, rescaled
                to Q15, evaluated via fxp32_cordic_tanh_q15 (with early saturation),
                then rescaled to the fxp16 target with \p y_frac fractional bits and
                saturated to the fxp16 range.

    \param[in]  y_frac   Fractional-bit count of the result format (fxp16 Qy_frac).
    \param[in]  x        fxp16 input value.
    \param[in]  x_frac   Fractional-bit count of the input format (fxp16 Qx_frac).

    \returns    \p tanh(x) as fxp16 in Qy_frac, saturated to fxp16 limits.
*/
fxp16_t fxp16_tanh(uint8_t y_frac, fxp16_t x, uint8_t x_frac);


fxp16_t fxp16_atanh(uint8_t y_frac, fxp16_t x, uint8_t x_frac);




fxp16_t fxp16_exp(uint8_t y_frac, fxp16_t x, uint8_t x_frac);


fxp16_t fxp16_log2(uint8_t y_frac, fxp16_t x, uint8_t x_frac);
fxp16_t fxp16_log(uint8_t y_frac, fxp16_t x, uint8_t x_frac);
fxp16_t fxp16_log10(uint8_t y_frac, fxp16_t x, uint8_t x_frac);
fxp16_t fxp16_log1p(uint8_t y_frac, fxp16_t x, uint8_t x_frac);


#if (FXP16_RADIX == 2)
    #define fxp16_logb fxp16_log2
#else
    #error "fxp16_logb not defined!"
#endif


int fxp16_ilogb(fxp16_t x, uint8_t x_frac);


/*!
    \defgroup   fxp16_rounding Rounding and remainder functions
    \brief      Fixed-point rounding utilities and remainder computation.
    \details    This module provides helpers to round fixed-point values to integral boundaries
                and to compute the remainder of a division. The functions operate on \c fxp16_t
                numbers with a caller-supplied number of fractional bits and include both
                function and macro forms.

            **Provided APIs**
            - \ref fxp16_ceil  – round upward to the next integral boundary.
            - \ref fxp16_floor – round downward (macro).
            - \ref fxp16_trunc – round toward zero (macro).
            - \ref fxp16_round – round to nearest, halfway cases away from zero.
            - \ref fxp16_fmod  – remainder with truncated quotient (like C \c fmod).
            - \ref fxp16_lround – round to nearest and cast to \c int.

            **Notes**
            - All rounding functions interpret \p xfrac as the number of fractional bits
              in the argument.
            - Macros evaluate their arguments; avoid passing expressions with side effects.

@{
*/

/*!
    \brief      Round up value
    \details    Rounds x upward, returning the smallest integral value that is not less than x.


    \param      x       Value to round up.
    \param      xfrac   Number of fractional bits for x.
    \return     The smallest integral value that is not less than x (as a fixed-point value).
*/
fxp16_t fxp16_ceil(fxp16_t x, uint8_t xfrac);
/*!
    \brief      Round down value
    \details    Rounds x downward, returning the largest integral value that is not greater than x.

    \param      x       Value to round down.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded downward (as a fixed-point value).
*/
#define fxp16_floor(x,xfrac) (x&~((1<<xfrac)-1))
/*!
    \brief      Truncate value
    \details    Rounds x toward zero, returning the nearest integral value that is not larger in magnitude than x.


    \param      x       Value to truncate.
    \param      xfrac   Number of fractional bits for x.
    \return     The nearest integral value that is not larger in magnitude than x (as a fixed-point value).
*/
#define fxp16_trunc(x,xfrac) ((x>0)?fxp16_floor(x,xfrac):fxp16_ceil(x,xfrac))
/*!
    \brief      Round to nearest
    \details    Returns the integral value that is nearest to x, with halfway cases rounded away from zero.

    \param      x       Value to round.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded to the nearest integral (as a fixed-point value).
*/
fxp16_t fxp16_round(fxp16_t x, uint8_t xfrac);
/*!
    \brief      Compute remainder of division
    \details    Returns the floating-point remainder of numer/denom (rounded towards zero):

                fmod = numer - tquot * denom

                Where tquot is the truncated (i.e., rounded towards zero) result of: numer/denom.

                 A similar function, remainder, returns the same but with the quotient rounded to the nearest integer (instead of truncated).

                 In the C Programming Language, the fmod function returns the remainder when x is divided by y.



    \param      x       Value of the quotient numerator.
    \param      xfrac   Number of fractional bits for x.
    \param      y       Value of the quotient denominator.
    \param      yfrac   Number of fractional bits for y.


    \return     The remainder of dividing the arguments. If denom is zero, the function may either
                return zero or cause a domain error (depending on the library implementation).
*/
fxp16_t fxp16_fmod(fxp16_t divident, uint8_t frac1, fxp16_t divisor, uint8_t frac2);
/*!
    \brief      Round to nearest and cast to integer
    \details    Returns the integer value that is nearest in value to x, with halfway cases rounded away from zero.

    \param      x       Value to round.
    \param      xfrac   Number of fractional bits for x.
    \return     The value of x rounded to the nearest integral, casted to a value of type int.
*/
int fxp16_lround(fxp16_t fp, uint8_t frac);

/*! @} */



/* Fixed-point manipulation functions */

/*!
    \brief      Copy sign
    \details    Returns a value with the magnitude of x and the sign of y.
                x and y must not have the same amount of fractional bits.

    \param      x   Value with the magnitude of the resulting value.
    \param      y   Value with the sign of the resulting value.
    \return     The value with a magnitude of x and the sign of y.
*/
fxp16_t fxp16_copysign(fxp16_t x, fxp16_t y);

/*!
    \brief      Next representable value
    \details    Returns the next representable value after x in the direction of y.
                x and y must have the same amount of fractional bits.

    \param      x   Base value.
    \param      y   Value toward which the return value is approximated.
    \return     The next representable value after x in the direction of y.
*/

#define fxp16_nextafter(x,y) ((fxp16_isless(x,y))?(x+1):((fxp16_isgreater(x,y))?(x-1):(y)))




/* Minimum, maximum, difference functions */


/*!
    \brief      Positive difference
    \details    The function returns x-y if x>y, and zero otherwise.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The positive difference between x and y.
*/
#define fxp16_fdim(x,y) ((fxp16_isgreater(x,y))?(fxp16_sub(x,y)):(0))




/*!
    \brief      Maximum value
    \details    Returns the larger of its arguments: either x or y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The maximum numeric value of its arguments.
*/
#define fxp16_fmax(x,y) (fxp16_isgreaterequal(x,y)?(x):(y))


/*!
    \brief      Minimum value
    \details    Returns the smaller of its arguments: either x or y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     The minimum numeric value of its arguments.
*/
#define fxp16_fmin(x,y) (fxp16_islessequal(x,y)?(x):(y))



/* functions to compute common mathematical operations and transformations */
fxp16_t fxp16_fabs (fxp16_t x);
fxp16_t fxp16_abs (fxp16_t x, uint8_t frac);



/*!
    \brief      Multiply-add
    \details    Returns x*y+z.

    \param     x
    \param     xfrac
    \param     y
    \param     yfrac
    \param     z
    \param     zfrac

    \returns   x*y+z with fractional bits of z

*/
fxp16_t fxp16_fma (fxp16_t x, uint8_t xfrac, fxp16_t y, uint8_t yfrac, fxp16_t z, uint8_t zfrac);


/* Comparison macro / functions */

/*!
    \brief      Is greater
    \details    Returns whether x is greater than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is greater than y. false (0) otherwise.
*/
#define fxp16_isgreater(x,y)  ((x)>(y))

/*!
    \brief      Is greater or equal
    \details    Returns whether x is greater than or equal to y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is greater than or equal to y. false (0) otherwise.
*/
#define fxp16_isgreaterequal(x,y)  ((x)>=(y))

/*!
    \brief      Is less
    \details    Returns whether x is less than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than y. false (0) otherwise.
*/
#define fxp16_isless(x,y)  ((x)<(y))


/*!
    \brief      Is less or equal
    \details    Returns whether x is less than or equal to y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than or equal to y. false (0) otherwise.
*/
#define fxp16_islessequal(x,y)  ((x)<=(y))


/*!
    \brief      Is less or greater
    \details    Returns whether x is less than or greater than y.
                x and y must have the same amount of fractional bits.

    \param      x   value to be compared.
    \param      y   value to be compared.
    \return     true (1) if x is less than or greater than y. false (0) otherwise.
*/
#define fxp16_islessgreater(x,y)   (x!=y)



/* Classification macro / functions */


/*!
    \brief      Sign bit
    \details    Returns whether the sign of x is negative.

    \param      x   fixed point numer to get the sign from
    \return     true (1) if negative, false (0) otherwise
*/
#define fxp16_signbit(x) (x<0)

#endif /* _FXP16_H_ */
