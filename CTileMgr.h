#pragma once

#include "CTile.h"
#include "TcharIO.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void	Initialize(const TCHAR* pStageFrameKey);
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
	void	Delete_Tile(POINT pt);		// 타일 삭제 함수 

public:
	void						Add_Tile(float fX, float fY, const TCHAR* pFrameKey);
	void						Picking(POINT pt, bool bState);
	void						Save_Tile(const TCHAR* pFilePath);
	void						Load_Tile();
	const vector<CObj*>&		Get_vecTile()  const { return m_vecTile; }
	void						Add_Tile_By_Obj(CTile* pTile);
	

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CTileMgr* m_pInstance;
	vector<CObj*>	m_vecTile;


};

