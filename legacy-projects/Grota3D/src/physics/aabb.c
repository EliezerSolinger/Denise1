#include "aabb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "core/mathematics.h"

AABBCollider *collider_aabb_alloc() {
    AABBCollider *aabb=calloc(1,sizeof(AABBCollider));
    aabb->size=vec3_create(1,1,1);
    aabb->type=COLLIDER_AABB;
    return aabb; 
}

AABBCollider *collider_aabb_destroy(AABBCollider *aabb) {
    free(aabb);
    return NULL;
}

SphereCollider *collider_sphere_alloc() {
    SphereCollider *sphere=calloc(1,sizeof(AABBCollider));
    sphere->radius=0.5f;
    sphere->type=COLLIDER_SPHERE;
    return sphere; 
}
SphereCollider *collider_sphere_destroy(SphereCollider *sphere) {
    free(sphere);
    return NULL;
}

static ColliderType get_collider_type(void *collider) {
    return *((ColliderType*) collider);
}
bool detect_colision(void* collider_a,void *collider_b) {
    if(!collider_a || !collider_b) {
        printf("NULL POINTER ON DETECT COLISION!");
        return false;
    }
    ColliderType a_type=get_collider_type(collider_a);
    ColliderType b_type=get_collider_type(collider_b);
    if(a_type==COLLIDER_AABB && b_type==COLLIDER_AABB) {
        AABBCollider a=*((AABBCollider*)collider_a);
        AABBCollider b=*((AABBCollider*)collider_b);
        Vec3 a_min=vec3_sub(a.position,vec3_scalar_mult(a.size,0.5f));
        Vec3 a_max=vec3_add(a.position,vec3_scalar_mult(a.size,0.5f));
        Vec3 b_min=vec3_sub(b.position,vec3_scalar_mult(b.size,0.5f));
        Vec3 b_max=vec3_add(b.position,vec3_scalar_mult(b.size,0.5f));

        Vec3 c1=vec3_sub(a_min,b_max);
        Vec3 c2=vec3_sub(a_max,b_min);
        bool colide= (a_min.x <= b_max.x && a_max.x >= b_min.x) &&
                    (a_min.y <= b_max.y && a_max.y >= b_min.y) &&
                    (a_min.z <= b_max.z && a_max.z >= b_min.z);
        /*
        if(colide) if(collision_point!=NULL)
        */  

        return colide;
       //return false;
    }else if(a_type==COLLIDER_SPHERE && b_type==COLLIDER_SPHERE){
        SphereCollider sphere_a=*((SphereCollider*)collider_a);
        SphereCollider sphere_b=*((SphereCollider*)collider_b);
       
        float distance = vec3_distance(sphere_a.position,sphere_b.position);
        bool colide=distance < (sphere_a.radius+sphere_b.radius);
        /*
        if(colide) if(collision_point!=NULL)
            *collision_point=vec3_lerp(sphere_a.position,sphere_b.position,sphere_a.position)
        */
        return colide;
    } else if((a_type==COLLIDER_AABB && b_type==COLLIDER_SPHERE) || (a_type==COLLIDER_SPHERE && b_type==COLLIDER_AABB)) {
        SphereCollider sphere;
        AABBCollider aabb;
        if(a_type==COLLIDER_SPHERE) {
            sphere=*((SphereCollider*)collider_a);
            aabb=*((AABBCollider*)collider_b);
        } else {
            sphere=*((SphereCollider*)collider_b);
            aabb=*((AABBCollider*)collider_a);
        }

    // get box closest point to sphere center by clamping

        Vec3 box_min=vec3_sub(aabb.position,vec3_scalar_mult(aabb.size,0.5f));
        Vec3 box_max=vec3_add(aabb.position,vec3_scalar_mult(aabb.size,0.5f));

        float x = fmax(box_min.x, fmin(sphere.position.x, box_max.x));
        float y = fmax(box_min.y, fmin(sphere.position.y, box_max.y));
        float z = fmax(box_min.z, fmin(sphere.position.z, box_max.z));

        // this is the same as isPointInsideSphere

        float distance = sqrtf((x - sphere.position.x) * (x - sphere.position.x) +
                                (y - sphere.position.y) * (y - sphere.position.y) +
                                (z - sphere.position.z) * (z - sphere.position.z));

        return distance < sphere.radius;
    }
     else {
        printf("UNSUPPORTED COLLIDER TYPES!\n");
        return false;
    }

}