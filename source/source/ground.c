#include "ground.h"

// Herbe vert vif, style anime coloré
#define GRASS 0.35f, 0.85f, 0.25f

#define SIZE 20.0f // demi-taille de l'île (40x40 unités au total)

const vertex groundVertices[6] = {
	{{-SIZE, 0.0f, -SIZE}, {GRASS}},
	{{ SIZE, 0.0f, -SIZE}, {GRASS}},
	{{ SIZE, 0.0f,  SIZE}, {GRASS}},

	{{-SIZE, 0.0f, -SIZE}, {GRASS}},
	{{ SIZE, 0.0f,  SIZE}, {GRASS}},
	{{-SIZE, 0.0f,  SIZE}, {GRASS}},
};

const int groundVertexCount = 6;
