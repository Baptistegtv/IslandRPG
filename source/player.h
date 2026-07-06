#pragma once
#include <citro3d.h>
#include "vertex.h"

// Sommets d'un cube unité (1x1x1) centré sur l'origine, couleur unie par face
extern const vertex playerCubeVertices[36];
extern const int playerCubeVertexCount;

typedef struct {
	float x, y, z;      // Position dans le monde
	float facingAngle;  // Angle de rotation autour de l'axe Y (radians)
} Player;

void Player_Init(Player* p);
void Player_Update(Player* p, float moveX, float moveZ, float cameraYaw, float dt);
