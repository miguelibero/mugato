#ifndef __mugato__SpineAnimationMixDefinition__
#define __mugato__SpineAnimationMixDefinition__

#include <string>

namespace mugato {

    class SpineAnimationMixDefinition
    {
    private:
        std::string _from;
        std::string _to;
        float _duration;

    public:

        SpineAnimationMixDefinition();
        SpineAnimationMixDefinition& withFrom(const std::string& from);
        SpineAnimationMixDefinition& withTo(const std::string& to);
        SpineAnimationMixDefinition& withDuration(float duration);

        const std::string& getFrom() const;
        const std::string& getTo() const;
        float getDuration() const;
    };

}

#endif
