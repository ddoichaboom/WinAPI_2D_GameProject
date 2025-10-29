#pragma once

// 사이즈에 맞게 변경
#define		WINCX				1384
#define		WINCY				720

#define		PURE				= 0

#define		OBJ_NOEVENT			0
#define		OBJ_DEAD			1

#define		VK_MAX				0xff

#define		GC					1500.f			// 중력 상수 ( Gravitational Constant : 9.81f) 
#define		PI					3.141592f
#define		INV_SQRT2			0.70710678f		// 1.0f / sqrtf(2.0f) : 1 / 루트 2 

#define		BOUNDARY_LEFT		0.f
#define		BOUNDARY_TOP		0.f
#define		BOUNDARY_RIGHT		5457.f
#define		BOUNDARY_BOTTOM		720.f

#define		INFINITY			numeric_limits<float>::infinity()

#define		STAGE01_SIZE		5457.f

#define		TILEX				44

#define		TILECX				124
#define		TILECY				140




extern HWND g_hWnd;

typedef struct tagInfo
{
	// Render Rect용 
	float fX;
	float fY;
	float fCX;				
	float fCY;

	// HitBox Rect용 
	float fHitCX;			
	float fHitCY;			
	float fHitX;
	float fHitY;

}INFO;

typedef struct tagPlayerInfo
{
	float fHp;
	float fScore;
}PLAYERINFO;

template<typename T>
void Safe_Delete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

enum OBJID
{
	PLATFORM,
	PLAYER,
	BUTTON,
	OBSTACLE,
	JELLY,
	GBJELLY,
	COIN,
	ITEM,
	PET,
	OBJ_END
};

enum RENDERID
{
	BACKGROUND, TILE, GAMEOBJECT, EFFECT, UI, RENDER_END
};

enum BUTTON_TYPE 
{ 
	JUMP, SLIDE, BT_END 
};

enum SCENEID 
{ 
	SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_END
};

typedef struct tagFrame
{
	int		iStart;
	int		iEnd;
	int		iMotion;
	DWORD	dwSpeed;
	DWORD	dwTime;
	DWORD	dwStateLock;

	float       frameElapsedSec = 0.0f;  // 현재 프레임 누적 경과시간
	float       frameIntervalSec = 0.2f;  // 한 프레임이 넘어갈 간격(예: 0.2s = 200ms)
	float       stateLockRemainSec = 0.0f;  // 상태 락이 남은 시간(초); 0이면 락 없음
	bool        bLoop = true;  // 루프 여부

}FRAME;

typedef struct tagLinePoint
{
	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY) : fX(_fX), fY(_fY) {}

	float	fX;
	float	fY;

}LINEPOINT;

typedef struct tagLine
{
	tagLine() { ZeroMemory(this, sizeof(tagLine)); }

	tagLine(LINEPOINT& tLeftPoint, LINEPOINT& tRightPoint)
		: tLeft(tLeftPoint), tRight(tRightPoint)
	{
		if (tLeft.fX != tRight.fX)
		{
			tfRadian = (float)atan2((tRight.fY - tLeft.fY), (tRight.fX - tLeft.fX));
			tfTilt = tanf(tfRadian);
			tfYInter = tLeft.fY - tfTilt * (tLeft.fX);
		}
		else
		{
			tfRadian = (float)atan2((tRight.fY - tLeft.fY), (tRight.fX - tLeft.fX));
			tfTilt = INFINITY;
			tfYInter = 0;
		}
	}

	LINEPOINT		tLeft;
	LINEPOINT		tRight;

	float			tfTilt;		// 기울기
	float			tfYInter;	// y 절편 ( x가 0일 때의 y값 )
	float			tfRadian;

}LINE;
