//
// Created by jiney on 2024-06-07.
//

#include "B2DMapping.h"

b2BodyType B2DMapping::GetBodyType(EPhysicsBodyType type) {
    switch (type) {
        case EPhysicsBodyType::Static:
            return b2_staticBody;

        case EPhysicsBodyType::Kinematic:
            return b2_kinematicBody;

        case EPhysicsBodyType::Dynamic:
            return b2_dynamicBody;
    }
}
