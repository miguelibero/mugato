
#ifndef __mugato__Easing__
#define __mugato__Easing__

#include <glm/gtc/constants.hpp>
#include <cmath>

namespace mugato {

	class Easing
	{
	private:
		Easing(){}

	public:

    	typedef double easing_t;

        template<typename T>
	    T linear(easing_t p, const T& b, easing_t c)
	    {
		    return c*p + b;
	    }

        template<typename T>
	    T swing(easing_t p, const T& b, easing_t c)
	    {
		    return linear(0.5 - cosf( p*glm::pi<easing_t>() ) / 2, b, c);
	    }

        template<typename T>
        T expo(easing_t p, const T& b, easing_t c)
        {
            return (p==0) ? b : c * pow(2, 10 * (p - 1)) + b;
        }
	};
}

#endif
