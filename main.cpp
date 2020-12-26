#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_LINE_SQUARE_COUNT = 7; // 棋盤一行有七格

void countChips(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int& playerScore, int& opponentScore, int& vacancy);
void playerChangeColor(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int beforeChipRow, int beforeChipCol, int afterChipRow, int afterChipCol, int& playerScore, int& opponentScore, int& vacancy);
int whoIsWinner(int status[][MAX_LINE_SQUARE_COUNT]);
bool haveVacancy(int x, int y, int status[][MAX_LINE_SQUARE_COUNT]);
void printStatus(int status[][MAX_LINE_SQUARE_COUNT]);
void opponentB(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT],int beforeChipRow,int beforeChipCol,int opponentBeforeChipRow,int opponentBeforeChipCol,int opponentAfterChipRow,int opponentAfterChipCol , int& playerScore, int& opponentScore, int& vacancy);
void algoBasic(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int& playerScore, int& opponentScore, int& vacancy);

int main()
{
	// status裡面存1(己方的棋)、-1(敵方的棋)、0(無置棋)
	// status[存row][存col]
	int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT] = {0};	// 7*7的空棋盤
	int whosTurn = 0;  		// 這回合輪到誰，目前還沒使用到
	int playerScore = 0, opponentScore = 0; 				     // 雙方在棋面上各擁有多少棋
	int vacancy = MAX_LINE_SQUARE_COUNT * MAX_LINE_SQUARE_COUNT; // 棋盤上的空位

	// 先各在左上和右下置雙方的棋
	status[0][0] = -1;
	status[MAX_LINE_SQUARE_COUNT-1][MAX_LINE_SQUARE_COUNT-1] = 1;

	// 選擇對戰對手
	char chooseOpponent;
	cout << "選擇您所要對戰的對手(輸入:大寫A或大寫B)" << endl;
	cin >> chooseOpponent;
	while (chooseOpponent != 'A' and chooseOpponent != 'B' and chooseOpponent != 'C')
	{
		cout << "無此對戰對手，請重新輸入(大寫A或大寫B)" << endl;
		cin >> chooseOpponent;
	}

	// player輸入，讀入下棋決策(選定棋子，要下哪裡)
	int beforeChipRow = -1, beforeChipCol = -1;	 // 要接受cin input 的值, 被選定的棋子
	int afterChipRow = -1, afterChipCol = -1;	 // 要接受cin input 的值, 要移去的位置
	int opponentBeforeChipRow = 0, opponentBeforeChipCol = 0;	 // 要接受對手 input 的值, 被選定的棋子
	int opponentAfterChipRow = -1, opponentAfterChipCol = -1;	 // 要接受對手 input 的值, 要移去的位置

	// 遊戲迴圈
	while (whoIsWinner(status) == 0) // 直接放蕙均的function? player是贏家：回傳1。 opponent是贏家：回傳-1。 還沒結束：回傳0
	{
		cout << "該回合輪到 player 下棋。" << endl;
		do
		{	
			// 讀入player的下棋決策(選定棋子)
			cout << "請輸入您所要移動的棋子的位置(例如:G 6)" << endl;
			cin >> beforeChipRow >> beforeChipCol;
			
			if ((beforeChipRow < 0) or (beforeChipCol < 0) or (beforeChipRow > MAX_LINE_SQUARE_COUNT-1) or (beforeChipCol > MAX_LINE_SQUARE_COUNT-1)) // 超出棋盤邊界
			{
				cout << "該選定位置已超出棋盤邊界" << endl;
				continue;   // 重新再輸入下棋決策
			}
			else if (status[beforeChipRow][beforeChipCol] != 1) // 假如player選定的棋不是palyer的棋或沒有棋
			{	
				cout << "該選定位置並無您的棋子" << endl;
				continue;	// 重新再輸入下棋決策
			}
			else	
			{
				bool haveVacancy = false; // 看該棋周圍有沒有可以走的位置
				for (int i = beforeChipRow-2; i <= beforeChipRow+2; i++)	
				{
					if (i >= 0 and i < MAX_LINE_SQUARE_COUNT)
					{
						for (int j = beforeChipCol-2; j <= beforeChipCol+2; j++)
						{
							if (j >= 0 and j < MAX_LINE_SQUARE_COUNT)
							{
								if (status[i][j] == 0)// 有這顆棋可以走的空位
								{
									haveVacancy = true;
								}
							}
						}
					}
				}
				if (haveVacancy == false)
				{
					 // 棋子被包夾死，找不到地方移動出去
					cout << "該棋已被包圍住，無法移動" << endl;
					continue;	// 重新再輸入下棋決策
				}
				else // 該棋子還有空位可走
				{
					break;
				}
				
			}

		} while (true);
		 
		do
		{	
			// 讀入player的下棋決策(要下在哪。)
			cout << "請輸入您的棋子所要放置的位置(例如:F 5)" << endl;
			cin >> afterChipRow >> afterChipCol;

			if ((beforeChipRow < 0) or (beforeChipCol < 0) or (beforeChipRow > MAX_LINE_SQUARE_COUNT-1) or (beforeChipCol > MAX_LINE_SQUARE_COUNT-1)) // 超出棋盤邊界
			{
				cout << "該選定位置已超出棋盤邊界" << endl;
				continue;   // 重新再輸入下棋決策
			}
			else if (status[afterChipRow][afterChipRow] != 0) // 假如player選定的位置已經有棋子
			{	
				cout << "該選定位置已放置過棋子" << endl;
				continue;	// 重新再輸入下棋決策
			}
			else if (abs(beforeChipRow - afterChipRow) > 2 or abs(beforeChipCol - afterChipCol) > 2) // 超出棋子可以移動的範圍
			{
				cout << "該選定位置超出此棋可移動的範圍" << endl;
				continue;   // 重新再輸入下棋決策
			}
			else
			{
				break;
			}

		} while (true);

		// 上面的兩個do-while迴圈判斷了「player輸入的下棋決策是無誤的」

		// 執行同化的變色function
		// 依芩的function幫忙改到 playerScore 和 opponentScore(雙方的棋盤上棋子數)
		playerChangeColor(status, beforeChipRow, beforeChipCol, afterChipRow, afterChipCol, playerScore, opponentScore, vacancy);

		countChips(status, playerScore, opponentScore, vacancy); // 算現在棋盤面的結果(player棋子數, opponent棋子數, 棋盤空格數)

		/*
			call 絲盈的棋盤輸出函數()
			在下面那一行
		*/
		printStatus(status);

		// player獲得勝利
		if (whoIsWinner(status) == 1) // 直接放蕙均的function?
		{
			cout << "恭喜player贏得勝利" << endl;
			break; // 跳出遊戲
		}

		cout << "player 所擁有的棋子數：" << playerScore << endl;
		cout << "opponent 所擁有的棋子數：" << opponentScore << endl;



		// player還沒贏，換opponent進攻
		if (chooseOpponent == 'A')  // 依芩的廢物對手
		{
			cout << "攻守交換。" << endl;
			cout << "該回合輪到 opponent " << chooseOpponent << "下棋。" << endl;

			/*
				call 依芩的函數()
			*/

			algoBasic(status, playerScore, opponentScore, vacancy);
			// 要變惡搞版的話，需要新增新函數的地方

		}
		else if (chooseOpponent == 'B') // 發發的演算法對手1
		{
			cout << "攻守交換。" << endl;
			cout << "該回合輪到 opponent " << chooseOpponent << "下棋。" << endl;
			
			/*
				call 發發的函數1()
			*/
			
			opponentB(status, beforeChipRow, beforeChipCol, opponentBeforeChipRow, opponentBeforeChipCol, opponentAfterChipRow, opponentAfterChipCol, playerScore, opponentScore, vacancy);
			// 要變惡搞版的話，需要新增新函數的地方

		}
		else if (chooseOpponent == 'C') // 發發的演算法對手2
		{
			cout << "攻守交換。" << endl;
			cout << "該回合輪到 opponent " << chooseOpponent << "下棋。" << endl;

			/*
				call 發發的函數2()
			*/

			// 要變惡搞版的話，需要新增新函數的地方

		}

		/*
			call 絲盈的棋盤輸出函數()
		*/
		printStatus(status);

		if (whoIsWinner(status) == -1) // opponent們活得勝利
		{
			cout << "攻守交換。" << endl;
			cout << "恭喜 opponent 贏得勝利" << endl;
			break; // 跳出遊戲迴圈
		}
		
		cout << "player 所擁有的棋子數：" << playerScore << endl;
		cout << "opponent 所擁有的棋子數：" << opponentScore << endl;

		cout << "攻守交換。" << endl;
	}

	cout << "遊戲結束" << endl;

}
void countChips(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int& playerScore, int& opponentScore, int& vacancy)
{
	int countPlayerScore = 0;
	int countOpponentScore = 0;
	int countVacancy = 0;

	for (int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
	{
		for (int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
			if (status[i][j] == 1) 
			{
				countPlayerScore += 1;
			}
			else if (status[i][j] == -1)
			{
				countOpponentScore += 1;
			}
			else
			{
				countVacancy += 1;
			}
		}
	}

	playerScore = countPlayerScore;
	opponentScore = countOpponentScore;
	vacancy = countVacancy;
}

void playerChangeColor(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int beforeChipRow, int beforeChipCol, int afterChipRow, int afterChipCol, int& playerScore, int& opponentScore, int& vacancy)
{
  //找到After附近八格
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <= 1; j++)
    {
      //如果超出範圍先跳過
      if(afterChipRow + i < 1 || afterChipRow + i > MAX_LINE_SQUARE_COUNT || afterChipCol + j < 1 || afterChipCol + j > MAX_LINE_SQUARE_COUNT)
        continue;
      //如果附近八格有棋子就變成player色
      if(status[afterChipRow + i][afterChipCol + j] != 0)
        status[afterChipRow + i][afterChipCol + j] = status[beforeChipRow][beforeChipCol];
    }
  }

  //判斷移動到第一圈or第二圈，決定要不要刪掉beforeChip
  int moveRow = abs(beforeChipRow - afterChipRow);
  int moveCol = abs(beforeChipCol - afterChipCol);

  //這行檢查用
  if(moveRow > 2 || moveCol > 2)
      cout << "moving error";

  if(moveRow == 1 || moveCol == 1)
      status[beforeChipRow][beforeChipCol] = status[beforeChipRow][beforeChipCol];;
  if(moveRow == 2 || moveCol == 2)
      status[beforeChipRow][beforeChipCol] = 0;

  countChips(status, playerScore, opponentScore, vacancy);
}



int whoIsWinner(int status[][MAX_LINE_SQUARE_COUNT]) 
{
	bool playerWin = true;
	bool opponentWin = true;
	for(int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
	{
		for(int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
			if(status[i][j] == 1) 
			{
				if(haveVacancy(i, j, status) == true)
				{
					opponentWin = false;
				}
			}
		}
		
	}
	for(int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
	{
		for(int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
			if(status[i][j] < 0) 
			{
				if(haveVacancy(i, j, status) == true)
				{
					playerWin = false;
				}
			}
		}
		
	}
	if(playerWin == false && opponentWin == false)
		return 0;
	else if(playerWin == true && opponentWin == false)
		return 1;
	else if(playerWin == false && opponentWin == true)
		return -1;
	else 
		return 0;
}

bool haveVacancy(int x, int y, int status[][MAX_LINE_SQUARE_COUNT])
{
	bool hasVacancy = false;
	for (int i = x - 2; i <= x + 2; i++)	
	{
		if (i >= 0 and i < MAX_LINE_SQUARE_COUNT)
		{
			for (int j = y - 2; j <= y + 2; j++)
			{
				if (j >= 0 and j < MAX_LINE_SQUARE_COUNT)
				{
					if (status[i][j] == 0)
						return true;
				}
			}
		}
	}
	return false;
}

void printStatus(int status[][MAX_LINE_SQUARE_COUNT])
{
	for(int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
	{
		for(int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
			cout << status[i][j] << " ";
		}
		cout << endl;
	}
}

/*
	依芩和發發的函數()
	{
		要能決定opponent棋子下在哪裡(主要演算法)
		然後直接在函數裡做playerChangeColor(opponentBeforeChipRow, opponentBeforeChipCol, opponentAfterChipRow, opponentAfterChipCol)
	}
*/

/*
	絲盈的函數()
	{
	
	}
*/
//需要多兩個資料庫ㄛ


void algoBasic(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT], int& playerScore, int& opponentScore, int& vacancy)
{
  bool chipChoose = 0;
  int opponentBeforeChipRow = 0, opponentBeforeChipCol = 0;
  int opponentAfterChipRow = 0, opponentAfterChipCol = 0;

  srand(time(nullptr));
  int rn = 0;
  while(chipChoose == 0)
  {
    //亂數選before位置
    cout<<rand()<<"\n";
    rn = rand()%(opponentScore+1); 
    cout << "before"<<rn<<"\n";
    int beforeCnt = 0; //現有的第幾個opponent棋子
    for(int i = 1; i <= MAX_LINE_SQUARE_COUNT; i++)
    {
      for(int j = 1; j <= MAX_LINE_SQUARE_COUNT; j++)
      {
        if(status[i][j] == -1)
        {
          if(beforeCnt == rn)
          {
            opponentBeforeChipRow = i;
            opponentBeforeChipCol = j;
            //cout << "before"<<"\n";
            //cout << "i" << i <<","<<"j"<<j<<"\n";
            i = MAX_LINE_SQUARE_COUNT + 1;
            break;
          }
          else
            beforeCnt += 1;
        }
        else
          continue;
      }
    }
    
    //cout << "after" << "\n";
    //確定after位置
    int afterCandidateCnt = 0;
    for(int i = -2; i <= 2; i++)
      for(int j = -2; j <= 2; j++)
      {
        if(opponentBeforeChipRow + i < 1 || opponentBeforeChipRow + i > MAX_LINE_SQUARE_COUNT || opponentBeforeChipCol + j < 1 || opponentBeforeChipCol + j > MAX_LINE_SQUARE_COUNT)
          continue;
        if(status[opponentBeforeChipRow+i][opponentBeforeChipCol+j] == 0)
          afterCandidateCnt += 1;
      }
    //cout<<"afterCandidateCnt" << afterCandidateCnt<<"\n";

    if(afterCandidateCnt == 0)
      chipChoose = 0;
    else
    {
      for(int i = -2; i <= 2; i++)
        for(int j = -2; j <= 2; j++)
        {
          if(opponentBeforeChipRow + i < 1 || opponentBeforeChipRow + i > MAX_LINE_SQUARE_COUNT || opponentBeforeChipCol + j < 1 || opponentBeforeChipCol + j > MAX_LINE_SQUARE_COUNT)
            continue;
          rn = rand()%(afterCandidateCnt+1);
          int afterCnt = 0; //選第幾個after棋子位置
          if(status[opponentBeforeChipRow + i][opponentBeforeChipCol + j] == 0) //棋子可放這裡！
          {
            if(afterCnt == rn)
            {
              opponentAfterChipRow = opponentBeforeChipRow + i;
              opponentAfterChipCol = opponentBeforeChipCol + j;
              //cout << "row" << opponentAfterChipRow <<","<<"col"<<opponentAfterChipCol<<"\n";
              chipChoose = 1;
              i = 3;
              break;
            }
            else
              afterCnt += 1;
          }
          else
            continue;
        }
    }
  }
  cout << "row" << opponentBeforeChipRow <<","<<"col"<<opponentBeforeChipCol<<"\n";
  cout << "row" << opponentAfterChipRow <<","<<"col"<<opponentAfterChipCol<<"\n";

  //下棋在before的位置
  status[opponentBeforeChipRow][opponentBeforeChipCol] = -1;
  playerChangeColor(status, opponentBeforeChipRow, opponentBeforeChipCol, opponentAfterChipRow, opponentAfterChipCol, playerScore, opponentScore, vacancy);
}

//	    srand( time(NULL));
//    for(int i = 0 ; i < 10 ;i++){
//        cout << rand() << endl;
//    }//產生一個亂數！


void opponentB(int status[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT],int beforeChipRow,int beforeChipCol,int opponentBeforeChipRow,int opponentBeforeChipCol,int opponentAfterChipRow,int opponentAfterChipCol , int& playerScore, int& opponentScore, int& vacancy)
{
	int record=0;;//紀錄某步下期策略模擬得到的分數
	int decisionCol=0;//目前決定下哪一格		
	int decisionRow=0;	
	int Duplicate[MAX_LINE_SQUARE_COUNT][MAX_LINE_SQUARE_COUNT];
	for (int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
	{
		for (int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
		Duplicate[i][j]=status[i][j];//複製一個模擬下棋的棋盤
		}
	}

	for (int i = 0; i< MAX_LINE_SQUARE_COUNT; i++)
	{
		for (int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
		{
			if (Duplicate[i][j]=-1)//如果某格有電腦的棋子！
			{

				srand( time(NULL));


				for (int m = i-2; m <= i+2; m++)
				{
					for (int n = j-2; n <= j+2; n++)
					{
					
						if (Duplicate[n][m]==0)//周圍有空位的話！就來模擬下完之後的影響
						{
									
			                int a=1;
			                int b=10;
			                int x = rand() % (b - a + 1) + a;

			                if (x<9)//有80%機率會分析到走這顆一步的影響！可以更改這個機率，就能增加這個電腦的分析次數！
			                //所以我想說可以更改分析次數來增加對手難度！
			                {
											//在複製出的棋盤上下這一步，判定分數是否成長
			                //注意！這時要丟的棋盤是Duplicate!
			                	playerChangeColor(status, opponentBeforeChipRow, opponentBeforeChipCol, opponentAfterChipRow, opponentAfterChipCol, playerScore, opponentScore, vacancy);
								countChips(status, playerScore, opponentScore, vacancy);

								if (opponentScore>record)//目前模擬分數最高
								{
									decisionRow=n;
									decisionCol=m;//目前決定下這格
								}

								//重置棋盤，進行下一次模擬
								for (int i = 0; i < MAX_LINE_SQUARE_COUNT; i++)
								{
									for (int j = 0; j < MAX_LINE_SQUARE_COUNT; j++)
									{
										Duplicate[i][j]=status[i][j];
									}	
								}                
		     			    }                  

						}		
					}
				}
							
			//看他走每一格的時候自己的顏色會不會變多。
			}				
		}		
	}
	//全部的空位都模擬結束之後，開始下棋！更動原本的棋盤
	
	status[decisionRow][decisionCol]=1;

	playerChangeColor(status, opponentBeforeChipRow, opponentBeforeChipCol, opponentAfterChipRow, opponentAfterChipCol, playerScore, opponentScore, vacancy);
	countChips(status, playerScore, opponentScore, vacancy);
}

