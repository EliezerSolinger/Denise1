
#include "core/mathematics.h"
#include "physics2d.h"
#include "stdlib.h"

Body *body_alloc() {
    Body *this=calloc(1,sizeof(Body));
    this->active=true;
    this->bouncy=0.1f;
    this->static_friction=1;
    this->dynamic_friction=0.3f;
    return this;
}
void body_destroy(Body *this) {
    free(this);
}
static bool body_collide(Body* this,Body* other) {
    if(!this->active || !other->active) return false;
    // Ignore collisions between objects with infinite mass
    if (this->inverse_mass == 0 && other->inverse_mass == 0) return false;
    
    if(this->type==BODY_BOX && other->type==BODY_BOX) {
        if (this->max.x < other->min.x || this->min.x > other->max.x) {
            return false;
        } else if (this->max.y < other->min.y || this->min.y > other->max.y) {
            return false;
        } else {
            return true;
        }
    } else if(this->type==BODY_SPHERE && other->type==BODY_SPHERE) {
        float distance=vec2_distance(this->position,other->position);
        float sizesum=(this->size.x/2.f) +(other->size.x/2.0f);
        //if(distance<sizesum) printf("dist %f, sizesum %f\n",distance,sizesum);
        return distance<sizesum;
    } else {
         // get box closest point to sphere center by clamping
         Body *sphere;
         Body *box;
         if(this->type==BODY_BOX) {
             box=this;sphere=other;
         } else {
             box=other;sphere=this;
         }
         Vec2 box_min=box->min;
         Vec2 box_max=box->max;
         Vec2 sphere_pos=sphere->position;
         float sphere_radius=sphere->size.x/2.0f;

        // this is the same as isPointInsideSphere
        float distance = vec2_distance(
            vec2_create(
                fmaxf(box_min.x, fminf(sphere_pos.x, box_max.x)),
                fmaxf(box_min.y, fminf(sphere_pos.y, box_max.y))
            ),
            sphere_pos
        );

        return distance < sphere_radius;
    }
}

static inline bool body_update_bounds(Body *this) {
    Vec2 half_size=vec2_scalar_mult(this->size,0.5f);
    this->min=vec2_sub(this->position,half_size);
    this->max=vec2_add(this->position,half_size);   
}

static inline void body_integrate_forces(Body *this,PhysicsWorld *world,float delta) {
    if(!this->active || this->inverse_mass==0) return;
    this->velocity=vec2_add(this->velocity,
        vec2_scalar_mult(vec2_add(world->gravity,this->force),this->inverse_mass*delta)
    );
    
    float air_resistance=(vec2_magnitude(this->size)/this->mass)*delta*world->air_density;
    this->velocity=vec2_scalar_mult(this->velocity,1-air_resistance);
}
static void body_integrate_velocity(Body *this,float delta) {
    if(!this->active || this->inverse_mass==0) return;
    this->position.x += this->velocity.x*delta;
    this->position.y += this->velocity.y*delta;
}

Mainfold * mainfold_alloc(Body *a,Body *b) {
    Mainfold *this=calloc(1,sizeof(Mainfold));
    this->a = a;
    this->b = b;
    this->e = fminf(a->bouncy, b->bouncy);
    /* this code below does nothing at all
    this->static_friction = 0;
    this->dynamic_friction = 0;
    this->normal = vec2_create(0, 0);
    this->penetration = 0;*/
}
void mainfold_init(Mainfold* this,Vec2 gravity) {
    
    // TODO is this correct?
    this->static_friction = sqrtf(this->a->static_friction * this->b->static_friction);
    this->dynamic_friction = sqrtf(this->a->dynamic_friction * this->b->dynamic_friction);

    // Figure out whether this is a resting collision, if so do not apply
    // any bouncy
    Vec2 r=vec2_sub(this->b->velocity,this->a->velocity);
    if (vec2_dot(r,r) < vec2_dot(gravity,gravity) + EQUAL_EPSILON) {
        this->e = 0.0f;
    }
}
static bool mainfold_solve_boxbox(Mainfold *this) {
    // Vector from A to B

    Vec2 n=vec2_sub(this->a->position,this->b->position);


    // Calculate half extends along x axis
    float aex = (this->a->max.x - this->a->min.x) / 2.0f,
          bex = (this->b->max.x - this->b->min.x) / 2.0f;

    // Overlap on x axis
    float xoverlap = aex + bex - fabsf(n.x);
    if (xoverlap > 0) { 

        // Calculate half extends along y axis
        float aey = (this->a->max.y - this->a->min.y) / 2.0f,
            bey = (this->b->max.y - this->b->min.y) / 2.0f;

        // Overlap on x axis
        float yoverlap = aey + bey - fabsf(n.y);
        if (yoverlap) {

            // Find out which axis is the axis of least penetration
            if (xoverlap < yoverlap) {

                // Point towards B knowing that n points from A to B
                this->normal.x = n.x < 0 ? 1 : -1;
                this->normal.y = 0;
                this->penetration = xoverlap;
                return true;

            } else {

                // Point towards B knowing that n points from A to B
                this->normal.x = 0;
                this->normal.y = n.y < 0 ? 1 : -1;
                this->penetration = yoverlap;
                return true;

            }

        }

    }

    return false;
}
bool mainfold_solve(Mainfold *this) {    
    if(this->a->mass==0 && this->b->mass==0) return false;
    if(this->a->trigger || this->b->trigger) return false;

    if(this->a->type==BODY_BOX && this->b->type==BODY_BOX) {
        return mainfold_solve_boxbox(this);
    } else if(this->a->type==BODY_SPHERE && this->b->type==BODY_SPHERE) {
        this->normal=vec2_normalize(vec2_sub(this->b->position,this->a->position));
        this->penetration=fabsf(vec2_distance(this->a->position,this->b->position)-((this->a->size.x/2.0f)+(this->b->size.x/2.f)));
        //printf("PET %f\n",this->penetration);
        return true;
    } else {
        // A TEM QUE SER A CAIXA
        // B TEM QUE SER A ESFERA 
         if(this->a->type==BODY_SPHERE) {
            Body *tmp_a=this->a;
             this->a=this->b;this->b=tmp_a;
         }
         // Vector from A to B
        Vec2 n=vec2_sub(this->a->position,this->b->position);

        Vec2 box_min=this->a->min;
        Vec2 box_max=this->a->max;
        Vec2 sphere_pos=this->b->position;

        float sphere_radius=this->b->size.x;

        float x = fmaxf(box_min.x, fminf(sphere_pos.x, box_max.x));
        float y = fmaxf(box_min.y, fminf(sphere_pos.y, box_max.y));


        // this is the same as isPointInsideSphere
        float distance = sqrtf((x - sphere_pos.x) * (x - sphere_pos.x) +
                                (y - sphere_pos.y) * (y - sphere_pos.y));

        float penetration=fabsf(distance - sphere_radius);
        // Calculate half extends along x axis
        float aex = (this->a->max.x - this->a->min.x) / 2.0f,
            bex = (this->b->max.x - this->b->min.x) / 2.0f;

        // Overlap on x axis
        float xoverlap = aex + bex - fabsf(n.x);
        if (xoverlap > 0) { 

            // Calculate half extends along y axis
            float aey = (this->a->max.y - this->a->min.y) / 2.0f,
                bey = (this->b->max.y - this->b->min.y) / 2.0f;

            // Overlap on x axis
            float yoverlap = aey + bey - fabsf(n.y);
            if (yoverlap) {

                // Find out which axis is the axis of least penetration
                if (xoverlap < yoverlap) {

                    // Point towards B knowing that n points from A to B
                    this->normal.x = n.x < 0 ? 1 : -1;
                    this->normal.y = 0;
                    this->penetration = penetration;
                } else {
                    // Point towards B knowing that n points from A to B
                    this->normal.x = 0;
                    this->normal.y = n.y < 0 ? 1 : -1;
                    this->penetration = penetration;
                }
                return true;

            }
            return false;

        }
    }

}

void mainfold_resolve(Mainfold *this) {
    Body  *a = this->a;
    Body  *b = this->b;
    if(a->mass==0 && b->mass==0) return;
    if(a->trigger || b->trigger) return;
    // Relative velocity from a to b
    Vec2 r=vec2_sub(b->velocity,a->velocity);

    float velAlongNormal = vec2_dot(r,this->normal);

    // If the velocities are separating do nothing
    if (velAlongNormal > 0 ) {
        return;
    } else {
        // Correct penetration
        float j = (-(1.0 + this->e) * velAlongNormal)/(a->inverse_mass + b->inverse_mass);

        // Apply the impulse each box gets a impulse based on its mass
        // ratio
        body_apply_impulse(a,vec2_scalar_mult(this->normal,-j));
        body_apply_impulse(b,vec2_scalar_mult(this->normal,j));

        // Apply Friction
        Vec2 t= vec2_sub(r,vec2_scalar_mult(this->normal,velAlongNormal));

        float tl= vec2_magnitude(t);

        if (tl > EQUAL_EPSILON) {
            t.x /= tl;
            t.y /= tl;
        }

        float jt = - vec2_dot(r,t);
        jt /= (a->inverse_mass + b->inverse_mass);

        // Don't apply tiny friction impulses
        if (sqrtf(jt) < EQUAL_EPSILON) {
            return;
        }

        if (sqrtf(jt) < j * this->static_friction) {
            t=vec2_scalar_mult(t,jt);
        } else {
            t=vec2_scalar_mult(t,-j * this->dynamic_friction);
        }
        
        body_apply_impulse(a,vec2_invert(t));
        body_apply_impulse(b,t);
    }

}

bool mainfold_positional_correction(Mainfold *this) {
    Body *a = this->a;
    Body *b = this->b;
    float m = fmaxf(this->penetration - PHYSICS_SLOP, 0.0f) / (a->inverse_mass + b->inverse_mass);

    Vec2 correction=vec2_scalar_mult(this->normal,m*PHYSICS_PERCENT*a->inverse_mass);
    a->position=vec2_sub(a->position,correction);
    b->position=vec2_add(b->position,correction);
}

PhysicsWorld* physics_alloc(size_t capacity) {
    PhysicsWorld *this=calloc(1,sizeof(PhysicsWorld));
    this->iterations = 1;
    this->air_density = 1;
    this->gravity = vec2_create(0, -1);
    //this->contacts = {0};
    this->bodys = calloc(capacity,sizeof(Body*));
    this->bodys_capacity=capacity;
    this->bodys_count=0;
    this->contacts = calloc(PHYSICS_MAX_COLLISIONS,sizeof(Mainfold*));
    this->contacts_count=0;
    return this;
}
static inline void delete_contacts(PhysicsWorld *this) {
    for(int c=0;c<this->contacts_count;c++)
        free(this->contacts[c]);
    this->contacts_count=0;
}
void physics_destroy(PhysicsWorld *this) {
    delete_contacts(this);
    free(this->bodys);
    free(this);
}

static void physics_find_collisions(PhysicsWorld *this) {
    delete_contacts(this);
    for(int i=0;i<this->bodys_count;i++) {
        for(int j=i+1;j<this->bodys_count;j++) {
            Body *a=this->bodys[i];
            Body *b=this->bodys[j];
            if (body_collide(a,b)) {
                Mainfold *c=mainfold_alloc(a,b);
                if (mainfold_solve(c)) {
                    this->contacts[this->contacts_count]=c;
                    this->contacts_count++;
                    if(this->contacts_count>=PHYSICS_MAX_COLLISIONS) {
                        // exit from loop
                        j=this->bodys_count;
                        i=this->bodys_count;
                        break;
                    }
                } else {
                    free(c);
                }
            }
        }
    }


}
 
static inline void physics_integrate_forces(PhysicsWorld *this,float delta) {
    for(int c=0;c<this->bodys_count;c++) 
        body_integrate_forces(this->bodys[c],this,delta);
}
 
static inline void physics_initialize_collisions(PhysicsWorld *this) {
    for(int c=0;c<this->contacts_count;c++) 
        mainfold_init(this->contacts[c],this->gravity);
}
 
static inline void physics_solve_collisions(PhysicsWorld *this) {
    //printf("Count %d\n",this->contacts_count);
    for(int i=0;i<this->iterations;i++)
        for(int c=0;c<this->contacts_count;c++) 
            mainfold_resolve(this->contacts[c]);
}
 
static inline void physics_integrate_velocities(PhysicsWorld *this,float delta) {
    for(int c=0;c<this->bodys_count;c++) {
        Body *b=this->bodys[c];
        body_integrate_velocity(b,delta);
        body_clear_forces(b);
        body_update_bounds(b);
    }
}
 
static inline void physics_correct_positions(PhysicsWorld *this) {
    for(int c=0;c<this->contacts_count;c++) 
        mainfold_positional_correction(this->contacts[c]);
}
 
void physics_update(PhysicsWorld *this,float delta,int fps_simulation) {
    if(this->paused) return;
    float recommended_delta=1.0f/(int) fps_simulation;
    int iterations=1;
    
    if(delta>recommended_delta+(recommended_delta*0.3f)) {
        iterations=(int) (delta/recommended_delta);
    }
    float sim_delta=delta/(float)iterations;

    for(int c=0;c<iterations;c++) {
        physics_find_collisions(this);
        physics_integrate_forces(this,sim_delta);
        physics_initialize_collisions(this);
        physics_solve_collisions(this);
        physics_integrate_velocities(this,sim_delta);
    }
}

void physics_add_body(PhysicsWorld *this,Body *body) {
    if(!this || !body) return;
    this->bodys[this->bodys_count]=body;
    this->bodys_count++;
}
 