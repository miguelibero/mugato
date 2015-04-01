#include <mugato/spine/SpineSkeleton.hpp>
#include <mugato/base/Rectangle.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/Kinds.hpp>
#include <spine/spine.h>
#include <buffer.hpp>
#include <algorithm>

namespace mugato {

    SpineSkeleton::SpineSkeleton():
    _skeleton(nullptr)
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
    _anim(_data->createAnimationState())
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
        }
        return *this;
    }

    void SpineSkeleton::update(double dt)
    {
        spSkeleton_update(_skeleton, dt);
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

    void SpineSkeleton::render(gorn::RenderQueue& queue) const
    {
	    //int additive = -1;

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
                positions.size();
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
		    default: ;
		    } 
		    if (material)
            {
                queue.addCommand()
                    .withMaterial(material)
                    .withAttribute(gorn::AttributeKind::Position,
                        std::move(positions), 2, gorn::BasicType::Float)
                    .withAttribute(gorn::AttributeKind::TexCoords,
                        std::move(texcoords), 2, gorn::BasicType::Float)
                    .withAttribute(gorn::AttributeKind::Color,
                        std::move(colors), 4, gorn::BasicType::Byte)
                    .withElements(elements);

                /*
			    if (slot->data->additiveBlending != additive) {
				    _batch->flush();
				    GL::blendFunc(_blendFunc.src, slot->data->additiveBlending ? GL_ONE : _blendFunc.dst);
				    additive = slot->data->additiveBlending;
			    }
			    color.a = _skeleton->a * slot->a * a * 255;
			    color.r = _skeleton->r * slot->r * r * 255;
			    color.g = _skeleton->g * slot->g * g * 255;
			    color.b = _skeleton->b * slot->b * b * 255;
                */

		    }
	    }

        /*
	    if (_debugSlots || _debugBones)
        {

		    if (_debugSlots) {
			    // Slots.
			    DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
			    glLineWidth(1);
			    Vec2 points[4];
			    V3F_C4B_T2F_Quad quad;
			    for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
				    spSlot* slot = _skeleton->drawOrder[i];
				    if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION) continue;
				    spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
				    spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
				    points[0] = Vec2(_worldVertices[0], _worldVertices[1]);
				    points[1] = Vec2(_worldVertices[2], _worldVertices[3]);
				    points[2] = Vec2(_worldVertices[4], _worldVertices[5]);
				    points[3] = Vec2(_worldVertices[6], _worldVertices[7]);
				    DrawPrimitives::drawPoly(points, 4, true);
			    }
		    }
		    if (_debugBones) {
			    // Bone lengths.
			    glLineWidth(2);
			    DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
			    for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
				    spBone *bone = _skeleton->bones[i];
				    float x = bone->data->length * bone->m00 + bone->worldX;
				    float y = bone->data->length * bone->m10 + bone->worldY;
				    DrawPrimitives::drawLine(Vec2(bone->worldX, bone->worldY), Vec2(x, y));
			    }
			    // Bone origins.
			    DrawPrimitives::setPointSize(4);
			    DrawPrimitives::setDrawColor4B(0, 0, 255, 255); // Root bone is blue.
			    for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
				    spBone *bone = _skeleton->bones[i];
				    DrawPrimitives::drawPoint(Vec2(bone->worldX, bone->worldY));
				    if (i == 0) DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
			    }
		    }
		    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	    }
        */
    }

    Rectangle SpineSkeleton::getBoundingBox() const
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
        return Rectangle(glm::vec2(minX, minY), glm::vec2(maxX - minX, maxY - minY));
    }

    glm::vec2 SpineSkeleton::getSize() const
    {
        auto p = getBoundingBox().max();
        return glm::vec2(p.x, p.y);
    }

}
