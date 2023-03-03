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

	CTextDraw::ChangeFontLog(pDC, fp, 36, "�L�n������", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 150, 425,"Press any key to start");

	CDDraw::ReleaseBackCDC();
}