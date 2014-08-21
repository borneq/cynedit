#include "LexC.h"

namespace ab{
	bool LexC::fENDL(){
		return false;
	}
	bool LexC::fLETTER(){
		return false;
	}
	bool LexC::fDIGIT(){
		return false;
	}
	void LexC::mTYPE(){}
	void LexC::mWS(){}
	void LexC::mCOMMENT(){}
	void LexC::mID(){}
	void LexC::mSTRING_LITERAL(){}
	void LexC::mINT(){}
	void LexC::mRETURN(){}
	void LexC::execute(){}
}