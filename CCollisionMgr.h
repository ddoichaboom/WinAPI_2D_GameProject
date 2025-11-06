#pragma once

#include "CObj.h"

class CCollisionMgr
{
public:
	static void	Collision_Rect(list<CObj*> Dst, list<CObj*> Src);

	static void	Collision_RectEx(list<CObj*> Dst, list<CObj*> Src);
	static bool Check_Rect(CObj* pDst, CObj* pSrc, float* pWidth, float* pHeight);

	static void	Collision_Circle(list<CObj*> Dst, list<CObj*> Src);
	static bool Check_Circle(CObj* pDst, CObj* pSrc);

	static void Collision_Collect(list<CObj*>& Dst, list<CObj*>& Src);
	static bool Collision_Obstacle(list<CObj*>& Dst, list<CObj*>& Src);

	static void Collision_Item(list<CObj*>& Dst, list<CObj*>& Src);
	
};

