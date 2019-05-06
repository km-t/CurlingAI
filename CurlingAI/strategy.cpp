#include "strategy.h"
#include <string>
using namespace std;

//! get distance^2 from center of House
float get_dist(float x, float y)
{
	return pow(x - TEE_X, 2) + pow(y - TEE_Y, 2);
}

//! get distance^2 of two cordinates
float get_dist(float x1, float y1, float x2, float y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}

//! is a Stone in House
bool is_in_House(float x, float y)
{
	if (get_dist(x, y) < pow(HOUSE_R + STONE_R, 2)) {
		return true;
	}
	else {
		return false;
	}
}

//! sort Shot number (rank[] = {0, 1, 2 ... 15})
//  by distance from center of House (TEEX, TEEY)
void get_ranking(int *rank, const GAMESTATE* const gs)
{
	// init array
	for (int i = 0; i < 16; i++) {
		rank[i] = i;
	}

	// sort
	int tmp;
	for (int i = 1; i < gs->ShotNum; i++) {
		for (int j = i; j > 0; j--) {
			if (get_dist(gs->body[rank[j]][0], gs->body[rank[j]][1]) < get_dist(gs->body[rank[j - 1]][0], gs->body[rank[j - 1]][1])) {
				// swap
				tmp = rank[j];
				rank[j] = rank[j - 1];
				rank[j - 1] = tmp;
			}
			else {
				break;
			}
		}
	}
}

//! get best Shot to takeout the objective stone
void CreateTakeoutShot(const GAMESTATE* const gs, unsigned int num_target, SHOTVEC *vec_ret)
{
	/*** this is a sample of 'Simulation' function ***/

	const unsigned int shot_variations = 2;

	SHOTPOS target;
	SHOTVEC vec[shot_variations];

	// get position of target stone
	target.x = gs->body[num_target][0];
	target.y = gs->body[num_target][1];

	// get a HitShot (right Curl)
	target.angle = 0;
	CreateHitShot(target, 16, &(vec[0]));

	// get a HitShot (left Curl)
	target.angle = 1;
	CreateHitShot(target, 16, &(vec[1]));

	// try a shot 100 times
	const unsigned int trials = 100;
	unsigned int count[shot_variations] = { 0, 0 };

	// create temporary GAMESTATE
	GAMESTATE *gstmp;
	gstmp = (GAMESTATE *)malloc(sizeof(GAMESTATE));

	// simulate each shot
	for (unsigned int i = 0; i < trials; i++) {
		for (int j = 0; j < shot_variations; j++) {
			// copy current state to temporary state
			memcpy_s(gstmp, sizeof(GAMESTATE), gs, sizeof(GAMESTATE));

			// Simulation
			Simulation(gstmp, vec[j], RANDOM, NULL, -1);

			// check objective stone was in House or not
			// NOTE: 'Simulation' rewites gstmp as a state after the simulation
			if ( !is_in_House(gstmp->body[num_target][0], gstmp->body[num_target][1])) {
				count[j]++;
			}
		}
	}

	// calculate accuracy of Takeout Shot
	unsigned int best_num = 0;
	unsigned int count_max = trials + 1;
	for (int i = 0; i < shot_variations; i++) {
		if (count_max < count[i]) {
			count_max = count[i];
			best_num = i;
		}


		// NOTE: use cerr if you want output something on command line
		cerr << "shot[" << i << "] (" << count[i] << "/" << trials << ")" << endl;
	}

	cerr << "best_num = " << best_num << endl;

	memcpy_s(vec_ret, sizeof(SHOTVEC), &(vec[best_num]), sizeof(SHOTVEC));
}

//! choose best Shot
//  This sample choose:
//    1.Takeout Shot: hit and push the objective Stone away from House 
//      - IF No.1 Stone (the nearest Stone from center of House) is opponent Stone.
//    2.Guard Shot: put a Stone as hiding the objective Stone.
//      - IF No.1 Stone is my Stone.
//    3.Tee Shot: put a Stone on center of House
//      - IF there is no Stone in House
void getBestShot(const GAMESTATE* const gs, SHOTVEC *vec_ret)
{
	SHOTPOS pos;

	// ranking of Shot number
	// rank[n] = x;
	//   n : the n th Stone from the center of House
	//   x : the x th Shot in this End (corresponding to the number of GAMESTATE->body[x])
	int rank[16];

	// sort by distane from center
	get_ranking(rank, gs);

	// create Shot according to condition of No.1 Stone
	if (is_in_House(gs->body[rank[0]][0], gs->body[rank[0]][1]))
	{
		// get position of the objective Stone
		pos.x = gs->body[rank[0]][0];
		pos.y = gs->body[rank[0]][1];

		if (rank[0] % 2 != gs->ShotNum % 2) {
			// choose Shot 1.
			if (0) {
				// without simulation
				if (gs->body[rank[0]][0] < TEE_X) {
					pos.angle = 1;
				}
				else {
					pos.angle = 0;
				}

				// generate a Shot(SHOTVEC) by CreateHitShot (pass the objective position)
				CreateHitShot(pos, 16, vec_ret);
			}
			else {
				// with simulation
				CreateTakeoutShot(gs, rank[0], vec_ret);
			}
		}
		else {
			// choose Shot 2.
			// generate a Shot by CreateShot (put a Stone on objective position: pos)
			pos.y += 2.0;
			if (gs->body[rank[0]][0] < TEE_X) {
				pos.angle = 0;
			}
			else {
				pos.angle = 1;
			}

			CreateShot(pos, vec_ret);
		}
	}
	else {
		// choose Shot 3.

		// set the objective position as center of House
		pos.x = TEE_X;
		pos.y = TEE_Y;
		pos.angle = 0;

		// generate a Shot by CreateShot (put a Stone on objective position: pos)
		CreateShot(pos, vec_ret);
	}
}


void takeStone(const GAMESTATE* const gs, SHOTVEC* vec_ret,int rank, int power, int angle, int where) {
	SHOTPOS pos;

	/*
	takeNo1Stone(--,--,power,angle,where)
	where: where==0 ストーンの左にぶつける
	where: where==1 ストーンの中心にぶつける
	where: where==2 ストーンの右にぶつける
	*/
	float stoneSize = 0.29;

	// ranking of Shot number
	// rank[n] = x;
	//   n : the n th Stone from the center of House
	//   x : the x th Shot in this End (corresponding to the number of GAMESTATE->body[x])
	int ranks[16];

	// sort by distane from center
	get_ranking(ranks, gs);

	// get position of the objective Stone
	pos.x = gs->body[ranks[rank]][0];
	pos.y = gs->body[ranks[rank]][1];
	pos.angle = angle;
	if (where == 0) {
		pos.x -= stoneSize / 2;
	}
	else if (where == 2) {
		pos.x += stoneSize / 2;
	}

	// without simulation
	// generate a Shot(SHOTVEC) by CreateHitShot (pass the objective position)
	CreateHitShot(pos, power, vec_ret);	
}


float getDistFromTee(float x, float y) {
	return sqrt((TEE_X - x) * (TEE_X - x) + (TEE_Y - y) * (TEE_Y - y));
}

int getStoneNum(const GAMESTATE* const gs) {
	int num = 0;
	for (int i = 0; i < 16; i++) {
		if (gs->body[i][0] * gs->body[i][1] != 0)num++;
	}
	return num;
}

bool isGuarded(const GAMESTATE* const gs, int rank, int angle) {
	int ranks[16];
	get_ranking(ranks, gs);
	for (int i = 0; i < 16; i++) {
		if (i != rank) {
			if (gs->body[ranks[rank]][0] >= gs->body[ranks[i]][0] && gs->body[ranks[rank]][0] + (2*angle-1) <= gs->body[ranks[i]][0]) {
				if (gs->body[ranks[i]][1] >= gs->body[ranks[rank]][1])return true;
			}
		}
	}
	return false;
}
std::string getVectorofStone(const GAMESTATE* const gs, int rank) {
	string ans="";
	int ranks[16];
	get_ranking(ranks, gs);
	float x = gs->body[ranks[rank]][0];
	float y = gs->body[ranks[rank]][1];
	
	if (rank == 0)ans += "1";
	else ans += "0";

	if (rank == 1)ans += "1";
	else ans += "0";

	if (rank == 2)ans += "1";
	else ans += "0";

	if (getDistFromTee(x, y) <= 0.61)ans += "111";
	else if (getDistFromTee(x, y) <= 1.22)ans += "011";
	else if (getDistFromTee(x, y) <= 1.83)ans += "001";
	else ans += "000";

	if (0 < x && x < 1.58)ans += "100";
	else if (1.58 <= x && x <= 3.16)ans += "010";
	else ans += "001";

	if (isGuarded(gs, rank, 0))ans += "1";
	else ans += "0";

	if (isGuarded(gs, rank, 1))ans += "1";
	else ans += "0";
	return ans;
}

int getScoreofVector(const GAMESTATE* const gs, int rank) {
	int ranks[16];
	get_ranking(ranks, gs);
	int answer[11];
	int score = 0;
	string str = getVectorofStone(gs, rank);
	for (int i = 0; i < 11; i++) {
		answer[i] = (int)(str[i])-48;
	}

	score += answer[0] * 3 + answer[1] * 2 + answer[2]; //ランクごとの点数
	for (int i = 3; i < 6; i++) {
		score += answer[i];//距離ごとの点数
	}
	score += answer[9] + answer[10];//ガードストーンの有無
	

	return score;
}

int getStoneNOfromRank(const GAMESTATE* const gs, int ra) {
	int ranks[16];
	get_ranking(ranks, gs);
	for (int i = 0; i < 16; i++) {
		if (ranks[i] == ra)return i;
	}
}

int getBoardScore(const GAMESTATE* const gs) {
	int allScore = 0;
	int ranks[16];
	get_ranking(ranks, gs);
	for (int i = 0; i < 8; i++) {
		if (gs->body[gs->WhiteToMove + 2 * i][0]!=0 && gs->body[gs->WhiteToMove + 2 * i][1] != 0) {
			allScore += getScoreofVector(gs, getStoneNOfromRank(gs, gs->WhiteToMove + 2 * i));
		}
		if (gs->body[(gs->WhiteToMove+1)%2 + 2 * i][0] != 0 && gs->body[(gs->WhiteToMove+1)%2 + 2 * i][1] != 0) {
			allScore -= getScoreofVector(gs, getStoneNOfromRank(gs, (gs->WhiteToMove + 1) % 2 + 2 * i));
		}
	}
	return allScore;
}


int getShotCount(const GAMESTATE* const gs) {
	int count = 0;
	for (int i = 0; i < 16; i++) {
		if (gs->body[i][0]!=0&&  gs->body[i][1] != 0)count++;
	}
	return count;
}

int getMyStoneNum(const GAMESTATE* const gs) {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		if (gs->body[gs->WhiteToMove + 2 * i][0] != 0 && gs->body[gs->WhiteToMove + 2 * i][1] != 0)count++;
	}
	return count;
}
int getOpoStoneNum(const GAMESTATE* const gs) {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		if (gs->body[(gs->WhiteToMove + 1) % 2 + 2 * i][0] != 0 && gs->body[(gs->WhiteToMove + 1) % 2 + 2 * i][1] != 0)count++;
	}
	return count;
}