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
    \file   fp.c

    \brief  Signed fixed point implementation for small embedded project

    \details
*/

#include "fxp16.h"
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>



/*!
    \brief      Converts a float to a fixed point type
    \details    Converts a float variable to a fixed point variable. Result gets
                saturated if float exceeds fixed point limits.
    \param[in]  var     Float variable to be converted
    \param[in]  frac    Number of fracional bits

    \returns    Fixed point interpretation of provided floating point number
*/
fxp16_t fxp16_flt2fp(float var, uint8_t frac)
{
   /*

   To convert from floating-point to fixed-point, we follow this algorithm:

   Calculate x = floating_input * 2^(fractional_bits)
   Round x to the nearest whole number (e.g. round(x))
   Store the rounded x in an integer container

   */

    var = round(var * (1 << frac));
    fxp16_sat_m(var);
    return (fxp16_t)var;
}

/*!
    \brief      Converts a a fixed point type to float
    \details    Converts a fixed point variable to a float variable.
    \param[in]  var     Fixed point variable to be converted
    \param[in]  frac    Number of fracional bits

    \returns Floating point interpretation of provided fixed point number
*/
float fxp16_fp2flt(fxp16_t var, uint8_t frac)
{
    return ((float)(var)) / (1 << frac);


}


/*!
    \brief      Converts an integer type to fixed point
    \details    Converts an integer variable to a fixed point variable.
    \param[in]  intpart  Integer variable to be converted
    \param[in]  frac     Number of fracional bits

    \returns Fixed point interpretation of provided integer number
*/
fxp16_t fxp16_int2fp(int16_t intpart, uint8_t frac)
{

    int32_t result = intpart;
    result <<= frac;
    fxp16_sat_m(result);
    return(fxp16_t)result;
}


#if FXP16CONF_ARSHIFT_W_ROUNDING

fxp16_t fxp16_arshift(fxp16_t fp, uint8_t shift)
{
   fpxx_arshift_m(fp,shift);
   return fp;
}

fxp32_t fxp32_arshift(fxp32_t var,uint8_t rshift)
{
    fpxx_arshift_m(var,rshift);
    return var;
}

#endif

/*!
    \brief      Left shifts fixed point number
    \details    Left shifts fixed point number

                A Left Arithmetic Shift of one position moves each bit to the left by one.
                The vacant least significant bit (LSB) is filled with zero and the most
                significant bit (MSB) is discarded. It is identical to Left Logical Shift.

    \param      fp       Fixed point number to left shift
    \param      shift     Number of bits to left shift

    \returns Left shifted fixed point number
*/
fxp16_t fxp16_alshift(fxp16_t fp, uint8_t shift)
{
   fxp32_t result = fp;
   result<<=shift;
   fxp16_sat_m(result);
   return(fxp16_t)result;
}



/*!
    \brief      Converts between fixed point number formats (Qx.y)
    \details    Converts between fixed point number formats (Qx.y)
    \param      fp       fixed point number to convert
    \param      fracold     Old number of fracional bits
    \param      fracnew     New number of fracional bits
    \returns    Fixed point number in new fixed point format
*/

fxp16_t fxp16_fp2fp(fxp16_t fp, uint8_t fracold, uint8_t fracnew)
{
    fxp32_t result = fp;
    fpxx_ashift_m(result, fracold - fracnew);
    fxp16_sat_m(result);
    return (fxp16_t) result;
}


fxp16_t fxp16_sat(fxp32_t fxp32)
{
    fxp16_sat_m(fxp32);
    return (fxp16_t) fxp32;
}

/*!
    \brief      Adds two fixed point numbers
    \details    Adds two fixed point numbers. Result gets saturated if it exceeds fixed point limits.

                result = a + b

                Be aware of that both fixed point numbers must be of same type (same number of fractional bits)!
                Otherwise the result may be undefined.

    \param[in]  summand1       First fixed point summand
    \param[in]  summand2       Second fixed point summand


    \returns Sum of first summand and second summand in fixed point format
*/
fxp16_t fxp16_add(fxp16_t summand1, fxp16_t summand2)
{
    int32_t result;
    result = summand1+summand2;
    fxp16_sat_m(result);
    return(fxp16_t)result;
}

/*!
    \brief     Subtracts two fixed point numbers
    \details    Adds two fixed point number. Result gets saturated if it exceeds fixed point limits.

                result = a - b

                Be aware of that both fixed point numbers must be of same type (same number of fractional bits)!
                Otherwise the result may be undefined.

    \param[in]  minuend     Minuend
    \param[in]  subtrahend  Subtrahend


    \returns Difference of minuend and subtrahend in fixed point format
*/

fxp16_t fxp16_sub(fxp16_t minuend, fxp16_t subtrahend)
{
   int32_t result;
   result = minuend-subtrahend;
   fxp16_sat_m(result);
   return(fxp16_t)result;
}





/*!
    \brief      Multiplies two fixed point numbers
    \details    Multiplies two fixed point numbers. The numbers may be of different fixed point format.
                The result is in the fixed point format of the multiplicand (1st fixed point parameter).
                Result gets saturated if it exceeds fixed point limits.

                When performing an integer multiplication the product is 2xWL if both the multiplier and
                multiplicand are WL long. If the integer multiplication is on fixed-point variables, the number of
                integer and fractional bits in the product is the sum of the corresponding multiplier and
                multiplicand Q-points.

                result = a*b


    \param[in]  mult1     multiplicator
    \param[in]  mult2     multiplicant


    \returns Product of multiplicator and multiplicant in fixed point format of multiplicator
*/
fxp16_t fxp16_mult(fxp16_t mult1, uint8_t frac1, fxp16_t mult2, uint8_t frac2)
{
    fxp32_t result = fxp32_arshift((fxp32_t)mult1*(fxp32_t)mult2,frac2);
    fxp16_sat_m(result);
    return (fxp16_t)result;
}


/*!
    \brief      Divides two fixed point numbers
    \details    Divides two fixed point numbers

                result = a/b


    \param      divident     divident
    \param      divisor      divisor


    \returns divident/divisor with fractional bits of divident
*/
fxp16_t fxp16_div(fxp16_t divident, uint8_t frac1, fxp16_t divisor, uint8_t frac2)
{
  fxp32_t result = (divident<<frac2)/divisor;
  fxp16_sat_m(result);
  return (fxp16_t)result;
}



fxp16_t fxp16_ceil(fxp16_t x, uint8_t xfrac)
{

   fxp32_t result = (fxp32_t)x&~((1<<xfrac)-1);

   if(result == x)
   {
      return (fxp16_t)result;
   }

   result+=(1<<(xfrac));
   fxp16_sat_m(result);

   return (fxp16_t)result;

}


fxp16_t fxp16_round(fxp16_t x, uint8_t xfrac)
{
   int32_t result = x;

   if (xfrac == 0 )
   {
      return x;
   }

   if( x < 0 )
   {
      result = -result;
      result += (1<<(xfrac-1));
      result &= ~((1<<xfrac)-1);
      result = -result;
   }
   else
   {
      result += (1<<(xfrac-1));
      result &= ~((1<<xfrac)-1);
   }


   fxp16_sat_m(result);
   return (fxp16_t)result;
}


fxp16_t fxp16_fmod(fxp16_t x, uint8_t xfrac, fxp16_t y, uint8_t yfrac)
{
   if ( y == 0 )
   {
      return 0;
   }

   int32_t result = (x<<yfrac)/y;
   fxp16_intcast_m(result,xfrac);

   result*=y;
   fpxx_arshift_m(result,yfrac);

   result = x - result;
   fxp16_sat_m(result);
   return (fxp16_t)result;
}



int fxp16_lround(fxp16_t x, uint8_t xfrac)
{
   int32_t result = x;

   if (xfrac == 0 )
   {
      return x;
   }

   if( x < 0 )
   {
      result = -result;
      result += (1<<(xfrac-1));
      result>>=xfrac;
      result = -result;
   }
   else
   {
      result += (1<<(xfrac-1));
      result>>=xfrac;
   }


   fxp16_sat_m(result);
   return (fxp16_t)result;

}




/*
 * sqrt_fixed16:
 *  - x:        16-Bit Fixed-Point-Wert im Qn-Format
 *  - frac_bits: Anzahl Nachkommabits n (z. B. 15 für Q1.15, 8 für Q8.8)
 *  -> Rückgabe: sqrt(x) im selben Qn-Format
 *
 * Implementiert die ganzzahlige "restoring" Wurzel direkt (ohne Helper).
 * Nutzt 32-Bit-Zwischenwerte und ist deterministisch (~16 Iterationen).
 */
fxp16_t fxp16_sqrt(fxp16_t  x, uint8_t frac_bits)
{
    if (frac_bits > 15) frac_bits = 15;     // Safety für 32-Bit-Zwischenwerte

    // Wir brauchen y_fixed = sqrt(x * 2^n)
    uint32_t a = (uint32_t)x << frac_bits;

    // isqrt32(a): restoring square root, ohne Mul/Div
    uint32_t r = 0;
    uint32_t bit = 1u << 30;                // höchstes gerades Bit (<= 2^30)
    while (bit > a) bit >>= 2;

    while (bit != 0) {
        if (a >= r + bit) {
            a -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }

    if (r > 0xFFFFu) r = 0xFFFFu;           // Sättigung in 16 Bit
    return (uint16_t)r;
}






fxp16_t fxp16_cbrt(fxp16_t a, uint8_t afrac)
{

    /*
        Newton's method
        x[n+1] = (a/x[n]^2 + 2*x[n])/3
    */


    int32_t x = 1<<afrac;


    if ( a == 0 )
    {
        return 0;
    }

    for(int tmp = 0; tmp < FXP16_CBRT_ITERATIONS; tmp++)
    {
        int32_t xx = x*x;
        fpxx_arshift_m(xx,afrac);
        x = (x<<1)+(a<<afrac)/xx;
        x = x/3;
    }

    fxp16_sat_m(x);
    return (fxp16_t)x;
}

// CORDIC-Skalierungsfaktor K ≈ 0.607252935 in Q1.15 (unverändert)
#define CORDIC_K_Q15   ((int16_t)0x4DBA)  // round(0.607252935 * 2^15) = 19898

// Nützliche Konstanten im "π-normalisierten" Q1.15
#define FXP16_Q15_NORM_ONE_PI            FXP16_Q15_ALMOST_ONE      // +π  (≈ +1.0)
#define FXP16_Q15_NORM_MINUS_PI          FXP16_Q15_MINUS_ONE       // -π  (exakt -1.0)
#define FXP16_Q15_NORM_HALF_PI           FXP16_Q15_ONE_HALF        // +π/2 (= +0.5)
#define FXP16_Q15_NORM_MINUS_HALF_PI     (-FXP16_Q15_ONE_HALF)     // -π/2 (= -0.5)

// atan(2^-i) Tabelle in Q1.15 *relativ zu π*, also: round(atan(2^-i)/π * 2^15)
// (i = 0..13); danach wird's 0 in dieser Q-Skalierung.
static const int16_t atan_table_q15_pi[14] = {
    0x2000, // atan(2^-0)/π
    0x12E4, // atan(2^-1)/π
    0x09FB, // atan(2^-2)/π
    0x0511, // atan(2^-3)/π
    0x028B, // atan(2^-4)/π
    0x0146, // atan(2^-5)/π
    0x00A3, // atan(2^-6)/π
    0x0051, // atan(2^-7)/π
    0x0029, // atan(2^-8)/π
    0x0014, // atan(2^-9)/π
    0x000A, // atan(2^-10)/π
    0x0005, // atan(2^-11)/π
    0x0003, // atan(2^-12)/π
    0x0001  // atan(2^-13)/π
};

// Eingabe:  angle_q15 in Q1.15, normiert auf π: [-1.000*π, +(1.000-LSB)*π]  -> z ∈ [-1.0, +0.999969...]
// Ausgabe: *sin_q15, *cos_q15 in Q1.15.
void cordic_sin_cos_q15_pi(fxp16_t angle_q15, fxp16_t* sin_q15, fxp16_t* cos_q15)
{
    // Hinweis: Die Eingabe ist bereits auf [-π, π) begrenzt (π-normalisierte Q1.15).
    // Optionales "Sicherheits-Clamping" könnte man hinzufügen, ist hier nicht nötig.



     // 1) In den Bereich [-π/2, π/2] spiegeln + Vorzeichen für Quadranten merken
    fxp32_t z = angle_q15;   // z ist jetzt "π-normalisierter" Winkel in Q1.15
    int16_t sign_s = 1, sign_c = 1;

    if (z > FXP16_Q15_NORM_HALF_PI)
    {
        // θ in (π/2, π): z' = π - θ  -> sin positiv, cos negativ
        // In normierter Darstellung: z' = 1.0 - z  (mit 1.0 ≈ 0x7FFF)
        z = FXP16_Q15_NORM_ONE_PI - z;
        sign_c = -1;
    }
    else if (z < FXP16_Q15_NORM_MINUS_HALF_PI)
    {
        // z' = -π - z  (in π-normalisiertem Q1.15: -1.0 - z)
        z = FXP16_Q15_NORM_MINUS_PI - z;
        sign_c = -1;   // bleibt -1
    }
    else
    {

    }

    // 2) CORDIC-Startvektor in Q1.15 (K vorab multipliziert)
    int16_t x = CORDIC_K_Q15;
    int16_t y = 0;

    // 3) Iterationen (14 Schritte für diese Winkelauflösung genügen)
    for (int i = 0; i < 14; ++i)
    {
        int16_t x_shift = (int16_t)(x >> i);
        int16_t y_shift = (int16_t)(y >> i);
        int16_t a = atan_table_q15_pi[i];  // Winkelinkrement in "π-normalisiertem" Q1.15

        if (z >= 0)
        {
            fxp32_t xn = (int32_t)x - y_shift;
            fxp32_t yn = (int32_t)y + x_shift;
            fxp16_sat_m(xn);
            fxp16_sat_m(yn);
            x = (int16_t)xn;
            y = (int16_t)yn;
            z = (z - a);
        }
        else
        {
            fxp32_t xn = (int32_t)x + y_shift;
            fxp32_t yn = (int32_t)y - x_shift;
            fxp16_sat_m(xn);
            fxp16_sat_m(yn);
            x = (int16_t)xn;
            y = (int16_t)yn;
            z = (z + a);
        }
    }

    // 4) Quadranten-Vorzeichen anwenden
    *cos_q15 = (sign_c > 0) ? x : (int16_t)(-x);
    *sin_q15 = (sign_s > 0) ? y : (int16_t)(-y);
}

fxp16_t fxp16_sin(fxp16_t rad)
{
    fxp16_t sin_q15, cos_q15;
    cordic_sin_cos_q15_pi(rad, &sin_q15, &cos_q15);
    return sin_q15;
}

fxp16_t fxp16_cos(fxp16_t rad)
{
    fxp16_t sin_q15,cos_q15;
    cordic_sin_cos_q15_pi(rad , &sin_q15, &cos_q15);
    return cos_q15;
}


fxp16_t fxp16_tan(fxp16_t fp, uint8_t frac)
{
    fxp32_t x;
    fxp16_t sin_q15, cos_q15;

    switch(fp)
    {
        case FXP16_Q15_NORM_MINUS_HALF_PI:
            errno = EDOM;
            return INT16_MAX;
        case FXP16_Q15_NORM_HALF_PI:
            errno = EDOM;
            return INT16_MIN;
    }

    cordic_sin_cos_q15_pi(fp, &sin_q15, &cos_q15);

    x = (sin_q15<<FXP16_Q15)/cos_q15;

    x = fxp32_arshift(x,FXP16_Q15-frac);
    fxp16_sat_m(x);
    return (fxp16_t)x;
}




fxp16_t fxp16_atan2(fxp16_t y_in, fxp16_t x_in)
{
    // Sonderfälle wie bei double atan2
    if (y_in == 0)
    {
        if (x_in > 0) return (fxp16_t)0;
        if (x_in < 0) return (fxp16_t)FXP16_Q15_NORM_ONE_PI;      // +π
        return (fxp16_t)0;                             // atan2(0,0) -> 0 (Konvention)
    }

    if (x_in == 0)
    {
        fxp16_t half_pi = (fxp16_t)(FXP16_Q15_NORM_ONE_PI >> 1);   // ±π/2
        return (y_in > 0) ? half_pi : (fxp16_t)(-half_pi);
    }

    // Merke Original-Vorzeichen für Quadrantenkorrektur
    const int y_orig_nonneg = (y_in >= 0);
    const int x_orig_neg    = (x_in < 0);

    // In rechte Halbebene spiegeln, damit Vectoring stabil konvergiert
    // (atan(y/x) wird dadurch nicht geändert; nur Quadranten-Offset verschiebt sich)
    int32_t Xi = x_in;
    int32_t Yi = y_in;

    if (x_orig_neg)
    {
        Xi = -Xi; Yi = -Yi;
    }  // jetzt Xi >= 0

    int32_t Z = 0;

    // CORDIC-Vectoring: z += sign(y) * atan(2^-i)
    for (int i = 0; i < 14; ++i) {
        int32_t x_shift = (Xi >> i);
        int32_t y_shift = (Yi >> i);
        int32_t a = (int32_t)atan_table_q15_pi[i]; // Q1.15, in Einheiten von π

        if (Yi > 0) {
            // rotate clockwise (vectoring)
            int32_t Xn = Xi + y_shift;
            int32_t Yn = Yi - x_shift;
            Xi = Xn; Yi = Yn; Z += a;
        } else {
            // rotate counter-clockwise (vectoring)
            int32_t Xn = Xi - y_shift;
            int32_t Yn = Yi + x_shift;
            Xi = Xn; Yi = Yn; Z -= a;
        }
        if (Yi == 0) break; // frühzeitiger Stopp, wenn genau getroffen
    }

    // Quadrantenkorrektur anhand der ORIGINALEN Vorzeichen
    if (x_orig_neg)
    {
        Z += y_orig_nonneg ? (int32_t)FXP16_Q15_NORM_ONE_PI      // +π
                           : (int32_t)(-FXP16_Q15_NORM_ONE_PI);  // −π
    }

    // End-Sättigung und Rückgabe
    fxp16_sat_m(Z);
    return (fxp16_t)(int16_t)Z;
}


fxp16_t fxp16_atan(fxp16_t y, uint8_t frac)
{
    fxp32_t x = FXP32_Q15_ONE;
    fxp32_t Y = y;

    fpxx_ashift_m(Y, frac-FXP16_Q15);

    while(Y > FXP32_Q15_ONE || Y < FXP16_Q15_MINUS_ONE)
    {
       Y = fxp32_arshift(Y,1);
       x = fxp32_arshift(x,1);
    }

    fxp16_sat_m(Y);
    fxp16_sat_m(x);

    return fxp16_atan2((fxp16_t)Y, (fxp16_t)x);
}



fxp16_t fxp16_asin(fxp16_t x)
{

    int32_t xi = x;

    // t = 1 − x^2  in Q1.15
    // Achtung: Für die "1" verwenden wir intermediär 1<<15 (=32768), damit bei |x|=1 exakt 0 herauskommt.
    uint32_t prod      = (uint32_t)(xi * xi);   // bis 1,073,741,824, passt in 32 Bit
    uint32_t prod_q15  = prod >> 15;            // zurück in Q1.15
    int32_t  t_ext     = (int32_t)(1u << 15) - (int32_t)prod_q15; // kann negativ werden
    if (t_ext < 0)      t_ext = 0;
    if (t_ext > 0x7FFF) t_ext = 0x7FFF;         // in darstellbaren Q1.15-Bereich klemmen
    fxp16_t   t_q15      = (fxp16_t)t_ext;

    // c = sqrt(1 - x^2) in Q1.15
    fxp16_t c = fxp16_sqrt(t_q15, 15);

    // asin(x) = atan2(x, c);  Ergebnis bereits π-normalisiert in Q1.15
    return fxp16_atan2((fxp16_t)xi, c);
}

fxp16_t fxp16_acos(fxp16_t x)
{

    int32_t xi = (int32_t)x;

    uint32_t prod      = (uint32_t)(xi * xi);   // bis 1,073,741,824, passt in 32 Bit
    uint32_t prod_q15  = prod >> 15;            // zurück in Q1.15
    int32_t  t_ext     = (int32_t)(1u << 15) - (int32_t)prod_q15; // kann negativ werden
    if (t_ext < 0)      t_ext = 0;
    if (t_ext > 0x7FFF) t_ext = 0x7FFF;
    fxp16_t   t_q15      = (fxp16_t)t_ext;

    // c = sqrt(1 - x^2) in Q1.15
    fxp16_t c = fxp16_sqrt(t_q15, 15);

    // acos(x) = atan2(c, x);  Ergebnis bereits π-normalisiert in Q1.15
    // Wertebereich: [0 .. 1) (entspricht [0 .. π))
    return fxp16_atan2(c, (fxp16_t)xi);
}

#define FXP32_Q15         (15)
#define FXP32_SAT_MAX     (INT32_MAX)
#define FXP32_SAT_MIN     (INT32_MIN)

/*!
    \brief      Saturating arithmetic left shift
    \details    Shifts the 32-bit value \p v left by \p n bits and saturates the result
                to the range [FXP32_SAT_MIN, FXP32_SAT_MAX].
                If \p n <= 0, \p v is returned unchanged.
                If \p n >= 31, the result is immediately saturated depending on the sign of \p v.

    \param[in]  v    32-bit input value (e.g. fixed-point number).
    \param[in]  n    Number of bits to shift (n ≥ 0).

    \returns    v << n with saturation, equivalent to saturating v · 2ⁿ.
*/
static inline fxp32_t fxp32_sat_shl(fxp32_t v, int n) {
    if (n <= 0) return v;
    if (n >= 31) return (v >= 0) ? FXP32_SAT_MAX : FXP32_SAT_MIN;
    int64_t w = (int64_t)v << n;
    if (w > FXP32_SAT_MAX) return FXP32_SAT_MAX;
    if (w < FXP32_SAT_MIN) return FXP32_SAT_MIN;
    return (fxp32_t)w;
}

/*!
    \brief      Arithmetic right shift with rounding toward +∞ for non-negative values
    \details    Shifts the 32-bit integer \p v right by \p n bits arithmetically.
                For \p v >= 0, adds 2^(n-1) before shifting to round toward +∞.
                For \p v < 0, performs a plain arithmetic shift (no +0.5 rounding).
                If \p n <= 0, returns \p v unchanged. If \p n >= 31, the result is
                0 for non-negative \p v, or -1 for negative \p v (all bits shifted out).
                Useful for fixed-point scaling equivalent to division by 2^n.

    \param[in]  v    32-bit signed input value (e.g., fixed-point).
    \param[in]  n    Number of bits to shift right (n ≥ 0).

    \returns    \p v >> n with arithmetic semantics; for \p v >= 0 the result is
                rounded toward +∞, otherwise no rounding is applied.
*/
static inline fxp32_t fxp32_shr_r(fxp32_t v, int n) {
    if (n <= 0) return v;
    if (n >= 31) return (v >= 0) ? 0 : -1; /* alles weg */
    if (v >= 0) return (v + (1 << (n - 1))) >> n;
    else        return (v >> n); /* negatives: arithmetisch, kein +0.5 Rundung */
}

/*!
    \brief      Q15 multiply with 64-bit intermediate and rounding
    \details    Multiplies two signed Q15 fixed-point values \p a and \p b using a 64-bit
                intermediate (Q30), adds 2^(Q15-1) for rounding, then shifts right by Q15.
                The final result is saturated to [FXP32_SAT_MIN, FXP32_SAT_MAX].

    \param[in]  a    32-bit signed Q15 operand.
    \param[in]  b    32-bit signed Q15 operand.

    \returns    Q15 product of \p a and \p b, rounded (via bias + shift) and saturated.
*/
static inline fxp32_t fxp32_mul_q15(fxp32_t a, fxp32_t b) {
    int64_t t = (int64_t)a * (int64_t)b;          // Q30
    t += (int64_t)1 << (FXP32_Q15 - 1);                 // rundung
    t >>= FXP32_Q15;
    if (t > FXP32_SAT_MAX) return FXP32_SAT_MAX;
    if (t < FXP32_SAT_MIN) return FXP32_SAT_MIN;
    return (fxp32_t)t;
}

/*!
    \brief      Q15 scaling by power of two
    \details    Scales \p v by 2^n in Q15 format:
                for \p n >= 0 uses saturating left shift (fxp32_sat_shl),
                for \p n < 0 uses arithmetic right shift with rounding (fxp32_shr_r).

    \param[in]  v    32-bit signed Q15 value to scale.
    \param[in]  n    Power-of-two exponent; n >= 0 ⇒ left shift, n < 0 ⇒ right shift.

    \returns    \p v · 2^n in Q15, with saturation for left shifts and rounding on right shifts.
*/
static inline fxp32_t fxp32_scale_pow2_q15(fxp32_t v, int n) {
    if (n >= 0) return fxp32_sat_shl(v, n);
    else        return fxp32_shr_r(v, -n);
}


/*!
    \brief      Saturating 32-bit addition without 64-bit intermediate
    \details    Adds \p a and \p b using 32-bit arithmetic and clamps the result to
                [FXP32_SAT_MIN, FXP32_SAT_MAX] on overflow or underflow. No int64 is used.

    \param[in]  a    32-bit signed addend.
    \param[in]  b    32-bit signed addend.

    \returns    a + b if representable; otherwise FXP32_SAT_MAX or FXP32_SAT_MIN.
*/
static inline fxp32_t fxp32_add_sat32(fxp32_t a, fxp32_t b) {
    if (b > 0 && a > FXP32_SAT_MAX - b) return FXP32_SAT_MAX;
    if (b < 0 && a < FXP32_SAT_MIN - b) return FXP32_SAT_MIN;
    return a + b;
}

/*!
    \brief      Sign-aware saturation to Q15 limits for sinh/cosh
    \details    Writes saturated approximations for hyperbolic functions:
                sets \p *out_cosh to FXP32_SAT_MAX (since cosh(x) ≥ 1 and grows unbounded),
                and sets \p *out_sinh to FXP32_SAT_MAX if \p x ≥ 0, else FXP32_SAT_MIN.
                Intended for overflow handling in Q15 fixed-point paths.

    \param[in]  x          32-bit fixed-point input (e.g., Q15) determining sinh sign.
    \param[out] out_cosh   Destination for saturated cosh(x); must be non-null.
    \param[out] out_sinh   Destination for saturated sinh(x); must be non-null.

    \returns    Nothing. Outputs are assigned unconditionally to the Q15 saturation bounds.
*/
static inline void fxp32_saturate_sinh_cosh_by_sign(fxp32_t x, fxp32_t *out_cosh, fxp32_t *out_sinh) {
    *out_cosh = FXP32_SAT_MAX;                         /* cosh(x) >= 1, wächst -> +MAX */
    *out_sinh = (x >= 0) ? FXP32_SAT_MAX : FXP32_SAT_MIN;    /* Vorzeichen von sinh(x) */
}


#define FXP32_Q15_M_LN2_Q15  FXP16_Q15_M_LN2  /* round(ln(2)*2^15)  ≈ 0.69314718 * 32768 */
#define FXP32_Q15_M_INV_LN2  47274;          /* round(1/ln(2)*2^15) ≈ 1.44269504 * 32768 */

/* Hyperbolic CORDIC: elementare Winkel artanh(2^-i), i=1..16, Q15.
   (Bei i >= 17 wäre die Q15-Darstellung 0.)
*/
static const fxp32_t fxp32_q15_atanh_tab[17] = {
/* i:  0      1      2      3      4      5      6      7      8 */
    0,  18000,  8369,  4118,  2051,  1024,   512,   256,   128,
/* i:  9     10     11     12     13     14     15     16 */
     64,    32,    16,     8,     4,     2,     1,     1
};


/*!
    \brief      Repeat-iteration selector for hyperbolic CORDIC (radix-2)
    \details    Returns non-zero if iteration \p i is a required repeat step in
                hyperbolic CORDIC to ensure convergence. For radix-2, repeats occur
                at i = 4 and i = 13.

    \param[in]  i    Iteration index (0-based).

    \returns    Non-zero if \p i ∈ {4, 13}; otherwise 0.
*/
static inline int is_repeat_i(int i)
{
    return (i == 4) || (i == 13);
}


/* NEU: K  (für i=1..16 mit Repeats bei i=4 und i=13) */
#define  FXP32_Q15_K_HYP 39567 // ~ 1.207497 * 2^15

/*!
    \brief      Range reduction by ln(2): x ≈ n·ln(2) + r
    \details    Computes \p n = round(x / ln(2)) and the residual \p r = x − n·ln(2),
                so that |r| ≤ ln(2)/2. Uses Q15 fixed-point constants:
                1/ln(2) (Q15) for forming a Q30 intermediate and ln(2) (Q15) for reconstruction.

    \param[in]  x       Input value (e.g., Q15 fixed-point).
    \param[out] n_out   Pointer to receive n = round(x/ln(2)); must be non-null.
    \param[out] r_out   Pointer to receive r = x − n·ln(2) (same format as x); non-null.

    \returns    Nothing. Writes \p *n_out and \p *r_out.
*/
static inline void fxp32_range_reduce_ln2(fxp32_t x, int *n_out, fxp32_t *r_out) {
    /* x * (1/ln2) liegt in Q30 */
    int64_t t = (int64_t)x * (int64_t)FXP32_Q15_M_INV_LN2; /* Q15*Q15 -> Q30 */
    int n;
    if (t >= 0) n = (int)((t + ((int64_t)1 << (2*FXP32_Q15 - 1))) >> (2*FXP32_Q15));
    else        n = -(int)(((-t) + ((int64_t)1 << (2*FXP32_Q15 - 1))) >> (2*FXP32_Q15));
    fxp32_t r = x - (fxp32_t)((int64_t)n * (int64_t)FXP32_Q15_M_LN2_Q15);
    *n_out = n;
    *r_out = r;
}

/*!
    \brief      Hyperbolic CORDIC (rotation) for small r in Q15
    \details    Computes (\p cosh(r), \p sinh(r)) using radix-2 hyperbolic CORDIC in rotation mode.
                Starts at x = K_HYP (gain already applied), y = 0, z = r and iterates i = 1..16.
                Repeat steps are performed where required by hyperbolic CORDIC (see is_repeat_i).
                Updates use arithmetic shifts and a Q15 atanh look-up table:
                x' = x ± (y >> i), y' = y ± (x >> i), z' = z ∓ atanh(2^-i).
                The outputs are Q15 fixed-point values.

    \param[in]  r       Angle in Q15 (assumed small; range-reduced elsewhere).
    \param[out] c_out   Destination for cosh(r) in Q15; must be non-null.
    \param[out] s_out   Destination for sinh(r) in Q15; must be non-null.

    \returns    Nothing. Writes \p *c_out = cosh(r) and \p *s_out = sinh(r).
*/
static inline void fxp32_cordic_cosh_sinh_small_q15(fxp32_t r, fxp32_t *c_out, fxp32_t *s_out) {
    fxp32_t x = FXP32_Q15_K_HYP;
    fxp32_t y = 0;
    fxp32_t z = r;

    for (int i = 1; i <= 16; ++i) {
        int reps = is_repeat_i(i) ? 2 : 1;
        for (int k = 0; k < reps; ++k) {
            int d = (z >= 0) ? +1 : -1;
            /* x' = x + d * (y >> i), y' = y + d * (x >> i), z' = z - d * atanh(2^-i) */
            fxp32_t x_shift = (x >> i);
            fxp32_t y_shift = (y >> i);
            fxp32_t x_new   = x + (d > 0 ? y_shift : -y_shift);
            fxp32_t y_new   = y + (d > 0 ? x_shift : -x_shift);
            fxp32_t z_new   = z - (d > 0 ? fxp32_q15_atanh_tab[i] : -fxp32_q15_atanh_tab[i]);
            x = x_new; y = y_new; z = z_new;
        }
    }
    /* Gain-Korrektur bereits ganz oben!*/
    //x = mul_q15(x, K_INV_Q15);
    //y = mul_q15(y, K_INV_Q15);
    *c_out = x; /* cosh(r) */
    *s_out = y; /* sinh(r) */
}

/*!
    \brief      cosh/sinh via hyperbolic CORDIC with ln(2) range reduction (Q15)
    \details    Computes (\p cosh(x), \p sinh(x)) in Q15 using:
                1) Range reduction x ≈ n·ln(2) + r with n = round(x/ln(2)), |r| ≤ ln(2)/2.
                2) Small-angle hyperbolic CORDIC to get (cosh(r), sinh(r)) = (cr, sr).
                3) Exact recomposition using A = 2^n and B = 2^{-n}:
                   cosh(x) = cr·(A+B)/2 + sr·(A−B)/2,
                   sinh(x) = sr·(A+B)/2 + cr·(A−B)/2.
                Early saturation is applied if |n| ≥ 16. All shifts/mults use Q15 helpers
                with rounding and saturating adds to prevent wraparound.

    \param[in]  x          Input in Q15.
    \param[out] out_cosh   Destination for cosh(x) in Q15; must be non-null.
    \param[out] out_sinh   Destination for sinh(x) in Q15; must be non-null.

    \returns    Nothing. Writes \p *out_cosh and \p *out_sinh (Q15, saturated).
*/
static void fxp32_cordic_cosh_sinh_q15(fxp32_t x, fxp32_t *out_cosh, fxp32_t *out_sinh) {
    int n = 0;
    fxp32_t r = 0;
    fxp32_range_reduce_ln2(x, &n, &r);

    /* --- NEU: Frühe Sättigung, bevor 2^±n berechnet wird --- */
    if (n >= 16 || n <= -16) {
        fxp32_saturate_sinh_cosh_by_sign(x, out_cosh, out_sinh);
        return;
    }

    fxp32_t cr, sr;                    /* Q15 */
    fxp32_cordic_cosh_sinh_small_q15(r, &cr, &sr);

    /* A = 2^n, B = 2^-n (ohne Overflow dank |n| <= 15) */
    fxp32_t A = fxp32_scale_pow2_q15(FXP32_Q15_ONE, n);     /* <= 1 << 30 */
    fxp32_t B = fxp32_scale_pow2_q15(FXP32_Q15_ONE, -n);

    fxp32_t ApB_2 = fxp32_shr_r(fxp32_add_sat32(A, B), 1);
    fxp32_t AmB_2 = fxp32_shr_r(fxp32_add_sat32(A, -B), 1);

    /* Produkte weiterhin mit deiner vorhandenen mul_q15() (nutzt intern 64-bit) */
    fxp32_t t1 = fxp32_mul_q15(cr, ApB_2);
    fxp32_t t2 = fxp32_mul_q15(sr, AmB_2);
    fxp32_t t3 = fxp32_mul_q15(sr, ApB_2);
    fxp32_t t4 = fxp32_mul_q15(cr, AmB_2);

    /* Summen nur noch saturierend addieren (kein Wraparound) */
    fxp32_t cosh_x = fxp32_add_sat32(t1, t2);
    fxp32_t sinh_x = fxp32_add_sat32(t3, t4);

    *out_cosh = cosh_x;
    *out_sinh = sinh_x;
}

/*!
    \brief      Q15 division with rounding and saturation to (-1, 1)
    \details    Computes (num / den) in Q15. Uses a 64-bit intermediate:
                (num << Q15) / den, with sign-aware ±0.5 bias for rounding to nearest.
                den == 0 returns the maximum magnitude less than 1 with the sign of num.
                Result is saturated to (-1, 1) in Q15 (i.e., ±(1 − 1/2^15)).

    \param[in]  num   Q15 numerator (signed 32-bit).
    \param[in]  den   Q15 denominator (signed 32-bit).

    \returns    Rounded Q15 quotient in (-1, 1), saturated on overflow or den == 0.
*/
static  inline fxp32_t fxp32_div_q15(fxp32_t num, fxp32_t den) {
    if (den == 0) return (num >= 0) ? (FXP32_Q15_ONE - 1) : -(FXP32_Q15_ONE - 1);

    /* Runden zum nächsten: Vorzeichen von Zähler/Nenner beachten */
    int64_t n = (int64_t)num << FXP32_Q15;      // Q15-Nenner-Ziel
    if (( (num ^ den) & 0x80000000 ) == 0) {
        // gleiches Vorzeichen -> +0.5 zum Runden
        n += (den >= 0 ? (den >> 1) : -((-(int64_t)den) >> 1));
    } else {
        // unterschiedliches Vorzeichen -> -0.5 zum Runden
        n -= (den >= 0 ? (den >> 1) : -((-(int64_t)den) >> 1));
    }

    int64_t q = n / den;

    /* Begrenzen in (-1,1) auf Q15: tanh erreicht nie exakt ±1 */
    if (q >= (int64_t)FXP32_Q15_ONE)     q = FXP32_Q15_ONE - 1;
    if (q <= -(int64_t)FXP32_Q15_ONE)    q = -(FXP32_Q15_ONE - 1);
    return (fxp32_t)q;
}

#define TANH_EARLY_SAT_Q15  ( (fxp32_t)(12 * FXP32_Q15_ONE) )  /* ~|x|>=12 -> ±1 */

/*!
    \brief      Q15 tanh via hyperbolic CORDIC with early saturation
    \details    Computes \p tanh(x) in Q15. For |x| ≥ TANH_EARLY_SAT_Q15, returns
                ±(1 − 2^-15). Otherwise computes (\p cosh(x), \p sinh(x)) using
                fxp32_cordic_cosh_sinh_q15, then returns \p sinh(x)/\p cosh(x) via
                fxp32_div_q15. If \p sinh(x) == 0, returns 0.

    \param[in]  x    Input in Q15.

    \returns    \p tanh(x) in Q15, saturated to (-1, 1).
*/
static fxp32_t fxp32_cordic_tanh_q15(fxp32_t x) {
    if (x >= TANH_EARLY_SAT_Q15)  return  FXP32_Q15_ONE - 1;
    if (x <= -TANH_EARLY_SAT_Q15) return -(FXP32_Q15_ONE - 1);

    fxp32_t s, c;
    fxp32_cordic_cosh_sinh_q15(x, &c, &s);
    if (s == 0) return 0;
    return fxp32_div_q15(s, c);
}


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
fxp16_t fxp16_sinh(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    fxp32_t fxp32_x = x;
    fxp32_t cosh, sinh;
    fpxx_ashift_m(fxp32_x, x_frac  - FXP16_Q15);
    fxp32_cordic_cosh_sinh_q15(fxp32_x, &cosh, &sinh);
    fpxx_ashift_m(sinh, FXP16_Q15 - y_frac);
    fxp16_sat_m(sinh);
    return (fxp16_t)sinh;
}


fxp16_t fxp16_cosh(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    fxp32_t fxp32_x = x;
    fxp32_t cosh, sinh;
    fpxx_ashift_m(fxp32_x, x_frac  - FXP16_Q15);
    fxp32_cordic_cosh_sinh_q15(fxp32_x, &cosh, &sinh);
    fpxx_ashift_m(cosh, FXP16_Q15 - y_frac);
    fxp16_sat_m(cosh);
    return (fxp16_t)cosh;
}


fxp16_t fxp16_tanh(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    fxp32_t fxp32_x = x;
    fxp32_t tanh;
    fpxx_ashift_m(fxp32_x, x_frac  - FXP16_Q15);
    tanh = fxp32_cordic_tanh_q15(fxp32_x);
    fpxx_ashift_m(tanh, FXP16_Q15 - y_frac);
    fxp16_sat_m(tanh);
    return (fxp16_t)tanh;
}




/*!
    \brief      fxp16 exp mit Formatkonvertierung, CORDIC (über cosh/sinh) und Sättigung
    \details    Berechnet e^x für ein fxp16 \p x mit \p x_frac Nachkommabits.
                Intern: Promotion nach 32 Bit, Reskalierung nach Q15, Auswertung via
                fxp32_cordic_cosh_sinh_q15, Zusammensetzen als cosh(x)+sinh(x) mit
                saturierender Addition, Rückskalierung ins Ziel-Q-Format und Sättigung.

    \param[in]  y_frac   Nachkommabits des Ergebnisformats (fxp16 Qy_frac).
    \param[in]  x        fxp16-Eingangswert.
    \param[in]  x_frac   Nachkommabits des Eingabeformats (fxp16 Qx_frac).

    \returns    e^x als fxp16 in Qy_frac, an die fxp16-Grenzen saturiert.
*/
fxp16_t fxp16_exp(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    fxp32_t fxp32_x = x;          /* Promotion nach 32 Bit */
    fxp32_t c, s;                  /* cosh und sinh in Q15 */
    fxp32_t e;                     /* e^x in Q15 */

    /* Eingabe von Qx_frac nach Q15 bringen */
    fpxx_ashift_m(fxp32_x, x_frac - FXP16_Q15);

    /* cosh/sinh in Q15 berechnen (CORDIC mit Range-Reduction & Sättigungen) */
    fxp32_cordic_cosh_sinh_q15(fxp32_x, &c, &s);

    /* e^x = cosh(x) + sinh(x), saturierende 32-bit Addition */
    e = fxp32_add_sat32(c, s);

    /* zurück von Q15 nach Qy_frac skalieren und ins fxp16-Intervall saturieren */
    fpxx_ashift_m(e, FXP16_Q15 - y_frac);
    fxp16_sat_m(e);

    return (fxp16_t)e;
}


/* Fallback-MSB-Suche für 32-bit positive Werte */
static inline int msb_u32(uint32_t v)
{
    int p = -1;
    while (v) { v >>= 1; ++p; }
    return p; /* -1 falls v==0 */
}

/* Erwartet/Erzeugt Q15 (FXP32_Q15): log2(x) in Q15, Domain x>0 */
static fxp32_t fxp32_log2_q15(fxp32_t x)
{
    if (x <= 0) {
        errno = EDOM;
        return INT32_MIN;
    }

    /* 1) Normieren: x = m * 2^p, mit m in [1,2) (alles in Q15-Repr.) */
    uint32_t ux = (uint32_t)x;
    int p = msb_u32(ux);
    fxp32_t m_q15;
    {
        int sh = 15 - p;
        if (sh >= 0)
            m_q15 = (fxp32_t)(ux << sh);
        else
            m_q15 = fxp32_shr_r((fxp32_t)ux, -sh);
    }

    /* 2) Ganzzahlteil in Q15: k = (p - 15) */
    fxp32_t acc_q15 = ((fxp32_t)(p - 15)) << 15;

    /* 3) Bitweise Bestimmung des Bruchteils via wiederholtem Quadrieren */
    const fxp32_t two_q15 = (fxp32_t)2 << 15;  /* 2.0 in Q15 */
    for (int i = 1; i <= 15; ++i) {
        /* m = m^2  (Q15) */
        fxp32_t m2_q15 = fxp32_mul_q15(m_q15, m_q15);  // ! EXPENSIVE!

        if (m2_q15 >= two_q15) {
            /* m >= 2 -> m /= 2 und setze i-tes Bit im Ergebnis */
            m_q15 = m2_q15 >> 1;
            acc_q15 += (fxp32_t)1 << (15 - i);
        } else {
            m_q15 = m2_q15;
        }
    }

    return acc_q15;
}



fxp32_t fxp32_logN_q15(fxp32_t x, fxp32_t logscale)
{

    /* Domäne prüfen */
    if (x <= 0)
    {
        errno = EDOM;
        return INT32_MIN;
    }

    /* 2) Q15 -> Q15: log2(x) */
    x = fxp32_log2_q15(x);

    /* Fehlerfall aus innerer Routine übernehmen (optional) */
    if (x == INT32_MIN && errno == EDOM)
    {
        return INT32_MIN;
    }


    /* 3) ln(x) = log2(x) * ln(2).  Q15 * Q15 -> Q15 */
    return fxp32_mul_q15(x, logscale);
}

static fxp16_t fxp16_logN(uint8_t y_frac, fxp16_t x, uint8_t x_frac, fxp32_t logscale)
{
    if (x <= 0)
    {
        errno = EDOM;
        return INT16_MIN;
    }

    fxp32_t tmp = x;

    fpxx_ashift_m(tmp,x_frac-FXP32_Q15);

    tmp = fxp32_logN_q15(tmp, logscale);

    fpxx_ashift_m(tmp,FXP32_Q15-y_frac);

    fxp16_sat_m(tmp);
    return (fxp16_t)tmp;
}


/*
 * fxp16_log2:
 *   y_frac:  Ziel-Q-Format (Nachkommabits)
 *   x:       Eingabe in Q(x_frac)
 *   x_frac:  Nachkommabits der Eingabe
 * Rückgabe:
 *   log2(x) in Q(y_frac). Für x <= 0: errno=EDOM, INT16_MIN.
 */
inline fxp16_t fxp16_log2(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    return fxp16_logN(y_frac, x, x_frac, FXP32_Q15_ONE ); /* 1.0 in Q15 */
}



inline fxp16_t fxp16_log(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    return fxp16_logN(y_frac, x, x_frac, FXP32_Q15_M_LN2_Q15);
}


#define FXP32_Q15_M_LOG10_2 9877


fxp16_t fxp16_log10(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    return fxp16_logN(y_frac, x, x_frac, FXP32_Q15_M_LOG10_2);
}


fxp16_t fxp16_log1p(uint8_t y_frac, fxp16_t x, uint8_t x_frac)
{
    if (x <= 0)
    {
        errno = EDOM;
        return INT16_MIN;
    }

    fxp32_t tmp = x;

    fpxx_ashift_m(tmp,x_frac-FXP32_Q15);

    tmp = fxp32_add_sat32(tmp, FXP32_Q15_ONE);

    tmp = fxp32_logN_q15(tmp, FXP32_Q15_M_LN2_Q15);

    fpxx_ashift_m(tmp,FXP32_Q15-y_frac);

    fxp16_sat_m(tmp);
    return (fxp16_t)tmp;
}




int fxp16_ilogb(fxp16_t x, uint8_t x_frac)
{
    /* Domain-Check: ilogb(0) ist undefiniert */
    if (x == 0) {
        errno = EDOM;
        return INT_MIN; /* analog zu FP_ILOGB0 */
    }

    /* Betrag als 16-Bit unsigned, robust auch für x == INT16_MIN */
    uint16_t ux = (x < 0) ? (uint16_t)(-x) : (uint16_t)x;

    /* MSB-Position (0-basiert) bestimmen */
    int p = -1;
    while (ux) {
        ux >>= 1;
        ++p;
    }

    /* Exponent relativ zur Binärpunktskalierung zurückgeben */
    return p - (int)x_frac;
}


inline fxp16_t fxp16_copysign(fxp16_t x, fxp16_t y)
{
   int32_t result = abs((int32_t)x);

   if(fxp16_signbit(y))
   {
      result = -result;
   }

   fxp16_sat_m(result);

   return (fxp16_t)result;
}










/*!
    \brief      Compute absolute value
    \details    Returns the absolute value of x: |x|.


    \param      x
    \returns     absolute value of x

*/
fxp16_t fxp16_fabs (fxp16_t x)
{
   int32_t result = (x < 0) ? (-(int32_t)x) : ((int32_t)x);
   fxp16_sat_m(result);
   return result;
}


/*!
    \brief      Compute absolute value (operates on int values)
    \details    Returns the absolute value of x: |int(x)|.


    \param     x
    \returns   absolute value of int(x)

*/
fxp16_t fxp16_abs (fxp16_t x, uint8_t frac)
{
   int32_t result = (x < 0) ? (-(int32_t)x) : ((int32_t)x);
   result&=~((1<<frac)-1);
   fxp16_sat_m(result);
   return result;
}



fxp16_t fxp16_fma (fxp16_t x, uint8_t xfrac, fxp16_t y, uint8_t yfrac, fxp16_t z, uint8_t zfrac)
{
   int32_t result;
   int8_t relshift = xfrac+yfrac-zfrac;

   result = (int32_t)x*(int32_t)y;
   fpxx_ashift_m(result,relshift);
   result += z;
   fxp16_sat_m(result);
   return (fxp16_t)result;
}

