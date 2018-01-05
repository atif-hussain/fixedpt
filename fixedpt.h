#include <math.h>

// ISO standard: float = (1sign)|(8exponent+127)|(23mantissa)
// Length of Mantissa for float, represents exponent position
const int MANTISSA_SIZE = 23;

template <int _q>
class fixedpt
{
private:
	int val;

public:
	/* Constructor summary */
	fixedpt (void) {}
	fixedpt (int fixed) { val=fixed; }
	int asFixed(void) { return val; }
	fixedpt (float f) {
		float b = f;
		int d = *((int*)&b) + (_q<<MANTISSA_SIZE);
		b = *((float*)&d);
		val = (int)floor(b+0.5f);
	}

	/* Initialization summary */
	template <int _q2>
	inline fixedpt&
		operator = (fixedpt<_q2> val2) { val = val2.asFixed<_q>(); return *this; }
	inline fixedpt&
		operator = (int i) { val = i<<_q; return *this; }
	inline fixedpt&
		operator = (float f) {
			float b = f;
			int d = *((int*)&b) + (_q<<MANTISSA_SIZE);
			b = *((float*)&d);
			val = (int)(b);
			return *this;
		}

	/* Get summary */
	template <int _q2>
	inline
		int asFixed () {
			if (_q>_q2)
				return (val>>(_q-_q2));
			else if (_q<_q2)
				return (val<<(_q2-_q));
			else
				return (val);
		}
	/* deprecated: see cast operator int */
	inline 
		int asInt () { return (val>>_q); }
	/* deprecated: see cast operator float */
	inline 
		float asFloat () {
			float b = (float)val;
			int d = val? (*((int*)&b) - (_q<<MANTISSA_SIZE)) : 0;
			return *((float*)&d);
		}

	/* cast operators */
	template <int _q2>
	inline
		operator fixedpt<_q2> () {
			fixedpt<_q2> val2 = *this;
			return (val2);
		}
	inline
		operator int () { return (val>>_q); }
	inline
		operator float () {
			float b = (float)val;
			int d = val? (*((int*)&b) - (_q<<MANTISSA_SIZE)) : 0;
			return *((float*)&d);
		}

	/* Arithmetic assignment summary */
	template <int _q2>
	inline fixedpt operator += (fixedpt<_q2> val2) { val += val2.asFixed<_q>(); return *this; }
	inline fixedpt operator += (int val2) { val += val2<<_q; return *this; }
	inline fixedpt operator += (float val2) { fixedpt<_q> tmp = val2; val += tmp; return *this; }

	template <int _q2>
	inline fixedpt operator -= (fixedpt<_q2> val2) { val -= val2.asFixed<_q>(); return *this; }
	inline fixedpt operator -= (int val2) { val -= val2<<_q; return *this; }
	inline fixedpt operator -= (float val2) { fixedpt<_q> tmp = val2; val -= tmp; return *this; }

	/* deprecated: in-place *= operator causes precision loss; so use c=multiply(a,b) */
	inline fixedpt operator *= (int val2) { val *= val2; return *this; }
	template <int _q2> inline
	fixedpt operator *= (fixedpt<_q2> val2) { val = (int)( ((long long int)val2.asFixed()*val) >> _q2); return *this; }
	template <int _q1, int _q2> inline
	void multiply (fixedpt<_q1> val1, fixedpt<_q2> val2) { val = (int)(((long long int)val2.asFixed()*val1.asFixed()) >> (_q1+_q2-_q)); }

	/* inverse(..) returns into a different fixedpt storage, as magnitude of input and output differ */
	template <int _q2> //inline; this function takes space and some calls, so not inline
		void inverse (fixedpt<_q2> denom) {
						// 	denom has _q2 fractional digits; _q2'th digit from right is units; 
						// say r2'th digit is leftmost 1; 1<<r2 <= val <  2<<r2
						// answer to store in this; _q fractional digits; _q'th digit is units, r'th digit is leftmost 1;
						// r2-q2 + r-q = -1 (unless denom is simple power of 2, where it's 0)

			// treat denom as 2.30 format, and call inverse, then shift right result to get final answer
			val = (int) (inverse2dt30(denom.asFixed()) >> (30+30-_q-_q2));
		}
		//find inverse of a fixed point integer in 2.30 format
private: 
		long long inverse2dt30(int a) {	//consider input a is 2.30 format, and to return its inverse 32+2.30;
				// if input/output is 12.20 then right shift answer by 10
			long long x;

			// approximate lookup of inverse function
			static const unsigned int tab[] = {	// domain 0.5 .. 1.0-*2^31
												// i.e. values are 0x4-0000-0000 divided by /8, /9, /A, /B, /C, /D, /E, /F
				0x80000000, 0x71c71c71, 0x66666666, 0x5d1745d1,
				0x55555555, 0x4ec4ec4e, 0x49249249, 0x44444444
			};
			bool sign = a >> 31; if (sign) a = -a;
			int dim = 0; //inverse is sensitive to magnitude
			if (!(a & 0x7fff8000)) { a <<= 16; dim += 16; }
			if (!(a & 0x7f800000)) { a <<= 8; dim += 8; }
			if (!(a & 0x78000000)) { a <<= 4; dim += 4; }
			if (!(a & 0x60000000)) { a <<= 2; dim += 2; }
			if (!(a & 0x40000000)) { a <<= 1; dim += 1; }

			x = tab[(a >> 27) & 7] >> 1;
			unsigned int Two = (unsigned int)2 <<30;
			// 2 iterations of newton-raphson  x = x(2-ax), to compute exact inverse
			x = (x * (Two - ((a*x) >>30))) >>30;
			x = (x * (Two - ((a*x) >>30))) >>30;

			return (sign)? (-x << dim) : (x << dim);
		}
};
