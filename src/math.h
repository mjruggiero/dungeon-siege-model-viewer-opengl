// math.h

#ifndef _MATH_H_
#define _MATH_H_

const double PI = 3.14159265358;

typedef struct
{
	float x, y, z;
} vector_t;

void init(vector_t &v, float x, float y, float z);
float length(const vector_t &v);
vector_t normalize(vector_t &v);
vector_t operator+(const vector_t &v1, const vector_t &v2);
vector_t operator-(const vector_t &v1, const vector_t &v2);
vector_t operator*(const float &s, const vector_t &v);
vector_t operator*(const vector_t &v, const float &s);
vector_t operator/(const vector_t &v, const float &s);
bool operator!=(const vector_t &v1, const vector_t &v2);
float dot(const vector_t &v1, const vector_t &v2);
vector_t cross(const vector_t &v1, const vector_t &v2);
vector_t operator*(const float m[16], const vector_t &v);

typedef struct
{
	float x, y, z, w;
} quaternion_t;

float norm(const quaternion_t &q);
float length(const quaternion_t &q);
quaternion_t conjugate(quaternion_t &q);
quaternion_t inverse(quaternion_t &q);

quaternion_t operator+(const quaternion_t &q1, const quaternion_t &q2);
quaternion_t operator-(const quaternion_t &q1, const quaternion_t &q2);
quaternion_t operator*(const quaternion_t &q1, const quaternion_t &q2);
quaternion_t slerp(const quaternion_t &q1, const quaternion_t &q2, float delta);

quaternion_t ConvertToAxisAngle(quaternion_t &q);

#endif // _MATH_H_
