#ifndef GROTA_MATH_H
#define GROTA_MATH_H
 
#include <stdlib.h>
#include <glad/glad.h>
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

#define RANDF_PRECISION 10000
#define randf() ((1.0f/RANDF_PRECISION)*(rand()%RANDF_PRECISION))


static inline GLfloat cos_from_sin(GLfloat _sin, GLfloat angle) {
    GLfloat cos = sqrtf(1.0f - _sin * _sin);
    GLfloat a = angle + HALF_PI;
    GLfloat b = a - (int)(a / DOUBLE_PI) * DOUBLE_PI;
    if (b < 0) b = DOUBLE_PI + b;
    if (b >= PI)
        return -cos;
    return cos;
}

typedef struct _Color3f{
    GLfloat r, g, b;
} Color3f;


#define COLOR3F_WHITE {1.0f, 1.0f ,1.0f}
#define COLOR3F_BLACK {0.0f, 0.0f, 0.0f}
#define COLOR3F_RED  {1.0f, 0.0f, 0.0f}
#define COLOR3F_GREEN {0.0f, 1.0f, 0.0f}
#define COLOR3F_BLUE {0.0f, 0.0f, 1.0f}

static inline Color3f color3f_create(GLfloat r,GLfloat g,GLfloat b) {
	Color3f result={r,g,b};
    return result;
}


typedef struct _Color4f{
    GLfloat r, g, b, a;
} Color4f;



#define COLOR4F_WHITE {1.0f, 1.0f ,1.0f, 1.0f}
#define COLOR4F_BLACK {0.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_RED  {1.0f, 0.0f, 0.0f, 1.0f}
#define COLOR4F_GREEN {0.0f, 1.0f, 0.0f, 1.0f}
#define COLOR4F_BLUE {0.0f, 0.0f, 1.0f, 1.0f}

static inline Color4f color4f_create(GLfloat r,GLfloat g,GLfloat b,GLfloat a) {
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
    GLfloat x, y;
} Vec2;


static inline Vec2 vec2_create(GLfloat x,GLfloat y) {
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

static inline GLfloat vec2_dot(Vec2 a, Vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

static inline Vec2 vec2_lerp(Vec2 a, Vec2 b,GLfloat d) {
    a.x = lerp(a.x,b.x,d);
    a.y = lerp(a.y,b.y,d);
    return a;
}

static inline Vec2 vec2_scalar_mult(Vec2 v,GLfloat s) {
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
static inline GLfloat vec2_magnitude(Vec2 v) {
    return sqrtf((v.x*v.x)+(v.y*v.y));
}
static inline GLfloat vec2_distance(Vec2 a,Vec2 b) {
    return vec2_magnitude(vec2_sub(a,b));
} 
static inline Vec2 vec2_normalize(Vec2 v) {
    GLfloat mag =  vec2_magnitude(v);
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

/*     MATRICES     */

#define MAT4_EMPTY {0}
#define MAT4_IDENTITY {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}

/**
 * @brief represent a 4x4 matrix.
 *
 */

typedef struct _Mat4{
    GLfloat data[4][4];
} Mat4;



static const Mat4 mat4_identity= {
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

static inline Mat4 mat4_create(GLfloat num) {
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

static inline void mat4_set_rotation(Mat4 *matrix,GLfloat angleX, GLfloat angleY, GLfloat angleZ) {
    GLfloat sinX = sinf(angleX);
    GLfloat cosX = cos_from_sin(sinX,angleX);
    GLfloat sinY =  sinf(angleY);
    GLfloat cosY =  cos_from_sin(sinY,angleY);
    GLfloat sinZ =  sinf(angleZ);
    GLfloat cosZ =  cos_from_sin(sinZ,angleZ);
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

static inline void mat4_rotate(Mat4 *matrix,GLfloat z){
    Mat4 rotation_matrix=mat4_create(1.0);
    mat4_set_rotation(&rotation_matrix,0,0,z);
    *matrix=mat4_mult(rotation_matrix,*matrix);
}

static inline Vec2 mat4_get_translation(Mat4 matrix) {
    return vec2_create(matrix.data[3][0],matrix.data[3][1]);
}

static inline Vec2 mat4_get_scale(Mat4 matrix) {
    return vec2_create(matrix.data[0][0],matrix.data[1][1]);
}
/*   TRANSFORMATIONS  */
typedef struct _Transform2D{
	Vec2 position;
	Vec2 size;
	GLfloat rotation;
} Transform2D;

#define TRANSFORM2D_IDENTITY {VEC2_ZERO, VEC2_FILL_ONE,0 }

static Transform2D transform2d_identity= TRANSFORM2D_IDENTITY;

static inline void transform2d_into_matrix(Transform2D transform2d,Mat4 *matrix) {
    *matrix=mat4_create(1.0);
    mat4_scale_vec2(matrix,transform2d.size);
    mat4_rotate(matrix,transform2d.rotation);
    mat4_translate_vec2(matrix,transform2d.position);
}
static inline Mat4 transform2d_to_matrix(Transform2D transform2d) {
    Mat4 matrix=mat4_create(1.0);
    mat4_scale_vec2(&matrix,transform2d.size);
    mat4_rotate(&matrix,transform2d.rotation);
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
    mat4_rotate(matrix,transform2d.rotation*-1);
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
    mat4_rotate(&matrix,transform2d.rotation*-1);
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
