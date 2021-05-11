#include "GameModel.h"
#include "time.h"
#include <QDebug>
GameModel::GameModel()
{

}

void GameModel::startGame(GameType type)
{
	this->gameType = type;
	gameMapVec.clear();
	for (int i=0;i<=kBoardSizeNum;i++)
	{
		vector<int> lineBoard;
		for (int j=0;j<=kBoardSizeNum;j++)
		{
			lineBoard.push_back(0);
		}
		gameMapVec.push_back(lineBoard);
	}
	if (gameType==BOT)
	{
		scoreMapVec.clear();
		for (int i = 0; i < kBoardSizeNum; i++) {
			vector<int> lineScore;
			for (int j=0;j<kBoardSizeNum;j++)
			{
				lineScore.push_back(0);
			}
			scoreMapVec.push_back(lineScore);
		}
	}
	playerFlag = true;

}

void GameModel::calculteScore()
{
	int personNum = 0;
	int botNum = 0;
	int emptyNum = 0;

	scoreMapVec.clear();
	//清空评分数组
	for (int i=0;i<kBoardSizeNum;i++)
	{
		std::vector<int> lineScores;
		for (int j=0;j<kBoardSizeNum;j++)
		{
			lineScores.push_back(0);
		}
		scoreMapVec.push_back(lineScores);
	}
	//计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格

	for (int row = 0; row < kBoardSizeNum; row++)
	{
		for (int col=0;col<kBoardSizeNum;col++)
		{
			qDebug() << "row: " << row << "  col  " << col;
			if (row>0&&col>0&&gameMapVec[row][col]==0)
			{
				//遍历八个方向
				for (int y=-1;y<=1;y++)
				{
					for (int x=-1;x<=1;x++)
					{
						//重置
						personNum = 0;
						botNum = 0;
						emptyNum = 0;
						//原坐标不算
						if (y!=0||x!=0)
						{
							//对玩家白子评分
							for (int i=1;i<=4;i++)
							{
								if (row+i*y>0&&row+i*y<kBoardSizeNum&&col+i*x>0&&col+i*x<kBoardSizeNum&&gameMapVec[row+i*y][col+i*x]==1)
								{
									personNum++;
								}
								else if (row + i * y > 0 && row + i * y < kBoardSizeNum&&col + i * x>0 && col + i * x < kBoardSizeNum&&gameMapVec[row + i * y][col + i * x] == 0)
								{
									emptyNum++;
									break;
								}
								else {
									break;
								}
							}
							for (int j = 1; j <= 4; j++)
							{
								int i = -j;
								if (row + i * y > 0 && row + i * y < kBoardSizeNum&&col + i * x>0 && col + i * x < kBoardSizeNum&&gameMapVec[row + i * y][col + i * x] == 1)
								{
									personNum++;
								}
								else if (row + i * y > 0 && row + i * y < kBoardSizeNum&&col + i * x>0 && col + i * x < kBoardSizeNum&&gameMapVec[row + i * y][col + i * x] == 0)
								{
									emptyNum++;
									break;
								}
								else {
									break;
								}
							}
							if (personNum==1)//杀二
							{
								scoreMapVec[row][col] += 10;
							}else if (personNum==2)//杀三
							{
								if (emptyNum==1)
								{
									scoreMapVec[row][col] += 30;
								}else if (emptyNum==2)
								{
									scoreMapVec[row][col] += 40;
								}
							}else if (personNum==3)
							{
								if (emptyNum == 1)
								{
									scoreMapVec[row][col] += 60;
								}else if (emptyNum==2)
								{
									scoreMapVec[row][col] += 110;
								}
							}
							else if (personNum==4)
							{
								scoreMapVec[row][col] += 10100;
							}
							emptyNum = 0;

							for (int i=1;i<=4;i++)
							{
								if (row+i*y>0&&row+i*y<kBoardSizeNum&&col+i*x>0&&col+i*x<kBoardSizeNum)
								{
									if (gameMapVec[row+i*y][col+i*x]==1)
									{
										botNum++;
									}else if (gameMapVec[row+i*y][col+i*x]==0)
									{
										emptyNum++;
										break;
									}
									else
									{
										break;
									}
								} else
								{
									break;
								}
							}
							for (int j = 1; j <= 4; j++)
							{
								int i = -j;
								if (row + i * y > 0 && row + i * y < kBoardSizeNum&&col + i * x>0 && col + i * x < kBoardSizeNum)
								{
									if (gameMapVec[row + i * y][col + i * x] == 1)
									{
										botNum++;
									}
									else if (gameMapVec[row + i * y][col + i * x] == 0)
									{
										emptyNum++;
										break;
									}
									else
									{
										break;
									}
								}
								else
								{
									break;
								}
							}

							if (botNum==0)
							{
								scoreMapVec[row][col] += 5;

							}else if (botNum==1)
							{
								scoreMapVec[row][col] += 10;
							}else if (botNum==2)
							{
								if (emptyNum==1)
								{
									scoreMapVec[row][col] += 25;
								}else if (emptyNum==2)
								{
									scoreMapVec[row][col] += 50;
								}
							}else if (botNum==3)
							{
								if (emptyNum==1)
								{
									scoreMapVec[row][col] += 55;
								}else if (emptyNum==2)
								{
									scoreMapVec[row][col] += 100;
								}
							}else if (botNum==4)
							{
								scoreMapVec[row][col] += 10000;
							}

						}
					}
				}
			}
		}
	}
}

void GameModel::actionByPerson(int row, int col)
{
	updateGameMap(row, col);
}

void GameModel::actionByAi(int &clickRow, int&clickCol)
{
	calculteScore();//计算评分
	int maxScore = 0;
	std::vector<std::pair<int, int>>maxPoints;
	for (int row=1;row<kBoardSizeNum;row++)
	{
		for (int col=1;col<kBoardSizeNum;col++)
		{
			if (gameMapVec[row][col]==0)
			{
				if (scoreMapVec[row][col]>maxScore)
				{
					maxPoints.clear();
					maxScore = scoreMapVec[row][col];
					maxPoints.push_back(std::make_pair(row, col));
				}else if (scoreMapVec[row][col]==maxScore)
				{
					maxPoints.push_back(std::make_pair(row, col));
				}
			}
		}
	}
	srand((unsigned)time(0));
	int index = rand() % maxPoints.size();
	std::pair<int, int> pointPair = maxPoints.at(index);
	clickRow = pointPair.first;
	clickCol = pointPair.second;
	updateGameMap(clickRow, clickCol);

}

void GameModel::updateGameMap(int row, int col)
{
	if (playerFlag)
	{
		gameMapVec[row][col] = 1;
	}
	else {
		gameMapVec[row][col] = -1;
	}
	playerFlag = !playerFlag;
}

bool GameModel::isWin(int row, int col)
{// 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
	// 水平方向
	for (int i=0;i<5;i++)
	{
		if (col-i>0&&col-i+4<kBoardSizeNum)
		{
			
			// 往左5个，往右匹配4个子，20种情况
			if (col - i > 0 &&
				col - i + 4 < kBoardSizeNum &&
				gameMapVec[row][col - i] == gameMapVec[row][col - i + 1] &&
				gameMapVec[row][col - i] == gameMapVec[row][col - i + 2] &&
				gameMapVec[row][col - i] == gameMapVec[row][col - i + 3] &&
				gameMapVec[row][col - i] == gameMapVec[row][col - i + 4])
				return true;
		}
	}
	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i > 0 &&
			row - i + 4 < kBoardSizeNum &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 1][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 2][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 3][col] &&
			gameMapVec[row - i][col] == gameMapVec[row - i + 4][col])
			return true;
	}

	// 左斜方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < kBoardSizeNum &&
			row + i - 4 > 0 &&
			col - i > 0 &&
			col - i + 4 < kBoardSizeNum &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 1][col - i + 1] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 2][col - i + 2] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 3][col - i + 3] &&
			gameMapVec[row + i][col - i] == gameMapVec[row + i - 4][col - i + 4])
			return true;
	}

	// 右斜方向
	for (int i = 0; i < 5; i++)
	{
		if (row - i > 0 &&
			row - i + 4 < kBoardSizeNum &&
			col - i > 0 &&
			col - i + 4 < kBoardSizeNum &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 1][col - i + 1] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 2][col - i + 2] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 3][col - i + 3] &&
			gameMapVec[row - i][col - i] == gameMapVec[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}

bool GameModel::isDeadGame()
{
	for each (auto cols in gameMapVec)
	{
		for each (auto entry in cols)
		{
			if (entry==0)
			{
				return false;
			}
		}
	}
	return true;
}
