//---------------------------------------------------------------------------

#ifndef TBallsSumH
#define TBallsSumH
//---------------------------------------------------------------------------
class TBallsSum {

    int GetIndex(UnicodeString s);

public:
    UnicodeString Sound[31]= {"À", "Á", "Â", "Ã", "Ä", "Å", "¨", "Æ", "Ç", "È",
    	"É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö",
        "×", "Ø", "Ù", "Û", "Ý", "Þ", "ß"};
    double Balls[31], Prepare[31];
    int
        N = 31,
    	Order[31];

    TBallsSum();
    ~TBallsSum();
    void Clear(void);
    void Add(UnicodeString s, double balls);
    void SoftAdd(UnicodeString s, double balls);
    void Commit(void);
    void Analyze(void);

};
#endif
