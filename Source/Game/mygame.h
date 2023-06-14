/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "myfunc.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
		
		// sound effect
		//CAudio *initBGM;

		// sprites
		CMovingBitmap character;
		CMovingBitmap etRed;
		CMovingBitmap etPink;
		CMovingBitmap etBlue;
		CMovingBitmap etYellow;
		CMovingBitmap start;
		CMovingBitmap frame;

		// constant
		int speed[2] = { 1,-1 };

		// functions
		int MovingAnimation(CMovingBitmap character, int mode);

		// bgm
		//CAudio bgm;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		/* sprites */
		// game
		CMovingBitmap background;
		MyBitmap character[6];
		MyBitmap etRed[8];
		MyBitmap etPink[8];
		MyBitmap etBlue[8];
		MyBitmap etYellow[8];
		MyBitmap cookie[500];
		MyBitmap spCookie[10];
		MyBitmap giraffe;
		// ui
		MyBitmap lives[3];
		MyBitmap oneP;
		MyBitmap scoreNum[10];
		MyBitmap bonusPoints[4];
		MyBitmap ready;
		MyBitmap gameover;
		MyBitmap trans;
		MyBitmap superIcon;
		

		/* constants */
		const int PIXWIDTH = 16;
		const int PIXHEIGHT = 16;
		const int MAPWIDTH = 28;
		const int MAPHEIGHT = 36;
		const int UP = 0;
		const int LEFT = 1;
		const int DOWN = 2;
		const int RIGHT = 3;
		const int UNSHOW = 4;
		const int SCARED = 5;
		const int TRANSIT = 6;
		const int DIED = 7;
		const int PACDIED = 5;
		
		/* flags & parameters */
		// game
		int phase = 1;
		int modePhase = 1;
		int phaseClear = 1;
		int nextPhaseTrans = 0;
		int edge = 5;	// space for turning around
		bool showPacDiedAni = false;
		bool isPacDied = false;

		int towards = RIGHT;	// pacman direction
		int keyDir = RIGHT;	// direction player controlled
		int speed = 2;	// moving speed
		int eatenGhostAmount = 0;	// count of eaten ghost
		
		int cookieAmount = 0;	// total amount of common cookies
		int spCookieAmount = 0;	// total amount of sp cookies
		int eatenCookie[500];	// use to record cookies are eaten or not
		int eatenSP[10];	// use to record sp cookies are eaten or not
		
		int ghostmode = 1;	// 0 for chaseMode, 1 for scatterMode, 2 for scareMode, 3 for transitMode
		int lastGhostmode = 1;
		int ghostspeed[4] = { 2,2,2,2 };
		int etTowards[4] = { LEFT,DOWN,UP,UP };	// ghost directions, 0 for red, 1 for pink, 2 for blue, 3 for yellow
		int roadGhostCanGo[4] = { 0,-1,-1,-1 };
		int dirCanGo[4][4];	// save all direction could go for each ghost
		int eatenLeft = 0, eatenTop = 0;	// the place ghost is eaten
		double target[4][2];	// save ghosts' target

		clock_t start_time;	// phase start time
		clock_t end_time;	// phase end time
		clock_t mode_time;	// mode start time
		clock_t scared_start;	// scared mode start time
		clock_t eat_time;	// the time that pacman eats a ghost
		clock_t died_time;	// the time pacman died
		clock_t last_time = clock();	// use in Delay function
		bool isMapLoaded = false;	// check if the map is loaded 
		bool isGameStartInit;
		bool isDied[4] = { false, false, false, false };	// is ghost eaten
		bool showOrigin[4] = { false,false,false,false };	// had ghost get back to ghost house
		bool superPlayer = false;
		// ui
		int score = 0;
		int stuffEaten[4] = { 0,0,0,0 };	// there are stuff eaten or not
		int life = 3;
		// bgm controller
		int cookieEaten = 0;
		int restartBgm = 0;

		/* functions */
		// game
		void ShowByPhase();
		void ReadMap(int phase, string filename);
		void InitStartTime();
		void InitMap();
		void InitEaten(int *p, int len);
		void InitCharacter();
		void CheckPhaseClear();
		void PacmanDir();
		void ChaseMode(char mode);
		void ScatterMode(char mode);
		void ScaredMode(char mode);
		void InitMode(char mode);
		void DiedMode(char mode);
		void NextDir(char mode);
		void TurnAround(int dirR, int dirP, int dirB, int dirY);
		void BgmPlayer();
		int GetPixelAttribute(int left, int top);
		double Distance(MyBitmap mb1, int mode, int nextdir);
		double Distance(MyBitmap mb1, MyBitmap mb2);
		bool Delay(int delaytime);
		bool Delay(int delaytime, clock_t start);
		bool Wait(int delaytime, clock_t start, int slot);
		bool FindElement(int *p, int len, int target);
		// ui
		void ShowUI();
		void ShowBonusPoint(int toShow, int i);

		/* Game Controller */
		void TimeController();

		/* map */
		int map[20][36][28];
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}
