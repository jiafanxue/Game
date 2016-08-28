#ifndef SNKAE_H
#define SANKE_H

#define SNAKE_MOVE			   1								// 定义蛇每次移动的时间
#define SAFE_DELETE_ARRAY(p)  {delete[] (p); (p) = NULL;}		// 释放数组指针
#define SAFE_DELETE_NARRAY(p) {delete (p); (p) = NULL;}			// 释放指针

#include <stdio.h>

// 定义蛇的位图
enum BitmapState{
	M_NONE, 
	M_UP_UP, 
	M_DOWN_DOWN, 
	M_LEFT_LEFT, 
	M_RIGHT_RIGHT,
	M_UP_LEFT, 
	M_UP_RIGHT, 
	M_LEFT_UP,
	M_LEFT_DOWN,
	M_RIGHT_UP, 
	M_RIGHT_DOWN, 
	M_DOWN_RIGHT, 
	M_DOWN_LEFT
};

// 定义蛇移动的运动状态
enum MoveState {
	S_NONE,
	S_UP,
	S_DOWN,
	S_LEFT,
	S_RIGHT
};

// 定义蛇坐标的数据结构
struct SPoint
{
	int x;
	int y;
};

class Snake
{
	struct Snake_Struct				// 定义蛇每个部位的结构
	{
		MoveState head;
		MoveState *body; 
		MoveState tail;
	};
public:
	Snake(int coordX = 0, int coordY = 0, int len = 1);			// 蛇类的含参构造函数
	~Snake();													// 蛇类的析构函数

public:
	void Move();												// 蛇的移动
	void ChangeDirect(MoveState d);								// 改变方向
	void AddBody(int len = 1);									// 增加身体长度
	void SetHeadPos(int coordX, int coordY);					// 设置头部的坐标
	BitmapState *GetStateArray();								// 获取蛇的状态数组
	SPoint *GetPos();											// 获取蛇的坐标指针
	int GetLength();											// 获取蛇的长度
	bool IsHeadTouchBody(int coordX, int coordY);				// 判断是否碰撞身体
	void Initial();												// 重置游戏，并初始化蛇

private:
	int m_length;												//蛇的长度
	Snake_Struct m_newSnake;									//蛇的新态的所有节点运动状态
	Snake_Struct m_oldSnake;									//蛇的原态的所有节点运动状态
	BitmapState *m_pStateArray;									//蛇的所有节点显示位图的状态
	SPoint *m_pPos;												//蛇体坐标

private:
	BitmapState GetRightState(MoveState oldDrect, MoveState newDirect);			// 根据前后的运动状态，获得正确的方向
};



#endif // !SNKAE_H
