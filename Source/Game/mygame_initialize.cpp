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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%
	Sleep(200);
	
	InitBackground();

	ShowInitProgress(50, "Initialize...");
	//
	// �}�l���J���
	//
	// Sleep(1000);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
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

	CTextDraw::ChangeFontLog(pDC, fp, 50, "�L�n������", RGB(255, 255, 255));
	//CTextDraw::Print(pDC, 175, 425,"Start");

	CDDraw::ReleaseBackCDC();
}