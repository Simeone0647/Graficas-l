#include <iostream>
#include "TwoDArray.h"
#include <chrono>

using namespace std;

int main()
{
	TwoDArray m_array(6, 9);

	cout << "Pitch: " << m_array.getPitch() << endl;
	cout << "Row Size: " << m_array.getRowSize() << endl << endl;

	m_array.Print();

	auto begin_for_timer = chrono::steady_clock::now();
	cout << m_array.getValueFor(2, 5) << endl;
	auto end_for_timer = chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds_for= end_for_timer - begin_for_timer;
	cout << "Time taked getting an element doing for loop: " << elapsed_seconds_for.count() << endl << endl;

	auto begin_pitch_timer = chrono::steady_clock::now();
	cout << m_array.getValuePitch(2, 5) << endl;
	auto end_pitch_timer = chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds_pitch = end_pitch_timer - begin_pitch_timer;
	cout << "Time taked getting an element doing pitch: " << elapsed_seconds_pitch.count() << endl << endl;

	cout << "Setting a value doing the for loop in [2][5]..." << endl;
	auto begin_setfor_timer = chrono::steady_clock::now();
	m_array.setValueFor('m', 2, 5);
	auto end_setfor_timer = chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds_setfor_timer = end_setfor_timer - begin_setfor_timer;
	cout << "Time taked setting an element with for loop: " << elapsed_seconds_setfor_timer.count() << endl;
	cout << m_array.getValueFor(2, 5) << endl << endl;

	cout << "Setting a value with pitch in [3][1]..." << endl;
	auto begin_setpitch_timer = chrono::steady_clock::now();
	m_array.setValuePitch('k', 3, 1);
	auto end_setpitch_timer = chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds_setpitch_timer = end_setpitch_timer - begin_setpitch_timer;
	cout << "Time taked setting an element with pitch: " << elapsed_seconds_setpitch_timer.count() << endl;
	cout << m_array.getValueFor(3, 1) << endl;
}
