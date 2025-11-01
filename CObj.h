#pragma once
#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual void	Initialize()						PURE;
	virtual int 	Update(float deltaTime = 0.f)		PURE;
	virtual void	Late_Update(float deltaTime = 0.f)	PURE;
	virtual void	Render(HDC hDC)						PURE;
	virtual void	Release()							PURE;

public:
	void		Set_Pos(float fX, float fY)
	{
		m_tInfo.fX = fX;
		m_tInfo.fY = fY;
	}

	void		Set_Hit_Pos(float fX, float fY)
	{
		m_tInfo.fHitX = fX;
		m_tInfo.fHitY = fY;
	}

	void			Set_Speed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	void			Set_Info(const INFO& _tInfo) { m_tInfo = _tInfo; }

	void			Set_Y_Axis_Speed(float fVy) { m_fVy = fVy; }
	float			Get_Y_Axis_Speed() { return m_fVy; }
	void			Set_OnGround(bool bOnGround) { m_bOnGround = bOnGround; }
	void			Set_PrevOnGround(bool bOnGround) { m_bPrevOnGround = bOnGround; }

	bool			Get_OnGround() { return m_bOnGround; }


	void			Set_PosX(float fX) { m_tInfo.fX += fX; }
	void			Set_PosY(float fY) { m_tInfo.fY += fY; }

	const RECT*		Get_Rect() { return &m_tRect; }
	const RECT*		Get_HitRect() { return &m_tHitRect; }
	const INFO*		Get_Info() { return &m_tInfo; }
	void			Set_Dead() { m_bDead = true; }
	bool			Get_Dead() { return m_bDead; }
	const TCHAR*	Get_FrameKey() const { return m_pFrameKey.c_str(); }

	void			Set_FrameKey(const TCHAR* key) { m_pFrameKey = key ? key : TEXT(""); }
	void			Set_Button_Type(BUTTON_TYPE eButtonType) { m_eButtonType = eButtonType; }


	const RENDERID		Get_RenderID() { return m_eRender; }
	void				Update_Rect(OBJID eID);



protected:
	void	Move_Frame(float deltaTime);

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	RECT			m_tHitRect;
	FRAME			m_tFrame;
	RENDERID		m_eRender;
	BUTTON_TYPE		m_eButtonType;



	bool			m_bOnGround;			// 지면 판정 ( 점프 가능 유무, 중력 적용 유무 판단 용도 - 기존 m_bJump랑 비슷함 )
	bool            m_bPrevOnGround;		
	float			m_fVx;					// x축 이동 속도
	float			m_fVy;					// y축 이동 속도
	float			m_fJumpSpeed;			// 점프 초기 속도
	float			m_fSpeed;
	bool			m_bDead;

	std::basic_string<TCHAR> m_pFrameKey;

};

