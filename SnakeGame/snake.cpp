#include "snake.h"

/*
*	�������ƣ�Snake
*	�������ã�����ĺ��ι��캯��
*	����������coordX -- ������
*			  coordY -- ������
*			  len -- ����
*/
Snake::Snake(int coordX, int coordY, int len)
{
	if (len<1) 
		len = 1;
	int i;

	m_length = len;  //�ߵ������峤

	//��ʼ���ߵ�����λ��
	m_pPos = new SPoint[m_length + 2];
	m_pPos[0].x = coordX; m_pPos[0].y = coordY;
	for (i = 1; i<m_length + 2; i++)
	{
		m_pPos[i].x = 0; m_pPos[i].y = 0;
	}

	//��ʼ���ߵ��˶�״̬
	m_newSnake.head = S_NONE;
	m_oldSnake.head = S_NONE;
	m_newSnake.body = new MoveState[m_length];
	m_oldSnake.body = new MoveState[m_length];
	for (i = 0; i<m_length; i++)
	{
		m_newSnake.body[i] = S_NONE;
		m_newSnake.body[i] = S_NONE;
	}
	m_newSnake.tail = S_NONE;
	m_oldSnake.tail = S_NONE;

	//��ʼ���ߵ�λͼ��ʾ״̬
	m_pStateArray = new BitmapState[m_length + 2];
	for (i = 0; i<m_length + 2; i++)
		m_pStateArray[i] = M_NONE;
}

/*
*	�������ƣ�~Snake
*	�������ã��������������
*	������������
*/
Snake::~Snake()
{
	SAFE_DELETE_ARRAY(m_pStateArray);
	SAFE_DELETE_ARRAY(m_pPos);
	if (m_newSnake.body != NULL)
	{
		SAFE_DELETE_ARRAY(m_newSnake.body);
	}
	if (m_oldSnake.body != NULL)
	{
		SAFE_DELETE_ARRAY(m_oldSnake.body);
	}
}

/*
*	�������ƣ�GetRightState
*	�������ã�����ǰ����˶�״̬�������ȷ�ķ���
*	����������oldDirect -- ��ǰ����
*			  newDirect -- ���ڷ���
*/
BitmapState Snake::GetRightState(MoveState oldDirect, MoveState newDirect)
{
	BitmapState res;
	switch (oldDirect)
	{
	case S_NONE:
		switch (newDirect)
		{
		case S_NONE:
			res = M_NONE;
			break;
		case S_UP:
			res = M_UP_UP;
			break;
		case S_DOWN:
			res = M_DOWN_DOWN;
			break;
		case S_LEFT:
			res = M_LEFT_LEFT;
			break;
		case S_RIGHT:
			res = M_RIGHT_RIGHT;
			break;
		}
		break;
	case S_UP:
		switch (newDirect)
		{
		case S_UP:
			res = M_UP_UP;
			break;
		case S_LEFT:
			res = M_UP_LEFT;
			break;
		case S_RIGHT:
			res = M_UP_RIGHT;
			break;
		}
		break;
	case S_DOWN:
		switch (newDirect)
		{
		case S_DOWN:
			res = M_DOWN_DOWN;
			break;
		case S_LEFT:
			res = M_DOWN_LEFT;
			break;
		case S_RIGHT:
			res = M_DOWN_RIGHT;
			break;
		}
		break;
	case S_LEFT:
		switch (newDirect)
		{
		case S_LEFT:
			res = M_LEFT_LEFT;
			break;
		case S_UP:
			res = M_LEFT_UP;
			break;
		case S_DOWN:
			res = M_LEFT_DOWN;
			break;
		}
		break;
	case S_RIGHT:
		switch (newDirect)
		{
		case S_RIGHT:
			res = M_RIGHT_RIGHT;
			break;
		case S_UP:
			res = M_RIGHT_UP;
			break;
		case S_DOWN:
			res = M_RIGHT_DOWN;
			break;
		}
		break;
	}
	return res;
}

/*
*	�������ƣ�ChangeDirect
*	�������ã��ı䷽��
*	����������d -- ����
*/
void Snake::ChangeDirect(MoveState d)
{
	//  �ı䷽����������Ƕ�������
	//  ֻ��Ϊ����ǰ���ҷ�
	switch (d)
	{
	case S_NONE:
		m_newSnake.head = d;
		break;
	case S_UP:
		if (m_newSnake.head != S_DOWN) 
			m_newSnake.head = d;
		break;
	case S_DOWN:
		if (m_newSnake.head != S_UP) 
			m_newSnake.head = d;
		break;
	case S_LEFT:
		if (m_newSnake.head != S_RIGHT) 
			m_newSnake.head = d;
		break;
	case S_RIGHT:
		if (m_newSnake.head != S_LEFT) 
			m_newSnake.head = d;
		break;
	}
}

/*
*	�������ƣ�Move
*	�������ã��ߵ��ƶ�
*	������������
*/
void Snake::Move(void)
{
	//���������������λ����״
	for (int i = 0; i<m_length; i++)
	{
		m_oldSnake.body[i] = m_newSnake.body[i];
	}

	//���������״̬����ǰ���״̬�䶯һ��
	m_newSnake.tail = m_newSnake.body[m_length - 1];
	for (int i = m_length - 1; i>0; i--)
	{
		m_newSnake.body[i] = m_newSnake.body[i - 1];
	}
	m_newSnake.body[0] = m_newSnake.head;

	//�����¾�״̬����ȡ��ȷ��״̬
	m_pStateArray[0] = GetRightState(m_oldSnake.head, m_newSnake.head);
	for (int i = 0; i<m_length; i++)
		m_pStateArray[i + 1] = GetRightState(m_oldSnake.body[i], m_newSnake.body[i]);
	m_pStateArray[m_length + 1] = GetRightState(m_oldSnake.tail, m_newSnake.tail);

	//����ͷ�⣬�������ֵ���λ��Ϊ��ǰһ���ֵ�ԭ��λ��
	for (int i = m_length + 1; i>0; i--)
		m_pPos[i] = m_pPos[i - 1];


	//��ͷ����λ�ø����ߵ��˶������ж�����Ӧƫ��
	switch (m_newSnake.head)
	{
	case S_UP:
		m_pPos[0].y -= SNAKE_MOVE;
		break;
	case S_DOWN:
		m_pPos[0].y += SNAKE_MOVE;
		break;
	case S_LEFT:
		m_pPos[0].x -= SNAKE_MOVE;
		break;
	case S_RIGHT:
		m_pPos[0].x += SNAKE_MOVE;
		break;
	}

}

/*
*	�������ƣ�AddBody
*	�������ã��������峤��
*	����������n -- ���ӵĳ���
*/
void Snake::AddBody(int n)
{
	//  �ߵĸ�������״̬
	//�����ߵ�״̬��Ϣ
	//  1.m_oldSnake
	//  2.m_newSnake
	//  3.m_pStateArray
	//  4.m_pPos

	//1
	Snake_Struct saveOldSnake;
	saveOldSnake.head = m_oldSnake.head;
	saveOldSnake.body = new MoveState[m_length];
	for (int i = 0; i<m_length; i++)
		saveOldSnake.body[i] = m_oldSnake.body[i];
	saveOldSnake.tail = m_oldSnake.tail;

	//2
	Snake_Struct saveNewSnake;
	saveNewSnake.head = m_newSnake.head;
	saveNewSnake.body = new MoveState[m_length];
	for (int i = 0; i<m_length; i++)
		saveNewSnake.body[i] = m_newSnake.body[i];
	saveNewSnake.tail = m_newSnake.tail;

	//3
	BitmapState *savestateArray;
	savestateArray = new BitmapState[m_length + 2];
	for (int i = 0; i<m_length + 2; i++)
		savestateArray[i] = m_pStateArray[i];

	//4
	SPoint *savePos;
	savePos = new SPoint[m_length + 2];
	for (int i = 0; i<m_length + 2; i++)
		savePos[i] = m_pPos[i];

	//����������
	m_length += n;

	//�ͷ������ߵ�����洢���ݿռ�
	SAFE_DELETE_ARRAY(m_pPos);
	SAFE_DELETE_ARRAY(m_pStateArray);
	SAFE_DELETE_ARRAY(m_newSnake.body);
	SAFE_DELETE_ARRAY(m_oldSnake.body);

	//��������ʼ����������ߵĴ洢���ݿռ�

	m_newSnake.head = S_NONE;
	m_oldSnake.head = S_NONE;
	m_newSnake.body = new MoveState[m_length];
	m_oldSnake.body = new MoveState[m_length];
	for (int i = 0; i<m_length; i++)
	{
		m_newSnake.body[i] = S_NONE;
		m_newSnake.body[i] = S_NONE;
	}
	m_newSnake.tail = S_NONE;
	m_oldSnake.tail = S_NONE;

	m_pStateArray = new BitmapState[m_length + 2];
	for (int i = 0; i<m_length + 2; i++)
		m_pStateArray[i] = M_NONE;

	m_pPos = new SPoint[m_length + 2];
	for (int i = 0; i<m_length + 2; i++)
	{
		m_pPos[i].x = 0;
		m_pPos[i].y = 0;
	}

	//�ָ�ԭ�����ȵ�����(�µ��ó�ʼ��������)
	//1. m_newSnake ,m_oldSnake
	//2. stateArray
	//3. pos

	//1
	m_newSnake.head = saveNewSnake.head;
	m_oldSnake.head = saveOldSnake.head;
	for (int i = 0; i<m_length - n; i++)
	{
		m_newSnake.body[i] = saveNewSnake.body[i];
		m_oldSnake.body[i] = saveOldSnake.body[i];
	}
	m_newSnake.tail = saveNewSnake.tail;
	m_oldSnake.tail = saveOldSnake.tail;

	//2
	for (int i = 0; i<m_length - n + 2; i++)
		m_pStateArray[i] = savestateArray[i];

	//3
	for (int i = 0; i<m_length - n + 2; i++)
		m_pPos[i] = savePos[i];
}

/*
*	�������ƣ�SetHeadPos
*	�������ã�����ͷ��������
*	����������coordX -- ������
*			  coordY -- ������
*/
void Snake::SetHeadPos(int coordX, int coordY)
{
	m_pPos[0].x = coordX;
	m_pPos[0].y = coordY;
}

/*
*	�������ƣ�GetStateArray
*	�������ã���ȡ�ߵ�״̬����
*	������������
*/
BitmapState* Snake::GetStateArray(void)
{
	return m_pStateArray;
}

/*
*	�������ƣ�GetPos
*	�������ã���ȡ�ߵ�����ָ��
*	������������
*/
SPoint* Snake::GetPos(void)
{
	return m_pPos;
}

/*
*	�������ƣ�GetLength
*	�������ã���ȡ�ߵĳ���
*	������������
*/
int Snake::GetLength(void)
{
	return m_length + 2;
}

/*
*	�������ƣ�IsHeadTouchBody
*	�������ã��ж��Ƿ���ײ����
*	����������coordX -- ������
*			  coordY -- ������
*/
bool Snake::IsHeadTouchBody(int coordX, int coordY)
{
	for (int i = 1; i<m_length + 2; i++)
		if (m_pPos[i].x == coordX && m_pPos[i].y == coordY)
			return true;
	return false;
}

/*
*	�������ƣ�Initial
*	�������ã�������Ϸ������ʼ����
*	������������
*/
void Snake::Initial(void)
{
	//�ͷ���ǰ�����д洢�ռ�
	SAFE_DELETE_ARRAY(m_pStateArray);
	SAFE_DELETE_ARRAY(m_pPos);
	if (m_newSnake.body != NULL)
	{
		SAFE_DELETE_ARRAY(m_newSnake.body);
	}
	if (m_oldSnake.body != NULL)
	{
		SAFE_DELETE_ARRAY(m_oldSnake.body);
	}

	//����������Ϊ1���ߣ��������ֳ�ʼ��
	int i;
	int x = 0;
	int y = 0;

	//��ʼ���ߵ�����λ��
	m_length = 1;
	m_pPos = new SPoint[m_length + 2];
	m_pPos[0].x = x; m_pPos[0].y = y;
	for (i = 1; i<m_length + 2; i++)
	{
		m_pPos[i].x = 0; m_pPos[i].y = 0;
	}

	//��ʼ���ߵ��˶�״̬
	m_newSnake.head = S_NONE;
	m_oldSnake.head = S_NONE;
	m_newSnake.body = new MoveState[m_length];
	m_oldSnake.body = new MoveState[m_length];
	for (i = 0; i<m_length; i++)
	{
		m_newSnake.body[i] = S_NONE;
		m_newSnake.body[i] = S_NONE;
	}
	m_newSnake.tail = S_NONE;
	m_oldSnake.tail = S_NONE;

	//��ʼ���ߵ�λͼ��ʾ״̬
	m_pStateArray = new BitmapState[m_length + 2];
	for (i = 0; i<m_length + 2; i++)
		m_pStateArray[i] = M_NONE;
}