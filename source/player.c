#include "player.h"
#include <math.h>

// Couleurs par face pour bien distinguer l'orientation du perso (placeholder,
// à remplacer par une vraie texture/modèle plus tard)
#define C_FRONT  0.3f, 0.8f, 0.3f   // vert vif (face avant)
#define C_BACK   0.2f, 0.6f, 0.2f
#define C_LEFT   0.3f, 0.7f, 0.9f   // bleu (côtés)
#define C_RIGHT  0.3f, 0.7f, 0.9f
#define C_TOP    0.9f, 0.85f, 0.3f  // jaune (dessus = "tête")
#define C_BOTTOM 0.4f, 0.3f, 0.2f

const vertex playerCubeVertices[36] = {
	// Face avant (+Z)
	{{-0.5f,-0.5f, 0.5f}, {C_FRONT}}, {{ 0.5f,-0.5f, 0.5f}, {C_FRONT}}, {{ 0.5f, 0.5f, 0.5f}, {C_FRONT}},
	{{-0.5f,-0.5f, 0.5f}, {C_FRONT}}, {{ 0.5f, 0.5f, 0.5f}, {C_FRONT}}, {{-0.5f, 0.5f, 0.5f}, {C_FRONT}},
	// Face arrière (-Z)
	{{ 0.5f,-0.5f,-0.5f}, {C_BACK}}, {{-0.5f,-0.5f,-0.5f}, {C_BACK}}, {{-0.5f, 0.5f,-0.5f}, {C_BACK}},
	{{ 0.5f,-0.5f,-0.5f}, {C_BACK}}, {{-0.5f, 0.5f,-0.5f}, {C_BACK}}, {{ 0.5f, 0.5f,-0.5f}, {C_BACK}},
	// Face gauche (-X)
	{{-0.5f,-0.5f,-0.5f}, {C_LEFT}}, {{-0.5f,-0.5f, 0.5f}, {C_LEFT}}, {{-0.5f, 0.5f, 0.5f}, {C_LEFT}},
	{{-0.5f,-0.5f,-0.5f}, {C_LEFT}}, {{-0.5f, 0.5f, 0.5f}, {C_LEFT}}, {{-0.5f, 0.5f,-0.5f}, {C_LEFT}},
	// Face droite (+X)
	{{ 0.5f,-0.5f, 0.5f}, {C_RIGHT}}, {{ 0.5f,-0.5f,-0.5f}, {C_RIGHT}}, {{ 0.5f, 0.5f,-0.5f}, {C_RIGHT}},
	{{ 0.5f,-0.5f, 0.5f}, {C_RIGHT}}, {{ 0.5f, 0.5f,-0.5f}, {C_RIGHT}}, {{ 0.5f, 0.5f, 0.5f}, {C_RIGHT}},
	// Face dessus (+Y)
	{{-0.5f, 0.5f, 0.5f}, {C_TOP}}, {{ 0.5f, 0.5f, 0.5f}, {C_TOP}}, {{ 0.5f, 0.5f,-0.5f}, {C_TOP}},
	{{-0.5f, 0.5f, 0.5f}, {C_TOP}}, {{ 0.5f, 0.5f,-0.5f}, {C_TOP}}, {{-0.5f, 0.5f,-0.5f}, {C_TOP}},
	// Face dessous (-Y)
	{{-0.5f,-0.5f,-0.5f}, {C_BOTTOM}}, {{ 0.5f,-0.5f,-0.5f}, {C_BOTTOM}}, {{ 0.5f,-0.5f, 0.5f}, {C_BOTTOM}},
	{{-0.5f,-0.5f,-0.5f}, {C_BOTTOM}}, {{ 0.5f,-0.5f, 0.5f}, {C_BOTTOM}}, {{-0.5f,-0.5f, 0.5f}, {C_BOTTOM}},
};

const int playerCubeVertexCount = 36;

void Player_Init(Player* p) {
	p->x = 0.0f;
	p->y = 0.5f;   // pose le cube (0.5 = moitié de sa hauteur) sur le sol y=0
	p->z = 0.0f;
	p->facingAngle = 0.0f;
}

void Player_Update(Player* p, float moveX, float moveZ, float cameraYaw, float dt) {
	const float speed = 3.0f; // unités monde par seconde

	if (moveX == 0.0f && moveZ == 0.0f) return;

	// Déplacement relatif à l'orientation de la caméra (standard 3e personne) :
	// on fait tourner le vecteur d'input (moveX, moveZ) de l'angle de la caméra.
	float sinY = sinf(cameraYaw);
	float cosY = cosf(cameraYaw);

	float worldX = moveX * cosY + moveZ * sinY;
	float worldZ = -moveX * sinY + moveZ * cosY;

	p->x += worldX * speed * dt;
	p->z += worldZ * speed * dt;

	// Le perso fait face à sa direction de déplacement
	p->facingAngle = atan2f(worldX, worldZ);
}
