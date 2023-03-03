#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	Sleep(200);
	
	InitBackground();

	ShowInitProgress(50, "Initialize...");
	//
	// 開始載入資料
	//
	// Sleep(1000);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	
}

void CGameStateInit::OnShow()
{
	character.ShowBitmap(5);
	etRed.ShowBitmap(5);
	etPink.ShowBitmap(5);
	etBlue.ShowBitmap(5);
	etYellow.ShowBitmap(5);
	InitText();
}

void CGameStateInit::InitBackground() {
	character.LoadBitmap({ "resources/pacman/pacman_circle.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(400, 175);
	etRed.LoadBitmapA({ "resources/ets/redET_lookup_1.bmp" }, RGB(255, 255, 255));
	etRed.SetTopLeft(500, 175);
	etPink.LoadBitmapA({ "resources/ets/pinkET_lookup_1.bmp" }, RGB(255, 255, 255));
	etPink.SetTopLeft(600, 175);
	etBlue.LoadBitmapA({ "resources/ets/blueET_lookup_1.bmp" }, RGB(255, 255, 255));
	etBlue.SetTopLeft(700, 175);
	etYellow.LoadBitmapA({ "resources/ets/yellowET_lookup_1.bmp" }, RGB(255, 255, 255));
	etYellow.SetTopLeft(800, 175);
}

void CGameStateInit::InitText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont* fp;

	CTextDraw::ChangeFontLog(pDC, fp, 36, "微軟正黑體", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 150, 425,"Press any key to start");

	CDDraw::ReleaseBackCDC();
}