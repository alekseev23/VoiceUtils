//---------------------------------------------------------------------------

#pragma hdrstop

#include "TVoiceSpectersDB4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::TVoiceSpectersDB4_Sound() {
}

//---------------------------------------------------------------------------
TVoiceSpectersDB4_Sound::~TVoiceSpectersDB4_Sound() {
    Clear();
}

//---------------------------------------------------------------------------
bool TVoiceSpectersDB4_Sound::Read(FILE *fd) {
    Clear();
    // Sound (6)
    wchar_t w[10];
    fread(w, 2, 3, fd);
    Sound = UnicodeString(w);
    // Id (4)
    fread(&Id, 4, 1, fd);
    // Count (4)
    fread(&Count, 4, 1, fd);
    // Вектор со спектрами
    TVoiceSpectersDB4_Specter A;
    for (auto i = 0; i < Count; i++) {
	    fread(&A, 8, TVoiceSpectersDB4_M * 3 + 1, fd);
        Specters.push_back(A);
    }
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
	if (p1 + n >= s1.size()) {
    std::cout << "1   -1" << std::endl;
    	return -1.;
    }
    if (p2 + n >= s2.size()) {
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
// Ищет, где разница минимальна, а похожесть максимальна
int TVoiceSpectersDB4_Sound::SearchMin(std::vector<TVoiceSpectersDB4_Specter> &s, int p, int n, double &dif, double &same) {
    int pos = -1, k;
    double cur_dif, cur_same;
    dif = 1e8;
    same = -1e8;
    k = Specters.size() - n;
    // Цикл по всем группам спектров
    for (int i = 0; i < k; i++) {
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
    TVoiceSpectersDB4_Sound sound;
    for (auto i = 0; i < Count; i++) {
        Sounds.push_back(sound);
    }
}

//---------------------------------------------------------------------------
// Чтение базы со спектрами
bool TVoiceSpectersDB4::Load(UnicodeString fname) {
    FILE *fd = _wfopen(fname.w_str(), L"rb");
    if (fd == NULL) return false;
    // Кол-во звуков
    fread(&Count, 4, 1, fd);
    SetSize(Count);
    // Сами звуки
    for (auto i = 0; i < Count; i++) {
        Sounds[i].Read(fd);
    }
    fclose(fd);
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
//std::wcout << R << "\tpos=" << pos << "\ti=" << i << "\tsame=" << same << "\tdif=" << dif << "\tdelta=" << (same - dif) << std::endl;
        	//sum.Add(Sounds[j].Sound, same - dif);
            sum.SoftAdd(Sounds[j].Sound, same - dif);
        }
        sum.Commit();
    }
    sum.Analyze();
    return true;
}
//---------------------------------------------------------------------------
