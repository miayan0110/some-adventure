#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "myfunc.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <time.h>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////
// Sound Effect Pointer Init
///////////////////////////////////////////

/* start music */
CAudio *readyBGM = CAudio::Instance();

/* game background music */
CAudio *bgm = CAudio::Instance();

/* sound effect when pacman died */
CAudio *pacDiedSE = CAudio::Instance();

/* scared mode background music */
CAudio *scaredModeBgm = CAudio::Instance();

/* sound effect when ghost is eaten */
CAudio *ghostDiedSE = CAudio::Instance();

/* sound effect while ghost going back to ghost house*/
CAudio *ghostReturnToHouseSE = CAudio::Instance();

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
	if (isPacDied) {
		if (Wait(1000, died_time, 300)) {
			showPacDiedAni = true;
			character[PACDIED].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
			towards = PACDIED;
			character[towards].ToggleAnimation();
		}
	}

	if (Delay(5*1000, start_time) && !isPacDied && phaseClear) {
		/* pacman direction */
		PacmanDir();

		/* pacman movement - go straight */
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
		}// through right tunnel
		else if (character[towards].GetRight() >= 435) {
			towards = 4;
			if (Delay(1200)) {
				towards = RIGHT;
				character[towards].SetTopLeft(1, character[towards].GetTop());
			}
		}// through left tunnel
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

		/* ghost movement */
		// red
		if ((etRed[etTowards[0]].GetLeft() % 16 <= 0 && etRed[etTowards[0]].GetTop() % 16 <= 0) || (etRed[etTowards[0]].GetLeft() % 16 >= 15 && etRed[etTowards[0]].GetTop() % 16 >= 15)) {
			if (isDied[0]) DiedMode('r');
			else if (roadGhostCanGo[0] == -1) InitMode('r');
			else if (ghostmode == 0 || showOrigin[0]) ChaseMode('r');
			else if (ghostmode == 1) ScatterMode('r');
			else if (ghostmode >= 2) ScaredMode('r');
			NextDir('r');
		}
		if (etTowards[0] == RIGHT && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetTop() + edge) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetBottom() - edge) >= roadGhostCanGo[0]) {
			etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft() + ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[SCARED].SetTopLeft(etRed[etTowards[0]].GetLeft() + ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[TRANSIT].SetTopLeft(etRed[etTowards[0]].GetLeft() + ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[DIED].SetFrameIndexOfBitmap(RIGHT);
			etRed[DIED].SetTopLeft(etRed[etTowards[0]].GetLeft() + ghostspeed[0], etRed[etTowards[0]].GetTop());
		}
		else if (etTowards[0] == LEFT && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetTop() + edge) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetBottom() - edge) >= roadGhostCanGo[0]) {
			etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft() - ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[SCARED].SetTopLeft(etRed[etTowards[0]].GetLeft() - ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[TRANSIT].SetTopLeft(etRed[etTowards[0]].GetLeft() - ghostspeed[0], etRed[etTowards[0]].GetTop());
			etRed[DIED].SetFrameIndexOfBitmap(LEFT);
			etRed[DIED].SetTopLeft(etRed[etTowards[0]].GetLeft() - ghostspeed[0], etRed[etTowards[0]].GetTop());
		}
		else if (etTowards[0] == UP && GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetTop() - 1) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetTop() - 1) >= roadGhostCanGo[0]) {
			etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() - ghostspeed[0]);
			etRed[SCARED].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() - ghostspeed[0]);
			etRed[TRANSIT].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() - ghostspeed[0]);
			etRed[DIED].SetFrameIndexOfBitmap(UP);
			etRed[DIED].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() - ghostspeed[0]);
		}
		else if (etTowards[0] == DOWN && GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetBottom() + 1) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetBottom() + 1) >= roadGhostCanGo[0]) {
			etRed[etTowards[0]].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() + ghostspeed[0]);
			etRed[SCARED].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() + ghostspeed[0]);
			etRed[TRANSIT].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() + ghostspeed[0]);
			etRed[DIED].SetFrameIndexOfBitmap(DOWN);
			etRed[DIED].SetTopLeft(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop() + ghostspeed[0]);
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
			if (isDied[0]) DiedMode('r');
			else if (roadGhostCanGo[0] == -1) InitMode('r');
			else if (ghostmode == 0 || showOrigin[0]) ChaseMode('r');
			else if (ghostmode == 1) ScatterMode('r');
			else if (ghostmode >= 2) ScaredMode('r');
			NextDir('r');
		}
		else {
			if (isDied[0]) DiedMode('r');
			else if (roadGhostCanGo[0] == -1) InitMode('r');
			else if (ghostmode == 0 || showOrigin[0]) ChaseMode('r');
			else if (ghostmode == 1) ScatterMode('r');
			else if (ghostmode >= 2) ScaredMode('r');
			NextDir('r');
		}

		/* unshow cookies */
		for (int i = 0; i < cookieAmount; i++) {
			if (cookie[i].IsEaten(cookie[i], character[towards]) && eatenCookie[i]) {
				cookie[i].SetFrameIndexOfBitmap(1);
				eatenCookie[i] = 0;
				score += 10;
				cookieEaten = 1;
			}
		}

		for (int i = 0; i < spCookieAmount; i++) {
			if (spCookie[i].IsEaten(character[towards], spCookie[i]) && eatenSP[i]) {
				spCookie[i].UnshowBitmap();
				spCookie[i].SetFrameIndexOfBitmap(1);
				eatenSP[i] = 0;
				ghostmode = 2;
				scared_start = clock();
				score += 50;
				cookieEaten = 1;
				scaredModeBgm->Play(5, true);
				bgm->Stop(1);
				restartBgm = 1;
			}
		}

		/* pacman died */
		if (!superPlayer) {
			if (life >= 0 && ghostmode < 2) {
				if (etRed[etTowards[0]].IsEaten(etRed[etTowards[0]], character[towards])) {
					died_time = clock();
					bgm->Stop(1);
					ghostReturnToHouseSE->Stop(7);
					scaredModeBgm->Stop(5);
					isPacDied = true;
					pacDiedSE->Play(4);	/* pacman died sound effect */
				}
				else if (etPink[etTowards[1]].IsEaten(etPink[etTowards[1]], character[towards])) {
					died_time = clock();
					bgm->Stop(1);
					ghostReturnToHouseSE->Stop(7);
					scaredModeBgm->Stop(5);
					isPacDied = true;
					pacDiedSE->Play(4);	/* pacman died sound effect */
				}
				else if (etBlue[etTowards[2]].IsEaten(etBlue[etTowards[2]], character[towards])) {
					died_time = clock();
					bgm->Stop(1);
					ghostReturnToHouseSE->Stop(7);
					scaredModeBgm->Stop(5);
					isPacDied = true;	/* pacman died sound effect */
					pacDiedSE->Play(4);
				}
				else if (etYellow[etTowards[3]].IsEaten(etYellow[etTowards[3]], character[towards])) {
					died_time = clock();
					bgm->Stop(1);
					ghostReturnToHouseSE->Stop(7);
					scaredModeBgm->Stop(5);
					isPacDied = true;
					pacDiedSE->Play(4);	/* pacman died sound effect */
				}
			}
		}

		/* ghost died */
		if (ghostmode > 1) {
			if (etRed[etTowards[0]].IsEaten(character[towards], etRed[etTowards[0]]) && !showOrigin[0] && !isDied[0]) {
				eat_time = clock();
				eatenLeft = etRed[etTowards[0]].GetLeft();
				eatenTop = etRed[etTowards[0]].GetTop();
				etRed[DIED].SetTopLeft(eatenLeft, eatenTop);
				isDied[0] = true;
				stuffEaten[eatenGhostAmount] = 1;
				score += int(pow(2, eatenGhostAmount++)) * 200;
				scaredModeBgm->Stop(5);
				ghostDiedSE->Play(6);
				ghostReturnToHouseSE->Play(7);
			}
			if (etPink[etTowards[1]].IsEaten(character[towards], etPink[etTowards[1]]) && !showOrigin[1] && !isDied[1]) {
				eat_time = clock();
				eatenLeft = etPink[etTowards[1]].GetLeft();
				eatenTop = etPink[etTowards[1]].GetTop();
				etPink[DIED].SetTopLeft(eatenLeft, eatenTop);
				isDied[1] = true;
				stuffEaten[eatenGhostAmount] = 1;
				score += int(pow(2, eatenGhostAmount++)) * 200;
				scaredModeBgm->Stop(5);
				ghostDiedSE->Play(6);
				ghostReturnToHouseSE->Play(7);
			}
			if (etBlue[etTowards[2]].IsEaten(character[towards], etBlue[etTowards[2]]) && !showOrigin[2] && !isDied[2]) {
				eat_time = clock();
				eatenLeft = etBlue[etTowards[2]].GetLeft();
				eatenTop = etBlue[etTowards[2]].GetTop();
				etBlue[DIED].SetTopLeft(eatenLeft, eatenTop);
				isDied[2] = true;
				stuffEaten[eatenGhostAmount] = 1;
				score += int(pow(2, eatenGhostAmount++)) * 200;
				scaredModeBgm->Stop(5);
				ghostDiedSE->Play(6);
				ghostReturnToHouseSE->Play(7);
			}
			if (etYellow[etTowards[3]].IsEaten(character[towards], etYellow[etTowards[3]]) && !showOrigin[3] && !isDied[3]) {
				eat_time = clock();
				eatenLeft = etYellow[etTowards[3]].GetLeft();
				eatenTop = etYellow[etTowards[3]].GetTop();
				etYellow[DIED].SetTopLeft(eatenLeft, eatenTop);
				isDied[3] = true;
				stuffEaten[eatenGhostAmount] = 1;
				score += int(pow(2, eatenGhostAmount++)) * 200;
				scaredModeBgm->Stop(5);
				ghostDiedSE->Play(6);
				ghostReturnToHouseSE->Play(7);
			}
		}
	}

	if (Delay(7 * 1000, start_time) && !isPacDied && phaseClear) {
		// pink
		if ((etPink[etTowards[1]].GetLeft() % 16 <= 0 && etPink[etTowards[1]].GetTop() % 16 <= 0) || (etPink[etTowards[1]].GetLeft() % 16 >= 15 && etPink[etTowards[1]].GetTop() % 16 >= 15)) {
			if (isDied[1]) DiedMode('p');
			else if (roadGhostCanGo[1] == -1) InitMode('p');
			else if (ghostmode == 0 || showOrigin[1]) ChaseMode('p');
			else if (ghostmode == 1) ScatterMode('p');
			else if (ghostmode >= 2) ScaredMode('p');
			NextDir('p');
		}
		if (etTowards[1] == RIGHT && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetTop() + edge) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetBottom() - edge) >= roadGhostCanGo[1]) {
			etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft() + ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[SCARED].SetTopLeft(etPink[etTowards[1]].GetLeft() + ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[TRANSIT].SetTopLeft(etPink[etTowards[1]].GetLeft() + ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[DIED].SetFrameIndexOfBitmap(RIGHT);
			etPink[DIED].SetTopLeft(etPink[etTowards[1]].GetLeft() + ghostspeed[1], etPink[etTowards[1]].GetTop());
		}
		else if (etTowards[1] == LEFT && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetTop() + edge) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetBottom() - edge) >= roadGhostCanGo[1]) {
			etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft() - ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[SCARED].SetTopLeft(etPink[etTowards[1]].GetLeft() - ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[TRANSIT].SetTopLeft(etPink[etTowards[1]].GetLeft() - ghostspeed[1], etPink[etTowards[1]].GetTop());
			etPink[DIED].SetFrameIndexOfBitmap(LEFT);
			etPink[DIED].SetTopLeft(etPink[etTowards[1]].GetLeft() - ghostspeed[1], etPink[etTowards[1]].GetTop());
		}
		else if (etTowards[1] == UP && GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetTop() - 1) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetTop() - 1) >= roadGhostCanGo[1]) {
			etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() - ghostspeed[1]);
			etPink[SCARED].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() - ghostspeed[1]);
			etPink[TRANSIT].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() - ghostspeed[1]);
			etPink[DIED].SetFrameIndexOfBitmap(UP);
			etPink[DIED].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() - ghostspeed[1]);
		}
		else if (etTowards[1] == DOWN && GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetBottom() + 1) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetBottom() + 1) >= roadGhostCanGo[1]) {
			etPink[etTowards[1]].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() + ghostspeed[1]);
			etPink[SCARED].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() + ghostspeed[1]);
			etPink[TRANSIT].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() + ghostspeed[1]);
			etPink[DIED].SetFrameIndexOfBitmap(DOWN);
			etPink[DIED].SetTopLeft(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop() + ghostspeed[1]);
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
			if (isDied[1]) DiedMode('p');
			else if (roadGhostCanGo[1] == -1) InitMode('p');
			else if (ghostmode == 0 || showOrigin[1]) ChaseMode('p');
			else if (ghostmode == 1) ScatterMode('p');
			else if (ghostmode >= 2) ScaredMode('p');
			NextDir('p');
		}
		else {
			if (isDied[1]) DiedMode('p');
			else if (roadGhostCanGo[1] == -1) InitMode('p');
			else if (ghostmode == 0 || showOrigin[1]) ChaseMode('p');
			else if (ghostmode == 1) ScatterMode('p');
			else if (ghostmode >= 2) ScaredMode('p');
			NextDir('p');
		}
	}

	if (Delay(8 * 1000, start_time) && !isPacDied && phaseClear) {
		// blue
		if ((etBlue[etTowards[2]].GetLeft() % 16 <= 0 && etBlue[etTowards[2]].GetTop() % 16 <= 0) || (etBlue[etTowards[2]].GetLeft() % 16 >= 15 && etBlue[etTowards[2]].GetTop() % 16 >= 15)) {
			if (isDied[2]) DiedMode('b');
			else if (roadGhostCanGo[2] == -1) InitMode('b');
			else if (ghostmode == 0 || showOrigin[2]) ChaseMode('b');
			else if (ghostmode == 1) ScatterMode('b');
			else if (ghostmode >= 2) ScaredMode('b');
			NextDir('b');
		}
		if (etTowards[2] == RIGHT && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetTop() + edge) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetBottom() - edge) >= roadGhostCanGo[2]) {
			etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft() + ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[SCARED].SetTopLeft(etBlue[etTowards[2]].GetLeft() + ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[TRANSIT].SetTopLeft(etBlue[etTowards[2]].GetLeft() + ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[DIED].SetFrameIndexOfBitmap(RIGHT);
			etBlue[DIED].SetTopLeft(etBlue[etTowards[2]].GetLeft() + ghostspeed[2], etBlue[etTowards[2]].GetTop());
		}
		else if (etTowards[2] == LEFT && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetTop() + edge) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetBottom() - edge) >= roadGhostCanGo[2]) {
			etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft() - ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[SCARED].SetTopLeft(etBlue[etTowards[2]].GetLeft() - ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[TRANSIT].SetTopLeft(etBlue[etTowards[2]].GetLeft() - ghostspeed[2], etBlue[etTowards[2]].GetTop());
			etBlue[DIED].SetFrameIndexOfBitmap(LEFT);
			etBlue[DIED].SetTopLeft(etBlue[etTowards[2]].GetLeft() - ghostspeed[2], etBlue[etTowards[2]].GetTop());
		}
		else if (etTowards[2] == UP && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetTop() - 1) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetTop() - 1) >= roadGhostCanGo[2]) {
			etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() - ghostspeed[2]);
			etBlue[SCARED].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() - ghostspeed[2]);
			etBlue[TRANSIT].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() - ghostspeed[2]);
			etBlue[DIED].SetFrameIndexOfBitmap(UP);
			etBlue[DIED].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() - ghostspeed[2]);
		}
		else if (etTowards[2] == DOWN && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetBottom() + 1) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetBottom() + 1) >= roadGhostCanGo[2]) {
			etBlue[etTowards[2]].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() + ghostspeed[2]);
			etBlue[SCARED].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() + ghostspeed[2]);
			etBlue[TRANSIT].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() + ghostspeed[2]);
			etBlue[DIED].SetFrameIndexOfBitmap(DOWN);
			etBlue[DIED].SetTopLeft(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop() + ghostspeed[2]);
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
			if (isDied[2]) DiedMode('b');
			else if (roadGhostCanGo[2] == -1) InitMode('b');
			else if (ghostmode == 0 || showOrigin[2]) ChaseMode('b');
			else if (ghostmode == 1) ScatterMode('b');
			else if (ghostmode >= 2) ScaredMode('b');
			NextDir('b');
		}
		else {
			if (isDied[2]) DiedMode('b');
			else if (roadGhostCanGo[2] == -1) InitMode('b');
			else if (ghostmode == 0 || showOrigin[2]) ChaseMode('b');
			else if (ghostmode == 1) ScatterMode('b');
			else if (ghostmode >= 2) ScaredMode('b');
			NextDir('b');
		}
	}

	if (Delay(11 * 1000, start_time) && !isPacDied && phaseClear) {
		// yellow
		if ((etYellow[etTowards[3]].GetLeft() % 16 <= 0 && etYellow[etTowards[3]].GetTop() % 16 <= 0) || (etYellow[etTowards[3]].GetLeft() % 16 >= 15 && etYellow[etTowards[3]].GetTop() % 16 >= 15)) {
			if (isDied[3]) DiedMode('y');
			else if (roadGhostCanGo[3] == -1) InitMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) > 8 && ghostmode == 0 || showOrigin[3]) ChaseMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) <= 8 || ghostmode == 1) ScatterMode('y');
			else if (ghostmode >= 2) ScaredMode('y');
			NextDir('y');
		}
		if (etTowards[3] == RIGHT && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetTop() + edge) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetBottom() - edge) >= roadGhostCanGo[3]) {
			etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft() + ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[SCARED].SetTopLeft(etYellow[etTowards[3]].GetLeft() + ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[TRANSIT].SetTopLeft(etYellow[etTowards[3]].GetLeft() + ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[DIED].SetFrameIndexOfBitmap(RIGHT);
			etYellow[DIED].SetTopLeft(etYellow[etTowards[3]].GetLeft() + ghostspeed[3], etYellow[etTowards[3]].GetTop());
		}
		else if (etTowards[3] == LEFT && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetTop() + edge) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetBottom() - edge) >= roadGhostCanGo[3]) {
			etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft() - ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[SCARED].SetTopLeft(etYellow[etTowards[3]].GetLeft() - ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[TRANSIT].SetTopLeft(etYellow[etTowards[3]].GetLeft() - ghostspeed[3], etYellow[etTowards[3]].GetTop());
			etYellow[DIED].SetFrameIndexOfBitmap(LEFT);
			etYellow[DIED].SetTopLeft(etYellow[etTowards[3]].GetLeft() - ghostspeed[3], etYellow[etTowards[3]].GetTop());
		}
		else if (etTowards[3] == UP && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetTop() - 1) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetTop() - 1) >= roadGhostCanGo[3]) {
			etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() - ghostspeed[3]);
			etYellow[SCARED].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() - ghostspeed[3]);
			etYellow[TRANSIT].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() - ghostspeed[3]);
			etYellow[DIED].SetFrameIndexOfBitmap(UP);
			etYellow[DIED].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() - ghostspeed[3]);
		}
		else if (etTowards[3] == DOWN && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetBottom() + 1) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetBottom() + 1) >= roadGhostCanGo[3]) {
			etYellow[etTowards[3]].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() + ghostspeed[3]);
			etYellow[SCARED].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() + ghostspeed[3]);
			etYellow[TRANSIT].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() + ghostspeed[3]);
			etYellow[DIED].SetFrameIndexOfBitmap(DOWN);
			etYellow[DIED].SetTopLeft(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop() + ghostspeed[3]);
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
			if (isDied[3]) DiedMode('y');
			else if (roadGhostCanGo[3] == -1) InitMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) > 8 && ghostmode == 0 || showOrigin[3]) ChaseMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) <= 8 || ghostmode == 1) ScatterMode('y');
			else if (ghostmode >= 2) ScaredMode('y');
			NextDir('y');
		}
		else {
			if (isDied[3]) DiedMode('y');
			else if (roadGhostCanGo[3] == -1) InitMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) > 8 && ghostmode == 0 || showOrigin[3]) ChaseMode('y');
			else if (Distance(etYellow[etTowards[3]], character[towards]) <= 8 || ghostmode == 1) ScatterMode('y');
			else if (ghostmode >= 2) ScaredMode('y');
			NextDir('y');
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	///////////////////////////////////////////
	// Sound Effect Pointer Init
	///////////////////////////////////////////

	/* start music */
	readyBGM->Load(3, "resources/sound/ready.wav");

	/* game background music */
	bgm->Load(1, "resources/sound/bgm.wav");

	/* sound effect for pacman died */
	pacDiedSE->Load(4, "resources/sound/pacman_died.wav");

	/* scared mode background music */
	scaredModeBgm->Load(5, "resources/sound/ghost_scaredMode.wav");

	/* sound effect when ghost is eaten */
	ghostDiedSE->Load(6, "resources/sound/ghost_died.mp3");

	/* sound effect while ghost going back to ghost house*/
	ghostReturnToHouseSE->Load(7, "resources/sound/ghost_return_to_house.mp3");

	///////////////////////////////////////////
	// UI 
	///////////////////////////////////////////

	/* player lives */
	for (int i = 0; i < 3; i++) {
		lives[i].LoadBitmapByString({
			"resources/pacman/pacman_towardL_1.bmp",
			"resources/pacman/pacman_unshow.bmp",
			}, RGB(255, 255, 255));
		lives[i].SetTopLeft(16 + 30 * i, 550);
	}

	/* player amount */
	oneP.LoadBitmapByString({
		"resources/stuff/1up.bmp"
		}, RGB(255, 255, 255));
	oneP.SetTopLeft(0, 15);

	/* score board */
	for (int i = 0; i < 7; i++) {
		scoreNum[i].LoadBitmapByString({
			"resources/stuff/0.bmp",
			"resources/stuff/1.bmp",
			"resources/stuff/2.bmp",
			"resources/stuff/3.bmp",
			"resources/stuff/4.bmp",
			"resources/stuff/5.bmp",
			"resources/stuff/6.bmp",
			"resources/stuff/7.bmp",
			"resources/stuff/8.bmp",
			"resources/stuff/9.bmp",
			"resources/stuff/num_empty.bmp"
			}, RGB(255, 255, 255));
		scoreNum[i].SetTopLeft(168 - 18 * i, 15);
	}

	/* bonus points for eating ghost */
	bonusPoints[0].LoadBitmapByString({
		"resources/stuff/200.bmp",
		}, RGB(255, 255, 255));

	bonusPoints[1].LoadBitmapByString({
		"resources/stuff/400.bmp",
		}, RGB(255, 255, 255));

	bonusPoints[2].LoadBitmapByString({
		"resources/stuff/800.bmp",
		}, RGB(255, 255, 255));

	bonusPoints[3].LoadBitmapByString({
		"resources/stuff/1600.bmp",
		}, RGB(255, 255, 255));

	/* ready */
	ready.LoadBitmapByString({
		"resources/stuff/ready.bmp"
		}, RGB(255, 255, 255));
	ready.SetTopLeft(182, 320);

	/* transition */
	trans.LoadBitmapByString({
		"resources/stuff/black_circle.bmp"
		}, RGB(255, 255, 255));
	trans.SetTopLeft(0, 0);
	
	/* super player icon */
	superIcon.LoadBitmapByString({
		"resources/stuff/super_player_icon.bmp"
		}, RGB(255, 255, 255));
	superIcon.SetTopLeft(410, 550);

	/* gameover */
	gameover.LoadBitmapByString({
		"resources/stuff/gameover.bmp",
		}, RGB(255, 255, 255));
	gameover.SetTopLeft(146, 320);

	///////////////////////////////////////////
	// GAME
	///////////////////////////////////////////

	/* init flags */
	isGameStartInit = true;
	isMapLoaded = false;

	/* load map */
	background.LoadBitmapByString({
		"resources/map/phase_1.bmp",
		"resources/map/phase_2.bmp",
		"resources/map/phase_3.bmp",
		"resources/map/phase_4.bmp",
		"resources/map/phase_5.bmp",
		"resources/map/phase_6.bmp",
		"resources/map/phase_7.bmp",
		"resources/map/phase_8.bmp",
		"resources/map/phase_9.bmp",
		"resources/map/phase_10.bmp",
		"resources/map/phase_11.bmp",
		"resources/map/phase_12.bmp",
		"resources/map/phase_13.bmp",
		"resources/map/phase_14.bmp",
		"resources/map/phase_15.bmp",
		"resources/map/phase_16.bmp",
		"resources/map/phase_17.bmp",
		"resources/map/phase_18.bmp",
		"resources/map/phase_19.bmp",
		"resources/map/phase_20.bmp",
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
	character[RIGHT].SetTopLeft(216, 416);
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

	character[UNSHOW].LoadBitmapByString({
		"resources/pacman/pacman_unshow.bmp",
		"resources/pacman/pacman_unshow.bmp",
		}, RGB(255, 255, 255));

	character[PACDIED].LoadBitmapByString({
		"resources/pacman/pacman_circle.bmp",
		"resources/pacman/pacman_died_1.bmp",
		"resources/pacman/pacman_died_2.bmp",
		"resources/pacman/pacman_died_3.bmp",
		"resources/pacman/pacman_died_4.bmp",
		"resources/pacman/pacman_died_5.bmp",
		"resources/pacman/pacman_died_6.bmp",
		"resources/pacman/pacman_died_7.bmp",
		"resources/pacman/pacman_died_8.bmp",
		"resources/pacman/pacman_died_9.bmp",
		"resources/pacman/pacman_died_10.bmp",
		"resources/pacman/pacman_died_11.bmp",
		"resources/pacman/pacman_unshow.bmp",
		}, RGB(255, 255, 255));
	character[PACDIED].SetAnimation(100, 1);

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
	
	etRed[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etRed[SCARED].SetAnimation(100, 0);

	etRed[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etRed[TRANSIT].SetAnimation(500, 0);

	etRed[DIED].LoadBitmapByString({
		"resources/ets/et_diedU.bmp",
		"resources/ets/et_diedL.bmp",
		"resources/ets/et_diedD.bmp",
		"resources/ets/et_diedR.bmp",
		}, RGB(255, 255, 255));

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
	
	etPink[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etPink[SCARED].SetAnimation(100, 0);

	etPink[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etPink[TRANSIT].SetAnimation(500, 0);

	etPink[DIED].LoadBitmapByString({
		"resources/ets/et_diedU.bmp",
		"resources/ets/et_diedL.bmp",
		"resources/ets/et_diedD.bmp",
		"resources/ets/et_diedR.bmp",
		}, RGB(255, 255, 255));

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
	
	etBlue[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[SCARED].SetAnimation(100, 0);

	etBlue[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etBlue[TRANSIT].SetAnimation(500, 0);

	etBlue[DIED].LoadBitmapByString({
		"resources/ets/et_diedU.bmp",
		"resources/ets/et_diedL.bmp",
		"resources/ets/et_diedD.bmp",
		"resources/ets/et_diedR.bmp",
		}, RGB(255, 255, 255));

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
	
	etYellow[SCARED].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_scared_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[SCARED].SetAnimation(100, 0);

	etYellow[TRANSIT].LoadBitmapByString({
		"resources/ets/et_scared_1.bmp",
		"resources/ets/et_transit_2.bmp",
		}, RGB(255, 255, 255));
	etYellow[TRANSIT].SetAnimation(500, 0);

	etYellow[DIED].LoadBitmapByString({
		"resources/ets/et_diedU.bmp",
		"resources/ets/et_diedL.bmp",
		"resources/ets/et_diedD.bmp",
		"resources/ets/et_diedR.bmp",
		}, RGB(255, 255, 255));

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/* pacman control */
	if (nChar == VK_RIGHT) {	// going right
		keyDir = RIGHT;
	}
	else if (nChar == VK_LEFT) {	// going left
		keyDir = LEFT;
	}
	else if (nChar == VK_UP) {	// going up
		keyDir = UP;
	}
	else if (nChar == VK_DOWN) {	// going down
		keyDir = DOWN;
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
	else if (nChar == 0x32) {
		/* super player */
		superPlayer = !superPlayer;
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
	if (phase < 21) {
		ShowByPhase();
		ShowUI();
		BgmPlayer();
	}
	else {
		GotoGameState(GAME_STATE_OVER);
	}
}

void CGameStateRun::ShowUI() {
	/* show player mode */
	oneP.ShowBitmap(2);

	/* show if is super player */
	if (superPlayer) superIcon.ShowBitmap(2);

	/* show ready */
	if (!Delay(5 * 1000, start_time)) ready.ShowBitmap(2);

	/* show player lives */
	if (life == 3 && Delay(5 * 1000, start_time)) {
		life--;
	}
	for (int i = 0; i < life; i++) {
		lives[i].ShowBitmap(1.25);
	}

	/* show score board */
	for (int i = 6; i >= 0; i--) {
		int len = to_string(score).size();
		if (i >= len) {
			scoreNum[i].SetFrameIndexOfBitmap(10);
			scoreNum[i].ShowBitmap(2);
		}
		else {
			scoreNum[i].SetFrameIndexOfBitmap(to_string(score)[len-i-1] - '0');
			scoreNum[i].ShowBitmap(2);
		}
	}

	/* show gameover */
	if (life < 0 && Delay(35 * 100, died_time)) gameover.ShowBitmap(2);

	/* transit to next phase */
	if (nextPhaseTrans) {
		for (int i = 0; i < 100; i++) {
			if (Delay(5 * 10 * i + 1500, end_time)) {
				trans.ShowBitmap(1 * i);
			}
		}
	}
}

void CGameStateRun::ShowByPhase() {
	/* check and change phase */
	if (phase < 21) {
		if (!isMapLoaded) {
			InitStartTime();
			InitMap();
			InitCharacter();
		}

		if (ghostmode != lastGhostmode && lastGhostmode < 2) {
			TurnAround(etTowards[0], etTowards[1], etTowards[2], etTowards[3]);
		}
		TimeController();

		CheckPhaseClear();
	}

	if (phase < 21) {
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

		/* show points get if ghost is eaten */
		for (int i = 0; i < 4; i++) {
			ShowBonusPoint(stuffEaten[i], i);
		}

		/* show ghosts */
		if (ghostmode < 2 && !showPacDiedAni) {
			for (int i = 0; i < 4; i++) {
				showOrigin[i] = false;
			}
			if (!isDied[0]) etRed[etTowards[0]].ShowBitmap(1.25);
			else etRed[DIED].ShowBitmap(1.25);

			if (!isDied[1]) etPink[etTowards[1]].ShowBitmap(1.25);
			else etPink[DIED].ShowBitmap(1.25);

			if (!isDied[2]) etBlue[etTowards[2]].ShowBitmap(1.25);
			else etBlue[DIED].ShowBitmap(1.25);

			if (!isDied[3]) etYellow[etTowards[3]].ShowBitmap(1.25);
			else etYellow[DIED].ShowBitmap(1.25);
		}
		else if (ghostmode == 2 && !showPacDiedAni) {
			etRed[SCARED + isDied[0] * 2].ShowBitmap(1.25);
			etPink[SCARED + isDied[1] * 2].ShowBitmap(1.25);
			etBlue[SCARED + isDied[2] * 2].ShowBitmap(1.25);
			etYellow[SCARED + isDied[3] * 2].ShowBitmap(1.25);
		}
		else if (ghostmode == 3 && !showPacDiedAni) {
			etRed[TRANSIT + isDied[0]].ShowBitmap(1.25);
			etPink[TRANSIT + isDied[1]].ShowBitmap(1.25);
			etBlue[TRANSIT + isDied[2]].ShowBitmap(1.25);
			etYellow[TRANSIT + isDied[3]].ShowBitmap(1.25);
		}
		if (showOrigin[0] && !showPacDiedAni) etRed[etTowards[0]].ShowBitmap(1.25);
		if (showOrigin[1] && !showPacDiedAni) etPink[etTowards[1]].ShowBitmap(1.25);
		if (showOrigin[2] && !showPacDiedAni) etBlue[etTowards[2]].ShowBitmap(1.25);
		if (showOrigin[3] && !showPacDiedAni) etYellow[etTowards[3]].ShowBitmap(1.25);

		if (character[towards].IsAnimationDone() && isPacDied && life > 0) {
			if (Delay(4 * 1000, died_time)) {
				InitStartTime();
				InitCharacter();
				life--;
			}
		}
		else if (character[towards].IsAnimationDone() && isPacDied && life == 0) {
			life--;
		}
	}
}

void CGameStateRun::ReadMap(int phase, string filename) {
	ifstream inputfile(filename);

	for (int r = 0; r < MAPHEIGHT; r++) {
		for (int c = 0; c < MAPWIDTH; c++) {
			inputfile >> map[phase-1][r][c];
		}
	}
}

void CGameStateRun::InitMap() {
	string filename = "resources/map2int/phase" + to_string(phase) + ".txt";
	ReadMap(phase, filename);

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

void CGameStateRun::InitStartTime() {
	start_time = clock();
	mode_time = start_time;
	readyBGM->Play(3);
	bgm->Stop(1);
}

void CGameStateRun::InitCharacter() {
	isPacDied = false;
	showPacDiedAni = false;
	ghostmode = 1;
	modePhase = 1;
	for (int i = 1; i < 4; i++) roadGhostCanGo[i] = -1;
	towards = RIGHT;
	etTowards[0] = LEFT;
	etTowards[1] = DOWN;
	etTowards[2] = UP;
	etTowards[3] = UP;
	character[towards].SetTopLeft(216, 416);
	etRed[etTowards[0]].SetTopLeft(216, 224);
	etPink[etTowards[1]].SetTopLeft(216, 272);
	etBlue[etTowards[2]].SetTopLeft(186, 272);
	etYellow[etTowards[3]].SetTopLeft(246, 272);
}

void CGameStateRun::CheckPhaseClear() {
	if (!FindElement(eatenCookie, cookieAmount, 1) && !FindElement(eatenSP, spCookieAmount, 1)) {
		bgm->Stop(1);
		if (phaseClear) {
			end_time = clock();
			phaseClear--;
			nextPhaseTrans = 1;
		}
		if (Delay(35 * 100, end_time)) {
			phase++;
			InitEaten(eatenCookie, cookieAmount);
			InitEaten(eatenSP, spCookieAmount);
			cookieAmount = 0;
			spCookieAmount = 0;
			isMapLoaded = false;
			nextPhaseTrans = 0;
			phaseClear = 1;
		}
	}
}

void CGameStateRun::PacmanDir() {
	int nextdir = towards;
	int pacDirCanGo[4] = { -1,-1,-1,-1 };

	if (GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetRight() + 1, character[towards].GetBottom() - edge) >= 0) {
		pacDirCanGo[RIGHT] = RIGHT;
	}
	if (GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetTop() + edge) >= 0 && GetPixelAttribute(character[towards].GetLeft() - 1, character[towards].GetBottom() - edge) >= 0) {
		pacDirCanGo[LEFT] = LEFT;
	}
	if (GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetTop() - 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetTop() - 1) >= 0) {
		pacDirCanGo[UP] = UP;
	}
	if (GetPixelAttribute(character[towards].GetLeft() + edge, character[towards].GetBottom() + 1) >= 0 && GetPixelAttribute(character[towards].GetRight() - edge, character[towards].GetBottom() + 1) >= 0) {
		pacDirCanGo[DOWN] = DOWN;
	}

	for (int i = 0; i < 4; i++) {
		if (pacDirCanGo[i] == i && keyDir == i) {
			nextdir = i;
		}
	}

	character[nextdir].SetTopLeft(character[towards].GetLeft(), character[towards].GetTop());
	towards = nextdir;
}

void CGameStateRun::ChaseMode(char mode) {
	for (int i = 0; i < 4; i++) {
		ghostspeed[i] = 2;
	}
	/* set target for chase mode */
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
	for (int i = 0; i < 4; i++) {
		ghostspeed[i] = 2;
	}
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

void CGameStateRun::ScaredMode(char mode) {
	srand(clock());
	/* set target for scared mode */
	switch (mode)
	{
	case 'r':
		target[0][0] = rand() % 401 + 24;
		target[0][1] = rand() % 449 + 72;
		break;
	case 'p':
		target[1][0] = rand() % 401 + 24;
		target[1][1] = rand() % 449 + 72;
	case 'b':
		target[2][0] = rand() % 401 + 24;
		target[2][1] = rand() % 449 + 72;
	case 'y':
		target[3][0] = rand() % 401 + 24;
		target[3][1] = rand() % 449 + 72;
	default:
		break;
	}
}

void CGameStateRun::InitMode(char mode) {
	ghostReturnToHouseSE->Stop(7);
	scaredModeBgm->Play(5, true);
	for (int i = 0; i < 4; i++) {
		ghostspeed[i] = 2;
	}
	/* set target for init mode */
	switch (mode)
	{
	case 'r':
		if (etRed[etTowards[0]].GetTop() >= 232) {
			target[0][0] = 224;
			target[0][1] = 232;
		}
		else {
			roadGhostCanGo[0] = 0;
		}
		break;
	case 'p':
		if (etPink[etTowards[1]].GetTop() >= 232) {
			target[1][0] = 224;
			target[1][1] = 232;
		}
		else {
			roadGhostCanGo[1] = 0;
		}
		break;
	case 'b':
		if (etBlue[etTowards[2]].GetTop() >= 232) {
			target[2][0] = 224;
			target[2][1] = 232;
		}
		else {
			roadGhostCanGo[2] = 0;
		}
		break;
	case 'y':
		if (etYellow[etTowards[3]].GetTop() >= 232) {
			target[3][0] = 224;
			target[3][1] = 232;
		}
		else {
			roadGhostCanGo[3] = 0;
		}
		break;
	default:
		break;
	}
}

void CGameStateRun::DiedMode(char mode) {
	/* set target for died mode */
	switch (mode)
	{
	case 'r':
		if (GetPixelAttribute(etRed[etTowards[0]].GetLeft(), etRed[etTowards[0]].GetTop()) == -1) {
			isDied[0] = false;
			showOrigin[0] = true;
			break;
		}
		target[0][0] = 216;
		target[0][1] = 272;
		ghostspeed[0] = 4;
		roadGhostCanGo[0] = -1;
		break;
	case 'p':
		if (GetPixelAttribute(etPink[etTowards[1]].GetLeft(), etPink[etTowards[1]].GetTop()) == -1) {
			isDied[1] = false;
			showOrigin[1] = true;
			break;
		}
		target[1][0] = 216;
		target[1][1] = 272;
		ghostspeed[1] = 4;
		roadGhostCanGo[1] = -1;
		break;
	case 'b':
		if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft(), etBlue[etTowards[2]].GetTop()) == -1) {
			isDied[2] = false;
			showOrigin[2] = true;
			break;
		}
		target[2][0] = 216;
		target[2][1] = 272;
		ghostspeed[2] = 4;
		roadGhostCanGo[2] = -1;
		break;
	case 'y':
		if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft(), etYellow[etTowards[3]].GetTop()) == -1) {
			isDied[3] = false;
			showOrigin[3] = true;
			break;
		}
		target[3][0] = 216;
		target[3][1] = 272;
		ghostspeed[3] = 4;
		roadGhostCanGo[3] = -1;
		break;
	default:
		break;
	}
}

void CGameStateRun::NextDir(char mode) {
	double mindis[4] = { 2000000000,2000000000,2000000000,2000000000 };
	int nextdir[4] = { etTowards[0],etTowards[1],etTowards[2],etTowards[3] };

	/* clear dirCanGo for each ghost */
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dirCanGo[i][j] = -1;
		}
	}

	/* decide the next direction */
	if (mode == 'r') {
		if (etRed[etTowards[0]].CenterX() != target[0][0] || etRed[etTowards[0]].CenterY() != target[0][1]) {
			if (GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetTop() + edge) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() + 1, etRed[etTowards[0]].GetBottom() - edge) >= roadGhostCanGo[0]) {
				dirCanGo[0][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetTop() + edge) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetLeft() - 1, etRed[etTowards[0]].GetBottom() - edge) >= roadGhostCanGo[0]) {
				dirCanGo[0][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetTop() - 1) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetTop() - 1) >= roadGhostCanGo[0]) {
				dirCanGo[0][UP] = UP;
			}
			if (GetPixelAttribute(etRed[etTowards[0]].GetLeft() + edge, etRed[etTowards[0]].GetBottom() + 1) >= roadGhostCanGo[0] && GetPixelAttribute(etRed[etTowards[0]].GetRight() - edge, etRed[etTowards[0]].GetBottom() + 1) >= roadGhostCanGo[0]) {
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
			if (GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetTop() + edge) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() + 1, etPink[etTowards[1]].GetBottom() - edge) >= roadGhostCanGo[1]) {
				dirCanGo[1][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetTop() + edge) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetLeft() - 1, etPink[etTowards[1]].GetBottom() - edge) >= roadGhostCanGo[1]) {
				dirCanGo[1][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetTop() - 1) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetTop() - 1) >= roadGhostCanGo[1]) {
				dirCanGo[1][UP] = UP;
			}
			if (GetPixelAttribute(etPink[etTowards[1]].GetLeft() + edge, etPink[etTowards[1]].GetBottom() + 1) >= roadGhostCanGo[1] && GetPixelAttribute(etPink[etTowards[1]].GetRight() - edge, etPink[etTowards[1]].GetBottom() + 1) >= roadGhostCanGo[1]) {
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
			if (GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetTop() + edge) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() + 1, etBlue[etTowards[2]].GetBottom() - edge) >= roadGhostCanGo[2]) {
				dirCanGo[2][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetTop() + edge) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetLeft() - 1, etBlue[etTowards[2]].GetBottom() - edge) >= roadGhostCanGo[2]) {
				dirCanGo[2][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetTop() - 1) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetTop() - 1) >= roadGhostCanGo[2]) {
				dirCanGo[2][UP] = UP;
			}
			if (GetPixelAttribute(etBlue[etTowards[2]].GetLeft() + edge, etBlue[etTowards[2]].GetBottom() + 1) >= roadGhostCanGo[2] && GetPixelAttribute(etBlue[etTowards[2]].GetRight() - edge, etBlue[etTowards[2]].GetBottom() + 1) >= roadGhostCanGo[2]) {
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
			if (GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetTop() + edge) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() + 1, etYellow[etTowards[3]].GetBottom() - edge) >= roadGhostCanGo[3]) {
				dirCanGo[3][RIGHT] = RIGHT;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetTop() + edge) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetLeft() - 1, etYellow[etTowards[3]].GetBottom() - edge) >= roadGhostCanGo[3]) {
				dirCanGo[3][LEFT] = LEFT;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetTop() - 1) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetTop() - 1) >= roadGhostCanGo[3]) {
				dirCanGo[3][UP] = UP;
			}
			if (GetPixelAttribute(etYellow[etTowards[3]].GetLeft() + edge, etYellow[etTowards[3]].GetBottom() + 1) >= roadGhostCanGo[3] && GetPixelAttribute(etYellow[etTowards[3]].GetRight() - edge, etYellow[etTowards[3]].GetBottom() + 1) >= roadGhostCanGo[3]) {
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
	if (!isDied[0]) {
		if (dirR == UP || dirR == LEFT) {
			etRed[dirR + 2].SetTopLeft(etRed[dirR].GetLeft(), etRed[dirR].GetTop());
			etTowards[0] = dirR + 2;
		}
		else if (dirR == DOWN || dirR == RIGHT) {
			etRed[dirR - 2].SetTopLeft(etRed[dirR].GetLeft(), etRed[dirR].GetTop());
			etTowards[0] = dirR - 2;
		}
	}

	/* pink ghost */
	if (!isDied[1]) {
		if (dirP == UP || dirP == LEFT) {
			etPink[dirP + 2].SetTopLeft(etPink[dirP].GetLeft(), etPink[dirP].GetTop());
			etTowards[1] = dirP + 2;
		}
		else if (dirP == DOWN || dirP == RIGHT) {
			etPink[dirP - 2].SetTopLeft(etPink[dirP].GetLeft(), etPink[dirP].GetTop());
			etTowards[1] = dirP - 2;
		}
	}

	/* blue ghost */
	if (!isDied[2]) {
		if (dirB == UP || dirB == LEFT) {
			etBlue[dirB + 2].SetTopLeft(etBlue[dirB].GetLeft(), etBlue[dirB].GetTop());
			etTowards[2] = dirB + 2;
		}
		else if (dirB == DOWN || dirB == RIGHT) {
			etBlue[dirB - 2].SetTopLeft(etBlue[dirB].GetLeft(), etBlue[dirB].GetTop());
			etTowards[2] = dirB - 2;
		}
	}

	/* yellow ghost */
	if (!isDied[3]) {
		if (dirY == UP || dirY == LEFT) {
			etYellow[dirY + 2].SetTopLeft(etYellow[dirY].GetLeft(), etYellow[dirY].GetTop());
			etTowards[3] = dirY + 2;
		}
		else if (dirY == DOWN || dirY == RIGHT) {
			etYellow[dirY - 2].SetTopLeft(etYellow[dirY].GetLeft(), etYellow[dirY].GetTop());
			etTowards[3] = dirY - 2;
		}
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

bool CGameStateRun::Delay(int delaytime, clock_t start) {
	if (clock() - start >= delaytime) {
		return true;
	}
	return false;
}

bool CGameStateRun::Wait(int delaytime, clock_t start, int slot) {
	if (clock() - start >= delaytime && clock() - start < delaytime + slot) {
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

void CGameStateRun::ShowBonusPoint(int toShow, int i) {
	if (toShow == 1) {
		/* show bonus for eating ghost */
		bonusPoints[i].SetTopLeft(eatenLeft, eatenTop);
		bonusPoints[i].ShowBitmap(1.5);
	}

	/* show for 2 seconds */
	if (Delay(2 * 1000, eat_time)) {
		stuffEaten[i] = 0;
	}
}

void CGameStateRun::TimeController() {
	lastGhostmode = ghostmode;
	
	if (ghostmode >= 2) {
		if (clock() - scared_start <= 7 * 1000) {
			ghostmode = 2;
		}
		else if (clock() - scared_start <= 9 * 1000) {
			ghostmode = 3;
		}
		else {
			ghostmode = 0;
			eatenGhostAmount = 0;
			for (int i = 0; i < 4; i++) {
				showOrigin[i] = false;
			}
		}
	}
	else if (phase < 5 && modePhase < 3) {
		if (clock() - mode_time <= 7 * 1000) {
			ghostmode = 1;
		}
		else if (clock() - mode_time <= 27 * 1000) {
			ghostmode = 0;
		}
		else {
			modePhase++;
			mode_time = clock();
		}
	}
	else if (phase >= 5 && modePhase < 3) {
		if (clock() - mode_time <= 5 * 1000) {
			ghostmode = 1;
		}
		else if (clock() - mode_time <= 25 * 1000) {
			ghostmode = 0;
		}
		else {
			modePhase++;
			mode_time = clock();
		}
	}
	else if (phase == 1 && modePhase == 3) {
		if (clock() - mode_time <= 5 * 1000) {
			ghostmode = 1;
		}
		else if (clock() - mode_time <= 25 * 1000) {
			ghostmode = 0;
		}
		else {
			modePhase++;
			mode_time = clock();
		}
	}
	else if (phase > 1 && phase < 5 && modePhase == 3) {
		if (clock() - mode_time <= 5 * 1000) {
			ghostmode = 1;
		}
		else if (clock() - mode_time <= 1033 * 1000) {
			ghostmode = 0;
		}
		else {
			modePhase++;
			mode_time = clock();
		}
	}
	else if (phase >= 5 && modePhase == 3) {
		if (clock() - mode_time <= 5 * 1000) {
			ghostmode = 1;
		}
		else if (clock() - mode_time <= 1037 * 1000) {
			ghostmode = 0;
		}
		else {
			modePhase++;
			mode_time = clock();
		}
	}
	else if (phase == 1 && modePhase > 3) {
		if (clock() - mode_time <= 5 * 1000) {
			ghostmode = 1;
		}
		else {
			ghostmode = 0;
		}
	}
	else if (phase > 1 && phase>3) {
		if (clock() - mode_time <= 0.01 * 1000) {
			ghostmode = 1;
		}
		else {
			ghostmode = 0;
		}
	}
}

void CGameStateRun::BgmPlayer() {
	/* play start music */
	if (Wait(5 * 1000, start_time, 300)) {
		readyBGM->Stop(3);
	}

	/* play background music */
	if (Wait(4 * 1000, start_time, 300) || (ghostmode < 2 && restartBgm)) {
		bgm->Play(1, true);
		restartBgm = 0;
	}

	/* play scared mode backgruond music */
	if (ghostmode < 2) {
		scaredModeBgm->Stop(5);
	}
}