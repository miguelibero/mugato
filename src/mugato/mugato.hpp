#ifndef __mugato__
#define __mugato__

#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <mugato/base/Kinds.hpp>
#include <mugato/base/OcTree.hpp>
#include <mugato/base/OcTreeElement.hpp>
#include <mugato/base/OcTreeNode.hpp>
#include <mugato/base/Application.hpp>
#include <mugato/base/Alignment.hpp>
#include <mugato/base/Light.hpp>

#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/GridFontAtlasLoader.hpp>
#include <mugato/label/Label.hpp>
#include <mugato/label/FontAtlas.hpp>
#include <mugato/label/FontAtlasRegion.hpp>
#include <mugato/label/LabelCharacter.hpp>
#include <mugato/label/LabelDefinition.hpp>
#include <mugato/label/LabelFont.hpp>
#include <mugato/label/LabelManager.hpp>

#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/SpriteAnimation.hpp>
#include <mugato/sprite/SpriteAnimationDefinition.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <mugato/sprite/SpriteDefinition.hpp>
#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/sprite/SpriteEnums.hpp>

#include <mugato/scene/Component.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/scene/EntityStack.hpp>
#include <mugato/scene/SpriteComponent.hpp>
#include <mugato/scene/LabelComponent.hpp>
#include <mugato/scene/OcTreeRenderComponent.hpp>
#include <mugato/scene/RenderInfoComponent.hpp>
#include <mugato/scene/ActionComponent.hpp>
#include <mugato/scene/TouchComponent.hpp>
#include <mugato/scene/ButtonComponent.hpp>
#include <mugato/scene/AlignComponent.hpp>
#include <mugato/scene/MeshComponent.hpp>
#include <mugato/scene/CameraComponent.hpp>
#include <mugato/scene/Action.hpp>
#include <mugato/scene/EmptyAction.hpp>
#include <mugato/scene/TweenAction.hpp>

#include <mugato/spine/SpineManager.hpp>
#include <mugato/spine/SpineSkeleton.hpp>
#include <mugato/spine/SpineSkeletonComponent.hpp>
#include <mugato/spine/SpineSkeletonDefinition.hpp>

#endif
