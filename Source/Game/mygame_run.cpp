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
	
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	background.LoadBitmapByString({
		"resources/initialize_background.bmp"
	}, RGB(0, 0, 0));
	background.SetTopLeft(20, 75);

	character.LoadBitmapByString({
		"resources/pacman_towardR_1.bmp",
		"resources/pacman_towardR_2.bmp",
	}, RGB(255, 255, 255));
	character.SetTopLeft(120, 150);
	//character.SetAnimation(500, 0);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//if (phase == 1) {
	//	if (nChar == VK_RIGHT) {
	//		character.ShowBitmap();
	//	}
	//}

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
		character.ShowBitmap(2);
	}
}