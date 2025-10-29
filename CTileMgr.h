#pragma once

#include "CTile.h"
#include "TcharIO.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

public:
	void						Add_Tile(float fX, float fY);
	void						Picking(POINT pt, bool bState);
	void						Save_Tile(const TCHAR* pFilePath);
	void						Load_Tile();
	const vector<CObj*>&		Get_vecTile()  const { return m_vecTile; }
	

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

