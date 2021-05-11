#pragma once
#include <vector>
using namespace std;
enum GameType
{
	PERSON,
	BOT
};
enum  GameStatus
{
	PALYING,
	WIN,
	DEAD
};
const int kBoardSizeNum = 15;
class GameModel
{
public:
	GameModel();
public:
	vector<vector<int>> gameMapVec;
	vector<vector<int>> scoreMapVec;

	bool playerFlag;

	GameType gameType;
	GameStatus gameStatus;


	void startGame(GameType type);
	void calculteScore();
	void actionByPerson(int row, int col);
	void actionByAi(int &clickRow, int&clickCol);
	void updateGameMap(int row, int col);
	bool isWin(int row, int col);
	bool isDeadGame();
};

