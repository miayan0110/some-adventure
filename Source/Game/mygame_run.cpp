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
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	if (character.GetSelectShowBitmap() == 0 || character.GetSelectShowBitmap() == 1) {
		character.SetTopLeft(character.Left() + 5, character.Top());
	}
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	background.LoadBitmapByString({
		"resources/initialize_background.bmp"
	}, RGB(255, 255, 255));
	background.SetTopLeft(20, 75);

	character[0].LoadBitmapByString({
		"resources/pacman_towardR_1.bmp",
		"resources/pacman_towardR_2.bmp",
		"resources/pacman_towardL_1.bmp",
	}, RGB(255, 255, 255));
	character[0].SetTopLeft(120, 150);
	character[0].SetAnimation(50, 0);

	character[1].LoadBitmapByString({
		"resources/pacman_towardL_1.bmp",
		}, RGB(255, 255, 255));
	character[1].SetTopLeft(120, 150);
	character[1].SetAnimation(50, 0);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (phase == 1) {
		if (nChar == 0x41) {
			character[1].ShowBitmap(2);
		}
	}

	/*
	if (nChar == 0x44) {
		character.SetTopLeft(character.Left() + 50, character.Top());	// D for going right
	}
	else if (nChar == 0x41) {
		character.SetTopLeft(character.Left() - 50, character.Top());	// A for going left
	}
	else if (nChar == 0x57) {
		character.SetTopLeft(character.Left(), character.Top() - 50);	// W for going up
	}
	else if (nChar == 0x53) {
		character.SetTopLeft(character.Left(), character.Top() + 50);	// S for going down
	}
	*/
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
}

void CGameStateRun::OnShow()
{
	ShowByPhase();
}

void CGameStateRun::ShowByPhase() {
	background.ShowBitmap(3);
	if (phase == 1) {
		character[0].ShowBitmap(2);
	}
}