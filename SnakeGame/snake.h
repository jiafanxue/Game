#ifndef SNKAE_H
#define SANKE_H

#define SNAKE_MOVE			   1								// ������ÿ���ƶ���ʱ��
#define SAFE_DELETE_ARRAY(p)  {delete[] (p); (p) = NULL;}		// �ͷ�����ָ��
#define SAFE_DELETE_NARRAY(p) {delete (p); (p) = NULL;}			// �ͷ�ָ��

#include <stdio.h>

// �����ߵ�λͼ
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

// �������ƶ����˶�״̬
enum MoveState {
	S_NONE,
	S_UP,
	S_DOWN,
	S_LEFT,
	S_RIGHT
};

// ��������������ݽṹ
struct SPoint
{
	int x;
	int y;
};

class Snake
{
	struct Snake_Struct				// ������ÿ����λ�Ľṹ
	{
		MoveState head;
		MoveState *body; 
		MoveState tail;
	};
public:
	Snake(int coordX = 0, int coordY = 0, int len = 1);			// ����ĺ��ι��캯��
	~Snake();													// �������������

public:
	void Move();												// �ߵ��ƶ�
	void ChangeDirect(MoveState d);								// �ı䷽��
	void AddBody(int len = 1);									// �������峤��
	void SetHeadPos(int coordX, int coordY);					// ����ͷ��������
	BitmapState *GetStateArray();								// ��ȡ�ߵ�״̬����
	SPoint *GetPos();											// ��ȡ�ߵ�����ָ��
	int GetLength();											// ��ȡ�ߵĳ���
	bool IsHeadTouchBody(int coordX, int coordY);				// �ж��Ƿ���ײ����
	void Initial();												// ������Ϸ������ʼ����

private:
	int m_length;												//�ߵĳ���
	Snake_Struct m_newSnake;									//�ߵ���̬�����нڵ��˶�״̬
	Snake_Struct m_oldSnake;									//�ߵ�ԭ̬�����нڵ��˶�״̬
	BitmapState *m_pStateArray;									//�ߵ����нڵ���ʾλͼ��״̬
	SPoint *m_pPos;												//��������

private:
	BitmapState GetRightState(MoveState oldDrect, MoveState newDirect);			// ����ǰ����˶�״̬�������ȷ�ķ���
};



#endif // !SNKAE_H
