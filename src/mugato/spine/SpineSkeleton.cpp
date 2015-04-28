#include <mugato/spine/SpineSkeleton.hpp>
#include <gorn/base/Rect.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <spine/spine.h>
#include <buffer.hpp>
#include <algorithm>

namespace mugato {

    SpineSkeleton::SpineSkeleton():
    _skeleton(nullptr), _anim(nullptr)
    {
    }

    SpineSkeleton::SpineSkeleton(const std::shared_ptr<Data>& data):
    _data(data), _skeleton(_data->createSkeleton()),
    _anim(_data->createAnimationState())
    {
    }

    SpineSkeleton::~SpineSkeleton()
    {
        cleanup();
    }

    void SpineSkeleton::setSlotsMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _slotsMaterial = mat;
    }

    void SpineSkeleton::setBonesMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _bonesMaterial = mat;
    }

    void SpineSkeleton::cleanup()
    {
        if(_skeleton != nullptr)
        {
            spSkeleton_dispose(_skeleton);
            _skeleton = nullptr;
        }
        if(_anim != nullptr)
        {
            spAnimationState_dispose(_anim);
            _anim = nullptr;
        }
    }

    SpineSkeleton::SpineSkeleton(const SpineSkeleton& other):
    _data(other._data), _skeleton(_data->createSkeleton()),
    _anim(_data->createAnimationState()),
    _slotsMaterial(other._slotsMaterial),
    _bonesMaterial(other._bonesMaterial)
    {
    }

    SpineSkeleton& SpineSkeleton::operator=(const SpineSkeleton& other)
    {
        if(this != &other)
        {
            cleanup();
            _data = other._data;
            _skeleton = _data->createSkeleton();
            _anim = _data->createAnimationState();
            _slotsMaterial = other._slotsMaterial;
            _bonesMaterial = other._bonesMaterial;
        }
        return *this;
    }

    void SpineSkeleton::update(double dt)
    {
        spSkeleton_update(_skeleton, dt);
	    spAnimationState_update(_anim, dt);
	    spAnimationState_apply(_anim, _skeleton);
        spSkeleton_updateWorldTransform(_skeleton);
    }

    void SpineSkeleton::setAnimation(const std::string& name,
        int track, bool loop)
    {
        spAnimation* anim = spSkeletonData_findAnimation(
            _skeleton->data, name.c_str());
	    if (!anim)
        {
		    throw Exception("Animation not found");
	    }
	    spAnimationState_setAnimation(_anim, track, anim, loop);
    }

    void SpineSkeleton::setAnimation(const std::string& name, bool loop)
    {
        setAnimation(name, 0, loop);
    }

    void SpineSkeleton::addAnimation(const std::string& name,
        int track, bool loop, float delay)
    {
        spAnimation* anim = spSkeletonData_findAnimation(
            _skeleton->data, name.c_str());
	    if (!anim)
        {
		    throw Exception("Animation not found");
	    }
	    spAnimationState_addAnimation(_anim, track, anim, delay, loop);
    }

    void SpineSkeleton::addAnimation(const std::string& name,
        bool loop, float delay)
    {
        addAnimation(name, 0, loop, delay);
    }

    std::shared_ptr<gorn::Material> getAttachmentMaterial(
        spRegionAttachment* attach)
    {
    	return *(std::shared_ptr<gorn::Material>*)(
            (spAtlasRegion*)attach->rendererObject)->page->rendererObject;
    }

    std::shared_ptr<gorn::Material> getAttachmentMaterial(
        spMeshAttachment* attach)
    {
	    return *(std::shared_ptr<gorn::Material>*)(
            (spAtlasRegion*)attach->rendererObject)->page->rendererObject;
    }

    std::shared_ptr<gorn::Material> getAttachmentMaterial(
        spSkinnedMeshAttachment* attach) {
	    return *(std::shared_ptr<gorn::Material>*)(
            (spAtlasRegion*)attach->rendererObject)->page->rendererObject;
    }

    void SpineSkeleton::renderSkeleton(gorn::RenderQueue& queue) const
    {
        for (int i = 0, n = _skeleton->slotsCount; i < n; i++)
        {
		    spSlot* slot = _skeleton->drawOrder[i];
		    if (!slot->attachment)
            {
                continue;
            }
		    std::shared_ptr<gorn::Material> material;
            gorn::RenderCommand::Elements elements;
            buffer colors;
            buffer texcoords;
            buffer positions;
            
		    switch (slot->attachment->type)
            {
		    case SP_ATTACHMENT_REGION: {
			    auto attach = reinterpret_cast<spRegionAttachment*>(slot->attachment);
			    material = getAttachmentMaterial(attach);
                size_t size = 2*sizeof(float)*4;
                positions.size(size);
			    spRegionAttachment_computeWorldVertices(
                    attach, slot->bone, reinterpret_cast<float*>(positions.data()));
                texcoords.assign(attach->uvs, size);
			    elements = {0, 1, 2, 2, 3, 0};
			    colors = { attach->r, attach->g, attach->b, attach->a };
                break;
		    }
		    case SP_ATTACHMENT_MESH: {
			    auto attach = reinterpret_cast<spMeshAttachment*>(slot->attachment);
			    material = getAttachmentMaterial(attach);
                size_t size = 2*sizeof(float)*attach->verticesCount;
                positions.size(size);
			    spMeshAttachment_computeWorldVertices(
                    attach, slot, reinterpret_cast<float*>(positions.data()));
			    texcoords.assign(attach->uvs, size);
			    elements.assign(attach->triangles,
                    attach->triangles + attach->trianglesCount);
                colors = { attach->r, attach->g, attach->b, attach->a };
			    break;
		    }
		    case SP_ATTACHMENT_SKINNED_MESH: {
			    auto attach = (spSkinnedMeshAttachment*)slot->attachment;
			    material = getAttachmentMaterial(attach);
                size_t size = 2*sizeof(float)*attach->uvsCount;
                positions.size(size);
			    spSkinnedMeshAttachment_computeWorldVertices(
                    attach, slot, reinterpret_cast<float*>(positions.data()));
                texcoords.assign(attach->uvs, size);
			    elements.assign(attach->triangles,
                    attach->triangles + attach->trianglesCount);
                colors = { attach->r, attach->g, attach->b, attach->a };
			    break;
		    }
		    default:
                break;
		    } 
		    if (material)
            {
                // flip y coordinates
                auto coords = reinterpret_cast<float*>(texcoords.data());
                for(size_t i=1; i<texcoords.size()/sizeof(float); i+=2)
                {
                    coords[i] = 1.0f - coords[i];
                }

                queue.addCommand()
                    .withMaterial(material)
                    .withAttribute(gorn::AttributeKind::Position,
                        std::move(positions), 2, gorn::BasicType::Float)
                    .withAttribute(gorn::AttributeKind::TexCoords,
                        std::move(texcoords), 2, gorn::BasicType::Float)
                    .withAttribute(gorn::AttributeKind::Color,
                        std::move(colors), 4, gorn::BasicType::Byte)
                    .withElements(elements);
		    }
	    }
    }

    void SpineSkeleton::renderSlots(gorn::RenderQueue& queue) const
    {
        if (!_slotsMaterial)
        {
            return;
        }
	    for (int i = 0, n = _skeleton->slotsCount; i < n; i++)
        {
		    spSlot* slot = _skeleton->drawOrder[i];
		    if (!slot->attachment)
            {
                continue;
            }
            gorn::RenderCommand::Elements elements;
            gorn::RenderCommand::Elements linelms;
            buffer positions;
		    switch (slot->attachment->type)
            {
		    case SP_ATTACHMENT_REGION: {
			    auto attach = reinterpret_cast<spRegionAttachment*>(slot->attachment);
                size_t size = 2*sizeof(float)*4;
                positions.size(size);
			    spRegionAttachment_computeWorldVertices(
                    attach, slot->bone, reinterpret_cast<float*>(positions.data()));
			    linelms = {0, 1, 1, 2, 2, 3, 3, 0};
                break;
		    }
		    case SP_ATTACHMENT_MESH: {
			    auto attach = reinterpret_cast<spMeshAttachment*>(slot->attachment);
                size_t size = 2*sizeof(float)*attach->verticesCount;
                positions.size(size);
			    spMeshAttachment_computeWorldVertices(
                    attach, slot, reinterpret_cast<float*>(positions.data()));
			    elements.assign(attach->triangles,
                    attach->triangles + attach->trianglesCount);
			    break;
		    }
		    case SP_ATTACHMENT_SKINNED_MESH: {
			    auto attach = (spSkinnedMeshAttachment*)slot->attachment;
                size_t size = 2*sizeof(float)*attach->uvsCount;
                positions.size(size);
			    spSkinnedMeshAttachment_computeWorldVertices(
                    attach, slot, reinterpret_cast<float*>(positions.data()));
			    elements.assign(attach->triangles,
                    attach->triangles + attach->trianglesCount);
			    break;
		    }
		    default:
                break;
		    }

            if(linelms.empty() && !elements.empty())
            {
                linelms.reserve(elements.size()*2);
                for(auto itr = elements.begin(); itr != elements.end(); itr+=3)
                {
                    auto a = *itr;
                    auto b = *(itr+1);
                    auto c = *(itr+2);
                    linelms.insert(linelms.end(), {a, b, b, c, c, a});
                }
            }
            queue.addCommand()
                .withMaterial(_slotsMaterial)
                .withAttribute(gorn::AttributeKind::Position,
                    std::move(positions), 2, gorn::BasicType::Float)
                .withDrawMode(gorn::DrawMode::Lines)
                .withElements(linelms);
	    }
    }

    void SpineSkeleton::renderBones(gorn::RenderQueue& queue) const
    {
        if (!_bonesMaterial)
        {
            return;
        }
	    for (int i = 0, n = _skeleton->bonesCount; i < n; i++)
        {
		    spBone *bone = _skeleton->bones[i];
		    float x = bone->data->length * bone->m00 + bone->worldX;
		    float y = bone->data->length * bone->m10 + bone->worldY;
            buffer positions{bone->worldX, bone->worldY, x, y};
            queue.addCommand()
                .withMaterial(_bonesMaterial)
                .withAttribute(gorn::AttributeKind::Position,
                    std::move(positions), 2, gorn::BasicType::Float)
                .withDrawMode(gorn::DrawMode::Lines);
	    }
    }

    void SpineSkeleton::render(gorn::RenderQueue& queue) const
    {
	    renderSkeleton(queue);
        renderSlots(queue);
        renderBones(queue);
    }

    gorn::Rect SpineSkeleton::getBoundingBox() const
    {
        float worldVertices[1000];
        float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
        for (int i = 0; i < _skeleton->slotsCount; ++i)
        {
	        spSlot* slot = _skeleton->slots[i];
	        if (!slot->attachment) continue;
	        int verticesCount;
	        if (slot->attachment->type == SP_ATTACHMENT_REGION)
            {
		        spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
		        spRegionAttachment_computeWorldVertices(attachment, slot->bone, worldVertices);
		        verticesCount = 8;
	        }
            else if (slot->attachment->type == SP_ATTACHMENT_MESH)
            {
		        spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
		        spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);
		        verticesCount = mesh->verticesCount;
	        }
            else if (slot->attachment->type == SP_ATTACHMENT_SKINNED_MESH)
            {
		        spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)slot->attachment;
		        spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);
		        verticesCount = mesh->uvsCount;
	        }
            else
            {
		        continue;
            }
	        for (int ii = 0; ii < verticesCount; ii += 2)
            {
		        float x = worldVertices[ii];
                float y = worldVertices[ii + 1];
		        minX = std::min(minX, x);
		        minY = std::min(minY, y);
		        maxX = std::max(maxX, x);
		        maxY = std::max(maxY, y);
	        }
        }
        return gorn::Rect(glm::vec2(minX, minY), glm::vec2(maxX - minX, maxY - minY));
    }

    glm::vec2 SpineSkeleton::getSize() const
    {
        auto p = getBoundingBox().max();
        return glm::vec2(p.x, p.y);
    }

}
