#include "table.h"

/*
*	�������ƣ�Table
*	�������ã�Ĭ�Ϲ��캯�������û�����������ֵ
*	������������
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
*	�������ƣ�Table
*	�������ã����ι��캯��
*	������������
*/
Table::Table(int width, int height)
{
	m_iBlockNumber = 0;
	m_iFoodNumber = 0;
	m_iHeight = height;
	m_iWidth = width;

	// ��ʼ������
	// ֱ��InitialTable(width, height)?
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
	// ��������Ϊǽ
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i][0] = TABLE_STATE_SBLOCK;
		m_pBoard[i][m_iWidth - 1] = TABLE_STATE_SBLOCK;
	}
	// ��������Ϊǽ
	for (int i = 0; i < width; i++)
	{
		m_pBoard[0][i] = TABLE_STATE_SBLOCK;
		m_pBoard[m_iHeight - 1][i] = TABLE_STATE_SBLOCK;
	}
}

/*
*	�������ƣ�~Table
*	�������ã��������������ָ��
*	������������
*/
Table::~Table()
{
	// ������ָ�벻Ϊ�յ�ʱ��
	if (m_pBoard != NULL)
	{
		SAFE_DELETE_ARRAY(m_pBoard);
	}
}

/*
*	�������ƣ�InitalTable
*	�������ã���ʼ������
*	����������width -- ����Ŀ������꣩
*			  height -- ����ĸߣ������꣩
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
	// ��������Ϊǽ
	for (int i = 0; i < height; i++)
	{
		m_pBoard[i][0] = TABLE_STATE_SBLOCK;
		m_pBoard[i][m_iWidth - 1] = TABLE_STATE_SBLOCK;
	}
	// ��������Ϊǽ
	for (int i = 0; i < width; i++)
	{
		m_pBoard[0][i] = TABLE_STATE_SBLOCK;
		m_pBoard[m_iHeight - 1][i] = TABLE_STATE_SBLOCK;
	}
}

/*
*	�������ƣ�AddFood
*	�������ã����ʳ��
*	����������width -- ����Ŀ������꣩
*			  height -- ����ĸߣ������꣩
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
*	�������ƣ�AddBlock
*	�������ã���Ӷ���
*	����������width -- ����Ŀ������꣩
*			  height -- ����ĸߣ������꣩
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
*	�������ƣ�ClearFood
*	�������ã��������
*	����������width -- ����Ŀ������꣩
*			  height -- ����ĸߣ������꣩
*/
bool Table::ClearFood(int coordX, int coordY)
{
	m_pBoard[coordY][coordX] = TABLE_STATE_OK; // ʳ��1 �� ����0
	return true;
}

/*
*	�������ƣ�getSnake
*	�������ã���ȡ�����ϵ���
*	������������
*/
Snake *Table::GetSnake()
{
	return &m_sSnake;
}

/*
*	�������ƣ�getBoard
*	�������ã���ȡ��������
*	������������
*/
int **Table::GetBoard()
{
	return m_pBoard;
}

/*
*	�������ƣ�getDateInBoard
*	�������ã���ȡ�����ϵ���Ϣ
*	����������width -- ����Ŀ������꣩
*			  height -- ����ĸߣ������꣩
*/
int Table::GetData(int coordX, int coordY)
{
	return m_pBoard[coordY][coordX];
}

/*
*	�������ƣ�SnakeMove
*	�������ã��ߵ��ƶ�
*	������������
*/
void Table::SnakeMove()
{
	m_sSnake.Move();
}

/*
*	�������ƣ�ChangeSnakeDirect
*	�������ã��ı��ߵķ���
*	����������MoveState -- ö�ٷ������
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