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

	void			Set_Speed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	void			Set_PosX(float fX) { m_tInfo.fX += fX; }
	void			Set_PosY(float fY) { m_tInfo.fY += fY; }

	const RECT*		Get_Rect() { return &m_tRect; }
	const INFO*		Get_Info() { return &m_tInfo; }
	void			Set_Dead() { m_bDead = true; }
	bool			Get_Dead() { return m_bDead; }

	void			Set_FrameKey(const TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }
	void			Set_Button_Type(BUTTON_TYPE eButtonType) { m_eButtonType = eButtonType; }


	const RENDERID		Get_RenderID() { return m_eRender; }



protected:
	void	Update_Rect();
	void	Move_Frame(float deltaTime);

protected:
	INFO			m_tInfo;
	RECT			m_tRect;
	RECT			m_tHitRect;
	FRAME			m_tFrame;
	RENDERID		m_eRender;
	BUTTON_TYPE		m_eButtonType;



	bool			m_bOnGround;			// 지면 판정 ( 점프 가능 유무, 중력 적용 유무 판단 용도 - 기존 m_bJump랑 비슷함 )
	float			m_fVx;					// x축 이동 속도
	float			m_fVy;					// y축 이동 속도
	float			m_fJumpSpeed;			// 점프 초기 속도
	float			m_fSpeed;
	bool			m_bDead;

	const	TCHAR* m_pFrameKey;

};

