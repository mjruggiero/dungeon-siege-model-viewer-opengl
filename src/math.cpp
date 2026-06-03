#include "math.h"

#include <math.h>

void init(vector_t& v, float x, float y, float z)
{
	v.x = x;
	v.y = y;
	v.z = z;
}

float length(const vector_t& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vector_t normalize(vector_t& v)
{
	vector_t result;
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	return result;
}

vector_t operator+(const vector_t& v1, const vector_t& v2)
{
	vector_t result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

vector_t operator-(const vector_t& v1, const vector_t& v2)
{
	vector_t result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

vector_t operator*(const float& s, const vector_t& v)
{
	vector_t result;
	result.x = s * v.x;
	result.y = s * v.y;
	result.z = s * v.z;
	return result;
}

vector_t operator*(const vector_t& v, const float& s)
{
	vector_t result;
	result.x = s * v.x;
	result.y = s * v.y;
	result.z = s * v.z;
	return result;
}

vector_t operator/(const vector_t& v, const float& s)
{
	vector_t result;
	result.x = v.x / s;
	result.y = v.y / s;
	result.z = v.z / s;
	return result;
}

float dot(const vector_t& v1, const vector_t& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vector_t cross(const vector_t& v1, const vector_t& v2)
{
	vector_t result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

vector_t operator*(const float m[16], const vector_t& v)
{
	vector_t result;
	result.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12];
	result.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13];
	result.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
	float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
	return (result / w);
}

bool operator!=(const vector_t& v1, const vector_t& v2)
{
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

float norm(const quaternion_t& q)
{
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float length(const quaternion_t& q)
{
	return (float)sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

quaternion_t conjugate(quaternion_t& q)
{
	quaternion_t result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;
	return result;
}

quaternion_t inverse(quaternion_t& q)
{
	quaternion_t result;
	float norm = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	result = conjugate(q);
	result.x /= norm;
	result.y /= norm;
	result.z /= norm;
	result.w /= norm;
	return result;
}

quaternion_t operator+(const quaternion_t& q1, const quaternion_t& q2)
{
	quaternion_t result;
	result.x = q1.x + q2.x;
	result.y = q1.x + q2.y;
	result.z = q1.x + q2.z;
	result.w = q1.x + q2.w;
	return result;
}

quaternion_t operator-(const quaternion_t& q1, const quaternion_t& q2)
{
	quaternion_t result;
	result.x = q1.x - q2.x;
	result.y = q1.x - q2.y;
	result.z = q1.x - q2.z;
	result.w = q1.x - q2.w;
	return result;
}

quaternion_t operator*(const quaternion_t& q1, const quaternion_t& q2)
{
	quaternion_t result;
	// todo
	return result;
}

quaternion_t slerp(const quaternion_t& q1, const quaternion_t& q2, float t)
{
	quaternion_t q1b;
	quaternion_t result;
	double omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	// adjust signs (if necessary)
	if (cosom < 0.0)
	{
		cosom = -cosom;

		q1b.x = -q2.x;
		q1b.y = -q2.y;
		q1b.z = -q2.z;
		q1b.w = -q2.w;

	}
	else {

		q1b.x = q2.x;
		q1b.y = q2.y;
		q1b.z = q2.z;
		q1b.w = q2.w;

	}

	// calculate coefficients

	if ((1.0 - cosom) > 0.000001)
	{
		// standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;

	}
	else {
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation

		scale0 = 1.0 - t;
		scale1 = t;
	}

	// calculate final values
	result.x = scale0 * q1.x + scale1 * q1b.x;
	result.y = scale0 * q1.y + scale1 * q1b.y;
	result.z = scale0 * q1.z + scale1 * q1b.z;
	result.w = scale0 * q1.w + scale1 * q1b.w;

	return result;

	/*
		quaternion_t q1b;
		quaternion_t result;
		double omega, cosom, sinom, scale0, scale1;

		cosom = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

		if((1.0 + cosom) > 0.0001)
		{
			if((1.0 - cosom) > 0.0001)
			{
				omega = acos(cosom);
				sinom = sin(omega);
				scale0 = sin((1.0 - t) * omega) / sinom;
				scale1 = sin(t * omega) / sinom;
			}
			else
			{
				// for small angles
				scale0 = 1.0 - t;
				scale1 = t;
			}
			result.x = scale0 * q1.x + scale1 * q2.x;
			result.y = scale0 * q1.y + scale1 * q2.y;
			result.z = scale0 * q1.z + scale1 * q2.z;
			result.w = scale0 * q1.w + scale1 * q2.w;
		}
		else
		{
			result.x = -q2.y;
			result.y = q2.x;
			result.z = -q2.w;
			result.w = q2.z;

			scale0 = sin((1.0 - t) * PI / 2.0);
			scale1 = sin(t * PI / 2.0);
			result.x = scale0 * q1.x + scale1 * q2.x;
			result.y = scale0 * q1.y + scale1 * q2.y;
			result.z = scale0 * q1.z + scale1 * q2.z;
			result.w = scale0 * q1.w + scale1 * q2.w;
		}

		return result;
	*/
}

quaternion_t ConvertToAxisAngle(quaternion_t& q)
{
	float scale, tw;

	tw = (float)acos(q.w) * 2;
	scale = (float)sin(tw / 2.0);

	quaternion_t result;
	result.x = q.x / scale;
	result.y = q.y / scale;
	result.z = q.z / scale;

	result.w = tw * 180.0 / PI;
	return result;
}
