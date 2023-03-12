#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <math.h>

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
	int speed = 2;	// walking speed

	// character movement
	if (towards == RIGHT && GetPixelAttribute(character[towards].Right() + 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Right() + 1, character[towards].Bottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left() + speed, character[towards].Top());
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left() + speed, character[towards].Top());
		}*/
	}
	else if (towards == LEFT && GetPixelAttribute(character[towards].Left() - 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Left() - 1, character[towards].Bottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left() - speed, character[towards].Top());
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left() - speed, character[towards].Top());
		}*/
	}
	else if (towards == UP && GetPixelAttribute(character[towards].Left() + edge, character[towards].Top() - 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Top() - 1) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - speed);
		/*
		if (Delay(85)) {
			character[towards].SetTopLeft(character[towards].Left(), character[towards].Top() - speed);
		}*/
	}
	else if (towards == DOWN && GetPixelAttribute(character[towards].Left() + edge, character[towards].Bottom() + 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Bottom() + 1) >= 0) {
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
	// red
	if (etRed[etTowards[0]].Left() % 16 == 0 && etRed[etTowards[0]].Top() % 16 == 0 && CheckRoad(etRed[etTowards[0]], 0)) {
		ChaseMode();
	}

	if (etTowards[0] == RIGHT && GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left() + speed, etRed[etTowards[0]].Top());
	}
	else if (etTowards[0] == LEFT && GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left() - speed, etRed[etTowards[0]].Top());
	}
	else if (etTowards[0] == UP && GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Top() - 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Top() - 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left(), etRed[etTowards[0]].Top() - speed);
	}
	else if (etTowards[0] == DOWN && GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Bottom() + 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Bottom() + 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].Left(), etRed[etTowards[0]].Top() + speed);
	}
	else {
		ChaseMode();
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
		"resources/map/phase_1.bmp",
		"resources/map/phase_2.bmp",
		"resources/map/phase_3.bmp",
	}, RGB(255, 255, 255));
	background.SetTopLeft(0, 0);

	// cookies
	cookieAmount = 0;
	spCookieAmount = 0;
	InitMap();

	// character
	character[RIGHT].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardR_1.bmp",
		"resources/pacman/pacman_towardR_2.bmp",
	}, RGB(255, 255, 255));
	character[RIGHT].SetTopLeft(16, 64);
	character[RIGHT].SetAnimation(50, 0);

	character[LEFT].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardL_1.bmp",
		"resources/pacman/pacman_towardL_2.bmp",
		}, RGB(255, 255, 255));
	character[LEFT].SetAnimation(50, 0);

	character[UP].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardU_1.bmp",
		"resources/pacman/pacman_towardU_2.bmp",
		}, RGB(255, 255, 255));
	character[UP].SetAnimation(50, 0);

	character[DOWN].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_towardD_1.bmp",
		"resources/pacman/pacman_towardD_2.bmp",
		}, RGB(255, 255, 255));
	character[DOWN].SetAnimation(50, 0);

	//// monster
	// red
	etRed[RIGHT].LoadBitmapByString({
		"resources/ets/redET_towardR_1.bmp",
		"resources/ets/redET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etRed[RIGHT].SetAnimation(100, 0);

	etRed[LEFT].LoadBitmapByString({
		"resources/ets/redET_towardL_1.bmp",
		"resources/ets/redET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etRed[LEFT].SetTopLeft(208, 224);
	etRed[LEFT].SetAnimation(100, 0);

	etRed[UP].LoadBitmapByString({
		"resources/ets/redET_towardU_1.bmp",
		"resources/ets/redET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etRed[UP].SetAnimation(100, 0);

	etRed[DOWN].LoadBitmapByString({
		"resources/ets/redET_towardD_1.bmp",
		"resources/ets/redET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etRed[DOWN].SetAnimation(100, 0);

	// blue
	etBlue[RIGHT].LoadBitmapByString({
		"resources/ets/blueET_towardR_1.bmp",
		"resources/ets/blueET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[RIGHT].SetTopLeft(216, 272);
	etBlue[RIGHT].SetAnimation(100, 0);

	etBlue[LEFT].LoadBitmapByString({
		"resources/ets/blueET_towardL_1.bmp",
		"resources/ets/blueET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[LEFT].SetAnimation(100, 0);

	etBlue[UP].LoadBitmapByString({
		"resources/ets/blueET_towardU_1.bmp",
		"resources/ets/blueET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[UP].SetAnimation(100, 0);

	etBlue[DOWN].LoadBitmapByString({
		"resources/ets/blueET_towardD_1.bmp",
		"resources/ets/blueET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[DOWN].SetAnimation(100, 0);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {	// going right
		if (GetPixelAttribute(character[towards].Right() + 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Right() + 1, character[towards].Bottom() - edge) >= 0) {
			character[RIGHT].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = RIGHT;
		}
	}
	else if (nChar == VK_LEFT) {	// going left
		if (GetPixelAttribute(character[towards].Left() - 1, character[towards].Top() + edge) >= 0 && GetPixelAttribute(character[towards].Left() - 1, character[towards].Bottom() - edge) >= 0) {
			character[LEFT].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = LEFT;
		}
	}
	else if (nChar == VK_UP) {	// going up
		if (GetPixelAttribute(character[towards].Left() + edge, character[towards].Top() - 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Top() - 1) >= 0) {
			character[UP].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = UP;
		}
	}
	else if (nChar == VK_DOWN) {	// going down
		if (GetPixelAttribute(character[towards].Left() + edge, character[towards].Bottom() + 1) >= 0 && GetPixelAttribute(character[towards].Right() - edge, character[towards].Bottom() + 1) >= 0) {
			character[DOWN].SetTopLeft(character[towards].Left(), character[towards].Top());
			towards = DOWN;
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
			character[RIGHT].SetTopLeft(16, 64);
		}
		CheckPhaseClear();

		// monster
		etRed[etTowards[0]].ShowBitmap(1.25);
		//etBlue[etTowards[1]].ShowBitmap(1.25);
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
	//int lastdir[4] = { etTowards[0],etTowards[1] ,etTowards[2] ,etTowards[3] };
	double mindis[4] = { 2000000000,2000000000,2000000000,2000000000 };
	int nextdir[4] = { -1,-1,-1,-1 };

	// clear dirCanGo for each et(ghost)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dirCanGo[i][j] = -1;
		}
	}

	// red
	if (etRed[etTowards[0]].Top() != character[towards].Top() || etRed[etTowards[0]].Left() != character[towards].Left()) {
		if (GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() + 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
			dirCanGo[0][RIGHT] = RIGHT;
		}
		if (GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Top() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Left() - 1, etRed[etTowards[0]].Bottom() - edge) >= -1) {
			dirCanGo[0][LEFT] = LEFT;
		}
		if (GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Top() - 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Top() - 1) >= -1) {
			dirCanGo[0][UP] = UP;
		}
		if (GetPixelAttribute(etRed[etTowards[0]].Left() + edge, etRed[etTowards[0]].Bottom() + 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].Right() - edge, etRed[etTowards[0]].Bottom() + 1) >= -1) {
			dirCanGo[0][DOWN] = DOWN;
		}
		dirCanGo[0][etTowards[0]] = -1;
		
		for (int i = 3; i > -1; i--) {
			if (dirCanGo[0][i] == i) {
				if (Distance(etRed[etTowards[0]], character[towards], i) <= mindis[0]) {
					mindis[0] = Distance(etRed[etTowards[0]], character[towards], i);
					nextdir[0] = i;
				}
			}
		}
		etRed[nextdir[0]].SetTopLeft(etRed[etTowards[0]].Left(), etRed[etTowards[0]].Top());
		etTowards[0] = nextdir[0];
	}
}

double CGameStateRun::Distance(CMovingBitmap et, CMovingBitmap target, int nextdir) {
	if (nextdir == RIGHT) {
		return sqrt(pow(et.CenterX() + 16 - target.CenterX(), 2) + pow(et.CenterY() - target.CenterY(), 2));
	}
	else if (nextdir == LEFT) {
		return sqrt(pow(et.CenterX() - 16 - target.CenterX(), 2) + pow(et.CenterY() - target.CenterY(), 2));
	}
	else if (nextdir == UP) {
		return sqrt(pow(et.CenterX() - target.CenterX(), 2) + pow(et.CenterY() - 16 - target.CenterY(), 2));
	}
	else if (nextdir == DOWN) {
		return sqrt(pow(et.CenterX() - target.CenterX(), 2) + pow(et.CenterY() + 16 - target.CenterY(), 2));
	}
	return 2000000000;
}

bool CGameStateRun::CheckRoad(CMovingBitmap et, int etNo) {
	int dirCanGo = 0;
	if (GetPixelAttribute(et.Right() + 8, et.Top() + edge) >= -1 && GetPixelAttribute(et.Right() + 8, et.Bottom() - edge) >= -1) {
		dirCanGo++;
	}
	if (GetPixelAttribute(et.Left() - 8, et.Top() + edge) >= -1 && GetPixelAttribute(et.Left() - 8, et.Bottom() - edge) >= -1) {
		dirCanGo++;
	}
	if (GetPixelAttribute(et.Left() + edge, et.Top() - 8) >= -1 && GetPixelAttribute(et.Right() - edge, et.Top() - 8) >= -1) {
		dirCanGo++;
	}
	if (GetPixelAttribute(et.Left() + edge, et.Bottom() + 8) >= -1 && GetPixelAttribute(et.Right() - edge, et.Bottom() + 8) >= -1) {
		dirCanGo++;
	}

	if (dirCanGo-1 >= 2)	return true;
	return false;
}