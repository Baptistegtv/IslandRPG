#include <3ds.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "vertex.h"
#include "player.h"
#include "camera.h"
#include "ground.h"
#include "vshader_shbin.h"

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

static C3D_RenderTarget* targetTop;
static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection, uLoc_modelView;
static C3D_Mtx projection;

static void* groundVbo = NULL;
static void* playerVbo = NULL;

static void sceneInit(void) {
	// Charge et lie le vertex shader
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView  = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");

	// Configuration des attributs de sommet : position (v0) + couleur (v1)
	static C3D_AttrInfo attrInfo;
	AttrInfo_Init(&attrInfo);
	AttrInfo_AddLoader(&attrInfo, 0, GPU_FLOAT, 3); // position
	AttrInfo_AddLoader(&attrInfo, 1, GPU_FLOAT, 3); // couleur

	// Buffers de sommets en VRAM
	groundVbo = linearAlloc(sizeof(vertex) * groundVertexCount);
	memcpy(groundVbo, groundVertices, sizeof(vertex) * groundVertexCount);

	playerVbo = linearAlloc(sizeof(vertex) * playerCubeVertexCount);
	memcpy(playerVbo, playerCubeVertices, sizeof(vertex) * playerCubeVertexCount);

	// Matrice de projection perspective (FOV 70°, aspect écran du haut)
	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(70.0f), C3D_AspectRatioTop, 0.1f, 100.0f, false);
}

static void sceneExit(void) {
	linearFree(groundVbo);
	linearFree(playerVbo);
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

static void drawMesh(void* vbo, int vertexCount, C3D_Mtx* modelView) {
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo, sizeof(vertex), 2, 0x10);

	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView, modelView);

	C3D_DrawArrays(GPU_TRIANGLES, 0, vertexCount);
}

int main(int argc, char* argv[]) {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	targetTop = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(targetTop, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	consoleInit(GFX_BOTTOM, NULL); // console texte sur l'écran du bas pour debug

	sceneInit();

	Player player;
	Player_Init(&player);

	Camera camera;
	Camera_Init(&camera);

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		circlePosition cpad;
		hidCircleRead(&cpad);
		float moveX = (float)cpad.dx / 156.0f; // normalisation approximative [-1,1]
		float moveZ = (float)cpad.dy / 156.0f;
		if (moveX > 1.0f) moveX = 1.0f; if (moveX < -1.0f) moveX = -1.0f;
		if (moveZ > 1.0f) moveZ = 1.0f; if (moveZ < -1.0f) moveZ = -1.0f;

		// D-Pad gauche/droite pour tourner la caméra autour du joueur
		float rotateInput = 0.0f;
		u32 kHeld = hidKeysHeld();
		if (kHeld & KEY_DLEFT)  rotateInput = -1.0f;
		if (kHeld & KEY_DRIGHT) rotateInput = 1.0f;

		float dt = 1.0f / 60.0f;

		Camera_Update(&camera, rotateInput, dt);
		// Le déplacement est relatif à l'orientation actuelle de la caméra (moveZ inversé car "avant" = éloigné caméra)
		Player_Update(&player, moveX, -moveZ, camera.yaw, dt);

		printf("\x1b[1;1HPos: %.1f, %.1f     ", player.x, player.z);

		C3D_Mtx view;
		Camera_ComputeView(&camera, &player, &view);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C3D_RenderTargetClear(targetTop, C3D_CLEAR_ALL, 0x68C8FFFF, 0); // ciel bleu clair
		C3D_FrameDrawOn(targetTop);

		C3D_CullFace(GPU_CULL_BACK_CCW);
		C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);

		// --- Sol (statique, matrice modèle = identité, seule la vue compte) ---
		drawMesh(groundVbo, groundVertexCount, &view);

		// --- Joueur : matrice modèle (position + rotation) combinée à la vue ---
		C3D_Mtx model;
		Mtx_Identity(&model);
		Mtx_Translate(&model, player.x, player.y, player.z, true);
		Mtx_RotateY(&model, player.facingAngle, true);

		C3D_Mtx modelView;
		Mtx_Multiply(&modelView, &view, &model);

		drawMesh(playerVbo, playerCubeVertexCount, &modelView);

		C3D_FrameEnd(0);
	}

	sceneExit();
	C3D_Fini();
	gfxExit();
	return 0;
}
