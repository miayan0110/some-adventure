#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "myfunc.h"

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
	/* pacman movement */
	if (towards == RIGHT && GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetBottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].GetLeft() + speed, character[towards].GetTop());
	}
	else if (towards == LEFT && GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetBottom() - edge) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].GetLeft() - speed, character[towards].GetTop());
	}
	else if (towards == UP && GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetTop() - 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetTop() - 1) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop() - speed);
	}
	else if (towards == DOWN && GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetBottom() + 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetBottom() + 1) >= 0) {
		character[towards].StopKeepAni(true);
		character[towards].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop() + speed);
	}// through tunnel
	else if (character[towards].GetRight() >= 435) {
		towards = 4;
		if (Delay(1200)) {
			towards = RIGHT;
			character[towards].SetTopLeft(1, character[towards].GetTop());
		}
	}// through tunnel
	else if (character[towards].GetLeft() <= 3) {
		towards = 4;
		if (Delay(1200)) {
			towards = LEFT;
			character[towards].SetTopLeft(445, character[towards].GetTop());
		}
	}// stop walking
	else {
		character[towards].StopKeepAni(false);
		character[towards].SetFrameIndexOfBitmap(1);
	}


	/* ghost */
	// red
	if ((etRed[etTowards[0]].GetLeft() % 16 <= 0 && etRed[etTowards[0]].GetTop() % 16 <= 0) || (etRed[etTowards[0]].GetLeft() % 16 >= 15 && etRed[etTowards[0]].GetTop() % 16 >= 15)) {
		ChaseMode('r');
	}
	if (etTowards[0] == RIGHT && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetTop() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetBottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft() + speed, etRed[etTowards[0]].GetTop());
	}
	else if (etTowards[0] == LEFT && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetTop() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetBottom() - edge) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft() - speed, etRed[etTowards[0]].GetTop());
	}
	else if (etTowards[0] == UP && GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetTop() - 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetTop() - 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() - speed);
	}
	else if (etTowards[0] == DOWN && GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetBottom() + 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetBottom() + 1) >= -1) {
		etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() + speed);
	}
	else if (etRed[etTowards[0]].GetRight() >= 440) {
		etTowards[0] = 4;
		if (Delay(100)) {
			etTowards[0] = RIGHT;
			etRed[etTowards[0]].SetTopLeft(1, etRed[etTowards[0]].GetTop());
		}
	}
	else if (etRed[etTowards[0]].GetLeft() <= 3) {
		etTowards[0] = 4;
		if (Delay(100)) {
			etTowards[0] = LEFT;
			etRed[etTowards[0]].SetTopLeft(445, etRed[etTowards[0]].GetTop());
		}
	}
	else if (etRed[etTowards[0]].GetLeft() % 16 == 0 && etRed[etTowards[0]].GetTop() % 16 == 0) {
		ChaseMode('r');
	}
	else {
		ChaseMode('r');
	}


	/* unshow cookies */
	for (int i = 0; i < cookieAmount; i++) {
		if (cookie[i].IsEaten(cookie[i], character[towards])) {
			cookie[i].SetFrameIndexOfBitmap(1);
			eatenCookie[i] = 0;
		}
	}

	for (int i = 0; i < spCookieAmount; i++) {
		if (spCookie[i].IsEaten(character[towards], spCookie[i])) {
			spCookie[i].UnshowBitmap();
			spCookie[i].SetFrameIndexOfBitmap(1);
			eatenSP[i] = 0;
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	/* load map */
	background.LoadBitmapByString({
		"resources/map/phase_1_test.bmp",
		"resources/map/phase_2.bmp",
		"resources/map/phase_3.bmp",
		}, RGB(255, 255, 255));
	background.SetTopLeft(0, 0);

	/* init cookies */
	cookieAmount = 0;
	spCookieAmount = 0;
	InitMap();

	/* character */
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

	character[4].LoadBitmapByString({
		"resources/pacman/pacman_unshow.bmp",
		"resources/pacman/pacman_unshow.bmp",
		}, RGB(255, 255, 255));

	/* ghost */
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

	etRed[4].LoadBitmapByString({
		"resources/ets/et_unshow.bmp",
		"resources/ets/et_unshow.bmp",
		}, RGB(255, 255, 255));
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/* pacman control */
	if (nChar == 0x44) {	// going right
		if (GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetBottom() - edge) >= 0) {
			character[RIGHT].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
			towards = RIGHT;
		}
	}
	else if (nChar == 0x41) {	// going left
		if (GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetBottom() - edge) >= 0) {
			character[LEFT].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
			towards = LEFT;
		}
	}
	else if (nChar == 0x57) {	// going up
		if (GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetTop() - 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetTop() - 1) >= 0) {
			character[UP].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
			towards = UP;
		}
	}
	else if (nChar == 0x53) {	// going down
		if (GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetBottom() + 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetBottom() + 1) >= 0) {
			character[DOWN].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
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
	/* show phase background */
	background.SetFrameIndexOfBitmap(phase - 1);
	background.ShowBitmap();

	/* show character */
	character[towards].ShowBitmap(1.25);

	/* show cookies */
	for (int i = 0; i < cookieAmount; i++) {
		cookie[i].ShowBitmap(0.5);
	}
	for (int i = 0; i < spCookieAmount; i++) {
		spCookie[i].ShowBitmap(1.5);
	}

	/* show ghosts */
	etRed[etTowards[0]].ShowBitmap(1.25);

	/* check and change phase */
	if (phase < 5) {
		if (!isMapLoaded) {
			InitMap();
			character[RIGHT].SetTopLeft(16, 64);
		}
		CheckPhaseClear();
	}
}

void CGameStateRun::InitMap() {
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (map[phase - 1][i][j] == 0) {
				cookie[cookieAmount].LoadBitmapByString({ "resources/stuff/cookie.bmp", "resources/stuff/cookie_empty.bmp" }, RGB(255, 255, 255));
				cookie[cookieAmount].SetTopLeft(4 + PIXWIDTH * j, 4 + PIXHEIGHT * i);
				cookie[cookieAmount].SetFrameIndexOfBitmap(0);
				eatenCookie[cookieAmount] = 1;
				cookieAmount++;
			}
			else if (map[phase - 1][i][j] == 1) {
				spCookie[spCookieAmount].LoadBitmapByString({ "resources/stuff/cookie.bmp", "resources/stuff/cookie_empty.bmp" }, RGB(255, 255, 255));
				spCookie[spCookieAmount].SetTopLeft(PIXWIDTH*j - 2, PIXHEIGHT*i - 2);
				spCookie[spCookieAmount].SetFrameIndexOfBitmap(0);
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

void CGameStateRun::ChaseMode(char mode) {
	double mindis[4] = { 2000000000,2000000000,2000000000,2000000000 };
	int nextdir[4] = { -1,-1,-1,-1 };
	double target[2];

	// init target for each ghost
	switch (mode) {
	case 'r':
		target[0] = character[towards].CenterX();
		target[1] = character[towards].CenterY();
		break;
	case 'p':
		if (towards == RIGHT) {
			target[0] = character[towards].CenterX() + 2 * PIXWIDTH;
			target[1] = character[towards].CenterY();
		}
		else if (towards == LEFT) {
			target[0] = character[towards].CenterX() - 2 * PIXWIDTH;
			target[1] = character[towards].CenterY();
		}
		else if (towards == UP) {
			target[0] = character[towards].CenterX();
			target[1] = character[towards].CenterY() - 2 * PIXHEIGHT;
		}
		else if (towards == DOWN) {
			target[0] = character[towards].CenterX();
			target[1] = character[towards].CenterY() + 2 * PIXHEIGHT;
		}
	}

	// clear dirCanGo for each ghost
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dirCanGo[i][j] = -1;
		}
	}

	// decide the next direction
	if (mode == 'r') {
		if (etRed[etTowards[0]].CenterX() != target[0] || etRed[etTowards[0]].CenterY() != target[1]) {
			if (GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetTop() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetBottom() - edge) >= -1) {
				dirCanGo[0][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetTop() + edge) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetBottom() - edge) >= -1) {
				dirCanGo[0][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetTop() - 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetTop() - 1) >= -1) {
				dirCanGo[0][UP] = UP;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetBottom() + 1) >= -1 && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetBottom() + 1) >= -1) {
				dirCanGo[0][DOWN] = DOWN;
			}

			if (etTowards[0] == DOWN || etTowards[0] == RIGHT) {
				dirCanGo[0][etTowards[0] - 2] = -1;
			}
			if (etTowards[0] == UP || etTowards[0] == LEFT) {
				dirCanGo[0][etTowards[0] + 2] = -1;
			}


			for (int i = 3; i > -1; i--) {
				if (dirCanGo[0][i] == i) {
					if (Distance(etRed[etTowards[0]], character[towards], i) <= mindis[0]) {
						mindis[0] = Distance(etRed[etTowards[0]], character[towards], i);
						nextdir[0] = i;
					}
				}
			}

			if (etRed[etTowards[0]].IsBitmapLoaded()) {
				if (etRed[etTowards[0]].GetRight() >= 440 || etRed[etTowards[0]].GetLeft() <= 10) {
					return;
				}
				etRed[nextdir[0]].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop());
				etTowards[0] = nextdir[0];
			}
		}
		
	}
}

int CGameStateRun::GetPixelAttribute(int left, int top) {
	return map[phase - 1][top / PIXHEIGHT][left / PIXWIDTH];
}

double CGameStateRun::Distance(MyBitmap mb1, MyBitmap mb2, int nextdir) {
	if (nextdir == RIGHT) {
		return sqrt(pow((int)mb1.CenterX() / 16 + 1 - (int)mb2.CenterX() / 16, 2) + pow((int)mb1.CenterY() / 16 - (int)mb2.CenterY() / 16, 2));
	}
	else if (nextdir == LEFT) {
		return sqrt(pow((int)mb1.CenterX() / 16 - 1 - (int)mb2.CenterX() / 16, 2) + pow((int)mb1.CenterY() / 16 - (int)mb2.CenterY() / 16, 2));
	}
	else if (nextdir == UP) {
		return sqrt(pow((int)mb1.CenterX() / 16 - (int)mb2.CenterX() / 16, 2) + pow((int)mb1.CenterY() / 16 - 1 - (int)mb2.CenterY() / 16, 2));
	}
	else if (nextdir == DOWN) {
		return sqrt(pow((int)mb1.CenterX() / 16 - (int)mb2.CenterX() / 16, 2) + pow((int)mb1.CenterY() / 16 + 1 - (int)mb2.CenterY() / 16, 2));
	}
	return 2000000000;
}

bool CGameStateRun::Delay(int delaytime) {
	if (clock() - last_time >= delaytime) {
		last_time = clock();
		return true;
	}
	return false;
}

bool CGameStateRun::FindElement(int *p, int len, int target) {
	for (int i = 0; i < len; i++) {
		if (p[i] == target)	return true;
	}
	return false;
}