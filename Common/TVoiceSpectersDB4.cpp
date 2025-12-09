//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpectersDB4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
extern int MMM;
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Peaks::TVoiceSpectersDB4_Peaks() {
    Tag = MMM;
    MMM++;
    Max = new double[100];
    Sum2 = new double[100];
    for (int i = 0; i < 100; i++) {
        Max[i] = i;
        Sum2[i] = i;
    }
    std::cout << "TVoiceSpectersDB4_Peaks Tag=" << Tag << std::endl;
}
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Peaks::~TVoiceSpectersDB4_Peaks() {
std::cout << "<<<<<<Pizdez!>>>>>    Tag=" << Tag << std::endl;
    if (Max != NULL) delete[] Max;
    if (Sum2 != NULL) delete[] Sum2;
}
//---------------------------------------------------------------------------
void TVoiceSpectersDB4_Peaks::Set(double *values, int n) {
    int k = 0, d = 131;
//std::cout << "Set Point 1"<< std::endl;
    //if (Max != NULL) delete[] Max;
    //if (Sum != NULL) delete[] Sum;
    N = n;
    Count = N / d;
    //Max = new double [Count];
    //Sum = new double [Count];
std::cout << "Set Point 2 N=" << N << " Count=" << Count << " Tag=" << Tag << std::endl;
    for (int i = 0; i < Count; i++) {
        Max[i] = 0.;
        Sum2[i] = 0.;
	    for (int j = 0; j < d; j++) {
            if (values[k] > Max[i]) Max[i] = values[k];
            Sum2[i] += values[k];
            k++;
        }
std::cout << "Set Point 3  i=" << i << " k=" << k << " Count=" << Count << " Max=" << Max[i] <<  " Sum2=" << Sum2[i] << " values=" << values[k] << std::endl;
    }
    GlobalSum = GlobalMax = 0.;
    for (auto i = 0; i < Count; i++) {
        if (Max[i] > GlobalMax) GlobalMax = Max[i];
        GlobalSum += Sum2[i];
    }
//std::cout << "GlobalSum=" << GlobalSum << "\tGlobalMax=" << GlobalMax;
//for (auto i = 0; i < Count; i++) std::cout << "\t" << Max[i] << "," << Sum[i];
//std::cout << std::endl;
}
//---------------------------------------------------------------------------
double TVoiceSpectersDB4_Peaks::CompareMax(double *max, double *sum, double &dif, double &same) {
    // Вычисление масштабных коэффициентов
    double s1 = 0., m1 = 0., s2 = 0., m2 = 0., k, v1, v2;
    for (auto i = 0; i < Count; i++) {
        if (Max[i] > m1) m1 = Max[i];
        if (max[i] > m2) m2 = max[i];
        s1 += Sum2[i];
        s2 += sum[i];
    }
    k = 1. * (m2 / m1 + s2 / s1) / 2;
std::cout << "\t\tm1=" << m1 << "\tm2=" << m2 << "\ts1=" << s1 << "\ts2=" << s2 << "\tk=" << k << std::endl;
    // Вычисление похожести и различий
    dif = same = 0.;
    for (auto i = 0; i < Count; i++) {
        v1 = k * Max[i];
        v2 = max[i];
std::cout << "v1=" << v1 << "\tv2=" << v2 << "\tk=" << k << "\tMax=" << Max[i] << "\tmax=" << max[i] << std::endl;
        dif += fabs(v1 - v2);
        if (v1 > v2) same += v2;
        else same += v1;
    }
    return (same - dif);
}
//---------------------------------------------------------------------------
double TVoiceSpectersDB4_Peaks::CompareMax(TVoiceSpectersDB4_Peaks *peak, double &dif, double &same) {
// double *max, double *sum, double &dif, double &same) {
std::cout << "CompareMax" << std::endl;
    // Вычисление масштабных коэффициентов
    double k, v1, v2;
    k = 1. * (peak->GlobalMax / GlobalMax + peak->GlobalSum / GlobalSum) / 2;
    // Вычисление похожести и различий
    dif = same = 0.;
std::cout << "CompareMax 2 Count=" << Count << std::endl;
    for (auto i = 0; i < Count; i++) {
if (peak->Max[i] != Max[i]) std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        v1 = k * Max[i];
        v2 = peak->Max[i];
std::cout << "v1=" << v1 << "\tv2=" << v2 << "\tk=" << k << "\tMax=" << Max[i] << "\tmax=" << peak->Max[i] << "\tTag=" << Tag << std::endl;
        dif += fabs(v1 - v2);
        if (v1 > v2) same += v2;
        else same += v1;
    }
    return (same - dif);
}
//---------------------------------------------------------------------------
double TVoiceSpectersDB4_Peaks::CompareSum(double *max, double *sum, double &dif, double &same) {
    // Вычисление масштабных коэффициентов
    double s1 = 0., m1 = 0., s2 = 0., m2 = 0., k, v1, v2;
    for (auto i = 0; i < Count; i++) {
        if (Max[i] > m1) m1 = Max[i];
        if (max[i] > m2) m2 = max[i];
        s1 += Sum2[i];
        s2 += sum[i];
    }
    k = (m2 / m1 + s2 / s1) / 2;
    // Вычисление похожести и различий
    dif = same = 0.;
    for (auto i = 0; i < Count; i++) {
        v1 = k * Sum2[i];
        v2 = sum[i];
        dif += fabs(v1 - v2);
        if (v1 > v2) same += v2;
        else same += v1;
    }
    return (same - dif);
}
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::TVoiceSpectersDB4_Sound() {
	Peaks = new TVoiceSpectersDB4_Peaks;
}
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::TVoiceSpectersDB4_Sound(const TVoiceSpectersDB4_Sound &s) {
std::cout << "Copy constructor" << std::endl;
}
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::TVoiceSpectersDB4_Sound(TVoiceSpectersDB4_Sound &&s) {
std::cout << "Move constructor" << std::endl;
}
//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::~TVoiceSpectersDB4_Sound() {
    Clear();
    delete Peaks;
}

//---------------------------------------------------------------------------
bool TVoiceSpectersDB4_Sound::Read(FILE *fd) {
//std::cout << "read Point 1" << std::endl;
    Clear();
//std::cout << "read Point 2" << std::endl;
    // Sound (6)
    wchar_t w[10];
    fread(w, 2, 3, fd);
    Sound = UnicodeString(w);
    // Id (4)
    fread(&Id, 4, 1, fd);
    // Count (4)
    fread(&Count, 4, 1, fd);
//std::cout << "read Point 3 Count=" << Count << std::endl;
    // Вектор со спектрами
    //TVoiceSpectersDB4_Specter A;
    //TVoiceSpectersDB4_Specter *A = new class TVoiceSpectersDB4_Specter;
    for (auto i = 0; i < Count; i++) {
	    //TVoiceSpectersDB4_Specter *A;
        //A = new class TVoiceSpectersDB4_Specter;
        //fread(A, 8, TVoiceSpectersDB4_M * 3 + 1, fd);
        //Specters.push_back(*A);
        TVoiceSpectersDB4_Specter A;
        fread(&A.Sum, 8, 1, fd);
        for (auto j = 0; j < TVoiceSpectersDB4_M; j++) {
            fread(&A.Points[j].Min, 8, 1, fd);
            fread(&A.Points[j].Max, 8, 1, fd);
            fread(&A.Points[j].Middle, 8, 1, fd);
        }
	    //fread(A.Points, 8, TVoiceSpectersDB4_M * 3, fd);
        //Specters.push_back(A);
        Specters.push_back(std::move(A));
    }
//std::cout << "read Point 4" << std::endl;
    // Предполагаем, что векторов у нас только один
    double *values = new double [TVoiceSpectersDB4_M];
//std::cout << "read Point 45 size=" << Specters.size() << std::endl;
    for (auto i = 0; i < TVoiceSpectersDB4_M; i++) values[i] = Specters[0].Points[i].Middle;
//std::cout << "read Point 5" << std::endl;
    Peaks->Set(values, TVoiceSpectersDB4_M);
//std::cout << "read\tCount=" << Count << "\t" << values[0] << " " << values[1] << " " << values[2] << " " << values[3] << std::endl;
    delete[] values;
//std::cout << "read Point 6" << std::endl;
    return true;
}

//---------------------------------------------------------------------------
bool TVoiceSpectersDB4_Sound::Write(FILE *fd) {
    // Пишем Sound (2 * 3)
    wchar_t w[10];
    swprintf(w, L"%ls", Sound.w_str());
    fwrite(w, 2, 3, fd);
    // Id (4)
    fwrite(&Id, 4, 1, fd);
    // Count (4)
    fwrite(&Count, 4, 1, fd);
    // Вектор со спектрами [Count == Specters.Size()]
    for (auto i = 0; i < Count; i++) {
	    fwrite(&Specters[i], 8, TVoiceSpectersDB4_M * 3 + 1, fd);
    }
    return true;
}

//---------------------------------------------------------------------------
void TVoiceSpectersDB4_Sound::Set(UnicodeString sound, int id) {
    Sound = sound;
    Id = id;
}

//---------------------------------------------------------------------------
void TVoiceSpectersDB4_Sound::Clear(void) {
    Specters.clear();
    Count = 0;
    Sound = "";
    Id = 0;
}

//---------------------------------------------------------------------------
void TVoiceSpectersDB4_Sound::Add(class TVoiceSpectersDB4_Specter &s) {
    Specters.push_back(s);
    Count++;
}

//---------------------------------------------------------------------------
void TVoiceSpectersDB4_Sound::Add(double *min, double *max, double *middle) {
    TVoiceSpectersDB4_Specter s;
    s.Sum = 0.;
    for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
        s.Points[i].Min = min[i];
    	s.Points[i].Max = max[i];
    	s.Points[i].Middle = middle[i];
        s.Sum += middle[i];
    }
    Specters.push_back(s);
    Count++;
}

//---------------------------------------------------------------------------
class TVoiceSpectersDB4_Specter* TVoiceSpectersDB4_Sound::Get(int i) {
    if (i >= Count) return NULL;
    return &Specters[i];
}

//---------------------------------------------------------------------------
/* Сравнение двух спектров s1 и s2 и возвращение величины разницы и совпадений
s1, s2 - вектора спектров
p1, p2 - позиции, начиная с которых в каждом из векторов будут сравниваться последующие n векторов
dif - разница в сравнении
same - общая часть в сравнении
*/
double TVoiceSpectersDB4_Sound::Compare(std::vector<TVoiceSpectersDB4_Specter> &s1,
	std::vector<TVoiceSpectersDB4_Specter> &s2, int p1, int p2, int n, double &dif, double &same) {
    double v1, v2;
    int i1, i2;
    i1 = p1;
    i2 = p2;
    dif = 0.;
    same = 0.;
    // Проверка индексов спектров
	if (p1 + n > s1.size()) {
    std::cout << "1   -1" << std::endl;
    	return -1.;
    }
    if (p2 + n > s2.size()) {
    std::cout << "2   -1" << std::endl;
    	return -1.;
    }
    // Цикл по диапазону спектров n
    for (auto j = 0; j < n; j++) {
        // Цикл по точкам спектров
	    for (auto i = 0; i < TVoiceSpectersDB4_M; i++) {
            // Отнормированные значения спектров
            v1 = 100. * s1[i1].Points[i].Middle / s1[p1].Sum;
            v2 = 100. * s2[i2].Points[i].Middle / s2[p2].Sum;
    	    //dif += fabs(v1 - v2);
            //if (v1 < v2) same += v1; else same += v2;
            dif += (v1 - v2) * (v1 - v2);
            if (v1 < v2) same += v1 * v1; else same += v2 * v2;
	    }
        i1++;
        i2++;
    }
    return same - dif;
}

//---------------------------------------------------------------------------
/*
Ищет, где разница минимальна, а похожесть максимальна
s - вектор спектров
p - позиция в векторе
n - количество сравниваемых элементов
dif - разница значений
same - одиноковая часть значений
*/
int TVoiceSpectersDB4_Sound::SearchMin(std::vector<TVoiceSpectersDB4_Specter> &s, int p, int n, double &dif, double &same) {
    int pos = -1, k;
    double cur_dif, cur_same;
    dif = 1e8;
    same = -1e8;
    k = Specters.size() - n;
    // Цикл по всем группам спектров
    for (int i = 0; i <= k; i++) {
//std::cout << "TVoiceSpectersDB4_Sound::SearchMin Count=" << Count << " Specters.size()=" << Specters.size() << " n=" << n << " i=" << i << std::endl;
        if (Compare(Specters, s, i, p, n, cur_dif, cur_same) > same - dif) {
            dif = cur_dif;
            same = cur_same;
            pos = i;
        }
    }
//std::cout << "k=" << k << "\tdif=" << dif << "\tsame=" << same << std::endl;
    return pos;
}

//---------------------------------------------------------------------------
TVoiceSpectersDB4::TVoiceSpectersDB4() {

}

//---------------------------------------------------------------------------
TVoiceSpectersDB4::~TVoiceSpectersDB4() {
    Clear();
}

//---------------------------------------------------------------------------
// Создание базы спектров
void TVoiceSpectersDB4::SetSize(int count) {
    Clear();
    Count = count;
    //TVoiceSpectersDB4_Sound *sound;
    for (int i = 0; i < Count; i++) {
	    //TVoiceSpectersDB4_Sound sound;
        //Sounds.push_back(sound);
        //sound = new TVoiceSpectersDB4_Sound;
        //Sounds.push_back(std::move(*sound));
        //Sounds.emplace_back(*sound);
        //sound = NULL;
        Sounds.emplace_back(new class TVoiceSpectersDB4_Sound);
    }
}

//---------------------------------------------------------------------------
// Чтение базы со спектрами
bool TVoiceSpectersDB4::Load(UnicodeString fname) {
    FILE *fd = _wfopen(fname.w_str(), L"rb");
    if (fd == NULL) return false;
    // Кол-во звуков
    fread(&Count, 4, 1, fd);
std::cout << "Point 1 Count=" << Count << std::endl;
    SetSize(Count);
std::cout << "Point 2" << std::endl;
    // Сами звуки
    for (auto i = 0; i < Count; i++) {
        Sounds[i].Read(fd);
    }
std::cout << "Point 3" << std::endl;
    fclose(fd);
std::cout << "Point 4" << std::endl;
    return true;
}

//---------------------------------------------------------------------------
// Запись базы со спектрами
bool TVoiceSpectersDB4::Save(UnicodeString fname) {
    FILE *fd = _wfopen(fname.w_str(), L"wb");
    if (fd == NULL) return false;
    // Кол-во звуков
    fwrite(&Count, 4, 1, fd);
    // Сами звуки
    for (auto i = 0; i < Count; i++) {
        Sounds[i].Write(fd);
    }
    fclose(fd);
    return true;
}

//---------------------------------------------------------------------------
// Очистка списка
void TVoiceSpectersDB4::Clear(void) {
    Sounds.clear();
    Count = 0;
}

//---------------------------------------------------------------------------
// Поиск наиболее похожего спектра
UnicodeString TVoiceSpectersDB4::Find(std::vector<TVoiceSpectersDB4_Specter> &s,
	int p, int n, double &dif, double &same, int &id, int &fpos) {
    UnicodeString Sound;
    double local_dif, local_same;
    int pos;
    dif = 1e8;
    same = 0.;
    // Цикл по всем звукам в базе
    for (auto i = 0; i < Count; i++) {
//std::cout << "i=" << i << std::endl;
        // Ищет, где разница минимальна, а похожесть максимальна
		pos = Sounds[i].SearchMin(s, p, n, local_dif, local_same);
        if (local_same - local_dif > same - dif) {
            same = local_same;
            dif = local_dif;
            fpos = pos;
            id = Sounds[i].Id;
            Sound = Sounds[i].Sound;
        }
    }
    return Sound;
}

//---------------------------------------------------------------------------
// Поиск наиболее похожего звука
bool TVoiceSpectersDB4::Search(std::vector<TVoiceSpectersDB4_Specter> &s, int from, int len, int dn, TBallsSum &sum) {
    int pos;
    double dif, same;
    UnicodeString R;
    // Очищаем сумматор
    sum.Clear();
    // Цикл по выбранным спектрам
    for (auto i = from; i < from + len; i++) {
        // Цикл по всем звукам и их вариантам
        for (auto j = 0; j < Count; j++) {

        /*if (Sounds[j].Sound == "В") R = "V";
        else if (Sounds[j].Sound == "Ё") R = "Yo";
        else continue;*/

	        // Ищет, где разница минимальна, а похожесть максимальна
			pos = Sounds[j].SearchMin(s, i, dn, dif, same);
//std::wcout << R << "\tpos=" << pos << "\ti=" << i << "\tdn=" << dn << "\tsame=" << same << "\tdif=" << dif << "\tdelta=" << (same - dif) << std::endl;
        	//sum.Add(Sounds[j].Sound, same - dif);
            sum.SoftAdd(Sounds[j].Sound, same - dif);
        }
        sum.Commit();
    }
    sum.Analyze();
    return true;
}
//---------------------------------------------------------------------------
// Поиск наиболее похожего звука
bool TVoiceSpectersDB4::SearchPeaks(std::vector<TVoiceSpectersDB4_Specter> &s, TBallsSum &sum) {
    int pos;
    double dif, same;
    double values[TVoiceSpectersDB4_M];
    UnicodeString R;
    // Очищаем сумматор
    sum.Clear();
    // Считаем текущие пики
    for (auto i = 0; i < TVoiceSpectersDB4_M; i++) values[i] = s[0].Points[i].Middle;
    TVoiceSpectersDB4_Peaks *cur_peaks = new TVoiceSpectersDB4_Peaks;
    std::cout << std::endl;
    cur_peaks->Set(values, TVoiceSpectersDB4_M);

for (auto i = 0; i < cur_peaks->Count; i++) {
    std::cout << i << "\t" << cur_peaks->Max[i] << "\t" << cur_peaks->Sum2[i] << std::endl;
}
    // Цикл по всем звукам и их вариантам
    for (auto j = 0; j < Count; j++) {
		// Ищет, где разница минимальна, а похожесть максимальна
        //Sounds[j].Peaks->CompareMax(cur_peaks->Max, cur_peaks->Sum, dif, same);
        Sounds[j].Peaks->CompareMax(cur_peaks, dif, same);
        //pos = Sounds[j].SearchMin(s, 0, dn, dif, same);
//std::wcout << R << "\tpos=" << pos << "\ti=" << i << "\tdn=" << dn << "\tsame=" << same << "\tdif=" << dif << "\tdelta=" << (same - dif) << std::endl;
std::cout << j << "\t" << same << "\t" << dif << "\t" << (same - dif) << std::endl;
       	sum.SoftAdd(Sounds[j].Sound, same - dif);
	}
    sum.Commit();
    sum.Analyze();
    delete cur_peaks;
    return true;
}
//---------------------------------------------------------------------------
