#ifndef TABLE_H
#define TABLE_H

#define TABLE_STATE_OK		0			// ����
#define TABLE_STATE_FOOD	1			// ʳ��
#define TABLE_STATE_BLOCK	2			// ����
#define TABLE_STATE_SBLOCK	3			// ǽ��

#include "snake.h"

class Table
{
public:
	Table();									// ���캯��
	Table(int width, int height);				// ���ι��캯��
	~Table();									// ��������

	void InitalTable(int width, int height);	// ��ʼ������
	// �����ϵ�ʳ��
	bool AddFood(int coordX, int coordY);		// ���ʳ��
	bool AddBlock(int coordX, int coordY);		// ��Ӷ���
	bool ClearFood(int coordX, int coordY);		// ���ʳ��
	// ��ȡ��Ϣ
	Snake *GetSnake();							// ��ȡ�����ϵ���	
	int **GetBoard();							// ��ȡ��������	
	int GetData(int coordX, int coordY);	// ��ȡ�����ϵ���Ϣ
	int GetWidth();
	int GetHeight();

	// �ߵĲ���
	void SnakeMove();
	bool ChangeSnakeDirect(MoveState d);

private:
	int m_iWidth;				// ���ӵĿ��
	int m_iHeight;				// ���ӵĸ߶�
	int m_iFoodNumber;			// ˮ������Ŀ
	int m_iBlockNumber;			// ��������Ŀ
	Snake m_sSnake;				// �����ϵ���
	int **m_pBoard;				// �������� -- ���
};

#endif // !TABLE_H
