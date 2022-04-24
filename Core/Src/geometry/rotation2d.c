#include "rotation2d.h"

void rot2d_fromDegrees(rotation2d_t * this, float deg_in){
	this->m_cos = cosf(deg_in * M_PI/180.0);
	this->m_sin = sinf(deg_in * M_PI/180.0);
}

void rot2d_fromComponents(rotation2d_t * this, float x_in, float y_in){
	float mag = hypotf(x_in, y_in);
	this->m_sin = y_in / mag;
	this->m_cos = x_in / mag;
}

void rot2d_rotateBy(rotation2d_t * this, rotation2d_t * other){
	rotation2d_t tmp;

    tmp.m_cos = this->m_cos * other->m_cos - this->m_sin * other->m_sin;
	tmp.m_sin = this->m_cos * other->m_sin + this->m_sin * other->m_cos;

	rot2d_copy(this, &tmp);
}

float rot2d_toDegrees(rotation2d_t * this){
	return atan2f(this->m_sin, this->m_cos) * 180.0/M_PI;
}

void rot2d_scale(rotation2d_t * this, float scaleFactor){
	float tmp = rot2d_toDegrees(this);
	tmp *= scaleFactor;
	rot2d_fromDegrees(this, tmp);
}

void rot2d_copy(rotation2d_t * dst, rotation2d_t * src){
	dst->m_cos = src->m_cos;
	dst->m_sin = src->m_sin;
}

void rot2d_integrate(rotation2d_t * this, rotation2d_t * other, float delta_t){
	rotation2d_t tmp;
	rot2d_copy(&tmp, other);
	rot2d_scale(&tmp, delta_t);
	rot2d_rotateBy(this, &tmp);
}
