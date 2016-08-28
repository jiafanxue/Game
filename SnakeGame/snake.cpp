#include "snake.h"

/*
*	函数名称：Snake
*	函数作用：蛇类的含参构造函数
*	函数参数：coordX -- 横坐标
*			  coordY -- 纵坐标
*			  len -- 长度
*/
Snake::Snake(int coordX, int coordY, int len)
{
	if (len<1) 
		len = 1;
	int i;

	m_length = len;  //蛇的身体体长

	//初始化蛇的坐标位置
	m_pPos = new SPoint[m_length + 2];
	m_pPos[0].x = coordX; m_pPos[0].y = coordY;
	for (i = 1; i<m_length + 2; i++)
	{
		m_pPos[i].x = 0; m_pPos[i].y = 0;
	}

	//初始化蛇的运动状态
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

	//初始化蛇的位图显示状态
	m_pStateArray = new BitmapState[m_length + 2];
	for (i = 0; i<m_length + 2; i++)
		m_pStateArray[i] = M_NONE;
}

/*
*	函数名称：~Snake
*	函数作用：蛇类的析构函数
*	函数参数：无
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
*	函数名称：GetRightState
*	函数作用：根据前后的运动状态，获得正确的方向
*	函数参数：oldDirect -- 先前方向
*			  newDirect -- 现在方向
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
*	函数名称：ChangeDirect
*	函数作用：改变方向
*	函数参数：d -- 方向
*/
void Snake::ChangeDirect(MoveState d)
{
	//  改变方向的条件：非对立方向
	//  只能为其左，前，右方
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
*	函数名称：Move
*	函数作用：蛇的移动
*	函数参数：无
*/
void Snake::Move(void)
{
	//保存蛇身体各个部位的形状
	for (int i = 0; i<m_length; i++)
	{
		m_oldSnake.body[i] = m_newSnake.body[i];
	}

	//将蛇身体的状态根据前面的状态变动一次
	m_newSnake.tail = m_newSnake.body[m_length - 1];
	for (int i = m_length - 1; i>0; i--)
	{
		m_newSnake.body[i] = m_newSnake.body[i - 1];
	}
	m_newSnake.body[0] = m_newSnake.head;

	//根据新旧状态特性取正确的状态
	m_pStateArray[0] = GetRightState(m_oldSnake.head, m_newSnake.head);
	for (int i = 0; i<m_length; i++)
		m_pStateArray[i + 1] = GetRightState(m_oldSnake.body[i], m_newSnake.body[i]);
	m_pStateArray[m_length + 1] = GetRightState(m_oldSnake.tail, m_newSnake.tail);

	//除蛇头外，其他部分的新位置为其前一部分的原来位置
	for (int i = m_length + 1; i>0; i--)
		m_pPos[i] = m_pPos[i - 1];


	//蛇头的新位置根据蛇的运动方向判断做相应偏移
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
*	函数名称：AddBody
*	函数作用：增加身体长度
*	函数参数：n -- 增加的长度
*/
void Snake::AddBody(int n)
{
	//  蛇的各种数据状态
	//保存蛇的状态信息
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

	//将长度增长
	m_length += n;

	//释放所有蛇的身体存储数据空间
	SAFE_DELETE_ARRAY(m_pPos);
	SAFE_DELETE_ARRAY(m_pStateArray);
	SAFE_DELETE_ARRAY(m_newSnake.body);
	SAFE_DELETE_ARRAY(m_oldSnake.body);

	//创建并初始化增长后的蛇的存储数据空间

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

	//恢复原来长度的数据(新的用初始化的数据)
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
*	函数名称：SetHeadPos
*	函数作用：设置头部的坐标
*	函数参数：coordX -- 横坐标
*			  coordY -- 纵坐标
*/
void Snake::SetHeadPos(int coordX, int coordY)
{
	m_pPos[0].x = coordX;
	m_pPos[0].y = coordY;
}

/*
*	函数名称：GetStateArray
*	函数作用：获取蛇的状态数组
*	函数参数：无
*/
BitmapState* Snake::GetStateArray(void)
{
	return m_pStateArray;
}

/*
*	函数名称：GetPos
*	函数作用：获取蛇的坐标指针
*	函数参数：无
*/
SPoint* Snake::GetPos(void)
{
	return m_pPos;
}

/*
*	函数名称：GetLength
*	函数作用：获取蛇的长度
*	函数参数：无
*/
int Snake::GetLength(void)
{
	return m_length + 2;
}

/*
*	函数名称：IsHeadTouchBody
*	函数作用：判断是否碰撞身体
*	函数参数：coordX -- 横坐标
*			  coordY -- 纵坐标
*/
bool Snake::IsHeadTouchBody(int coordX, int coordY)
{
	for (int i = 1; i<m_length + 2; i++)
		if (m_pPos[i].x == coordX && m_pPos[i].y == coordY)
			return true;
	return false;
}

/*
*	函数名称：Initial
*	函数作用：重置游戏，并初始化蛇
*	函数参数：无
*/
void Snake::Initial(void)
{
	//释放以前的所有存储空间
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

	//创建蛇身长度为1的蛇，并做各种初始化
	int i;
	int x = 0;
	int y = 0;

	//初始化蛇的坐标位置
	m_length = 1;
	m_pPos = new SPoint[m_length + 2];
	m_pPos[0].x = x; m_pPos[0].y = y;
	for (i = 1; i<m_length + 2; i++)
	{
		m_pPos[i].x = 0; m_pPos[i].y = 0;
	}

	//初始化蛇的运动状态
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

	//初始化蛇的位图显示状态
	m_pStateArray = new BitmapState[m_length + 2];
	for (i = 0; i<m_length + 2; i++)
		m_pStateArray[i] = M_NONE;
}