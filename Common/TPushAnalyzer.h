//---------------------------------------------------------------------------

#ifndef TPushItemH
#define TPushItemH
//---------------------------------------------------------------------------
/*
јнализ звуковых данных в виде ударных плоскостей

*/

#define _Q_SIZE 10

struct T_Q {

	unsigned char V[_Q_SIZE];
	short Sum;

	void Clear(void) {
		for (auto i = 0; i < _Q_SIZE; i++) V[i] = 0;
	}

	void Add(int k) {
		if (V[k] < 255) {
			V[k] += 1;
			Sum += 1;
		}
	}

	void CopyTo(struct T_Q &obj) {
		obj.Sum = Sum;
		for (auto i = 0; i < _Q_SIZE; i++) obj.V[i] = V[i];
	}

	void CopyFrom(struct T_Q &obj) {
		Sum = obj.Sum;
		for (auto i = 0; i < _Q_SIZE; i++) V[i] = obj.V[i];
	}

	int Compare(struct T_Q &obj, int &dif) {
		int same = 0;
		dif = 0;
		for (auto i = 0; i < _Q_SIZE; i++) {
			dif += abs(V[i] - obj.V[i]);
			if (V[i] >= obj.V[i]) same += obj.V[i];
			else same += V[i];
		}
		return same;
	}

};
//---------------------------------------------------------------------------
struct T_Q4 {

	struct T_Q
		VP, // ѕоложительные смещени€ по вертикали
		VM, // ќтрицательные смещени€ по вертикали
		HP, // ѕоложительные смещени€ по горизонтали
		HM; // ќтрицательные смещени€ по горизонтали

	void Clear(void) {
		VP.Clear();
		VM.Clear();
		HP.Clear();
		HM.Clear();
	}

	void Add(int type, int k) {
		switch (type) {
			case 0: VP.Add(k); break;
			case 1: VM.Add(k); break;
			case 2: HP.Add(k); break;
			case 3: HM.Add(k); break;
		}
	}

	void CopyTo(struct T_Q4 &obj) {
		VP.CopyTo(obj.VP);
		VM.CopyTo(obj.VM);
		HP.CopyTo(obj.HP);
		HM.CopyTo(obj.HM);
	}

	void CopyFrom(struct T_Q4 &obj) {
		VP.CopyFrom(obj.VP);
		VM.CopyFrom(obj.VM);
		HP.CopyFrom(obj.HP);
		HM.CopyFrom(obj.HM);
	}

	int Compare(struct T_Q4 &obj, int &dif) {
		int same = 0, d;
		dif = 0;
		same += VP.Compare(obj.VP, d); dif += d;
		same += VM.Compare(obj.VM, d); dif += d;
		same += HP.Compare(obj.HP, d); dif += d;
		same += HM.Compare(obj.HM, d); dif += d;
		return same;
	}


};
//---------------------------------------------------------------------------
class TPushItem {

	int Max = 32768,
		VStep[_Q_SIZE],
		HStep[_Q_SIZE];

	short GetVerticalIndex(short val);
	short GetHorizontalIndex(short val);
	void Print(void);

public:
	T_Q4 Data;

	TPushItem();
	~TPushItem();
	void Analyze(short *A, int n, int pos, int range);

	short FindMaxAmp(short *A, int n, int pos, int radius);

};
//---------------------------------------------------------------------------
class TPushAnalyzer {


public:

	int Count;
	struct T_Q4 *Data = NULL;

	TPushAnalyzer();
	~TPushAnalyzer();
	void Analyze(short *A, int n, int a, int b, int range, int step);

};

#endif
