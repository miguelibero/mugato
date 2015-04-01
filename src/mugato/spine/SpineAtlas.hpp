#ifndef __mugato__SpineAtlas__
#define __mugato__SpineAtlas__

#include <string>

struct spAtlas;
struct spSkeletonJson;

namespace mugato {

    class SpineAtlas
    {
    private:
        spAtlas* _data;

    public:
        SpineAtlas(const std::string& file);
        ~SpineAtlas();
        SpineAtlas(const SpineAtlas& other) = delete;

        spSkeletonJson* createJson() const;
    };

}


#endif
