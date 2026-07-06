#include "camera.h"
#include <math.h>

void Camera_Init(Camera* c) {
	c->yaw = 0.0f;
	c->distance = 3.0f;
	c->height = 1.8f;
}

void Camera_Update(Camera* c, float rotateInput, float dt) {
	const float rotateSpeed = 2.2f; // radians/seconde
	c->yaw += rotateInput * rotateSpeed * dt;
}

void Camera_ComputeView(const Camera* c, const Player* p, C3D_Mtx* viewMtx) {
	// Position de la caméra : derrière le joueur selon c->yaw, surélevée de c->height
	float camX = p->x - sinf(c->yaw) * c->distance;
	float camZ = p->z - cosf(c->yaw) * c->distance;
	float camY = p->y + c->height;

	// Cible : légèrement au-dessus de la position du joueur (hauteur des "yeux")
	float targetX = p->x;
	float targetY = p->y + 0.6f;
	float targetZ = p->z;

	Mtx_LookAt(viewMtx,
		FVec3_New(camX, camY, camZ),
		FVec3_New(targetX, targetY, targetZ),
		FVec3_New(0.0f, 1.0f, 0.0f),
		false);
}
