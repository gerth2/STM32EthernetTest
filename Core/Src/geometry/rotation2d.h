#ifndef _ROTATION2D_H
#define _ROTATION2D_H

#include <math.h>

typedef struct rotation2d {
	float m_sin;
	float m_cos;
} rotation2d_t;

void rot2d_fromDegrees(rotation2d_t * this, float deg_in);
void rot2d_fromComponents(rotation2d_t * this, float x_in, float y_in);
void rot2d_rotateBy(rotation2d_t * this, rotation2d_t * other);
float rot2d_toDegrees(rotation2d_t * this);
void rot2d_scale(rotation2d_t * this, float scaleFactor);
void rot2d_copy(rotation2d_t * dst, rotation2d_t * src);
void rot2d_integrate(rotation2d_t * this, rotation2d_t * other, float delta_T);

#endif
