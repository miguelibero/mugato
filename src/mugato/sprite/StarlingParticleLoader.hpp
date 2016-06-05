
#ifndef __mugato__StarlingParticleLoader__
#define __mugato__StarlingParticleLoader__

#include <gorn/asset/DataAssetLoader.hpp>

namespace mugato {

    class ParticleSystemConfig;

    class StarlingParticleLoader : public gorn::DataAssetLoader<ParticleSystemConfig>
    {
    public:
		StarlingParticleLoader();

        bool validate(const buffer& data) const NOEXCEPT;
		ParticleSystemConfig load(const buffer& data) const;

    };
}

#endif
