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
	if (clock()-start_time <= 3*1000) {
		Sleep(3000);
	}

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
		if (ghostmode == 0) ChaseMode('r');
		else if (ghostmode == 1) ScatterMode('r');
		NextDir('r');
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
		etTowards[0] = UNSHOW;
		if (Delay(100)) {
			etTowards[0] = RIGHT;
			etRed[etTowards[0]].SetTopLeft(0, etRed[etTowards[0]].GetTop());
		}
	}
	else if (etRed[etTowards[0]].GetLeft() <= 3) {
		etTowards[0] = UNSHOW;
		if (Delay(100)) {
			etTowards[0] = LEFT;
			etRed[etTowards[0]].SetTopLeft(446, etRed[etTowards[0]].GetTop());
		}
	}
	else if (etRed[etTowards[0]].GetLeft() % 16 == 0 && etRed[etTowards[0]].GetTop() % 16 == 0) {
		if (ghostmode == 0) ChaseMode('r');
		else if (ghostmode == 1) ScatterMode('r');
		NextDir('r');
	}
	else {
		if (ghostmode == 0) ChaseMode('r');
		else if (ghostmode == 1) ScatterMode('r');
		NextDir('r');
	}

	// pink
	//if(clock())
	if ((etPink[etTowards[1]].GetLeft() % 16 <= 0 && etPink[etTowards[1]].GetTop() % 16 <= 0) || (etPink[etTowards[1]].GetLeft() % 16 >= 15 && etPink[etTowards[1]].GetTop() % 16 >= 15)) {
		if (ghostmode == 0) ChaseMode('p');
		else if (ghostmode == 1) ScatterMode('p');
		NextDir('p');
	}
	if (etTowards[1] == RIGHT && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetTop() + edge) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetBottom() - edge) >= -1) {
		etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft() + speed, etPink[etTowards[1]].GetTop());
	}
	else if (etTowards[1] == LEFT && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetTop() + edge) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetBottom() - edge) >= -1) {
		etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft() - speed, etPink[etTowards[1]].GetTop());
	}
	else if (etTowards[1] == UP && GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetTop() - 1) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetTop() - 1) >= -1) {
		etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() - speed);
	}
	else if (etTowards[1] == DOWN && GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetBottom() + 1) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetBottom() + 1) >= -1) {
		etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() + speed);
	}
	else if (etPink[etTowards[1]].GetRight() >= 440) {
		etTowards[1] = UNSHOW;
		if (Delay(100)) {
			etTowards[1] = RIGHT;
			etPink[etTowards[1]].SetTopLeft(0, etPink[etTowards[1]].GetTop());
		}
	}
	else if (etPink[etTowards[1]].GetLeft() <= 3) {
		etTowards[1] = UNSHOW;
		if (Delay(100)) {
			etTowards[1] = LEFT;
			etPink[etTowards[1]].SetTopLeft(446, etPink[etTowards[1]].GetTop());
		}
	}
	else if (etPink[etTowards[1]].GetLeft() % 16 == 0 && etPink[etTowards[1]].GetTop() % 16 == 0) {
		if (ghostmode == 0) ChaseMode('p');
		else if (ghostmode == 1) ScatterMode('p');
		NextDir('p');
	}
	else {
		if (ghostmode == 0) ChaseMode('p');
		else if (ghostmode == 1) ScatterMode('p');
		NextDir('p');
	}

	// blue
	//if(clock())
	if ((etBlue[etTowards[2]].GetLeft() % 16 <= 0 && etBlue[etTowards[2]].GetTop() % 16 <= 0) || (etBlue[etTowards[2]].GetLeft() % 16 >= 15 && etBlue[etTowards[2]].GetTop() % 16 >= 15)) {
		if (ghostmode == 0) ChaseMode('b');
		else if (ghostmode == 1) ScatterMode('b');
		NextDir('b');
	}
	if (etTowards[2] == RIGHT && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetTop() + edge) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetBottom() - edge) >= -1) {
		etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft() + speed, etBlue[etTowards[2]].GetTop());
	}
	else if (etTowards[2] == LEFT && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetTop() + edge) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetBottom() - edge) >= -1) {
		etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft() - speed, etBlue[etTowards[2]].GetTop());
	}
	else if (etTowards[2] == UP && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetTop() - 1) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetTop() - 1) >= -1) {
		etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() - speed);
	}
	else if (etTowards[2] == DOWN && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetBottom() + 1) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetBottom() + 1) >= -1) {
		etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() + speed);
	}
	else if (etBlue[etTowards[2]].GetRight() >= 440) {
		etTowards[2] = UNSHOW;
		if (Delay(100)) {
			etTowards[2] = RIGHT;
			etBlue[etTowards[2]].SetTopLeft(0, etBlue[etTowards[2]].GetTop());
		}
	}
	else if (etBlue[etTowards[2]].GetLeft() <= 3) {
		etTowards[2] = UNSHOW;
		if (Delay(100)) {
			etTowards[2] = LEFT;
			etBlue[etTowards[2]].SetTopLeft(446, etBlue[etTowards[2]].GetTop());
		}
	}
	else if (etBlue[etTowards[2]].GetLeft() % 16 == 0 && etBlue[etTowards[2]].GetTop() % 16 == 0) {
		if (ghostmode == 0) ChaseMode('b');
		else if (ghostmode == 1) ScatterMode('b');
		NextDir('b');
	}
	else {
		if (ghostmode == 0) ChaseMode('b');
		else if (ghostmode == 1) ScatterMode('b');
		NextDir('b');
	}

	// yellow
	//if(clock())
	if ((etYellow[etTowards[3]].GetLeft() % 16 <= 0 && etYellow[etTowards[3]].GetTop() % 16 <= 0) || (etYellow[etTowards[3]].GetLeft() % 16 >= 15 && etYellow[etTowards[3]].GetTop() % 16 >= 15)) {
		if (Distance(etYellow[etTowards[3]], character[towards]) > 128 && ghostmode == 0) ChaseMode('y');
		else if (Distance(etYellow[etTowards[3]], character[towards]) <= 128 || ghostmode == 1) ScatterMode('y');
		NextDir('y');
	}
	if (etTowards[3] == RIGHT && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetTop() + edge) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetBottom() - edge) >= -1) {
		etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft() + speed, etYellow[etTowards[3]].GetTop());
	}
	else if (etTowards[3] == LEFT && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetTop() + edge) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetBottom() - edge) >= -1) {
		etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft() - speed, etYellow[etTowards[3]].GetTop());
	}
	else if (etTowards[3] == UP && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetTop() - 1) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetTop() - 1) >= -1) {
		etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() - speed);
	}
	else if (etTowards[3] == DOWN && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetBottom() + 1) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetBottom() + 1) >= -1) {
		etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() + speed);
	}
	else if (etYellow[etTowards[3]].GetRight() >= 440) {
		etTowards[3] = UNSHOW;
		if (Delay(100)) {
			etTowards[3] = RIGHT;
			etYellow[etTowards[3]].SetTopLeft(0, etYellow[etTowards[3]].GetTop());
		}
	}
	else if (etYellow[etTowards[3]].GetLeft() <= 3) {
		etTowards[3] = UNSHOW;
		if (Delay(100)) {
			etTowards[3] = LEFT;
			etYellow[etTowards[3]].SetTopLeft(446, etYellow[etTowards[3]].GetTop());
		}
	}
	else if (etYellow[etTowards[3]].GetLeft() % 16 == 0 && etYellow[etTowards[3]].GetTop() % 16 == 0) {
		if (Distance(etYellow[etTowards[3]], character[towards]) > 128 && ghostmode == 0) ChaseMode('y');
		else if (Distance(etYellow[etTowards[3]], character[towards]) <= 128 || ghostmode == 1) ScatterMode('y');
		NextDir('y');
	}
	else {
		if (Distance(etYellow[etTowards[3]], character[towards]) > 128 && ghostmode == 0) ChaseMode('y');
		else if (Distance(etYellow[etTowards[3]], character[towards]) <= 128 || ghostmode == 1) ScatterMode('y');
		NextDir('y');
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
	etRed[LEFT].SetTopLeft(216, 224);
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

	etRed[UNSHOW].LoadBitmapByString({
		"resources/ets/et_unshow.bmp",
		"resources/ets/et_unshow.bmp",
		}, RGB(255, 255, 255));
	/*
	etRed[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etRed[SCARED].SetAnimation(100, 0);

	etRed[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etRed[TRANSIT].SetAnimation(100, 0);*/

	// pink
	etPink[RIGHT].LoadBitmapByString({
		"resources/ets/pinkET_towardR_1.bmp",
		"resources/ets/pinkET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etPink[RIGHT].SetAnimation(100, 0);

	etPink[LEFT].LoadBitmapByString({
		"resources/ets/pinkET_towardL_1.bmp",
		"resources/ets/pinkET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etPink[LEFT].SetAnimation(100, 0);

	etPink[UP].LoadBitmapByString({
		"resources/ets/pinkET_towardU_1.bmp",
		"resources/ets/pinkET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etPink[UP].SetAnimation(100, 0);

	etPink[DOWN].LoadBitmapByString({
		"resources/ets/pinkET_towardD_1.bmp",
		"resources/ets/pinkET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etPink[DOWN].SetTopLeft(216, 272);
	etPink[DOWN].SetAnimation(100, 0);

	etPink[UNSHOW].LoadBitmapByString({
		"resources/ets/et_unshow.bmp",
		"resources/ets/et_unshow.bmp",
		}, RGB(255, 255, 255));
	/*
	etPink[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etPink[SCARED].SetAnimation(100, 0);

	etPink[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etPink[TRANSIT].SetAnimation(100, 0);*/

	// blue
	etBlue[RIGHT].LoadBitmapByString({
		"resources/ets/blueET_towardR_1.bmp",
		"resources/ets/blueET_towardR_2.bmp",
		}, RGB(255, 255, 255));
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
	etBlue[UP].SetTopLeft(186, 272);
	etBlue[UP].SetAnimation(100, 0);

	etBlue[DOWN].LoadBitmapByString({
		"resources/ets/blueET_towardD_1.bmp",
		"resources/ets/blueET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[DOWN].SetAnimation(100, 0);

	etBlue[UNSHOW].LoadBitmapByString({
		"resources/ets/et_unshow.bmp",
		"resources/ets/et_unshow.bmp",
		}, RGB(255, 255, 255));
	/*
	etBlue[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[SCARED].SetAnimation(100, 0);

	etBlue[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[TRANSIT].SetAnimation(100, 0);*/

	// yellow
	etYellow[RIGHT].LoadBitmapByString({
		"resources/ets/yellowET_towardR_1.bmp",
		"resources/ets/yellowET_towardR_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[RIGHT].SetAnimation(100, 0);

	etYellow[LEFT].LoadBitmapByString({
		"resources/ets/yellowET_towardL_1.bmp",
		"resources/ets/yellowET_towardL_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[LEFT].SetAnimation(100, 0);

	etYellow[UP].LoadBitmapByString({
		"resources/ets/yellowET_towardU_1.bmp",
		"resources/ets/yellowET_towardU_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[UP].SetTopLeft(246, 272);
	etYellow[UP].SetAnimation(100, 0);

	etYellow[DOWN].LoadBitmapByString({
		"resources/ets/yellowET_towardD_1.bmp",
		"resources/ets/yellowET_towardD_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[DOWN].SetAnimation(100, 0);

	etYellow[UNSHOW].LoadBitmapByString({
		"resources/ets/et_unshow.bmp",
		"resources/ets/et_unshow.bmp",
		}, RGB(255, 255, 255));
	/*
	etYellow[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[SCARED].SetAnimation(100, 0);

	etYellow[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[TRANSIT].SetAnimation(100, 0);*/
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
		/* change phase by hand */
		phase++;
		InitEaten(eatenCookie, cookieAmount);
		InitEaten(eatenSP, spCookieAmount);
		cookieAmount = 0;
		spCookieAmount = 0;
		isMapLoaded = false;
	}
	else if (nChar == VK_UP) {
		ghostmode = 0;	/* change ghost mode to chase mode by hand */
		TurnAround(etTowards[0], etTowards[1], etTowards[2], etTowards[3]);
	}
	else if (nChar == VK_DOWN) {
		ghostmode = 1;	/* change ghost mode to scatter mode by hand */
		TurnAround(etTowards[0], etTowards[1], etTowards[2], etTowards[3]);
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
	/* check and change phase */
	if (phase < 5) {
		if (!isMapLoaded) {
			start_time = clock();
			InitMap();
			character[towards].SetTopLeft(16, 64);
			etRed[etTowards[0]].SetTopLeft(216, 224);
			etPink[etTowards[1]].SetTopLeft(216, 272);
			etBlue[etTowards[2]].SetTopLeft(186, 272);
		}
		CheckPhaseClear();
	}

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
	etPink[etTowards[1]].ShowBitmap(1.25);
	etBlue[etTowards[2]].ShowBitmap(1.25);
	etYellow[etTowards[3]].ShowBitmap(1.25);
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
/*
void CGameStateRun::InitGhostMovement(char mode) {
	switch (mode)
	{
	case 'b':
		while (etBlue[etTowards[2]].GetLeft() != 216 && etBlue[etTowards[2]].GetTop() != 272) {
			etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft() + 1, etBlue[etTowards[2]].GetTop());
		}
		break;
	default:
		break;
	}
}*/

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
	/* init target for each ghost */
	switch (mode) {
	case 'r':
		target[0][0] = character[towards].CenterX();
		target[0][1] = character[towards].CenterY();
		break;
	case 'p':
		if (towards == RIGHT) {
			target[1][0] = character[towards].CenterX() + 4 * PIXWIDTH;
			target[1][1] = character[towards].CenterY();
		}
		else if (towards == LEFT) {
			target[1][0] = character[towards].CenterX() - 4 * PIXWIDTH;
			target[1][1] = character[towards].CenterY();
		}
		else if (towards == UP) {
			target[1][0] = character[towards].CenterX() - 4 * PIXWIDTH;
			target[1][1] = character[towards].CenterY() - 4 * PIXHEIGHT;
		}
		else if (towards == DOWN) {
			target[1][0] = character[towards].CenterX();
			target[1][1] = character[towards].CenterY() + 4 * PIXHEIGHT;
		}
		break;
	case 'b':
		if (towards == RIGHT) {
			target[2][0] = 2 * (character[towards].CenterX() + 2 * PIXWIDTH) - etRed[etTowards[0]].CenterX();
			target[2][1] = 2 * character[towards].CenterY() - etRed[etTowards[0]].CenterY();
		}
		else if (towards == LEFT) {
			target[2][0] = 2 * (character[towards].CenterX() - 2 * PIXWIDTH) - etRed[etTowards[0]].CenterX();
			target[2][1] = 2 * character[towards].CenterY() - etRed[etTowards[0]].CenterY();
		}
		else if (towards == UP) {
			target[2][0] = 2 * (character[towards].CenterX() - 2 * PIXWIDTH) - etRed[etTowards[0]].CenterX();
			target[2][1] = 2 * (character[towards].CenterY() - 2 * PIXHEIGHT) - etRed[etTowards[0]].CenterY();
		}
		else if (towards == DOWN) {
			target[2][0] = 2 * (character[towards].CenterX()) - etRed[etTowards[0]].CenterX();
			target[2][1] = 2 * character[towards].CenterY() + 2 * PIXHEIGHT - etRed[etTowards[0]].CenterY();
		}
		break;
	case 'y':
		target[3][0] = character[towards].CenterX();
		target[3][1] = character[towards].CenterY();
		break;
	default:
		break;
	}
}

void CGameStateRun::ScatterMode(char mode) {
	/* turn back when enter scatter mode */
	/* set target for scatter mode */
	switch (mode)
	{
	case 'r':
		target[0][0] = 398;
		target[0][1] = 24;
		break;
	case 'p':
		target[1][0] = 40;
		target[1][1] = 24;
	case 'b':
		target[2][0] = 398;
		target[2][1] = 568;
	case 'y':
		target[3][0] = 40;
		target[3][1] = 568;
	default:
		break;
	}
}

void CGameStateRun::NextDir(char mode) {
	double mindis[4] = { 2000000000,2000000000,2000000000,2000000000 };
	int nextdir[4] = { -1,-1,-1,-1 };

	/* clear dirCanGo for each ghost */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dirCanGo[i][j] = -1;
		}
	}

	/* decide the next direction */
	if (mode == 'r') {
		if (etRed[etTowards[0]].CenterX() != target[0][0] || etRed[etTowards[0]].CenterY() != target[0][1]) {
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
					if (Distance(etRed[etTowards[0]], 0, i) <= mindis[0]) {
						mindis[0] = Distance(etRed[etTowards[0]], 0, i);
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
	else if (mode == 'p') {
		if (etPink[etTowards[1]].CenterX() != target[1][0] || etPink[etTowards[1]].CenterY() != target[1][1]) {
			if (GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetTop() + edge) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetBottom() - edge) >= -1) {
				dirCanGo[1][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetTop() + edge) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetBottom() - edge) >= -1) {
				dirCanGo[1][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetTop() - 1) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetTop() - 1) >= -1) {
				dirCanGo[1][UP] = UP;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetBottom() + 1) >= -1 && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetBottom() + 1) >= -1) {
				dirCanGo[1][DOWN] = DOWN;
			}

			if (etTowards[1] == DOWN || etTowards[1] == RIGHT) {
				dirCanGo[1][etTowards[1] - 2] = -1;
			}
			if (etTowards[1] == UP || etTowards[1] == LEFT) {
				dirCanGo[1][etTowards[1] + 2] = -1;
			}


			for (int i = 3; i > -1; i--) {
				if (dirCanGo[1][i] == i) {
					if (Distance(etPink[etTowards[1]], 1, i) <= mindis[1]) {
						mindis[1] = Distance(etPink[etTowards[1]], 1, i);
						nextdir[1] = i;
					}
				}
			}

			if (etPink[etTowards[1]].IsBitmapLoaded()) {
				if (etPink[etTowards[1]].GetRight() >= 440 || etPink[etTowards[1]].GetLeft() <= 10) {
					return;
				}
				etPink[nextdir[1]].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop());
				etTowards[1] = nextdir[1];
			}
		}
	}
	else if (mode == 'b') {
		if (etBlue[etTowards[2]].CenterX() != target[2][0] || etBlue[etTowards[2]].CenterY() != target[2][1]) {
			if (GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetTop() + edge) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetBottom() - edge) >= -1) {
				dirCanGo[2][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetTop() + edge) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetBottom() - edge) >= -1) {
				dirCanGo[2][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetTop() - 1) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetTop() - 1) >= -1) {
				dirCanGo[2][UP] = UP;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetBottom() + 1) >= -1 && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetBottom() + 1) >= -1) {
				dirCanGo[2][DOWN] = DOWN;
			}

			if (etTowards[2] == DOWN || etTowards[2] == RIGHT) {
				dirCanGo[2][etTowards[2] - 2] = -1;
			}
			if (etTowards[2] == UP || etTowards[2] == LEFT) {
				dirCanGo[2][etTowards[2] + 2] = -1;
			}


			for (int i = 3; i > -1; i--) {
				if (dirCanGo[2][i] == i) {
					if (Distance(etBlue[etTowards[2]], 2, i) <= mindis[2]) {
						mindis[2] = Distance(etBlue[etTowards[2]], 2, i);
						nextdir[2] = i;
					}
				}
			}

			if (etBlue[etTowards[2]].IsBitmapLoaded()) {
				if (etBlue[etTowards[2]].GetRight() >= 440 || etBlue[etTowards[2]].GetLeft() <= 10) {
					return;
				}
				etBlue[nextdir[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop());
				etTowards[2] = nextdir[2];
			}
		}
	}
	else if (mode == 'y') {
		if (etYellow[etTowards[3]].CenterX() != target[3][0] || etYellow[etTowards[3]].CenterY() != target[3][1]) {
			if (GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetTop() + edge) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetBottom() - edge) >= -1) {
				dirCanGo[3][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetTop() + edge) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetBottom() - edge) >= -1) {
				dirCanGo[3][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetTop() - 1) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetTop() - 1) >= -1) {
				dirCanGo[3][UP] = UP;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetBottom() + 1) >= -1 && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetBottom() + 1) >= -1) {
				dirCanGo[3][DOWN] = DOWN;
			}

			if (etTowards[3] == DOWN || etTowards[3] == RIGHT) {
				dirCanGo[3][etTowards[3] - 2] = -1;
			}
			if (etTowards[3] == UP || etTowards[3] == LEFT) {
				dirCanGo[3][etTowards[3] + 2] = -1;
			}


			for (int i = 3; i > -1; i--) {
				if (dirCanGo[3][i] == i) {
					if (Distance(etYellow[etTowards[3]], 3, i) <= mindis[3]) {
						mindis[3] = Distance(etYellow[etTowards[3]], 3, i);
						nextdir[3] = i;
					}
				}
			}

			if (etYellow[etTowards[3]].IsBitmapLoaded()) {
				if (etYellow[etTowards[3]].GetRight() >= 440 || etYellow[etTowards[3]].GetLeft() <= 10) {
					return;
				}
				etYellow[nextdir[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop());
				etTowards[3] = nextdir[3];
			}
		}
	}
}

void CGameStateRun::TurnAround(int dirR, int dirP, int dirB, int dirY) {
	/* red ghost */
	if (dirR == UP || dirR == LEFT) {
		etRed[dirR + 2].SetTopLeft(etRed[dirR].GetLeft(), etRed[dirR].GetTop());
		etTowards[0] = dirR + 2;
	}
	else if (dirR == DOWN || dirR == RIGHT) {
		etRed[dirR - 2].SetTopLeft(etRed[dirR].GetLeft(), etRed[dirR].GetTop());
		etTowards[0] = dirR - 2;
	}

	/* pink ghost */
	if (dirP == UP || dirP == LEFT) {
		etPink[dirP + 2].SetTopLeft(etPink[dirP].GetLeft(), etPink[dirP].GetTop());
		etTowards[1] = dirP + 2;
	}
	else if (dirP == DOWN || dirP == RIGHT) {
		etPink[dirP - 2].SetTopLeft(etPink[dirP].GetLeft(), etPink[dirP].GetTop());
		etTowards[1] = dirP - 2;
	}

	/* blue ghost */
	if (dirB == UP || dirB == LEFT) {
		etBlue[dirB + 2].SetTopLeft(etBlue[dirB].GetLeft(), etBlue[dirB].GetTop());
		etTowards[2] = dirB + 2;
	}
	else if (dirB == DOWN || dirB == RIGHT) {
		etBlue[dirB - 2].SetTopLeft(etBlue[dirB].GetLeft(), etBlue[dirB].GetTop());
		etTowards[2] = dirB - 2;
	}

	/* yellow ghost */
	if (dirY == UP || dirY == LEFT) {
		etYellow[dirY + 2].SetTopLeft(etYellow[dirY].GetLeft(), etYellow[dirY].GetTop());
		etTowards[3] = dirY + 2;
	}
	else if (dirY == DOWN || dirY == RIGHT) {
		etYellow[dirY - 2].SetTopLeft(etYellow[dirY].GetLeft(), etYellow[dirY].GetTop());
		etTowards[3] = dirY - 2;
	}
}

int CGameStateRun::GetPixelAttribute(int left, int top) {
	return map[phase - 1][top / PIXHEIGHT][left / PIXWIDTH];
}

double CGameStateRun::Distance(MyBitmap mb1, int mode, int nextdir) {
	if (nextdir == RIGHT) {
		return sqrt(pow((int)mb1.CenterX() / 16 + 1 - target[mode][0] / 16, 2) + pow((int)mb1.CenterY() / 16 - target[mode][1] / 16, 2));
	}
	else if (nextdir == LEFT) {
		return sqrt(pow((int)mb1.CenterX() / 16 - 1 - target[mode][0] / 16, 2) + pow((int)mb1.CenterY() / 16 - target[mode][1] / 16, 2));
	}
	else if (nextdir == UP) {
		return sqrt(pow((int)mb1.CenterX() / 16 - target[mode][0] / 16, 2) + pow((int)mb1.CenterY() / 16 - 1 - target[mode][1] / 16, 2));
	}
	else if (nextdir == DOWN) {
		return sqrt(pow((int)mb1.CenterX() / 16 - target[mode][0] / 16, 2) + pow((int)mb1.CenterY() / 16 + 1 - target[mode][1] / 16, 2));
	}
	return 2000000000;
}

double CGameStateRun::Distance(MyBitmap mb1, MyBitmap mb2) {
	return sqrt(pow((int)mb1.CenterX() / 16 - (int)mb2.CenterX() / 16, 2) + pow((int)mb1.CenterY() / 16 - (int)mb2.CenterY() / 16, 2));
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