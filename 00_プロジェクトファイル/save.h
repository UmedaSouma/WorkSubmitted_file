//===========================================================================================================================================================
// 
// save.cpp‚Ìƒwƒbƒ_[ [save.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SAVE_H_
#define _SAVE_H_
#include "main.h"

class CSave
{
public:
	CSave();
	~CSave();
	static void StageText();
	static void AddModeFilepass(const char* name, const char* address);

	static std::string SetSaveTypeText(int type);
private:

};



#endif // !SAVE_H_