#pragma once
#include <citro3d.h>
#include "player.h"

typedef struct {
	float yaw;      // angle horizontal autour du joueur (radians)
	float distance; // distance derrière le joueur
	float height;   // hauteur au-dessus du joueur
} Camera;

void Camera_Init(Camera* c);
void Camera_Update(Camera* c, float rotateInput, float dt);
// Remplit viewMtx avec la matrice "look-at" positionnée derrière le joueur
void Camera_ComputeView(const Camera* c, const Player* p, C3D_Mtx* viewMtx);
