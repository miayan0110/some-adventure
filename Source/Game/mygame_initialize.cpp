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
	

	character.ShowBitmap(2.5);
	character.LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardR_2.bmp",
		}, RGB(255, 255, 255));
	character.SetAnimation(300, 0);
	character.SetTopLeft(100, 200);

	etRed.ShowBitmap(2.5);
	etRed.LoadBitmapByString({ "resources/ets/redET_towardU_1.bmp", "resources/ets/redET_towardU_2.bmp" }, RGB(255, 255, 255));
	etRed.SetAnimation(100, 0);
	etRed.SetTopLeft(150, 200);

	etPink.ShowBitmap(2.5);
	etPink.LoadBitmapByString({ "resources/ets/pinkET_towardU_1.bmp", "resources/ets/pinkET_towardU_2.bmp" }, RGB(255, 255, 255));
	etPink.SetAnimation(200, 0);
	etPink.SetTopLeft(200, 200);

	etBlue.ShowBitmap(2.5);
	etBlue.LoadBitmapByString({ "resources/ets/blueET_towardU_1.bmp", "resources/ets/blueET_towardU_2.bmp" }, RGB(255, 255, 255));
	etBlue.SetAnimation(100, 0);
	etBlue.SetTopLeft(250, 200);

	etYellow.ShowBitmap(2.5);
	etYellow.LoadBitmapByString({ "resources/ets/yellowET_towardU_1.bmp", "resources/ets/yellowET_towardU_2.bmp" }, RGB(255, 255, 255));
	etYellow.SetAnimation(200, 0);
	etYellow.SetTopLeft(300, 200);

	start.ShowBitmap(0.2);
	start.LoadBitmapByString({ "resources/start_1.bmp", "resources/start_2.bmp" }, RGB(255, 255, 255));
	start.SetAnimation(200, 0);
	start.SetTopLeft(110, 300);
	
	InitText();
}

void CGameStateInit::InitBackground() {
	character.LoadBitmap({ "resources/pacman/pacman_circle.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(100, 200);
	etRed.LoadBitmap({ "resources/ets/redET_towardU_1.bmp" }, RGB(255, 255, 255));
	etRed.SetTopLeft(150, 200);
	etPink.LoadBitmap({ "resources/ets/pinkET_towardU_1.bmp" }, RGB(255, 255, 255));
	etPink.SetTopLeft(200, 200);
	etBlue.LoadBitmap({ "resources/ets/blueET_towardU_1.bmp" }, RGB(255, 255, 255));
	etBlue.SetTopLeft(250, 200);
	etYellow.LoadBitmap({ "resources/ets/yellowET_towardU_1.bmp" }, RGB(255, 255, 255));
	etYellow.SetTopLeft(300, 200);
	start.LoadBitmap({ "resources/start_1.bmp" }, RGB(255, 255, 255));
	//start.SetTopLeft(175, 425);
	
}

void CGameStateInit::InitText() {
	CDC *pDC = CDDraw::GetBackCDC();
	CFont* fp;

	CTextDraw::ChangeFontLog(pDC, fp, 50, "微軟正黑體", RGB(255, 255, 255));
	//CTextDraw::Print(pDC, 175, 425,"Start");

	CDDraw::ReleaseBackCDC();
}