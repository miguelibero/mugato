
#ifndef __mugato__Easing__
#define __mugato__Easing__

#include <glm/gtc/constants.hpp>
#include <cmath>
#include <functional>

namespace mugato {

  	typedef double easing_t;

    template<typename T>
	class Easing
	{
	public:
        typedef std::function<T(easing_t p, const T& b, const T& c)> Callback;

        Easing() = delete;

	    static T linear(easing_t p, const T& b, const T& c)
	    {
		    return c*(float)p + b;
	    }

	    static T swing(easing_t p, const T& b, const T& c)
	    {
		    return linear(0.5 - cos( p*glm::pi<easing_t>() ) / 2, b, c);
	    }

        static T expo(easing_t p, const T& b, const T& c)
        {
            return (p==0) ? b : c * pow(2, 10 * (p - 1)) + b;
        }
	};
}

#endif
