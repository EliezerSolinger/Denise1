#ifndef DENISE_MATH_H
#define DENISE_MATH_H
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define HALF_PI 1.57079632679489661923
#define DOUBLE_PI 6.28318530718

#define DEG2RAD( a ) ( ( (a) * PI ) / 180.0F )
#define RAD2DEG( a ) ( ( (a) * 180.0f ) / PI )

#define	EQUAL_EPSILON	0.001
#define FLOAT_EQUALS(x,v) (((v - EQUAL_EPSILON) < x) && (x <( v + EQUAL_EPSILON)))

#define lerp(a,b,d) ((1.0f-d)*a + (d*b))
#define fast_lerp(a,b,d) (a + t * (b - a))

#define randf() ((float)rand()/(float)RAND_MAX)


static inline float cos_from_sin(float _sin, float angle) {
    float cos = sqrtf(1.0f - _sin * _sin);
    float a = angle + HALF_PI;
    float b = a - (int)(a / DOUBLE_PI) * DOUBLE_PI;
    if (b < 0) b = DOUBLE_PI + b;
    if (b >= PI)
        return -cos;
    return cos;
}

typedef struct _Color3f{
    float r, g, b;
} Color3f;

#define COLOR3F_WHITE {1.0f, 1.0f ,1.0f}
#define COLOR3F_BLACK {0.0f, 0.0f, 0.0f}
#define COLOR3F_RED  {1.0f, 0.0f, 0.0f}
#define COLOR3F_GREEN {0.0f, 1.0f, 0.0f}
#define COLOR3F_BLUE {0.0f, 0.0f, 1.0f}

static inline Color3f color3f_create(float r,float g,float b) {
	Color3f result={r,g,b};
    return result;
}

typedef struct _Color4f{
    float r, g, b, a;
} Color4f;

#define COLOR4F_WHITE {1.0f, 1.0f ,1.0f, 1.0f}
#define COLOR4F_BLACK {0.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_RED  {1.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_GREEN {0.0f, 1.0f, 0.0f, 1.0f}
#define COLOR4F_BLUE {0.0f, 0.0f, 1.0f, 1.0f}

static inline Color4f color4f_create(float r,float g,float b,float a) {
	Color4f result={r,g,b,a};
    return result;
}
static inline Color4f color4f_hex(uint32_t hex) {
    return color4f_create(
        ((float)((unsigned char)(hex>>16)))/255.0f,
        ((float)((unsigned char)(hex>>8)))/255.0f,
        ((float)((unsigned char)(hex>>0)))/255.0f,
        1
    );
}

static inline Color4f color4f_hexstr(const char *hexstr) {
    uint32_t val = 0;
    while (*hexstr) {
        // get current character then increment
        uint8_t byte = *hexstr++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return color4f_hex(val);
}
///////////////////////////
#define VEC2_ZERO {0.0f, 0.0f}
#define VEC2_FILL_ONE {1.0f, 1.0f}
/* two dimensional vector */
typedef struct _Vec2{
    float x, y;
} Vec2;

static Vec2 vec2_zero={0,0};
static Vec2 vec2_up={0,1};
static Vec2 vec2_down={0,-1};
static Vec2 vec2_left={-1,0};
static Vec2 vec2_right={1,0};

static inline Vec2 vec2_create(float x,float y) {
    Vec2 result={x,y};
    return result;
}

static inline Vec2 vec2_add(Vec2 a,Vec2 b) {
    a.x += b.x; a.y += b.y;
    return a;
}
static inline Vec2 vec2_mult(Vec2 a,Vec2 b) {
    a.x *= b.x; a.y *= b.y;
    return a;
}
static inline Vec2 vec2_div(Vec2 a,Vec2 b) {
    a.x /= b.x; a.y /= b.y;
    return a;
}
static inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
    a.x -=b.x; a.y -=b.y;
    return a;
}

static inline float vec2_dot(Vec2 a, Vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

static inline Vec2 vec2_lerp(Vec2 a, Vec2 b,float d) {
    a.x = lerp(a.x,b.x,d);
    a.y = lerp(a.y,b.y,d);
    return a;
}

static inline Vec2 vec2_scalar_mult(Vec2 v,float s) {
    v.x *=s; v.y *=s;
    return v;
}

static inline Vec2 vec2_invert(Vec2 v) {
    v.x=-v.x;v.y=-v.y;
    return v;
};
static inline Vec2 vec2_abs(Vec2 v) {
    v.x = fabsf(v.x); v.y = fabsf(v.y);
    return v;
}
static inline float vec2_magnitude(Vec2 v) {
    return sqrtf((v.x*v.x)+(v.y*v.y));
}
static inline float vec2_distance(Vec2 a,Vec2 b) {
    return vec2_magnitude(vec2_sub(a,b));
} 
static inline Vec2 vec2_normalize(Vec2 v) {
    float mag =  vec2_magnitude(v);
	v.x/=mag;v.y/=mag;
    return v;
}

typedef struct _Rect{
	Vec2 size; Vec2 position;
} Rect;

#define RECT_NEW {VEC2_FILL_ONE,VEC2_ZERO}
static Rect rect_empty=RECT_NEW;

typedef struct _Circle {
	Vec2 position;
	float radius;
} Circle;

#define CIRCLE_NEW {VEC2_ZERO,1}
static Rect circle_new=RECT_NEW;

/*     3D VECTOR */

/* 3D MATH */

///////////////////////////
#define VEC3_ZERO {0.0f, 0.0f}
#define VEC3_FILL_ONE {1.0f, 1.0f}
/* two dimensional vector */
typedef struct _Vec3{
    float x, y,z;
} Vec3;


static Vec3 vec3_zero={0,0,0};
static Vec3 vec3_up={0,1,0};
static Vec3 vec3_down={0,-1,0};
static Vec3 vec3_left={-1,0,0};
static Vec3 vec3_right={1,0,0};
static Vec3 vec3_forward={0,0,1};
static Vec3 vec3_back={0,0,-1};

static inline Vec3 vec3_create(float x,float y,float z) {
    Vec3 result={x,y,z};
    return result;
}

static inline Vec3 vec3_add(Vec3 a,Vec3 b) {
    a.x += b.x; a.y += b.y; a.z += b.z;
    return a;
}
static inline Vec3 vec3_mult(Vec3 a,Vec3 b) {
    a.x *= b.x; a.y *= b.y; a.z *= b.z;
    return a;
}
static inline Vec3 vec3_div(Vec3 a,Vec3 b) {
    a.x /= b.x; a.y /= b.y; a.z /= b.z;
    return a;
}
static inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
    a.x -=b.x; a.y -=b.y;  a.z -=b.z;
    return a;
}

static inline float vec3_dot(Vec3 a, Vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline Vec3 vec3_lerp(Vec3 a, Vec3 b,float d) {
    a.x = lerp(a.x,b.x,d);
    a.y = lerp(a.y,b.y,d);
    a.z = lerp(a.z,b.z,d);
    return a;
}

static inline Vec3 vec3_scalar_mult(Vec3 v,float s) {
    v.x *=s; v.y *=s; v.z *=s;
    return v;
}

static inline Vec3 vec3_invert(Vec3 v) {
    v.x=-v.x;v.y=-v.y; v.z=-v.z;
    return v;
};
static inline Vec3 vec3_abs(Vec3 v) {
    v.x = fabsf(v.x); v.y = fabsf(v.y); v.z = fabsf(v.z);
    return v;
}
static inline float vec3_magnitude(Vec3 v) {
    return sqrtf(vec3_dot(v,v));
}
static inline float vec3_distance(Vec3 a,Vec3 b) {
    return vec3_magnitude(vec3_sub(a,b));
} 
static inline Vec3 vec3_normalize(Vec3 v) {
    float mag=vec3_magnitude(v);
    if(mag==0.0) return v;
    else return vec3_scalar_mult(v,1.0/mag);
}

typedef struct _Box{
	Vec3 size; Vec3 position;
} Box;

#define BOX_NEW {VEC3_FILL_ONE,VEC3_ZERO}
static Box box_empty=BOX_NEW;

typedef struct _Sphere {
	Vec3 position;
	float radius;
} Sphere;

#define SPHERE_NEW {VEC3_ZERO,1}
static Sphere sphere_new=SPHERE_NEW;

/*     MATRICES     */

#define MAT4_EMPTY {0}
#define MAT4_IDENTITY {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}

/**
 * @brief represent a 4x4 matrix.
 *
 */
typedef struct _Mat4{
    float data[4][4];
} Mat4;


static const Mat4 mat4_identity= {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

static inline Mat4 mat4_create(float num) {
    Mat4 result = {0};
    result.data[0][0] = num;
    result.data[1][1] = num;
    result.data[2][2] = num;
    result.data[3][3] = num;
    return result;
}

static inline Mat4 mat4_mult(Mat4 m1, Mat4 m2) {
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

static inline void mat4_translate_vec2(Mat4 *matrix,Vec2 translation) {
    matrix->data[3][0]+=translation.x;
    matrix->data[3][1]+=translation.y;
} 
static inline void mat4_scale_vec2(Mat4 *matrix,Vec2 scale) {
    matrix->data[0][0]*=scale.x;
    matrix->data[1][1]*=scale.y;
}

static inline void mat4_set_rotation(Mat4 *matrix,float angleX, float angleY, float angleZ) {
    float sinX = sinf(angleX);
    float cosX = cos_from_sin(sinX,angleX);
    float sinY =  sinf(angleY);
    float cosY =  cos_from_sin(sinY,angleY);
    float sinZ =  sinf(angleZ);
    float cosZ =  cos_from_sin(sinZ,angleZ);
    float m_sinX = -sinX;
    float m_sinY = -sinY;
    float m_sinZ = -sinZ;

    // rotateX
    float nm11 = cosX;
    float nm12 = sinX;
    float nm21 = m_sinX;
    float nm22 = cosX;
    // rotateY
    float nm00 = cosY;
    float nm01 = nm21 * m_sinY;
    float nm02 = nm22 * m_sinY;
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

static inline void mat4_rotate2D(Mat4 *matrix,float z){
    Mat4 rotation_matrix=mat4_create(1.0);
    mat4_set_rotation(&rotation_matrix,0,0,z);
    *matrix=mat4_mult(rotation_matrix,*matrix);
}


static inline Vec2 mat4_get_translation_vec2(Mat4 matrix) {
    return vec2_create(matrix.data[3][0],matrix.data[3][1]);
}

static inline Vec2 mat4_get_scale_vec2(Mat4 matrix) {
    return vec2_create(matrix.data[0][0],matrix.data[1][1]);
}

static inline void mat4_translate_vec3(Mat4 *matrix,Vec3 translation) {
    matrix->data[3][0]+=translation.x;
    matrix->data[3][1]+=translation.y;
    matrix->data[3][2]+=translation.z;
} 
static inline void mat4_scale_vec3(Mat4 *matrix,Vec3 scale) {
    matrix->data[0][0]*=scale.x;
    matrix->data[1][1]*=scale.y;
    matrix->data[2][2]*=scale.z;
}

/*   TRANSFORMATIONS  */
typedef struct _Transform2D{
	Vec2 position;
	Vec2 size;
	float rotation;
} Transform2D;

#define TRANSFORM2D_IDENTITY {VEC2_ZERO, VEC2_FILL_ONE,0 }

static Transform2D transform2d_identity= TRANSFORM2D_IDENTITY;

static inline void transform2d_into_matrix(Transform2D transform2d,Mat4 *matrix) {
    *matrix=mat4_create(1.0);
    mat4_scale_vec2(matrix,transform2d.size);
    mat4_rotate2D(matrix,transform2d.rotation);
    mat4_translate_vec2(matrix,transform2d.position);
}
static inline Mat4 transform2d_to_matrix(Transform2D transform2d) {
    Mat4 matrix=mat4_create(1.0);
    mat4_scale_vec2(&matrix,transform2d.size);
    mat4_rotate2D(&matrix,transform2d.rotation);
    mat4_translate_vec2(&matrix,transform2d.position);
    return matrix;
}

/*
 * view matrix is the relationship between world and camera
 * then, is the inverse process of model matrix
 *  */

static inline void transform2d_into_view_matrix(Transform2D transform2d,Mat4 *matrix) {
    *matrix=mat4_create(1.0);
    //mat4_scale_vec3(matrix,transform2d.size);
    
    mat4_translate_vec2(matrix,vec2_scalar_mult(transform2d.position,-1));
    mat4_rotate2D(matrix,transform2d.rotation*-1);
}
static inline Mat4 transform2d_to_view_matrix(Transform2D transform2d) {
    Mat4 matrix=mat4_create(1.0);
    mat4_scale_vec2(&matrix,transform2d.size);
     /*invert z axis*/ 
    Vec2 adjusted_translation;
    adjusted_translation.x=-transform2d.position.x;
    adjusted_translation.y=-transform2d.position.y;
    //adjusted_translation.z=transform2d.position.z;
    
    mat4_translate_vec2(&matrix,vec2_scalar_mult(transform2d.position,-1));
    mat4_rotate2D(&matrix,transform2d.rotation*-1);
    return matrix;
}


/**
 * @brief Creates a matrix for a symetric perspective-view frustum based on
 *        the default handedness.
 * @param fovy Specifies the field of view angle in the y direction.
 *             Expressed in radians.
 * @param aspect Specifies the aspect ratio that determines the field of view
 *               in the x direction. The aspect ratio is the ratio of
 *               x (width) to y (height).
 * @param near Specifies the distance from the viewer to the near clipping
 *             plane (always positive).
 * @param far Specifies the distance from the viewer to the far clipping
 *            plane (always positive).
 */


static inline Mat4 mat4_perspective(
        float fovy,
        float aspect,
        float zNear,
        float zFar) {
            
    float tanHalfFovy = (float) tan(fovy / (float) 2);

    Mat4 result = mat4_create(0.0f);
    result.data[0][0] = (float) 1 / (aspect * tanHalfFovy);
    result.data[1][1] = (float) 1 / (tanHalfFovy);
    result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.data[2][3] = -(float) 1;
    result.data[3][2] = -((float) 2 * zFar * zNear) / (zFar - zNear);

    return result;
}
/**
 * @brief Creates a matrix for an orthographic parallel viewing volume.
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param zNear
 * @param zFar
 */
static inline Mat4 mat4_ortho(
        float left,
        float right,
        float bottom,
        float top,
        float zNear,
        float zFar) {
    Mat4 result = mat4_create(1.0f);
    result.data[0][0] = (float) 2 / (right - left);
    result.data[1][1] = (float) 2 / (top - bottom);
    result.data[2][2] = -(float) 2 / (zFar - zNear);
    result.data[3][0] = -(right + left) / (right - left);
    result.data[3][1] = -(top + bottom) / (top - bottom);
    result.data[3][2] = -(zFar + zNear) / (zFar - zNear);

    return result;
}
/**
 * @brief Creates a frustum matrix.
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */

static inline Mat4 mat4_frustum(
        float left,
        float right,
        float bottom,
        float top,
        float zNear,
        float zFar) {

    Mat4 result = MAT4_EMPTY;
    result.data[0][0] = ((float) 2 * zNear) / (right - left);
    result.data[1][1] = ((float) 2 * zNear) / (top - bottom);
    result.data[2][0] = (right + left) / (right - left);
    result.data[2][1] = (top + bottom) / (top - bottom);
    result.data[2][2] = -(zFar + zNear) / (zFar - zNear);
    result.data[2][3] = -(float) 1;
    result.data[3][2] = -((float) 2 * zFar * zNear) / (zFar - zNear);

    return result;
}


/**
 * @brief Build a look at view matrix based on the default handedness.
 * @param eye Position of the camera
 * @param center Position where the camera is looking at
 * @param up Normalized up vector, how the camera is oriented.
 *           Typically (0, 0, 1)
 */
Mat4 mat4_look_at(
        Vec3 eye,
        Vec3 center,
        Vec3 up);



/* 3D Transformations  */ 
typedef struct _Quaternion{
	float x,y,z,w;
} Quaternion;

#define QUATERNION_IDENTITY  {0.0f, 0.0f, 0.0f, 1.0f}
static Quaternion quat_identity= QUATERNION_IDENTITY;

typedef struct _Transform3D{
	Vec3 position;
	Vec3 size;
	Vec3 rotation;
} Transform3D;

#ifdef DMATH_QUATERNIONS
    /*!
    * @brief creates NEW quaternion with axis vector
    *
    * @param[in]   angle angle (radians)
    * @param[in]   axis  axis
    */
    static inline Quaternion quat_create(float angle, Vec3 axis) {
    Quaternion q=quat_identity;
    Vec3 k=vec3_normalize(axis);
    float a, c, s;

    a = angle * 0.5f;
    c = cosf(a);
    s = sinf(a);

    q.x = s * k.x;
    q.y = s * k.y;
    q.z = s * k.z;
    q.w = c;
    return q;
    }
    /*!
    * @brief dot product of two quaternion
    *
    */
    static inline float quat_dot(Quaternion a,Quaternion b) {
    return (a.x*b.x)+(a.y*b.y)+(a.z*b.z)+(a.w*b.w);
    }
    /*!
    * @brief returns magnitude of quaternion
    *
    */
    static inline float quat_magnitude(Quaternion q) {
    return sqrtf(quat_dot(q,q));
    }


    /*!
    * @brief conjugate of quaternion
    *
    * @param[in]   q     quaternion
    */
    static inline Quaternion quat_conjugate(Quaternion q) {
    q.x=-q.x;q.y=-q.y;q.z=-q.z;
    return q;
    }

    /*!
    * @brief conjugate of quaternion
    *
    * @param[in]   q     quaternion
    */
    static inline Quaternion quat_negate(Quaternion q) {
    q.x=-q.x;q.y=-q.y;q.z=-q.z;q.w=-q.w;
    return q;
    }
    static inline Quaternion quat_componentwise_scale(Quaternion q,float n) {
    q.x*=n; q.y*=n; q.z*=n; q.w*=n;
    return q;
    }
    static inline Quaternion quat_componentwise_add(Quaternion a,Quaternion b) {
    a.x+=a.x; a.y+=a.y;a.z+=a.z;a.w+=a.w;
    return a;
    }
    /*!
    * @brief inverse of non-zero quaternion
    *
    * @param[in]   q    quaternion
    */
    static inline Quaternion quat_inverse(Quaternion q) {
    float div=1.0f / quat_dot(q,q);
    q=quat_conjugate(q);
    q.x*=div;q.y*=div;q.z*=div;
    return q;
    }


    /*!
    * @brief returns imaginary part of quaternion
    *
    * @param[in]   q    quaternion
    */
    static inline Vec3 quat_imaginary(Quaternion q) {
    return vec3_create(q.x,q.y,q.z);
    }

    /*!
    * @brief returns normalized imaginary part of quaternion
    *
    * @param[in]   q    quaternion
    */
    static inline Vec3 quat_imaginary_norm(Quaternion q) {
    return vec3_normalize(quat_imaginary(q));
    }


    /*!
    * @brief returns real part of quaternion
    *
    * @param[in]   q    quaternion
    */
    static inline float quat_real(Quaternion q) {
    return q.w;
    }

    /*!
    * @brief returns angle of quaternion
    *
    * @param[in]   q    quaternion
    */

    static inline float quat_angle(Quaternion q) {
    /*
    sin(theta / 2) = length(x*x + y*y + z*z)
    cos(theta / 2) = w
    theta          = 2 * atan(sin(theta / 2) / cos(theta / 2))
    */
    return 2.0f * atan2f(quat_imaginary_length(q), quat_real(q));
    }


    /*!
    * @brief axis of quaternion
    *
    * @param[in]   q    quaternion
    */
    static inline Vec3 quat_axis(Quaternion q) {
    return quat_imaginary_norm(q);
    }


    /*!
    * @brief multiplies two quaternion and stores result in dest
    *        this is also called Hamilton Product
    *
    * According to WikiPedia:
    * The product of two rotation quaternions [clarification needed] will be
    * equivalent to the rotation q followed by the rotation p
    *
    * @param[in]   p     quaternion 1
    * @param[in]   q     quaternion 2
    * @param[out]  dest  result quaternion
    */
    static inline Quaternion quat_mult(Quaternion p,Quaternion q) {
    /*
        + (a1 b2 + b1 a2 + c1 d2 − d1 c2)i
        + (a1 c2 − b1 d2 + c1 a2 + d1 b2)j
        + (a1 d2 + b1 c2 − c1 b2 + d1 a2)k
        a1 a2 − b1 b2 − c1 c2 − d1 d2
    */
    Quaternion result=quat_identity;
    result.x = p.w * p.x + p.x * p.w + p.y * p.z - p.z * p.y;
    result.y = p.w * p.y - p.x * p.z + p.y * p.w + p.z * p.x;
    result.z = p.w * p.z + p.x * p.y - p.y * p.x + p.z * p.w;
    result.w = p.w * p.w - p.x * p.x - p.y * p.y - p.z * p.z;
        return result;
    }


    /*!
    * @brief convert quaternion to mat4
    *
    * @param[in]   q     quaternion
    */
    static inline Mat4 quat_mat4(Quaternion q) {
        Mat4 dest=mat4_identity;
    float w, x, y, z,
            xx, yy, zz,
            xy, yz, xz,
            wx, wy, wz, norm, s;

    norm = quat_magnitude(q);
    s    = norm > 0.0f ? 2.0f / norm : 0.0f;

    x = q.x;y = q.y; z = q.z;  w = q.w;

    xx = s * x * x;   xy = s * x * y;   wx = s * w * x;
    yy = s * y * y;   yz = s * y * z;   wy = s * w * y;
    zz = s * z * z;   xz = s * x * z;   wz = s * w * z;

    dest.data[0][0] = 1.0f - yy - zz;
    dest.data[1][1] = 1.0f - xx - zz;
    dest.data[2][2] = 1.0f - xx - yy;

    dest.data[0][1] = xy + wz;
    dest.data[1][2] = yz + wx;
    dest.data[2][0] = xz + wy;

    dest.data[1][0] = xy - wz;
    dest.data[2][1] = yz - wx;
    dest.data[0][2] = xz - wy;

    dest.data[0][3] = 0.0f;
    dest.data[1][3] = 0.0f;
    dest.data[2][3] = 0.0f;
    dest.data[3][0] = 0.0f;
    dest.data[3][1] = 0.0f;
    dest.data[3][2] = 0.0f;
    dest.data[3][3] = 1.0f;
    return dest;
    }

    /*!
    * @brief convert quaternion to mat4 (transposed)
    *
    * @param[in]   q     quaternion
    * @param[out]  dest  result matrix as transposed
    */
    static inline Mat4 quat_mat4t(Quaternion q) {
    Mat4 dest=mat4_identity;
    float w, x, y, z,
            xx, yy, zz,
            xy, yz, xz,
            wx, wy, wz, norm, s;

    norm = quat_magnitude(q);
    s    = norm > 0.0f ? 2.0f / norm : 0.0f;

    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;

    xx = s * x * x;   xy = s * x * y;   wx = s * w * x;
    yy = s * y * y;   yz = s * y * z;   wy = s * w * y;
    zz = s * z * z;   xz = s * x * z;   wz = s * w * z;

    dest.data[0][0] = 1.0f - yy - zz;
    dest.data[1][1] = 1.0f - xx - zz;
    dest.data[2][2] = 1.0f - xx - yy;

    dest.data[1][0] = xy + wz;
    dest.data[2][1] = yz + wx;
    dest.data[0][2] = xz + wy;

    dest.data[0][1] = xy - wz;
    dest.data[1][2] = yz - wx;
    dest.data[2][0] = xz - wy;

    dest.data[0][3] = 0.0f;
    dest.data[1][3] = 0.0f;
    dest.data[2][3] = 0.0f;
    dest.data[3][0] = 0.0f;
    dest.data[3][1] = 0.0f;
    dest.data[3][2] = 0.0f;
    dest.data[3][3] = 1.0f;
    return dest;
    }

    /*!
    * @brief interpolates between two quaternions
    *        using linear interpolation (LERP)
    *
    * @param[in]   from  from
    * @param[in]   to    to
    * @param[in]   t     interpolant (amount)
    */
    static inline Quaternion quat_lerp(Quaternion from, Quaternion to, float t) {
    Quaternion result=quat_identity;
    result.x=lerp(from.x,to.x,t);
    result.y=lerp(from.y,to.y,t);
    result.z=lerp(from.z,to.z,t);
    result.w=lerp(from.w,to.w,t);
    return result;
    }


    /*!
    * @brief interpolates between two quaternions
    *        taking the shortest rotation path using
    *        normalized linear interpolation (NLERP)
    *
    * @param[in]   from  from
    * @param[in]   to    to
    * @param[in]   t     interpolant (amount)
    */
    static inline Quaternion quat_nlerp(Quaternion from, Quaternion to, float t) {
    Quaternion result=quat_identity;
    if(quat_dot(from, to) < 0) {
        to=quat_negate(to);
    }
    result=quat_lerp(from, to, t);
    result=quat_normalize(result);
    return result;
    }


    /*!
    * @brief interpolates between two quaternions
    *        using spherical linear interpolation (SLERP)
    *
    * @param[in]   from  from
    * @param[in]   to    to
    * @param[in]   t     amout
    * @param[out]  dest  result quaternion
    */
    static inline Quaternion quat_slerp(Quaternion from, Quaternion to, float t) {
    Quaternion q1,q2;
    
    float cosTheta, sinTheta, angle;

    cosTheta = quat_dot(from, to);
        q1=from;

    if (fabsf(cosTheta) >= 1.0f) {
        return q1;
    }

    if (cosTheta < 0.0f) {
        q1=quat_negate(q1);
        cosTheta = -cosTheta;
    }

    sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

    /* LERP to avoid zero division */
    if (fabsf(sinTheta) < 0.001f) {
        return quat_lerp(from, to, t);
    }

    /* SLERP */
    angle = acosf(cosTheta);
    q1=quat_componentwise_scale(q1, sinf((1.0f - t) * angle));
    q2=quat_componentwise_scale(to, sinf(t * angle));

    q1=quat_componentwise_add(q1, q2);
    return quat_componentwise_scale(q1, 1.0f / sinTheta);
    }
    /*
    * @brief creates view matrix using quaternion as camera orientation
    *
    * @param[in]   eye   eye
    * @param[in]   ori   orientation in world space as quaternion
    
    static inline Mat4 quat_look(Vec3 eye, Quaternion ori) {
    
    // orientation 
    Mat4 result=quat_mat4t(ori);

    // translate 
    Vec3 line3=vec3_create(result.data[3][0],result.data[3][1],result.data[3][2])
    glm_mat4_mulv3(result, eye, 1.0f, dest[3]);
    glm_vec3_negate(result[3]);
    return result;
    }
    */

    #define TRANSFORM3D_IDENTITY {VEC2_ZERO, VEC2_FILL_ONE,0 }

    static Transform3D transform3d_identity= TRANSFORM3D_IDENTITY;

    static inline void transform3d_into_matrix(Transform3D transform3d,Mat4 *matrix) {
        *matrix=mat4_create(1.0);
        mat4_scale_vec3(matrix,transform3d.size);
        mat4_rotate_vec3(matrix,transform3d.rotation);
        mat4_translate_vec3(matrix,transform3d.position);
    }
    static inline Mat4 transform3d_to_matrix(Transform3D transform3d) {
        Mat4 matrix=mat4_create(1.0);
        mat4_scale_vec3(&matrix,transform3d.size);
        mat4_rotate_vec3(&matrix,transform3d.rotation);
        mat4_translate_vec3(&matrix,transform3d.position);
        return matrix;
    }

    /*
    * view matrix is the relationship between world and camera
    * then, is the inverse process of model matrix
    *  */

    static inline void transform3d_into_view_matrix(Transform3D transform3d,Mat4 *matrix) {
        *matrix=mat4_create(1.0);
        //mat4_scale_vec3(matrix,transform3d.size);
        
        mat4_translate_vec3(matrix,vec3_scalar_mult(transform3d.position,-1));
    /* mat4_rotate_vec3(&matrix,vec3_invert(transform3d.rotation));*/
    }
    static inline Mat4 transform3d_to_view_matrix(Transform3D transform3d) {
        Mat4 matrix=mat4_create(1.0);
        mat4_scale_vec3(&matrix,transform3d.size);
        /*invert z axis*/ 
        Vec3 adjusted_translation;
        adjusted_translation.x=-transform3d.position.x;
        adjusted_translation.y=-transform3d.position.y;
        //adjusted_translation.z=transform3d.position.z;
        
        mat4_translate_vec3(&matrix,vec3_scalar_mult(transform3d.position,-1));
    /* mat4_rotate_vec3(&matrix,transform3d.rotation*-1);*/
        return matrix;
    }



    typedef enum {
        Xleft,
        Xcenter,
        Xright
    } XCorner;



    typedef enum {
        Ytop,
        Ycenter,
        Ybottom
    } YCorner;

#endif
#endif
