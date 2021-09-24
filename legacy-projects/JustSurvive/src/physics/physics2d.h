#ifndef SINUCA_PHYSICS
#define SINUCA_PHYSICS
#include "core/mathematics.h"
#include "core/list.h"
#include <stdbool.h>
#define PHYSICS_PERCENT 0.7F
#define PHYSICS_SLOP 0.05F
#define PHYSICS_EPSILON 0.0001
#define PHYSICS_MAX_COLLISIONS 32

typedef enum _BodyType {
    BODY_BOX,
    BODY_SPHERE
} BodyType;

typedef struct _Body {
    BodyType type;
    /// this pointer refers to gameobject or related
    void *parent;
    unsigned int flag;
    bool active;
    bool trigger;
    float mass;
    // How "bouncy" this box is. The higher the value, the more the box will
    // bounce of in case of a collision
    float bouncy;
    float static_friction;
    float dynamic_friction;
    Vec2 velocity;
    Vec2 position;
    Vec2 size;
    // Internal, temporary values only used during the collision phase
    Vec2 force;
    Vec2 min;
    Vec2 max;
    // The value 0 is used as a placeholder for infinite mass
    float inverse_mass; 

}Body;

static inline void body_setMass(Body *this,float mass) {
    this->mass=mass;
    this->inverse_mass = mass == 0 ? 0 : 1.0f / mass;
}
static inline void body_resize(Body *this,Vec2 size,float mass) {
    this->size=size;
    body_setMass(this,mass);
}

static inline void body_apply_impulse(Body *this,Vec2 impulse) {
    if(!this->active) return;
    this->velocity.x += this->inverse_mass * impulse.x;
    this->velocity.y += this->inverse_mass * impulse.y;
}
static inline void body_apply_force(Body *this,Vec2 force) {
    if(!this->active) return;
    this->force.x += force.x;
    this->force.y += force.y;
}

static inline void body_clear_forces(Body *this) {
    this->force=vec2_create(0,0);
}
static inline void body_stop(Body *this) {
    this->force=vec2_create(0,0);
    this->velocity=vec2_create(0,0);
}

Body *body_alloc();
void body_destroy(Body *this);
void body_resize(Body *this,Vec2 size,float mass);
void body_apply_impulse(Body *this,Vec2 impulse);
void body_apply_force(Body *this,Vec2 force);
void body_clear_forces(Body *this);
void body_stop(Body *this);

typedef struct _Mainfold {
    Body *a;
    Body *b;
    float bouncy;
    float static_friction;
    float dynamic_friction;
    Vec2 normal;
    float e;
    float penetration;
} Mainfold;

Mainfold * mainfold_alloc(Body *a,Body *b);


typedef struct _PhysicsWorld {
    unsigned int iterations;
    float air_density;
    Vec2 gravity;
    Mainfold **contacts;
    size_t contacts_count;
    Body **bodys;
    size_t bodys_count;
    size_t bodys_capacity;
    bool paused;
} PhysicsWorld;

PhysicsWorld* physics_alloc(size_t capacity);
void physics_destroy(PhysicsWorld *this);
void physics_update(PhysicsWorld *this,float delta,int fps_simulation);

void physics_add_body(PhysicsWorld *this,Body *body);
 
#endif