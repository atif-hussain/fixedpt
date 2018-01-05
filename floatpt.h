#include <stdio.h>
	// stdio instead of iostream as targetted use is for embedded systems

class floatpt
{
private:
	float val;
	float max;
	void update(void) {
		if (max < val) max = val;
		if (-val > max) max = -val;
	}
	void clear(void) { val = 0; max = 0; } // to clear tmp vars (eg. func parameters) to prevent logging in destructor

public:
	/* Constructor summary */
	floatpt(void) { }
	floatpt(float f) { val = f; update(); }
	/* Destructor */
	inline ~floatpt() {
		if(max!=0) printf("floatpt took a max abs value= %f\n", max);
	}

	/* Initialization summary */
	inline floatpt& operator = (floatpt val2) { val = val2.asFloat(); update(); val2.clear(); return *this; }
	inline floatpt& operator = (int i) { val = i; update(); return *this; }
	inline floatpt& operator = (float f) { val = f; update(); return *this; }

	/* deprecated: see cast operator int */
	inline int asInt () { return (int) val; }
	/* deprecated: see cast operator float */
	inline float asFloat(void) { return val; }

	/* cast operators */
	inline operator int () { return (int)val; }
	inline operator float() { return val; }

	/* Arithmetic assignment summary */
	inline floatpt operator += (floatpt val2) { val += (float)val2; update(); val2.clear(); return *this; }
	inline floatpt operator += (int val2) { val += val2; update(); return *this; }
	inline floatpt operator += (float val2) { val += val2; update(); return *this; }

	inline floatpt operator -= (floatpt val2) { val -= (float)val2; update(); val2.clear(); return *this; }
	inline floatpt operator -= (int val2) { val -= val2; update(); return *this; }
	inline floatpt operator -= (float val2) { val -= val2; update(); return *this; }

	/* deprecated: in-place *= operator causes precision loss; so use c=multiply(a,b) */
	inline floatpt operator *= (floatpt val2) { val *= (float)val2; update(); val2.clear();  return *this; }
	inline floatpt operator *= (int val2) { val *= val2; update(); return *this; }
	inline floatpt operator *= (float val2) { val *= val2; update(); return *this; }
	inline void multiply (floatpt val1, floatpt val2) { val = ((float)val2) * ((float)val1); update(); val1.clear(); val2.clear(); }

	/* inverse(..) returns into a different floatpt storage, as magnitude of input and output differ */
	void inverse (floatpt denom) { val = 1.0f/(float)denom; update(); denom.clear(); }
};
