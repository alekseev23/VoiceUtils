//---------------------------------------------------------------------------

#pragma hdrstop

#include "TPushAnalyzer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TPushItem::TPushItem() {
	VStep[_Q_SIZE];
	for (auto i = 0; i < _Q_SIZE; i++) {
		VStep[i] = (1 << i);
		HStep[i] = (1 << i);
	}
}

//---------------------------------------------------------------------------
TPushItem::~TPushItem() {

}

//---------------------------------------------------------------------------
void TPushItem::Analyze(short *A, int n, int pos, int range) {
	int a, b, min, max, min_pos = -1, max_pos = -1, k;
	// Очищаем результаты
	Data.Clear();
	//VP.Clear();
	//VM.Clear();
	//HP.Clear();
	//HM.Clear();
	// Цикл по диапазону
	if (pos > 0) a = pos; else a = pos + 1;
	if (pos + range >= n) b = n - 1; else b = pos + range;
	for (auto i = a; i < b; i++) {
		// Ищем точку максимума
		if ((A[i] > A[i - 1]) && (A[i] > A[i + 1])) {
			if (min_pos != -1) {
				Data.VP.V[GetVerticalIndex(A[i] - min)] += 1;
				Data.HP.V[GetHorizontalIndex(A[i] - min)] += 1;
			}
			max = A[i];
			max_pos = i;
		}
		// Ищем точку минимума
		if ((A[i] < A[i - 1]) && (A[i] < A[i + 1])) {
			if (max_pos != -1) {
				Data.VM.V[GetVerticalIndex(A[i] - max)] += 1;
				Data.HM.V[GetHorizontalIndex(A[i] - max)] += 1;
			}
			min = A[i];
			min_pos = i;
		}

	}

}

//---------------------------------------------------------------------------
// Ищем максимум в аудио данных
// A[n] - масиив аудио данных
// pos - позиция в массиве
// radius - радиус поиска
short TPushItem::FindMaxAmp(short *A, int n, int pos, int radius) {
	short max = 0;
	int a = pos - radius;
	if (a < 0) a = 0;
	int b = pos + radius;
	if (b >= n) b = n - 1;
	for (auto i = a; i < b; i++) {
		if (A[i] > max) max = A[i];
	}
	return max;
}

//---------------------------------------------------------------------------
short TPushItem::GetVerticalIndex(short val) {
	int res = VStep[_Q_SIZE - 1] * val / Max;
	for (short i = _Q_SIZE; i >= 0; i--) {
		if (res >= VStep[i]) return i;
	}
	return 0;
}
//---------------------------------------------------------------------------
short TPushItem::GetHorizontalIndex(short val) {
	for (short i = _Q_SIZE; i >= 0; i--) {
		if (val >= HStep[i]) return i;
	}
	return 0;
}
//---------------------------------------------------------------------------
void TPushItem::Print(void) {
	for (short i = 0; i < _Q_SIZE; i++) std::cout << Data.VP.V[i] << "\t";
	std::cout << std::endl;
	for (short i = 0; i < _Q_SIZE; i++) std::cout << Data.VM.V[i] << "\t";
	std::cout << std::endl;
	for (short i = 0; i < _Q_SIZE; i++) std::cout << Data.HP.V[i] << "\t";
	std::cout << std::endl;
	for (short i = 0; i < _Q_SIZE; i++) std::cout << Data.HM.V[i] << "\t";
	std::cout << std::endl;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*class TPushAnalyzer {

	int Count;
	struct T_Q *Data;

public:*/

TPushAnalyzer::TPushAnalyzer() {

}

TPushAnalyzer::~TPushAnalyzer() {
	if (Data != NULL) delete[] Data;
}

void TPushAnalyzer::Analyze(short *A, int n, int a, int b, int range, int step) {
	if (Data != NULL) delete[] Data;
	// Кол-во элементов
	Count = (b - a) / step;
	// Создаём массив данных
	Data = new struct T_Q4 [Count];
	TPushItem *PushItem = new TPushItem;
	int sm = (range - step) / 2;
	// Вычисляем значения элементов
	for (auto i = 0; i < Count; i++) {
		PushItem->Analyze(A, n, a - sm + i * step, range);
		PushItem->Data.CopyTo(Data[i]);
	}
	delete PushItem;

}

