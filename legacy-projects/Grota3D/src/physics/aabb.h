#ifndef GROTA_PHYSICS_AABB_H
#define GROTA_PHYSICS_AABB_H
#include "core/mathematics.h"
#include "core/list.h"
#include <stdbool.h>

typedef struct _RigidBody {
    float mass;
    float bouncy;
    float staticFriction;
    float dynamicFriction;
    float velocity;
    // Internal, temporary values only used during the collision phase
    Vec3 force;
} RigidBody;

#define RIGIDBODY_NEW {1,0.1f,1,0.3f,VEC3_ZERO,VEC3_ZERO}


typedef enum _ColliderType {
    COLLIDER_AABB,
    COLLIDER_SPHERE
}ColliderType;

typedef struct _AABBCollider {
    ColliderType type;// always COLLIDER_AABB is for introspection
    RigidBody body;
    Vec3 size;
    Vec3 position;
}AABBCollider;

#define AABBCOLLIDER_NEW {COLLIDER_AABB,RIGIDBODY_NEW,VEC3_FILL_ONE,VEC3_ZERO};
typedef struct _SphereCollider {
    ColliderType type;// always COLLIDER_SPHERE is for introspection
    RigidBody body;
    float radius;
    Vec3 position;
}SphereCollider;

#define SPHERECOLLIDER_NEW {COLLIDER_SPHERE,RIGIDBODY_NEW,1.0f,VEC3_ZERO};

AABBCollider *collider_aabb_alloc();
AABBCollider *collider_aabb_destroy(AABBCollider *aabb);
 
bool detect_colision(void* collider_a,void *collider_b);

#endif