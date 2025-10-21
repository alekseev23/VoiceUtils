//---------------------------------------------------------------------------

#ifndef VoiceFileH
#define VoiceFileH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "common.h"
#include "SpecterSet.h"

// Файл с голосом
class VoiceFile {

public:

	class SpecterSet *SpecterSet; // Наборы спектров (голоса)
	UnicodeString
		Name,           // Имя персоны
		FileName;       // Файл *.voice
	int
		SpecterTypes = 5, // Типов спектров
		Version,        // Версия VOICE-файла
		Items,         	// Кол-во наборов (голосов)
		Delta[5] = {2, 4, 5, 5, 5},      		// Минимальная разница между спектрами, чтобы считать спектр найденным
		DeltaColumn[5] = {7, 10, 15, 15, 15},	// Ограничитель на каждый элемент спектра
		Error;          // Код ошибки

    // Конструктор и деструктор
    VoiceFile();
    ~VoiceFile();

	// Установка параметров
	void __fastcall SetDelta(int *delta, int *delta_column);

	// Операции с файлами
	__fastcall bool Load(UnicodeString fname);
	__fastcall bool Save(UnicodeString fname);

    // Операции со структурами SpecterSet
	__fastcall void Clear(void);
	__fastcall bool Add(void);
	__fastcall bool Add(class SpecterSet *specter_set);
	__fastcall bool Delete(int n);

};

#endif