
#include <mugato/sprite/StarlingParticleLoader.hpp>
#include <mugato/sprite/ParticleSystemConfig.hpp>
#include <gorn/base/String.hpp>
#include <rapidxml/rapidxml.hpp>
#include <buffer.hpp>
#include <buffer_writer.hpp>

using namespace rapidxml;

namespace mugato
{
    void StarlingParticleLoadXmlDocument(rapidxml::xml_document<>& doc, const buffer& data)
    {
        buffer temp;
        buffer_writer output(temp);
        output.write(data);
        output.fill(0);
        doc.parse<0>(reinterpret_cast<char*>(temp.data()));
    }


	StarlingParticleLoader::StarlingParticleLoader()
    {
    }
    
    bool StarlingParticleLoader::validate(const buffer& data) const NOEXCEPT
    {
        if(data.binary())
        {
            return false;
        }
        xml_document<> doc;
		StarlingParticleLoadXmlDocument(doc, data);
        return std::string(doc.first_node()->name()) == "particleEmitterConfig";
    }

    ParticleSystemConfig StarlingParticleLoader::load(const buffer& data) const
    {
        xml_document<> doc;
		StarlingParticleLoadXmlDocument(doc, data);
        
		ParticleSystemConfig config;
        auto root = doc.first_node("plist");
        return config;
    }

}
