#include <mugato/spine/SpineSkeleton.hpp>
#include <mugato/base/Rectangle.hpp>
#include <spine/spine.h>
#include <algorithm>

namespace mugato {

    SpineSkeleton::SpineSkeleton():
    _skeleton(nullptr)
    {
    }

    SpineSkeleton::SpineSkeleton(const std::shared_ptr<spSkeletonData>& data):
    _data(data), _skeleton(spSkeleton_create(data.get()))
    {
    }

    SpineSkeleton::~SpineSkeleton()
    {
        spSkeleton_dispose(_skeleton);
    }

    void SpineSkeleton::update(double dt)
    {
        spSkeleton_update(_skeleton, dt);
    }

    void SpineSkeleton::play(const std::string& anim)
    {
    }

    void SpineSkeleton::render(gorn::RenderQueue& queue) const
    {
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
