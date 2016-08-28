#include "table.h"

/*
*	函数名称：Table
*	函数作用：默认构造函数，设置基础的左面数值
*	函数参数：无
*/

Table::Table()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_iBlockNumber = 0;
	m_iFoodNumber = 0;
	m_pBoard = NULL; // m)pBoard = nullptr
}

/*
*	函数名称：Table
*	函数作用：含参构造函数
*	函数参数：无
*/
Table::Table(int width, int height)
{
	m_iBlockNumber = 0;
	m_iFoodNumber = 0;
	m_iHeight = height;
	m_iWidth = width;

	// 初始化桌子
	// 直接InitialTable(width, height)?
	m_pBoard = new int*[m_iHeight];
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i] = new int[m_iWidth];
		for (int j = 0; j < width; j++)
		{
			m_pBoard[i][j] = 0;
		}
	}

	// 0 0 0 0 0		1 1 1 1 1
	// 0 0 0 0 0		1 0 0 0 1
	// 0 0 0 0 0		1 0 0 0 1
	// 0 0 0 0 0		1 1 1 1 1
	// 左右设置为墙
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i][0] = TABLE_STATE_SBLOCK;
		m_pBoard[i][m_iWidth - 1] = TABLE_STATE_SBLOCK;
	}
	// 上下设置为墙
	for (int i = 0; i < width; i++)
	{
		m_pBoard[0][i] = TABLE_STATE_SBLOCK;
		m_pBoard[m_iHeight - 1][i] = TABLE_STATE_SBLOCK;
	}
}

/*
*	函数名称：~Table
*	函数作用：析构函数，清除指针
*	函数参数：无
*/
Table::~Table()
{
	// 当桌面指针不为空的时候
	if (m_pBoard != NULL)
	{
		SAFE_DELETE_ARRAY(m_pBoard);
	}
}

/*
*	函数名称：InitalTable
*	函数作用：初始化桌子
*	函数参数：width -- 桌面的宽（横坐标）
*			  height -- 桌面的高（纵坐标）
*/
void Table::InitalTable(int width, int height)
{
	m_iHeight = height;
	m_iWidth = width;

	m_sSnake.Initial();
	if (m_pBoard != NULL)
	{
		SAFE_DELETE_ARRAY(m_pBoard);
	}

	m_pBoard = new int*[m_iHeight];
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i] = new int[m_iWidth];
		for (int j = 0; j < width; j++)
		{
			m_pBoard[i][j] = 0;
		}
	}

	// 0 0 0 0 0		1 1 1 1 1
	// 0 0 0 0 0		1 0 0 0 1
	// 0 0 0 0 0		1 0 0 0 1
	// 0 0 0 0 0		1 1 1 1 1
	// 左右设置为墙
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i][0] = TABLE_STATE_SBLOCK;
		m_pBoard[i][m_iWidth - 1] = TABLE_STATE_SBLOCK;
	}
	// 上下设置为墙
	for (int i = 0; i < width; i++)
	{
		m_pBoard[0][i] = TABLE_STATE_SBLOCK;
		m_pBoard[m_iHeight - 1][i] = TABLE_STATE_SBLOCK;
	}
}

/*
*	函数名称：AddFood
*	函数作用：添加食物
*	函数参数：width -- 桌面的宽（横坐标）
*			  height -- 桌面的高（纵坐标）
*/
bool Table::AddFood(int coordX, int coordY)
{
	if (coordX >= 0 && coordX < m_iWidth && coordY >= 0 && coordY < m_iHeight && m_pBoard[coordY][coordX] == TABLE_STATE_OK)
	{
		m_pBoard[coordY][coordX] = TABLE_STATE_FOOD;
		++m_iFoodNumber;
		return true;
	}
	else
	{
		return false;
	}
}

/*
*	函数名称：AddBlock
*	函数作用：添加毒果
*	函数参数：width -- 桌面的宽（横坐标）
*			  height -- 桌面的高（纵坐标）
*/
bool Table::AddBlock(int coordX, int coordY)
{
	if (coordX >= 0 && coordX < m_iWidth && coordY >= 0 && coordY < m_iHeight && m_pBoard[coordY][coordX] == TABLE_STATE_OK)
	{
		m_pBoard[coordY][coordX] = TABLE_STATE_BLOCK;
		++m_iBlockNumber;
		return true;
	}
	else
	{
		return false;
	}
}

/*
*	函数名称：ClearFood
*	函数作用：清除桌面
*	函数参数：width -- 桌面的宽（横坐标）
*			  height -- 桌面的高（纵坐标）
*/
bool Table::ClearFood(int coordX, int coordY)
{
	m_pBoard[coordY][coordX] = TABLE_STATE_OK; // 食物1 ， 正常0
	return true;
}

/*
*	函数名称：getSnake
*	函数作用：获取桌面上的蛇
*	函数参数：无
*/
Snake *Table::GetSnake()
{
	return &m_sSnake;
}

/*
*	函数名称：getBoard
*	函数作用：获取桌面数组
*	函数参数：无
*/
int **Table::GetBoard()
{
	return m_pBoard;
}

/*
*	函数名称：getDateInBoard
*	函数作用：获取桌面上的信息
*	函数参数：width -- 桌面的宽（横坐标）
*			  height -- 桌面的高（纵坐标）
*/
int Table::GetData(int coordX, int coordY)
{
	return m_pBoard[coordY][coordX];
}

/*
*	函数名称：SnakeMove
*	函数作用：蛇的移动
*	函数参数：无
*/
void Table::SnakeMove()
{
	m_sSnake.Move();
}

/*
*	函数名称：ChangeSnakeDirect
*	函数作用：改变蛇的方向
*	函数参数：MoveState -- 枚举方向参数
*/
bool Table::ChangeSnakeDirect(MoveState d)
{
	m_sSnake.ChangeDirect(d);
	return true;
}

int Table::GetHeight()
{
	return m_iHeight;
}

int Table::GetWidth()
{
	return m_iWidth;
}