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
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	if (towards == 0) {
		character[towards].SetTopLeft(character[towards].Left() + 5, character[towards].Top());
	}
	else if (towards == 1) {
		character[towards].SetTopLeft(character[towards].Left() - 5, character[towards].Top());
	}
	else if (towards == 2) {
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - 5);
	}
	else if (towards == 3) {
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() + 5);
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	background.LoadBitmapByString({
		"resources/initialize_background.bmp"
	}, RGB(255, 255, 255));
	background.SetTopLeft(20, 75);

	// character
	character[0].LoadBitmapByString({
		"resources/pacman/pacman_towardR_1.bmp",
		"resources/pacman/pacman_towardR_2.bmp",
	}, RGB(255, 255, 255));
	character[0].SetTopLeft(120, 150);
	character[0].SetAnimation(50, 0);

	character[1].LoadBitmapByString({
		"resources/pacman/pacman_towardL_1.bmp",
		"resources/pacman/pacman_towardL_2.bmp",
		}, RGB(255, 255, 255));
	character[1].SetAnimation(50, 0);

	character[2].LoadBitmapByString({
		"resources/pacman/pacman_towardU_1.bmp",
		"resources/pacman/pacman_towardU_2.bmp",
		}, RGB(255, 255, 255));
	character[2].SetAnimation(50, 0);

	character[3].LoadBitmapByString({
		"resources/pacman/pacman_towardD_1.bmp",
		"resources/pacman/pacman_towardD_2.bmp",
		}, RGB(255, 255, 255));
	character[3].SetAnimation(50, 0);

	// monster
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (phase == 1) {
		if (nChar == 0x44) {
			character[0].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = 0;
		}
		else if (nChar == 0x41) {
			character[1].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = 1;
		}
		else if (nChar == 0x57) {
			character[2].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = 2;
		}
		else if (nChar == 0x53) {
			character[3].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = 3;
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

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	ShowByPhase();
}

void CGameStateRun::ShowByPhase() {
	background.ShowBitmap(3);
	if (phase == 1) {
		if (towards == 0) {
			character[towards].ShowBitmap(2);
		}
		else if (towards == 1) {
			character[towards].ShowBitmap(2);
		}
		else if (towards == 2) {
			character[towards].ShowBitmap(2);
		}
		else if (towards == 3) {
			character[towards].ShowBitmap(2);
		}
	}
}