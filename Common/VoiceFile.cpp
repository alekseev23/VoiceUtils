#include "VoiceFile.h"
#include "../../_Common_/Lib/files.h"
#include "../../_Common_/Lib/strings.h"

__fastcall VoiceFile::VoiceFile() {
	Items = 0;
	SpecterSet = NULL;
	Version = _VOICE_VERSION;
}

__fastcall VoiceFile::~VoiceFile() {
	if (SpecterSet != NULL) delete[] SpecterSet;
}

//------------------------------------------------------------------------------
// Установка параметров
//------------------------------------------------------------------------------
void __fastcall VoiceFile::SetDelta(int *delta, int *delta_column) {
	for (int i = 0; i < SpecterTypes; i++) {
		Delta[i] = delta[i];
		DeltaColumn[i] = delta_column[i];
	}
	for (int i = 0; i < Items; i++) {
		SpecterSet[i].SetDelta(Delta, DeltaColumn);
	}
}

//------------------------------------------------------------------------------
// Чтение данных из файла
//------------------------------------------------------------------------------
__fastcall bool VoiceFile::Load(UnicodeString fname) {
	FileName = fname;
	Name = without_extension(get_file_name(FileName));
	Error = 0;
	// Удаляем существующие данные
	if (SpecterSet != NULL) delete[] SpecterSet;
	Items = 0;
	// Открываем файл на чтение
	FILE *fd = _wfopen(FileName.w_str(), L"rb");
	if (fd == NULL) { Error = 1; return false; }
	int k;
	char v[6];
	// Читаем сигнатуру
	if (fread(v, 1, 5, fd) != 5) { fclose(fd); Error = 2; return false; }
	if (strncmp(v, "VOICE", 5) != 0) { fclose(fd); Error = 3; return false; }
	// Читаем версию Voice-файла
	if (fread(&k, 4, 1, fd) != 1) { fclose(fd); Error = 4; return false; }

	//if (k != Version) { fclose(fd); Error = 5; return false; }

	// Читаем кол-во наборов спектров (голосов)
	if (fread(&Items, 4, 1, fd) != 1) { fclose(fd); Error = 6; return false; }
	// Создаём необходимое кол-во наборов спектров
	SpecterSet = new class SpecterSet [Items];
	// Цикл по наборам голосов
	for (int i = 0; i < Items; i++) {
		// Читаем набор голосов
		if (!SpecterSet[i].Read(fd)) {
		ShowMessage("Error = " + UnicodeString(SpecterSet[i].Error));
		fclose(fd); Error = 7; return false; }
	    SpecterSet[i].SetDelta(Delta, DeltaColumn);
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

//------------------------------------------------------------------------------
// Сохранение данных в файл
//------------------------------------------------------------------------------
__fastcall bool VoiceFile::Save(UnicodeString fname) {
    Error = 0;
	// Проверка что данные присутствуют
	if ((Items == 0) || (SpecterSet == NULL)) return false;
	// Сохраняем путь, находим имя
	FileName = fname;
	Name = without_extension(get_file_name(FileName));
	// Открываем файл на запись
	FILE *fd = _wfopen(FileName.w_str(), L"wb");
	if (fd == NULL) { Error = 10; return false; }
	// Сохраняем сигнатуру
	if (fwrite("VOICE", 1, 5, fd) != 5) { fclose(fd); Error = 11; return false; }
	// Сохраняем версию Voice-файла
	if (fwrite(&Version, 4, 1, fd) != 1) { fclose(fd); Error = 12; return false; }
	// Сохраняем кол-во наборов спектров (голосов)
	if (fwrite(&Items, 4, 1, fd) != 1) { fclose(fd); Error = 13; return false; }
	// Цикл по наборам голосов
	for (int i = 0; i < Items; i++) {
		// Сохраняем набор голосов
		if (!SpecterSet[i].Write(fd)) { fclose(fd); Error = 14; return false; }
	}
	// Закрываем файл
	fclose(fd);
	return true;
}

//------------------------------------------------------------------------------
// Очистка структур данных
//------------------------------------------------------------------------------
__fastcall void VoiceFile::Clear(void) {
	if (SpecterSet != NULL) delete[] SpecterSet;
	Items = 0;
}

//------------------------------------------------------------------------------
// Добавление нового набора спектров к уже существующим наборам
//------------------------------------------------------------------------------
__fastcall bool VoiceFile::Add(void) {
	if (Items == 0) {
		Items = 1;
		SpecterSet = new class SpecterSet [Items];
		SpecterSet[0].SetDelta(Delta, DeltaColumn);
		return true;
	}
	// Создаём временный SpecterSet
	class SpecterSet *temp = new class SpecterSet [Items];
	// Копируем в новый массив
	int i;
	for (i = 0; i < Items; i++) {
		temp[i].CopyFrom(SpecterSet[i]);
	}
	// Удаляем SpecterSet
	delete[] SpecterSet;
	// Увеличиваем кол-во элементов
	Items++;
	// Создаём новый SpecterSet
	SpecterSet = new class SpecterSet [Items];
	// Копируем данные обратно
	for (i = 0; i < Items - 1; i++) {
		SpecterSet[i].CopyFrom(temp[i]);
	}
	// Удаляем временный массив
	delete[] temp;
	//
	SpecterSet[Items - 1].SetDelta(Delta, DeltaColumn);
	// Возвращаем результат
	return true;
}
//------------------------------------------------------------------------------
// Добавление нового набора спектров к уже существующим наборам
//------------------------------------------------------------------------------
__fastcall bool VoiceFile::Add(class SpecterSet *specter_set) {
	if (Items == 0) {
		Items = 1;
		SpecterSet = new class SpecterSet [Items];
		//SpecterSet[0].SetDelta(Delta, DeltaColumn);
        SpecterSet[0].CopyFrom(*specter_set);
		return true;
	}
	int i;
	// Создаём временный SpecterSet
	class SpecterSet *temp = new class SpecterSet [Items];
	// Копируем в новый массив
	for (i = 0; i < Items; i++) {
		temp[i].CopyFrom(SpecterSet[i]);
	}
	// Удаляем SpecterSet
	delete[] SpecterSet;
	// Увеличиваем кол-во элементов
	Items++;
	// Создаём новый SpecterSet
	SpecterSet = new class SpecterSet [Items];
	// Копируем данные обратно
	for (i = 0; i < Items - 1; i++) {
		SpecterSet[i].CopyFrom(temp[i]);
	}
	// Удаляем временный массив
	delete[] temp;
	//
	//SpecterSet[Items - 1].SetDelta(Delta, DeltaColumn);
    SpecterSet[Items - 1].CopyFrom(*specter_set);
	// Возвращаем результат
	return true;
}
//------------------------------------------------------------------------------
// Удаление набора спектров
//------------------------------------------------------------------------------
__fastcall bool VoiceFile::Delete(int n) {
	// Проверка n
	if (n >= Items) return false;
	if (Items == 1) {
		Clear();
		return true;
	}
	// Создаём временный SpecterSet
	class SpecterSet *temp = new class SpecterSet [Items - 1];
	// Копируем неудаляемые в новый массив
	int i, j;
	for (i = j = 0; i < Items; i++) {
		if (i == n) continue;
		temp[j].CopyFrom(SpecterSet[i]);
		j++;
	}
	// Удаляем SpecterSet
	delete[] SpecterSet;
	// Уменьшаем кол-во элементов
	Items--;
	// Создаём новый SpecterSet
	SpecterSet = new class SpecterSet [Items];
	// Копируем данные обратно
	for (i = 0; i < Items; i++) {
		SpecterSet[i].CopyFrom(temp[i]);
	}
	// Удаляем временный массив
	delete[] temp;
	// Возвращаем результат
	return true;
}

