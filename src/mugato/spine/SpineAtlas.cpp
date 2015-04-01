
#include <mugato/spine/SpineAtlas.hpp>
#include <mugato/base/Exception.hpp>
#include <spine/Atlas.h>
#include <spine/SkeletonJson.h>

namespace mugato {

    SpineAtlas::SpineAtlas(const std::string& file):
    _data(spAtlas_createFromFile(file.c_str(), nullptr))
    {
        if(_data == nullptr)
        {
            throw Exception("Error reading atlas data.");
        }
    }

    SpineAtlas::~SpineAtlas()
    {
        spAtlas_dispose(_data);
    }

    spSkeletonJson* SpineAtlas::createJson() const
    {
        return spSkeletonJson_create(_data);
    }

}
