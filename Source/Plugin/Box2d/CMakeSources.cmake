set (PLUGIN_PHYSICS_INC
        "Physics/B2DBoxCollider2D.h"
        "Physics/B2DPhysics.h"
        "Physics/B2DPhysicsScene.h"
        "Physics/B2DSphereCollider2D.h")
set (PLUGIN_PHYSICS_SRC
        "Physics/B2DBoxCollider2D.cpp"
        "Physics/B2DPhysics.cpp"
        "Physics/B2DPhysicsScene.cpp"
        "Physics/B2DSphereCollider2D.cpp")

set (PLUGIN_NOFILTER_INC
        "B2DPrerequisites.h")
set (PLUGIN_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${PLUGIN_PHYSICS_INC} ${PLUGIN_PHYSICS_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})
