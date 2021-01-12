#include "TwoDArray.h"

TwoDArray::TwoDArray(int x, int y) : m_counter{ 97 }, m_pitch{ 0 }, m_x{ 0 }, m_y{ 0 }, m_rowsize{ 0 }, m_maxarrsize{ 0 }
{
	m_x = x;
	m_y = y;

	m_2Dchararr = make_unique<unique_ptr<char[]>[]>(m_x);

	for (int i = 0; i < m_x; i++)
	{
		m_secondSmartPtr2D = make_unique<char[]>(m_y);

		for (int j = 0; j < m_y; j++)
		{
			m_secondSmartPtr2D[j] = m_counter;
			m_counter++;
		}

		m_2Dchararr[i] = move(m_secondSmartPtr2D);

	}
	setMaxArrSize();
	setPitch();
	setRowSize();
}

TwoDArray::~TwoDArray()
{
}

void TwoDArray::setValueFor(char character, int x, int y)
{
	if ((x > m_x || x < 0) || (y > m_y || y < 0))
	{
		cout << "Error, array out of range. Try another numbers." << endl;
		return;
	}
	else
	{
		for (int i = 0; i < m_x; i++)
		{
			if (i == x)
			{
				for (int j = 0; j < m_y; j++)
				{
					if (j == y)
					{
						m_2Dchararr[i][j] = character;
					}
				}
			}
		}
		//m_2Dchararr[x][y] = character;
		cout << "The character '" << character << "' was assigned succesfully in [" << x << "][" << y << "]!" << endl;
	}
}

char TwoDArray::getValueFor(int x, int y)
{
	if ((x > m_x || x < 0) || (y > m_y || y < 0))
	{
		cout << "Error, array out of range. Try another numbers." << endl;
		return 32;
	}
	else
	{
		cout << "The element in [" << x << "][" << y << "] is:" << endl;
		//return m_2Dchararr[x][y];

		for (int i = 0; i < m_x; i++)
		{
			if (i == x)
			{
				for (int j = 0; j < m_y; j++)
				{
					if (j == y)
					{
						return m_2Dchararr[i][j];
					}
				}
			}
		}
	}
	cout << "Nothing happened." << endl;
	return 0;
}

char TwoDArray::getValuePitch(int x, int y)
{
	int px = sizeof(m_2Dchararr[0][0]) * x;
	int py = sizeof(m_2Dchararr[0][0]) * y;

	cout << "The element in [" << x << "][" << y << "] is:" << endl;
	return m_2Dchararr[px][py];
}

void TwoDArray::setValuePitch(char value, int x, int y)
{
	if ((x > m_x || x < 0) || (y > m_y || y < 0))
	{
		cout << "Error, array out of range. Try another numbers." << endl;
		return;
	}
	else
	{
		int px = sizeof(m_2Dchararr[0][0]) * x;
		int py = sizeof(m_2Dchararr[0][0]) * y;
		m_2Dchararr[px][py] = value;
		cout << "The character '" << value << "' was assigned succesfully in [" << x << "][" << y << "]!" << endl;
	}
	
}

void TwoDArray::Print()
{	
	cout << "2D Array: " << endl;
	for (int i = 0; i < m_x; i++)
	{
		for (int k = 0; k < m_y; k++)
		{
			cout << m_2Dchararr[i][k] << ",";
		}
		cout << endl;
	}
	cout << endl;
}

