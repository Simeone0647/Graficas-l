#pragma once
#include <iostream>

using namespace std;

class TwoDArray
{
public:
	TwoDArray(int x, int y);
	~TwoDArray();

	void setValueFor(char value, int x, int y);
	char getValueFor(int x, int y);

	char getValuePitch(int x, int y);
	void setValuePitch(char value, int x, int y);

	inline void setPitch() { m_pitch = sizeof(m_2Dchararr[0][0]) * m_maxarrsize; }
	inline int getPitch() { return m_pitch; }

	inline void setRowSize() { m_rowsize = sizeof(m_2Dchararr[0][0]) * m_y; }

	inline int getRows() { return m_x; }

	inline int getColumns() { return m_y; }

	inline int getRowSize() { return m_rowsize; }

	inline void setMaxArrSize() { m_maxarrsize = m_x * m_y; }

	void Print();

private:

	unique_ptr<unique_ptr<char[]>[]> m_2Dchararr;
	unique_ptr<char[]> m_secondSmartPtr2D;

	int m_counter;
	int m_pitch;
	int m_x;
	int m_y;

	int m_rowsize;
	int m_maxarrsize;
};

