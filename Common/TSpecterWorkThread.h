//---------------------------------------------------------------------------

#ifndef TSpecterWorkThreadH
#define TSpecterWorkThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TSpecterWorkThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall TSpecterWorkThread(bool CreateSuspended);
    int A,      			// Начаьный спектр
    	B,      			// Конечный спектр
    	N,      			// Кол-во аудио точек
        BufSize,			// Размер буфера
        Count,  			// Кол-во спектров всего (для всех потоков)
        Begin = 0,  		// Начальная позиция в аудио данных
        End = 0,    		// Конечная позиция в аудио данных
        WindowSize = 512,  	// Длина области, в которой вычисляется спектр
        ShapeType = 0;      // Тип шейпера
    double *Data, MaxValue = 0.;
    short *Buf;
    bool EnergySpecters = false;
};
//---------------------------------------------------------------------------
#endif
