#include "mathematics.h"


// QUATERNIONS
#define QUATERNION_IDENTITY = {0,0,0,1};

typedef struct {
    x,y,z,w;
} Quaternion;

static Quaternion quaternion_identity={0,0,0,1};

Quaternion quat_from_euler(Vec3 angles_degrees);
Vec3 quat_to_euler(Quaternion);

void mat4_set_rotation(Mat4 *matrix,GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat _sin = (GLfloat) sin(angle);
    GLfloat cos = (GLfloat) cos_from_sin(_sin, angle);
    GLfloat C = 1.0f - cos;

    GLfloat xy = x * y, xz = x * z, yz = y * z;

    matrix->data[0][0] = cos + x * x * C;
    matrix->data[0][1] = xy * C - z * _sin;
    matrix->data[0][2] = xz * C + y * _sin;

    matrix->data[1][0] = xy * C + z * _sin;
    matrix->data[1][1] = cos + y * y * C;
    matrix->data[1][2] = yz * C - x * _sin;

    matrix->data[2][0] =xz * C - y * _sin;

    matrix->data[2][1] = yz * C + x * _sin;
    matrix->data[2][2] = cos + z * z * C;
}

void mat4_set_translation(Mat4 *matrix,GLfloat x, GLfloat y, GLfloat z) {
    matrix->data[3][0]=x;
    matrix->data[3][1]=y;
    matrix->data[3][2]=z;
}
void mat4_translation_vec3(Mat4 *matrix,Vec3 translation) {
    matrix->data[3][0]=translation.x;
    matrix->data[3][1]=translation.y;
    matrix->data[3][2]=translation.z;
} 
void mat4_inverse_translation(Mat4 *matrix) {
    matrix->data[3][0]*=-1;
    matrix->data[3][1]*=-1;
    matrix->data[3][2]*=-1;
}

void mat4_translate(Mat4 *matrix,GLfloat x, GLfloat y, GLfloat z) {
    matrix->data[3][0]+=x;
    matrix->data[3][1]+=y;
    matrix->data[3][2]+=z;
}

void mat4_translateX(Mat4 *matrix,GLfloat distance) {
    matrix->data[3][0]+=distance;
}
void mat4_translateY(Mat4 *matrix,GLfloat distance) {
    matrix->data[3][1]+=distance;
}
void mat4_translateZ(Mat4 *matrix,GLfloat distance) {
    matrix->data[3][2]+=distance;
}

void mat4_scaling(Mat4 *matrix,GLfloat x, GLfloat y, GLfloat z) {
    matrix->data[0][0]=x;
    matrix->data[1][1]=y;
    matrix->data[2][2]=z;
}
void mat4_scaling_vec3(Mat4 *matrix,Vec3 scale) {
    mat4_scaling(matrix,scale.x,scale.y,scale.z);
}
void mat4_scale_all(Mat4 *matrix,GLfloat v) {
    matrix->data[0][0]*=v;
    matrix->data[1][1]*=v;
    matrix->data[2][2]*=v;
}
void mat4_scale(Mat4 *matrix,GLfloat x, GLfloat y, GLfloat z) {
    matrix->data[0][0]*=x;
    matrix->data[1][1]*=y;
    matrix->data[2][2]*=z;
}



void mat4_scalar_mult(Mat4 *m, GLfloat s) {
    for(int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            m->data[y][x] *= s;
        }
    }
}