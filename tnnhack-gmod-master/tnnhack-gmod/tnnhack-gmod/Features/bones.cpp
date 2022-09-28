#include "bones.h"
#include "../utils.h"
#include "../module.h"
#include "../sperma_cast.h"

CBones* g_pBones = nullptr;

void CBones::GetBonePosition(CBasePlayer* player, int idx, Vector3f* vector)
{
	Matrix3x4f matrix[128];
	player->GetAnimating()->SetupBones(matrix, 128, 256, 0);
	vector->x =  matrix[idx][0][3];
	vector->y =  matrix[idx][1][3];
	vector->z =  matrix[idx][2][3];
}