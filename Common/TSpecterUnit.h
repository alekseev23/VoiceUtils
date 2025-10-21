//---------------------------------------------------------------------------

#ifndef TSpecterUnitH
#define TSpecterUnitH
//---------------------------------------------------------------------------

struct TSpecterPoint {
    float
	    Min,
	    Max,
        Middle;
};

class TSpecterUnit {

public:
    int N;

    struct TSpecterPoint *SpecterPoint;

    __fastcall TSpecterUnit();
    __fastcall ~TSpecterUnit();
    __fastcall void Set();
    __fastcall bool Read();
    __fastcall bool Write();
    __fastcall void Compare();



};
#endif
