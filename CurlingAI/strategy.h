#pragma once

#include "main.h"
#include <string>
#include <iostream>
#include <fstream>

/*** constant numbers ***/
static const float RANDOM = (float)0.145;  // sigma of normal random number

//! positions on sheet
static const float TEE_X = (float)2.375;    // x of center of house
static const float TEE_Y = (float)4.880;    // y of center of house
static const float HOUSE_R = (float)1.830;  // radius of house
static const float STONE_R = (float)0.145;  // radius of stone

//! coodinate (x, y) is in playarea IF:
//   (PLAYAREA_X_MIN < x < PLAYAREA_X_MAX && PLAYAREA_Y_MIN < y < PLAYAREA_Y_MAX)
static const float PLAYAREA_X_MIN = (float)0.000 + STONE_R;
static const float PLAYAREA_X_MAX = (float)0.000 + (float)4.750 - STONE_R;
static const float PLAYAREA_Y_MIN = (float)3.050 + STONE_R;
static const float PLAYAREA_Y_MAX = (float)3.050 + (float)8.230 + STONE_R;

//! choose best shot
void getBestShot(const GAMESTATE* const gs, SHOTVEC *vec_ret);
void get_ranking(int* rank, const GAMESTATE* const gs);
float getDistFromTee(float x, float y);
void takeStone(const GAMESTATE* const gs, SHOTVEC * vec_ret, int rank,int power, int angle, int where);
int getStoneNum(const GAMESTATE* const gs);
bool isGuarded(const GAMESTATE* const gs, int rank,int angle);
std::string getVectorofStone(const GAMESTATE* const gs, int rank);
int getScoreofVector(const GAMESTATE* const gs, int rank);
int getStoneNOfromRank(const GAMESTATE* const gs, int NO);
int getBoardScore(const GAMESTATE* const gs);
int getShotCount(const GAMESTATE* const gs);
int getMyStoneNum(const GAMESTATE* const gs);
int getOpoStoneNum(const GAMESTATE * const gs);