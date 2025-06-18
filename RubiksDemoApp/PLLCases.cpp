#include "PLLCases.h"
void PLLCases::copyLastLayer() {
	mLastLayer.resize(9);
}
void PLLCases::getLastLayerCase() {
	//Read the last layer from the main case and initialize the variable
}
void PLLCases::setAllCases() {
	mAllCasesVec.push_back(UaCase);
	mAllCasesVec.push_back(UbCase);
	mAllCasesVec.push_back(ZCase);
	mAllCasesVec.push_back(HCase);
	mAllCasesVec.push_back(AaCase);
	mAllCasesVec.push_back(AbCase);
	mAllCasesVec.push_back(ECase);
	mAllCasesVec.push_back(TCase);
	mAllCasesVec.push_back(FCase);
	mAllCasesVec.push_back(JaCase);
	mAllCasesVec.push_back(JbCase);
	mAllCasesVec.push_back(RaCase);
	mAllCasesVec.push_back(RbCase);
	mAllCasesVec.push_back(VCase);
	mAllCasesVec.push_back(YCase);
	mAllCasesVec.push_back(NaCase);
	mAllCasesVec.push_back(NbCase);
	mAllCasesVec.push_back(GaCase);
	mAllCasesVec.push_back(GbCase);
	mAllCasesVec.push_back(GcCase);
	mAllCasesVec.push_back(GdCase);
}
void PLLCases::getCaseIndex() {
	setAllCases();

	for (int i = 0; i < mAllCasesVec.size(); i++) {
		if (mAllCasesVec[i] == mLastLayer) {
			mCaseIndex = i + 1;
		}
	}
}

