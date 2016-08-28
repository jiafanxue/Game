#ifndef TABLE_H
#define TABLE_H

#define TABLE_STATE_OK		0			// 正常
#define TABLE_STATE_FOOD	1			// 食物
#define TABLE_STATE_BLOCK	2			// 毒果
#define TABLE_STATE_SBLOCK	3			// 墙壁

#include "snake.h"

class Table
{
public:
	Table();									// 构造函数
	Table(int width, int height);				// 含参构造函数
	~Table();									// 析构函数

	void InitalTable(int width, int height);	// 初始化桌子
	// 桌面上的食物
	bool AddFood(int coordX, int coordY);		// 添加食物
	bool AddBlock(int coordX, int coordY);		// 添加毒果
	bool ClearFood(int coordX, int coordY);		// 清除食物
	// 获取信息
	Snake *GetSnake();							// 获取桌面上的蛇	
	int **GetBoard();							// 获取桌面数组	
	int GetData(int coordX, int coordY);	// 获取桌面上的信息
	int GetWidth();
	int GetHeight();

	// 蛇的操作
	void SnakeMove();
	bool ChangeSnakeDirect(MoveState d);

private:
	int m_iWidth;				// 桌子的宽度
	int m_iHeight;				// 桌子的高度
	int m_iFoodNumber;			// 水果的数目
	int m_iBlockNumber;			// 毒果的数目
	Snake m_sSnake;				// 桌面上的蛇
	int **m_pBoard;				// 桌面数组 -- 面板
};

#endif // !TABLE_H
