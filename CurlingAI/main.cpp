#pragma once
#include "main.h"

using namespace std;

// state of the game
GAMESTATE GameState;

// functions fron CurlingSimulator.dll
HMODULE hCSDLL;
SIMULATION_FUNC Simulation = nullptr;
CREATESHOT_FUNC CreateShot = nullptr;
CREATEHITSHOT_FUNC CreateHitShot = nullptr;
GETSCORE_FUNC GetScore = nullptr;

//! initialize GAMESTATE
void initGameState(GAMESTATE *pgs) {
	memset(pgs->body, 0x00, sizeof(float) * 32);
	memset(pgs->Score, 0x00, sizeof(int) * 10);
	pgs->LastEnd = 0;
	pgs->CurEnd = 0;
	pgs->ShotNum = 0;
	pgs->WhiteToMove = 0;
}

//! send command
void sendCommand(char *message)
{
	DWORD NumberOfBytesWritten;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), message, strlen(message), &NumberOfBytesWritten, NULL);
}

//! recieve command
void recvCommand(char *message, size_t size)
{
	DWORD NumberOfBytesRead;
	ReadFile(GetStdHandle(STD_INPUT_HANDLE), message, size, &NumberOfBytesRead, NULL);
}

//! load external library
bool LoadFunction()
{
	hCSDLL = LoadLibrary("CurlingSimulator.dll");

	if (hCSDLL != nullptr) {
		Simulation = (SIMULATION_FUNC)GetProcAddress(hCSDLL, "Simulation");
		CreateShot = (CREATESHOT_FUNC)GetProcAddress(hCSDLL, "CreateShot");
		CreateHitShot = (CREATEHITSHOT_FUNC)GetProcAddress(hCSDLL, "CreateHitShot");
		GetScore = (GETSCORE_FUNC)GetProcAddress(hCSDLL, "GetScore");
	}
	else {
		//MessageBox(nullptr, "CurlingSimulator.dll の読み込みに失敗しました", "エラー", MB_OK);
		return false;
	}

	return true;
}

//! delete newline
void DeleteNL(char *Message)
{
	char *p;
	p = Message;

	while (*p != 0x00) {
		if (*p == '\n' || *p == '\r') {
			*p = 0x00;
			break;
		}
		p++;
	}
	return;
}

//! get argument from command
bool GetArgument(char *lpResult, size_t numberOfElements, char *Message, int n)
{
	char *p, *q;

	if (Message != NULL) {
		p = Message;
		while (*p == ' ') {
			p++;
		}

		// ポインタを取得したい引数の先頭に合わせる
		for (int i = 0; i<n; i++) {
			while (*p != ' ') {
				if (*p == 0x00) {
					return false;
				}
				p++;
			}
			while (*p == ' ') {
				p++;
			}
		}

		// 取得したい引数をlpResultにコピーする
		q = strstr(p, " ");
		if (q == NULL) {
			strcpy(lpResult, p);
		}
		else {
			strncpy_s(lpResult, numberOfElements, p, q - p);
			if ((unsigned int)(q - p) < numberOfElements) {
				lpResult[q - p] = 0x00;
			}
		}
	}

	return true;
}

//! process command
bool processCommand(char *command)
{
	char cmd[BUFSIZE];
	char buffer[BUFSIZE];

	cerr << "recieved: " << command << endl;

	// delete newline
	DeleteNL(command);

	// get command
	if (!GetArgument(cmd, sizeof(cmd), command, 0)) {
		return false;
	}

	// process command
	if (_stricmp(cmd, "NEWGAME") == 0) {
	}
	else if (_stricmp(cmd, "ISREADY") == 0) {
		// initialize GameState
		initGameState(&GameState);
		sendCommand("READYOK");
	}
	else if (_stricmp(cmd, "POSITION") == 0) {
		for (int i = 0; i < 16; i++) {
			// get x cordinate
			if (!GetArgument(buffer, sizeof(buffer), command, 2 * i + 1)) {
				return false;
			}
			GameState.body[i][0] = (float)atof(buffer);

			// get y cordinate
			if (!GetArgument(buffer, sizeof(buffer), command, 2 * i + 2)) {
				return false;
			}
			GameState.body[i][1] = (float)atof(buffer);
		}
	}
	else if (_stricmp(cmd, "SETSTATE") == 0) {
		// number of current shots
		if (GetArgument(buffer, sizeof(buffer), command, 1) == FALSE) {
			return false;
		}
		GameState.ShotNum = atoi(buffer);

		// number of current ends
		if (GetArgument(buffer, sizeof(buffer), command, 2) == FALSE) {
			return false;
		}
		GameState.CurEnd = atoi(buffer);

		// number of last
		if (GetArgument(buffer, sizeof(buffer), command, 3) == FALSE) {
			return false;
		}
		GameState.LastEnd = atoi(buffer);
		if (GetArgument(buffer, sizeof(buffer), command, 4) == FALSE) {
			return false;
		}
		if (atoi(buffer) == 1) {
			GameState.WhiteToMove = true;
		}
		else {
			GameState.WhiteToMove = false;
		}
	}
	else if (_stricmp(cmd, "GO") == 0) {
		SHOTVEC vec;

		// get a Shot by getBestShot (defined in 'strategy.cpp')
		//getBestShot(&GameState, &vec);
//---------------------------------------------------------------------------------------------------------------------
//ここに求められるショットを入れる
		/*
		takeNo1Stone(--,--,power,angle,where)
		where: where==0 ストーンの左にぶつける
		where: where==1 ストーンの中心にぶつける
		where: where==2 ストーンの右にぶつける
		*/
		int power[5] = { 3, 5, 7, 12, 16 }; //3,5,7,12,16
		int angle[2] = { 0,1 };// 0,1
		int where[3] = { 0,1,2 };// 0,1,2
		int powerNum = 5;
		int angleNum = 2;
		int whereNum = 3;
		int rankNum = getShotCount(&GameState);

		for (int r = 0; r < rankNum; r++) {
			//string v = getVectorofStone(&GameState, r);
			//string fileName = "C:\\Users\\81802\\Desktop\\CurlingAI\\Debug\\vec" + v + ".csv";
			//cerr << v << "に書き込んだ" << endl;
			int stoneNo = getStoneNOfromRank(&GameState, r);
			int isMine = 0;
			if (stoneNo % 2 == GameState.WhiteToMove)isMine = 0;
			else isMine = 1;
			for (int p = 0; p < powerNum; p++) {
				for (int a = 0; a < angleNum; a++) {
					for (int w = 0; w < whereNum; w++) {
						takeStone(&GameState, &vec, r, power[p], angle[a], where[w]);
						GAMESTATE pGameState = GameState;
						Simulation(&pGameState, vec, 0.30f, NULL, -1);
						ofstream ofs;
						/*
						int allScore = getBoardScore(&pGameState);
						ofs.open(fileName, ios::app);
						ofs <<getMyStoneNum(&GameState) <<","<< getOpoStoneNum(&GameState) <<","<<isMine<<","<<v<<","<< where[w] << "," << angle[a] << "," << power[p] << "," << allScore << endl;
						ofs.close();
						*/
						ofs.open("C:\\Users\\81802\\Desktop\\CurlingAI\\Debug\\allLogs.csv", ios::app);
						for (int i = 0; i < 16; i++) {
							ofs << GameState.body[i][0] << "," << GameState.body[i][1] << ",";
						}
						ofs << where[w] << "," << angle[a] << "," << power[p] << ",";
						for (int i = 0; i < 16; i++) {
							if (i != 15)ofs << pGameState.body[i][0] << "," << pGameState.body[i][1] << ",";
							else ofs << pGameState.body[i][0] << "," << pGameState.body[i][1] << "," << stoneNo << endl;
						}
						ofs.close();
					}
				}
			}
		}
//---------------------------------------------------------------------------------------------------------------------
	
		//takeStone(&GameState, &vec, 0, 16, 0, 0);
		cerr << GameState.WhiteToMove << endl << endl;
		vec.x = 0;
		vec.y = 0;
		vec.angle = 0;
		// create BESTSHOT command
		sprintf_s(buffer, sizeof(char) * BUFSIZE, "BESTSHOT %f %f %d", vec.x, vec.y, vec.angle);
		// send BESTSHOT command
		sendCommand(buffer);
	}
	else if (_stricmp(cmd, "SCORE") == 0) {
		// get score of previous end
		if (GetArgument(buffer, sizeof(buffer), command, 1) == FALSE) {
			return false;
		}
		GameState.Score[GameState.CurEnd] = atoi(buffer);
	}

	return true;
}


int main()
{
	char message[BUFSIZE];

	// load CurlingSimulator.dll
	if (!LoadFunction()) {
		cerr << "failed to load CurlingSimulator.dll" << endl;
		return 0;
	}

	// process command
	while (1) {
		memset(message, 0x00, sizeof(message));
		recvCommand(message, sizeof(message));
		processCommand(message);
	}

	return 0;
}