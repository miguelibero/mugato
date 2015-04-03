
#include <mugato/spine/SpineAnimationMixDefinition.hpp>

namespace mugato {

    SpineAnimationMixDefinition::SpineAnimationMixDefinition():
    _duration(0.0f)
    {
    }

    SpineAnimationMixDefinition& SpineAnimationMixDefinition::
        withFrom(const std::string& from)
    {
        _from = from;
        return *this;
    }

    SpineAnimationMixDefinition& SpineAnimationMixDefinition::
        withTo(const std::string& to)
    {
        _to = to;
        return *this;
    }

    SpineAnimationMixDefinition& SpineAnimationMixDefinition::
        withDuration(float duration)
    {
        _duration = duration;
        return *this;
    }

    const std::string& SpineAnimationMixDefinition::getFrom() const
    {
        return _from;
    }

    const std::string& SpineAnimationMixDefinition::getTo() const
    {
        return _to;
    }

    float SpineAnimationMixDefinition::getDuration() const
    {
        return _duration;
    }

}

