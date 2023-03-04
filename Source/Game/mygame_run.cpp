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
	// character movement
	if (towards == 0) {
		character[towards].SetTopLeft(character[towards].Left() + 3, character[towards].Top());
	}
	else if (towards == 1) {
		character[towards].SetTopLeft(character[towards].Left() - 3, character[towards].Top());
	}
	else if (towards == 2 && character[towards].Top() > 95) {
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - 3);
	}
	else if (towards == 3 && character[towards].Top() < 435) {
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() + 3);
	}

	// eating cookies
	for (int i = 0; i < cookieAmount; i++) {
		gameClear &= (cookie[i].GetSelectShowBitmap() == 1);
		if (cookie[i].Left() >= character[towards].Left()
			&& cookie[i].Left() <= character[towards].Left() + character[towards].Width()
			&& cookie[i].Top() >= character[towards].Top()
			&& cookie[i].Top() <= character[towards].Top() + character[towards].Height()) {
			cookie[i].SelectShowBitmap(1);
		}
	}
	
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	PIXWIDTH = 37;
	PIXHEIGHT = 34;

	// map
	background.LoadBitmapByString({
		"resources/initialize_background.bmp",
		"resources/giraffe.bmp"
	}, RGB(255, 255, 255));
	background.SetTopLeft(20, 75);

	// cookies
	cookieAmount = 0;
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (map[i][j] == 0) {
				cookie[cookieAmount].LoadBitmapByString({ "resources/stuff/cookie.bmp", "resources/stuff/cookie_empty.bmp" }, RGB(255, 255, 255));
				cookie[cookieAmount].SetTopLeft(20 + PIXWIDTH * j, 75 + PIXHEIGHT * i);
				cookieAmount++;
			}
		}
	}

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
			character[0].SetTopLeft(character[towards].Left(), character[towards].Top());	// D for going right
			towards = 0;
		}
		else if (nChar == 0x41) {
			character[1].SetTopLeft(character[towards].Left(), character[towards].Top());	// A for going left
			towards = 1;
		}
		else if (nChar == 0x57) {
			character[2].SetTopLeft(character[towards].Left(), character[towards].Top());	// W for going up
			towards = 2;
		}
		else if (nChar == 0x53) {
			character[3].SetTopLeft(character[towards].Left(), character[towards].Top());	// S for going down
			towards = 3;
		}
	}
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
	if (phase == 1 && !gameClear) {
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

		for (int i = 0; i < cookieAmount; i++) {
			cookie[i].ShowBitmap();
		}
	}
	
}