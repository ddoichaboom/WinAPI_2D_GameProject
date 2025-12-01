#pragma once
#include "CScene.h"
class CLobby : public CScene
{
public:
	CLobby();
	virtual ~CLobby();

public:
	void Initialize() override;
	void Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	void	Key_Input();
	void	Character_Interaction();
	void	Move_Frame(float deltaTime);

	FRAME			m_tFrame;
	int				m_iClicked;
	POINT			m_ptStartBtn;
	SIZE			m_szStartBtn;
	RECT			m_rcStartBtn;

	// 캐릭터 관련
	POINT			m_ptCharacter;
	SIZE			m_szCharacter;
	RECT			m_rcCharacter;
	bool			m_bCharacterHovered;
	int				m_iPrevMotion;

};

