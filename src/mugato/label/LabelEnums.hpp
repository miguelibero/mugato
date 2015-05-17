

#ifndef __mugato__LabelEnums__
#define __mugato__LabelEnums__

namespace mugato
{   
    enum class LabelAlignment
    {
        TopLeft,
        TopCenter,
        TopRight,
        Left,
        Center,
        Right,
        BottomLeft,
        BottomCenter,
        BottomRight   
    };

    enum class LabelResizeMode
    {
        Original,
        FitWidth,
        FitHeight,
        Fit
    };
}

#endif
