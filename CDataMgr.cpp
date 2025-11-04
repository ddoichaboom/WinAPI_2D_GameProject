#include "pch.h"
#include "CDataMgr.h"
#include "CBmpMgr.h"

CDataMgr* CDataMgr::m_pInstance = nullptr;

CDataMgr::CDataMgr()
{

}

CDataMgr::~CDataMgr()
{
	Release();
}

void CDataMgr::Initialize()
{
	Load_Object_Data();
	Insert_Image();
}

void CDataMgr::Release()
{
	for (auto& pair : m_mapImageData)
	{
		Safe_Delete(pair.second);
	}
	m_mapImageData.clear();
}

const IMAGEDATA* CDataMgr::Get_ImageData(const wstring& wstrFrameKey) const
{
	auto iter = m_mapImageData.find(wstrFrameKey);

	if (iter == m_mapImageData.end())
		return nullptr;

	return iter->second;
}


void CDataMgr::Load_Object_Data()
{
	IMAGEDATA* pInfo = nullptr;

#pragma region 공통 메타 데이터 

	pInfo = new IMAGEDATA(9095.f, 720.f);
	pInfo->pFrameKey = L"STAGE01";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Stage01.bmp";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(9800.f, 720.f);
	pInfo->pFrameKey = L"STAGE02";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Stage02.bmp";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 236.f, 178.f);
	pInfo->pFrameKey = L"JUMP_BUTTON";
	pInfo->ImagePath = L"./Image/Ui/Button/Jump_button.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 236.f, 178.f);
	pInfo->pFrameKey = L"SLIDE_BUTTON";
	pInfo->ImagePath = L"./Image/Ui/Button/Slide_button.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 824.f, 64.f);
	pInfo->pFrameKey = L"UI_HP_BAR_BG";
	pInfo->ImagePath = L"./Image/Ui/Hp_bar/ui_hp_bar_bg.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 760.f, 26.f);		
	pInfo->pFrameKey = L"UI_HP_BAR";
	pInfo->ImagePath = L"./Image/Ui/Hp_bar/ui_hp_bar.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 310.f, 44.f);
	pInfo->pFrameKey = L"UI_BONUSTIME_BG";
	pInfo->ImagePath = L"./Image/Ui/Bonustime/imgtxt_bonustime_bg.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 310.f, 44.f);
	pInfo->pFrameKey = L"UI_BONUSTIME_EFFECT";
	pInfo->ImagePath = L"./Image/Ui/Bonustime/imgtxt_bonustime_effect.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 213.f, 30.f);
	pInfo->pFrameKey = L"UI_FONT_NUMBER";
	pInfo->ImagePath = L"./Image/Ui/Font/ui_font_number.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 10.f, 30.f);
	pInfo->pFrameKey = L"UI_FONT_COMMA";
	pInfo->ImagePath = L"./Image/Ui/Font/ui_font_comma.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 30.f, 30.f);
	pInfo->pFrameKey = L"UI_ICON_SCORE";
	pInfo->ImagePath = L"./Image/Ui/Icon/ui_icon_score.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(UI_OBJECT, 30.f, 30.f);
	pInfo->pFrameKey = L"UI_ICON_COIN";
	pInfo->ImagePath = L"./Image/Ui/Icon/ui_icon_coin.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(PLAYER, 364.f, 364.f, 120.f, 133.f);
	pInfo->pFrameKey = L"Cookie0001";
	pInfo->ImagePath = L"./Image/Player/Cookie0001.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 60.f, 60.f, 49.f, 49.f);
	pInfo->pFrameKey = L"JELLY_JELLYBEAN";
	pInfo->ImagePath = L"./Image/JELLY/jelly_jellybean.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 55.f, 51.f, 53.f, 51.f);
	pInfo->pFrameKey = L"JELLY_BEAR_PINK";
	pInfo->ImagePath = L"./Image/JELLY/jelly_bear_pink.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 55.f, 51.f, 55.f, 51.f);
	pInfo->pFrameKey = L"JELLY_BEAR_YELLOW";
	pInfo->ImagePath = L"./Image/JELLY/jelly_bear_yellow.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 144.f, 138.f, 144.f, 138.f);
	pInfo->pFrameKey = L"JELLY_BEAR_BIG";
	pInfo->ImagePath = L"./Image/JELLY/jelly_bear_big.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 48.f, 48.f, 48.f, 48.f);
	pInfo->pFrameKey = L"JELLY_COIN1";
	pInfo->ImagePath = L"./Image/JELLY/jelly_coin1.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 160.f, 161.f, 139.f, 138.f);
	pInfo->pFrameKey = L"JELLY_COIN1_BIG";
	pInfo->ImagePath = L"./Image/JELLY/jelly_coin1_big.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 58.f, 59.f, 48.f, 48.f);
	pInfo->pFrameKey = L"JELLY_COIN2";
	pInfo->ImagePath = L"./Image/JELLY/jelly_coin2.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 160.f, 161.f, 154.f, 155.f);
	pInfo->pFrameKey = L"JELLY_COIN2_BIG";
	pInfo->ImagePath = L"./Image/JELLY/jelly_coin2_big.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_A";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_a.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_B";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_b.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_C";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_c.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_D";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_d.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_E";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_e.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_F";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_f.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_G";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_g.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_H";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_h.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_I";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_i.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_J";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_j.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_K";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_k.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_L";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_l.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_M";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_m.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_N";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_n.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_O";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_o.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_P";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_p.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_Q";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_q.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_R";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_r.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_S";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_s.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_T";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_t.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_U";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_u.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_V";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_v.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_W";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_w.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_X";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_x.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_Y";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_y.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_ALPHABET_Z";
	pInfo->ImagePath = L"./Image/JELLY/jelly_alphabet_z.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(JELLY, 43.f, 47.f, 43.f, 47.f);
	pInfo->pFrameKey = L"JELLY_EXCLAMATION_MARK";
	pInfo->ImagePath = L"./Image/JELLY/jelly_exclamation_mark.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 85.f, 85.f);
	pInfo->pFrameKey = L"ITEM_ENERGY";
	pInfo->ImagePath = L"./Image/Item/item_energy.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 144.f, 144.f, 129.f, 134.f);
	pInfo->pFrameKey = L"ITEM_ENERGY_BIG";
	pInfo->ImagePath = L"./Image/Item/item_energy_big.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 88.f, 88.f);
	pInfo->pFrameKey = L"ITEM_BOOST";
	pInfo->ImagePath = L"./Image/Item/item_boost.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 88.f, 89.f);
	pInfo->pFrameKey = L"ITEM_GIANT";
	pInfo->ImagePath = L"./Image/Item/item_giant.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 87.f, 87.f);
	pInfo->pFrameKey = L"ITEM_MAGNET";
	pInfo->ImagePath = L"./Image/Item/item_magnet.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 85.f, 85.f);
	pInfo->pFrameKey = L"ITEM_COIN";
	pInfo->ImagePath = L"./Image/Item/item_coin.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 85.f, 85.f);
	pInfo->pFrameKey = L"ITEM_JELLY";
	pInfo->ImagePath = L"./Image/Item/item_jelly.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 90.f, 90.f, 85.f, 87.f);
	pInfo->pFrameKey = L"ITEM_FEVER";
	pInfo->ImagePath = L"./Image/Item/item_fever.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(ITEM, 132.f, 132.f, 132.f, 132.f);
	pInfo->pFrameKey = L"ITEM_FEVER_BIG";
	pInfo->ImagePath = L"./Image/Item/item_fever_big.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

#pragma endregion

#pragma region STAGE01 메타데이터 
	// (종류) 플랫폼 - 2가지, 장애물 6가지 
	pInfo = new IMAGEDATA(PLATFORM, 124.f, 140.f, 124.f, 140.f);
	pInfo->pFrameKey = L"STAGE01_PLATFORM01";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Platform/Platform01.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(PLATFORM, 124.f, 39.f, 124.f, 39.f);
	pInfo->pFrameKey = L"STAGE01_PLATFORM02";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Platform/Platform02.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 87.f, 118.f, 79.f, 98.f);
	pInfo = new IMAGEDATA(OBSTACLE, 87.f, 118.f, 59.f, 78.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE01";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle01.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 87.f, 118.f, 70.f, 98.f);
	pInfo = new IMAGEDATA(OBSTACLE, 87.f, 118.f, 50.f, 78.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE02";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle02.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 125.f, 227.f, 121.f, 216.f);
	pInfo = new IMAGEDATA(OBSTACLE, 125.f, 227.f, 101.f, 196.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE03";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle03.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 125.f, 227.f, 112.f, 216.f);
	pInfo = new IMAGEDATA(OBSTACLE, 125.f, 227.f, 92.f, 196.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE04";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle04.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 165.f, 493.f, 164.f, 482.f);
	pInfo = new IMAGEDATA(OBSTACLE, 165.f, 493.f, 144.f, 462.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE05";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle05.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	//pInfo = new IMAGEDATA(OBSTACLE, 165.f, 493.f, 115.f, 482.f);
	pInfo = new IMAGEDATA(OBSTACLE, 165.f, 493.f, 95.f, 462.f);
	pInfo->pFrameKey = L"STAGE01_OBSTACLE06";
	pInfo->ImagePath = L"./Image/Stage/Stage01/Obstacle/Obstacle06.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

#pragma endregion

#pragma region STAGE02 - 메타데이터
	// (종류) 플랫폼 - 2가지, 장애물 5가지 
	pInfo = new IMAGEDATA(PLATFORM, 124.f, 140.f, 124.f, 140.f);
	pInfo->pFrameKey = L"STAGE02_PLATFORM01";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Platform/Platform01.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(PLATFORM, 124.f, 39.f, 124.f, 39.f);
	pInfo->pFrameKey = L"STAGE02_PLATFORM02";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Platform/Platform02.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(OBSTACLE, 159.f, 124.f, 118.f, 114.f);		
	pInfo->pFrameKey = L"STAGE02_OBSTACLE01";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Obstacle/Obstacle01.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(OBSTACLE, 158.f, 249.f, 117.f, 239.f);
	pInfo->pFrameKey = L"STAGE02_OBSTACLE02";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Obstacle/Obstacle02.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(OBSTACLE, 158.f, 249.f, 117.f, 239.f);
	pInfo->pFrameKey = L"STAGE02_OBSTACLE03";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Obstacle/Obstacle03.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(OBSTACLE, 184.f, 634.f, 153.f, 634.f);
	pInfo->pFrameKey = L"STAGE02_OBSTACLE04";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Obstacle/Obstacle04.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;

	pInfo = new IMAGEDATA(OBSTACLE, 184.f, 632.f, 150.f, 632.f);
	pInfo->pFrameKey = L"STAGE02_OBSTACLE05";
	pInfo->ImagePath = L"./Image/Stage/Stage02/Obstacle/Obstacle05.png";
	m_mapImageData[pInfo->pFrameKey] = pInfo;
	
#pragma endregion
}

void		CDataMgr::Insert_Image()
{
	for (const auto& pair : m_mapImageData)
	{
		const IMAGEDATA* pData = pair.second;

		if (!pData->ImagePath.empty())
		{
			CBmpMgr::Get_Instance()->Insert_Bmp(pData->ImagePath.c_str(), pData->pFrameKey.c_str());
		}
	}
}





