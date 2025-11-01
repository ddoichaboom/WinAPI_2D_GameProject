#pragma once

// 사이즈에 맞게 변경
#define		WINCX				1384
#define		WINCY				720

#define		PURE				= 0

#define		OBJ_NOEVENT			0
#define		OBJ_DEAD			1

#define		VK_MAX				0xff

#define		GC					981.f			// 중력 상수 ( Gravitational Constant : 9.81f) 
#define		PI					3.141592f
#define		INV_SQRT2			0.70710678f		// 1.0f / sqrtf(2.0f) : 1 / 루트 2 

#define		BOUNDARY_BOTTOM		720.f
#define		TILEX				44

#define JUMP_APEX_TIME			0.30f;

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
	float	fHp;
	long	lScore;
	long	lCoin;
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
	UI_OBJECT,
	OBSTACLE,
	JELLY,
	ITEM,
	PET,
	OBJ_END
};



// LOGO 씬 - Intro / Lobby 씬 - MainBGM / 스테이지 씬 - STAGE01, STAGE02 ... 각 BGM / 
// 플레이어 점프, 슬라이드, 오브젝트와의 상호작용 사운드 등 존재 
enum SOUND_CHANNEL { SOUND_BGM, SOUND_EFFECT, SOUND_PLAYER_JUMP, SOUND_PLAYER_SLIDE, SOUND_UI, MAX_CHANNEL };


enum RENDERID
{
	BACKGROUND, TILE, GAMEOBJECT, EFFECT, UI, RENDER_END
};


enum SCENEID 
{ 
	SC_LOGO, SC_LOBBY, SC_EDIT, SC_STAGE, SC_END
};

typedef struct tagFrame
{
	int		iStart;
	int		iEnd;
	int		iMotion;

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

enum BUTTON_TYPE
{
	JUMP, SLIDE, PAUSE, BT_END
};

enum PLATFORM_TYPE
{
	FIRST_PLATFORM,
	SECOND_PLATFORM,
	PT_END
};

enum OBSTACLE_TYPE
{
	SMALL_OBSTACLE01,
	SMALL_OBSTACLE02,
	BIG_OBSTACLE01,
	BIG_OBSTACLE02,
	BIG_OBSTACLE03,
	BIG_OBSTACLE04,
	OT_END
};

enum JELLY_TYPE
{
	JELLY_JELLYBEAN,
	JELLY_BEAR_PINK,
	JELLY_BEAR_YELLOW,
	JELLY_BEAR_BIG,
	JELLY_SILVERCOIN,
	JELLY_SILVERCOIN_BIG,
	JELLY_GOLDCOIN,
	JELLY_GOLDCOIN_BIG,
	JELLY_ALPHABET,
	JT_END
};

enum ITEM_TYPE
{
	ITEM_ENERGY,
	ITEM_ENERGY_BIG,
	ITEM_BOOST,
	ITEM_BIGGEST,
	ITEM_MAGNET,
	ITEM_COIN,
	ITEM_JELLY,
	IT_END
};

enum STAGE_TYPE
{
	STAGE01,
	STAGE02,
	STAGE03,
	ST_END,
};

enum UI_TYPE
{
	BUTTON,
	HP_BAR,
	SCORE_BOARD,
	ICON,
	IMG_TXT,
	IMG_FONT,
	UT_END
};

typedef struct tagImageData
{
	wstring		pFrameKey;
	wstring		ImagePath;
	OBJID		eID;
	INFO		tInfo;

	tagImageData()
		: eID(OBJ_END)
	{
		ZeroMemory(&tInfo, sizeof(INFO));
	}

	tagImageData(float fCX, float fCY)
	{
		tInfo.fCX = fCX;
		tInfo.fCY = fCY;
	}

	tagImageData(OBJID eID, float fCX, float fCY)
	{
		eID = eID;
		tInfo.fCX = fCX;
		tInfo.fCY = fCY;
	}

	tagImageData(OBJID eID, float fCX, float fCY, float fHitCX, float fHitCY )
	{
		eID = eID;
		tInfo.fCX = fCX;
		tInfo.fCY = fCY;
		tInfo.fHitCX = fHitCX;
		tInfo.fHitCY = fHitCY;
	}

}IMAGEDATA;

typedef struct tagJellyInfo
{
	int iScore;
	int iCoin;
}JELLYINFO;

