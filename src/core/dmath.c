#include <math.h>
#include "dmath.h"
#include <stdlib.h>

GLfloat randf() {
    static float precision=10000.0f;
    return (GLfloat)((1.0f/precision)*(rand()%(int)precision));
}
/*-------------------------VEC2----------------------------*/
Vec2 vec2_abs(Vec2 v) {
    Vec2 result;
    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    return result;
}
GLfloat vec2_magnitude(Vec2 v) {
    return (GLfloat) sqrt(
        (v.x * v.x) + (v.y * v.y)
    );
}
GLfloat vec2_distance(Vec2 a,Vec2 b) {
    return vec2_magnitude(vec2_sub(a,b));
} 

Vec2 vec2_create(GLfloat x,GLfloat y) {
    Vec2 result={x,y};
    return result;
}

Vec2 vec2_normalize(Vec2 vector) {
    GLfloat x = vector.x * vector.x;
    GLfloat y = vector.y * vector.y;
    GLfloat all = (GLfloat) sqrt(x + y);

    Vec2 result;
    result.x = vector.x / all;
    result.y = vector.y / all;

    return result;
}

Vec2 vec2_add(Vec2 a,Vec2 b) {
    Vec2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

GLfloat vec2_dot(Vec2 a, Vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

Vec2 vec2_lerp(Vec2 a, Vec2 b,GLfloat d) {
    Vec2 result;
    result.x = lerp(a.x,b.x,d);
    result.y = lerp(a.y,b.y,d);

    return result;
}

Vec2 vec2_scalar_mult(Vec2 v,GLfloat s) {
    Vec2 result;
    result.x = v.x * s;
    result.y = v.y * s;

    return result;
}

Vec2 vec2_invert(Vec2 v) {
    Vec2 result;
    result.x=v.x;
    result.y=v.y;
    return result;
};

/*-------------------------VEC3----------------------------*/
Vec3 vec3_lerp(Vec3 a, Vec3 b,GLfloat d) {
    Vec3 result;
    result.x = lerp(a.x,b.x,d);
    result.y = lerp(a.y,b.y,d);
    result.z = lerp(a.z,b.z,d);

    return result;
}
Vec3 vec3_abs(Vec3 v) {
    Vec3 result;
    result.x = fabsf(v.x);
    result.y = fabsf(v.y);
    result.z = fabsf(v.z);
    return result;
}

GLfloat vec3_magnitude(Vec3 v) {
    return (GLfloat) sqrt(
        (v.x * v.x) + (v.y * v.y) + (v.z * v.z)
    );
}
GLfloat vec3_distance(Vec3 a,Vec3 b) {
    return vec3_magnitude(vec3_sub(a,b));
} 


Vec3 vec3_normalize(Vec3 vector) {
    Vec3 result;
    GLfloat magnitude=vec3_magnitude(vector);
    result.x = vector.x / magnitude;
    result.y = vector.y / magnitude;
    result.z = vector.z / magnitude;
    return result;
}

Vec3 vec3_create(GLfloat x,GLfloat y,GLfloat z) {
    Vec3 result={x,y,z};
    return result;
}

Vec3 vec3_add(Vec3 a,Vec3 b) {
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

Vec3 vec3_cross(Vec3 x,Vec3 y) {
    Vec3 result;
    result.x = x.y * y.z - y.y * x.z;
    result.y = x.z * y.x - y.z * x.x;
    result.z = x.x * y.y - y.x * x.y;

    return result;
}

GLfloat vec3_dot(Vec3 a, Vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


Vec3 vec3_scalar_mult(Vec3 v,GLfloat s) {
    Vec3 result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;

    return result;
}


Vec3 vec3_invert(Vec3 v) {
    Vec3 result;
    result.x=-v.x;
    result.y=-v.y;
    result.z=-v.z;
    return result;
};




/*             MATRICES               */

Mat4 mat4_create(GLfloat num) {
    Mat4 result = {0};
    
    result.data[0][0] = num;
    result.data[1][1] = num;
    result.data[2][2] = num;
    result.data[3][3] = num;
    return result;
}


Mat4 mat4_perspective(
        GLfloat fovy,
        GLfloat aspect,
        GLfloat zNear,
        GLfloat zFar) {
            
    GLfloat tanHalfFovy = (GLfloat) tan(fovy / (GLfloat) 2);

    Mat4 result = mat4_create(0.0f);
    result.data[0][0] = (GLfloat) 1 / (aspect * tanHalfFovy);
    result.data[1][1] = (GLfloat) 1 / (tanHalfFovy);
    result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.data[2][3] = -(GLfloat) 1;
    result.data[3][2] = -((GLfloat) 2 * zFar * zNear) / (zFar - zNear);

    return result;
}

Mat4 mat4_ortho(
        GLfloat left,
        GLfloat right,
        GLfloat bottom,
        GLfloat top,
        GLfloat zNear,
        GLfloat zFar) {
    Mat4 result = mat4_create(1.0f);
    result.data[0][0] = (GLfloat) 2 / (right - left);
    result.data[1][1] = (GLfloat) 2 / (top - bottom);
    result.data[2][2] = -(GLfloat) 2 / (zFar - zNear);
    result.data[3][0] = -(right + left) / (right - left);
    result.data[3][1] = -(top + bottom) / (top - bottom);
    result.data[3][2] = -(zFar + zNear) / (zFar - zNear);

    return result;
}

Mat4 mat4_frustum(
        GLfloat left,
        GLfloat right,
        GLfloat bottom,
        GLfloat top,
        GLfloat zNear,
        GLfloat zFar) {

    Mat4 result = MAT4_EMPTY;
    result.data[0][0] = ((GLfloat) 2 * zNear) / (right - left);
    result.data[1][1] = ((GLfloat) 2 * zNear) / (top - bottom);
    result.data[2][0] = (right + left) / (right - left);
    result.data[2][1] = (top + bottom) / (top - bottom);
    result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.data[2][3] = -(GLfloat) 1;
    result.data[3][2] = -((GLfloat) 2 * zFar * zNear) / (zFar - zNear);

    return result;
}

Mat4 mat4_look_at(
        Vec3 eye,
        Vec3 center,
        Vec3 up) {
    const Vec3 f = vec3_normalize(vec3_sub(center, eye));
    const Vec3 s = vec3_normalize(vec3_cross(f, up));
    const Vec3 u = vec3_cross(s, f);

    Mat4 result = mat4_create(1.0f);
    result.data[0][0] = s.x;
    result.data[1][0] = s.y;
    result.data[2][0] = s.z;
    result.data[0][1] = u.x;
    result.data[1][1] = u.y;
    result.data[2][1] = u.z;
    result.data[0][2] = -f.x;
    result.data[1][2] = -f.y;
    result.data[2][2] = -f.z;
    result.data[3][0] = -vec3_dot(s, eye);
    result.data[3][1] = -vec3_dot(u, eye);
    result.data[3][2] = vec3_dot(f, eye);

    return result;
}


Mat4 mat4_mult(Mat4 m1, Mat4 m2) {
    Mat4 result = 
    {
        {
           {
                m2.data[0][0] * m1.data[0][0] + m2.data[0][1] * m1.data[1][0] + m2.data[0][2] * m1.data[2][0] + m2.data[0][3] * m1.data[3][0] , 
                m2.data[0][0] * m1.data[0][1] + m2.data[0][1] * m1.data[1][1] + m2.data[0][2] * m1.data[2][1] + m2.data[0][3] * m1.data[3][1] , 
                m2.data[0][0] * m1.data[0][2] + m2.data[0][1] * m1.data[1][2] + m2.data[0][2] * m1.data[2][2] + m2.data[0][3] * m1.data[3][2] , 
                m2.data[0][0] * m1.data[0][3] + m2.data[0][1] * m1.data[1][3] + m2.data[0][2] * m1.data[2][3] + m2.data[0][3] * m1.data[3][3] 
            }, 
           {
                m2.data[1][0] * m1.data[0][0] + m2.data[1][1] * m1.data[1][0] + m2.data[1][2] * m1.data[2][0] + m2.data[1][3] * m1.data[3][0] , 
                m2.data[1][0] * m1.data[0][1] + m2.data[1][1] * m1.data[1][1] + m2.data[1][2] * m1.data[2][1] + m2.data[1][3] * m1.data[3][1] , 
                m2.data[1][0] * m1.data[0][2] + m2.data[1][1] * m1.data[1][2] + m2.data[1][2] * m1.data[2][2] + m2.data[1][3] * m1.data[3][2] , 
                m2.data[1][0] * m1.data[0][3] + m2.data[1][1] * m1.data[1][3] + m2.data[1][2] * m1.data[2][3] + m2.data[1][3] * m1.data[3][3] 
            },
           {
            m2.data[2][0] * m1.data[0][0] + m2.data[2][1] * m1.data[1][0] + m2.data[2][2] * m1.data[2][0] + m2.data[2][3] * m1.data[3][0],
            m2.data[2][0] * m1.data[0][1] + m2.data[2][1] * m1.data[1][1] + m2.data[2][2] * m1.data[2][1] + m2.data[2][3] * m1.data[3][1] , 
            m2.data[2][0] * m1.data[0][2] + m2.data[2][1] * m1.data[1][2] + m2.data[2][2] * m1.data[2][2] + m2.data[2][3] * m1.data[3][2] , 
            m2.data[2][0] * m1.data[0][3] + m2.data[2][1] * m1.data[1][3] + m2.data[2][2] * m1.data[2][3] + m2.data[2][3] * m1.data[3][3] 
            }, 

           {
            m2.data[3][0] * m1.data[0][0] + m2.data[3][1] * m1.data[1][0] + m2.data[3][2] * m1.data[2][0] + m2.data[3][3] * m1.data[3][0], 
            m2.data[3][0] * m1.data[0][1] + m2.data[3][1] * m1.data[1][1] + m2.data[3][2] * m1.data[2][1] + m2.data[3][3] * m1.data[3][1] , 
            m2.data[3][0] * m1.data[0][2] + m2.data[3][1] * m1.data[1][2] + m2.data[3][2] * m1.data[2][2] + m2.data[3][3] * m1.data[3][2] , 
            m2.data[3][0] * m1.data[0][3] + m2.data[3][1] * m1.data[1][3] + m2.data[3][2] * m1.data[2][3] + m2.data[3][3] * m1.data[3][3] 
          } 
        }
    };

    return result;
}


const float pi=3.1415;
const float half_pi=3.1415/2;
const float double_pi=3.1415*2;

static GLfloat cos_from_sin(GLfloat _sin, GLfloat angle) {
    GLfloat cos = sqrt(1.0 - _sin * _sin);
    GLfloat a = angle + half_pi;
    GLfloat b = a - (int)(a / double_pi) * double_pi;
    
    if (b < 0.0) 
        b = double_pi + b;
    if (b >= pi)
        return -cos;
    return cos;
}

GLfloat lerp(GLfloat a,GLfloat b,GLfloat d) {
    if(d>1) d=1;
    if(d<0) d=0;
     return (1.0f-d)*a + (d*b);
    //return a+(d*(b-a));
}


void mat4_set_rotation_vec3(Mat4 *matrix,GLfloat angleX, GLfloat angleY, GLfloat angleZ) {
    GLfloat sinX = (GLfloat) sin(angleX);
    GLfloat cosX = (GLfloat) cos_from_sin(sinX, angleX);
    GLfloat sinY = (GLfloat) sin(angleY);
    GLfloat cosY = (GLfloat) cos_from_sin(sinY, angleY);
    GLfloat sinZ = (GLfloat) sin(angleZ);
    GLfloat cosZ = (GLfloat) cos_from_sin(sinZ, angleZ);
    GLfloat m_sinX = -sinX;
    GLfloat m_sinY = -sinY;
    GLfloat m_sinZ = -sinZ;

    // rotateX
    GLfloat nm11 = cosX;
    GLfloat nm12 = sinX;
    GLfloat nm21 = m_sinX;
    GLfloat nm22 = cosX;
    // rotateY
    GLfloat nm00 = cosY;
    GLfloat nm01 = nm21 * m_sinY;
    GLfloat nm02 = nm22 * m_sinY;
    matrix->data[0][2] = sinY;
    matrix->data[1][2] = (nm21 * cosY);
    matrix->data[2][2] = (nm22 * cosY);
    // rotateZ
    matrix->data[0][0] = (nm00 * cosZ);
    matrix->data[1][0] = (nm01 * cosZ + nm11 * sinZ);
    matrix->data[2][0] = (nm02 * cosZ + nm12 * sinZ);
    matrix->data[0][1] = (nm00 * m_sinZ);
    matrix->data[1][1] = (nm01 * m_sinZ + nm11 * cosZ);
    matrix->data[2][1] = (nm02 * m_sinZ + nm12 * cosZ);
}

void mat4_rotate_vec3(Mat4 *matrix,Vec3 angles){
    Mat4 rotation_matrix=mat4_create(1.0);
    mat4_set_rotation_vec3(&rotation_matrix,angles.x,angles.y,angles.z);
    *matrix=mat4_mult(rotation_matrix,*matrix);
}
void mat4_translate_vec3(Mat4 *matrix,Vec3 translation) {
    matrix->data[3][0]+=translation.x;
    matrix->data[3][1]+=translation.y;
    matrix->data[3][2]+=translation.z;
}    
void mat4_scale_vec3(Mat4 *matrix,Vec3 scale) {
    matrix->data[0][0]*=scale.x;
    matrix->data[1][1]*=scale.y;
    matrix->data[2][2]*=scale.z;
}
void mat4_translate_vec2(Mat4 *matrix,Vec2 translation) {
    matrix->data[3][0]+=translation.x;
    matrix->data[3][1]+=translation.y;
} 
void mat4_scale_vec2(Mat4 *matrix,Vec2 scale) {
    matrix->data[0][0]*=scale.x;
    matrix->data[1][1]*=scale.y;
}
Vec3 mat4_get_translation(Mat4 matrix) {
    return vec3_create(matrix.data[3][0],matrix.data[3][1],matrix.data[3][2]);
}

void transform_into_matrix(Transform transform,Mat4 *matrix) {
    *matrix=mat4_create(1.0);
    mat4_scale_vec3(matrix,transform.size);
    mat4_rotate_vec3(matrix,transform.angles);
    mat4_translate_vec3(matrix,transform.position);
}
Mat4 transform_to_matrix(Transform transform) {
    Mat4 matrix=mat4_create(1.0);
    mat4_scale_vec3(&matrix,transform.size);
    mat4_rotate_vec3(&matrix,transform.angles);
    mat4_translate_vec3(&matrix,transform.position);
    return matrix;
}
void transform_into_view_matrix(Transform transform,Mat4 *matrix) {
    *matrix=mat4_create(1.0);
    mat4_scale_vec3(matrix,transform.size);

    mat4_translate_vec3(matrix,vec3_scalar_mult(transform.position,-1));
    mat4_rotate_vec3(matrix,vec3_scalar_mult(transform.angles,-1));
}
Mat4 transform_to_view_matrix(Transform transform) {
    Mat4 matrix=mat4_create(1.0);
    mat4_scale_vec3(&matrix,transform.size);
     /*invert z axis*/ 
    Vec3 adjusted_translation;
    adjusted_translation.x=-transform.position.x;
    adjusted_translation.y=-transform.position.y;
    adjusted_translation.z=transform.position.z;
    
    mat4_translate_vec3(&matrix,vec3_scalar_mult(transform.position,-1));
    mat4_rotate_vec3(&matrix,vec3_scalar_mult(transform.angles,-1));
    return matrix;
}



