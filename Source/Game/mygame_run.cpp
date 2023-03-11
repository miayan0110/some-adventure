#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <chrono>
#include <thread>

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
	int edge = 5; 	// space for turning around
	int speed = 2;	// walking speed

	// character movement
	if (towards == 0 && GetPixelAttribute(character[towards].Right() + 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Right() + 1, character[towards].Bottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left() + speed, character[towards].Top());
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left() + speed, character[towards].Top());
		}*/
	}
	else if (towards == 1 && GetPixelAttribute(character[towards].Left() - 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Left() - 1, character[towards].Bottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left() - speed, character[towards].Top());
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left() - speed, character[towards].Top());
		}*/
	}
	else if (towards == 2 && GetPixelAttribute(character[towards].Left() + edge, character[towards].Top() - 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Top() - 1) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - speed);
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - speed);
		}*/
	}
	else if (towards == 3 && GetPixelAttribute(character[towards].Left() + edge, character[towards].Bottom() + 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Bottom() + 1) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() + speed);
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() + speed);
		}*/
	}
	else {
		character[towards].StopKeepAni(false);
		character[towards].SelectShowBitmap(1);
	}

	// monster movement
	if (etTowards[0] == 0 && GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left() + speed, etRed[etTowards[0]].Top());
	}
	else if (etTowards[0] == 1 && GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left() - speed, etRed[etTowards[0]].Top());
	}
	else if (etTowards[0] == 2 && GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Top() - 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Top() - 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left(), etRed[etTowards[0]].Top() - speed);
	}
	else if (etTowards[0] == 3 && GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Bottom() + 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Bottom() + 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left(), etRed[etTowards[0]].Top() + speed);
	}
	else {

	}

	// unshow cookies
	for (int i = 0; i < cookieAmount; i++) {
		if (cookie[i].CenterX() >= character[towards].Left()
			&& cookie[i].CenterX() <= character[towards].Right()
			&& cookie[i].CenterY() >= character[towards].Top()
			&& cookie[i].CenterY() <= character[towards].Bottom()) {
			cookie[i].SelectShowBitmap(1);
			eatenCookie[i] = 0;
		}
	}
	
	for (int i = 0; i < spCookieAmount; i++) {
		if (character[towards].CenterX() >= spCookie[i].Left()
			&& character[towards].CenterX() <= spCookie[i].Right()
			&& character[towards].CenterY() >= spCookie[i].Top()
			&& character[towards].CenterY() <= spCookie[i].Bottom()) {
			spCookie[i].UnshowBitmap();
			spCookie[i].SelectShowBitmap(1);
			eatenSP[i] = 0;
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	PIXWIDTH = 16;
	PIXHEIGHT = 16;

	// map
	background.LoadBitmapByString({
		"resources/map/phase_1_test.bmp",
		"resources/map/phase_2.bmp",
		"resources/map/phase_3.bmp",
	}, RGB(255, 255, 255));
	background.SetTopLeft(0, 0);

	// cookies
	cookieAmount = 0;
	spCookieAmount = 0;
	InitMap();

	// character
	character[0].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardR_1.bmp",
		"resources/pacman/pacman_towardR_2.bmp",
	}, RGB(255, 255, 255));
	character[0].SetTopLeft(16, 64);
	character[0].SetAnimation(50, 0);

	character[1].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardL_1.bmp",
		"resources/pacman/pacman_towardL_2.bmp",
		}, RGB(255, 255, 255));
	character[1].SetAnimation(50, 0);

	character[2].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardU_1.bmp",
		"resources/pacman/pacman_towardU_2.bmp",
		}, RGB(255, 255, 255));
	character[2].SetAnimation(50, 0);

	character[3].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardD_1.bmp",
		"resources/pacman/pacman_towardD_2.bmp",
		}, RGB(255, 255, 255));
	character[3].SetAnimation(50, 0);

	//// monster
	// red
	etRed[0].LoadBitmapByString({
		"resources/ets/redET_towardR_1.bmp",
		"resources/ets/redET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etRed[0].SetTopLeft(192, 272);
	etRed[0].SetAnimation(100, 0);

	etRed[1].LoadBitmapByString({
		"resources/ets/redET_towardL_1.bmp",
		"resources/ets/redET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etRed[1].SetAnimation(100, 0);

	etRed[2].LoadBitmapByString({
		"resources/ets/redET_towardU_1.bmp",
		"resources/ets/redET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etRed[2].SetAnimation(100, 0);

	etRed[3].LoadBitmapByString({
		"resources/ets/redET_towardD_1.bmp",
		"resources/ets/redET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etRed[3].SetAnimation(100, 0);

	// blue
	etBlue[0].LoadBitmapByString({
		"resources/ets/blueET_towardR_1.bmp",
		"resources/ets/blueET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[0].SetTopLeft(216, 272);
	etBlue[0].SetAnimation(100, 0);

	etBlue[1].LoadBitmapByString({
		"resources/ets/blueET_towardL_1.bmp",
		"resources/ets/blueET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[1].SetAnimation(100, 0);

	etBlue[2].LoadBitmapByString({
		"resources/ets/blueET_towardU_1.bmp",
		"resources/ets/blueET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[2].SetAnimation(100, 0);

	etBlue[3].LoadBitmapByString({
		"resources/ets/blueET_towardD_1.bmp",
		"resources/ets/blueET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[3].SetAnimation(100, 0);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int edge = 5;	// space for turning around
	if (nChar == VK_RIGHT) {
		if (GetPixelAttribute(character[towards].Right() + 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Right() + 1, character[towards].Bottom() - edge) >= 0) {
			character[0].SetTopLeft(character[towards].Left(), character[towards].Top());	// D for going right
			towards = 0;
		}
	}
	else if (nChar == VK_LEFT) {
		if (GetPixelAttribute(character[towards].Left() - 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Left() - 1, character[towards].Bottom() - edge) >= 0) {
			character[1].SetTopLeft(character[towards].Left(), character[towards].Top());	// A for going left
			towards = 1;
		}
	}
	else if (nChar == VK_UP) {
		if (GetPixelAttribute(character[towards].Left() + edge, character[towards].Top() - 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Top() - 1) >= 0) {
			character[2].SetTopLeft(character[towards].Left(), character[towards].Top());	// W for going up
			towards = 2;
		}
	}
	else if (nChar == VK_DOWN) {
		if (GetPixelAttribute(character[towards].Left() + edge, character[towards].Bottom() + 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Bottom() + 1) >= 0) {
			character[3].SetTopLeft(character[towards].Left(), character[towards].Top());	// S for going down
			towards = 3;
		}
	}
	else if (nChar == VK_SPACE) {
		phase++;
		InitEaten(eatenCookie, cookieAmount);
		InitEaten(eatenSP, spCookieAmount);
		cookieAmount = 0;
		spCookieAmount = 0;
		isMapLoaded = false;
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
	// show phase background
	background.SelectShowBitmap(phase - 1);
	background.ShowBitmap();

	//character
	character[towards].ShowBitmap(1.25);

	// cookies
	for (int i = 0; i < cookieAmount; i++) {
		cookie[i].ShowBitmap(0.5);
	}
	for (int i = 0; i < spCookieAmount; i++) {
		spCookie[i].ShowBitmap(1.5);
	}

	// change phase
	if (phase < 5) {
		if (!isMapLoaded) {
			InitMap();
			character[0].SetTopLeft(16, 64);
		}
		CheckPhaseClear();

		// monster
		etRed[etTowards[0]].ShowBitmap(1.25);
		etBlue[etTowards[1]].ShowBitmap(1.25);
	}
}

bool CGameStateRun::Delay(int delaytime) {

	if (clock() - last_time >= delaytime) {
		last_time = clock();
		return true;
	}
	return false;
}

void CGameStateRun::InitMap() {
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (map[phase - 1][i][j] == 0) {
				cookie[cookieAmount].LoadBitmapByString({ "resources/stuff/cookie.bmp", "resources/stuff/cookie_empty.bmp" }, RGB(255, 255, 255));
				cookie[cookieAmount].SetTopLeft(4 + PIXWIDTH * j, 4 + PIXHEIGHT * i);
				cookie[cookieAmount].SelectShowBitmap(0);
				eatenCookie[cookieAmount] = 1;
				cookieAmount++;
			}
			else if (map[phase - 1][i][j] == 1) {
				spCookie[spCookieAmount].LoadBitmapByString({ "resources/stuff/cookie.bmp", "resources/stuff/cookie_empty.bmp" }, RGB(255, 255, 255));
				spCookie[spCookieAmount].SetTopLeft(PIXWIDTH*j - 2, PIXHEIGHT*i - 2);
				spCookie[spCookieAmount].SelectShowBitmap(0);
				spCookie[spCookieAmount].SetAnimation(200, 0);
				eatenSP[spCookieAmount] = 1;
				spCookieAmount++;
			}
		}
	}
	isMapLoaded = true;
}

void CGameStateRun::InitEaten(int *p, int len) {
	for (int i = 0; i < len; i++) {
		p[i] = 1;
	}
}

int CGameStateRun::GetPixelAttribute(int left, int top) {
	return map[phase-1][top/PIXHEIGHT][left/PIXWIDTH];
}

bool CGameStateRun::FindElement(int *p, int len, int val) {
	for (int i = 0; i < len; i++) {
		if (p[i] == val)	return true;
	}
	return false;
}

void CGameStateRun::CheckPhaseClear() {
	if (!FindElement(eatenCookie, cookieAmount, 1) && !FindElement(eatenSP, spCookieAmount, 1)) {
		phase++;
		InitEaten(eatenCookie, cookieAmount);
		InitEaten(eatenSP, spCookieAmount);
		cookieAmount = 0;
		spCookieAmount = 0;
		isMapLoaded = false;
	}
}

void CGameStateRun::ChaseMode() {
	// red

}