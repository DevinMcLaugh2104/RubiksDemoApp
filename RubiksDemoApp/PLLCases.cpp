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
void PLLCases::UaPerm() {
	cout << "Ua Perm - " << "M2 U' M U2 M' U M2";

	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayerPrime();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
}
void PLLCases::UbPerm() {
	cout << "Ub Perm - " << "M2 U M U2 M' U M2";

	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayerPrime();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
}
void PLLCases::ZPerm() {
	cout << "Z Perm - " << "U M2 U' M2 U' M U2 M2 U2 M U";

	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayerPrime();
	mainCube.moveUpLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayerPrime();
	mainCube.moveUpLayer();
}
void PLLCases::HPerm() {
	cout << "H Perm - " << "M2 U' M2 U2 M2 U' M2";

	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayer();
	mainCube.moveUpLayer();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveMLayer();
	mainCube.moveMLayer();
}
void PLLCases::AaPerm() {
	cout << "Aa Perm - " << "l' U R' D2 R U' R' D2 R2";

	mainCube.moveLwPrime();
	mainCube.moveUpLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayer();
	mainCube.moveDownLayer();
	mainCube.moveRightLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayer();
	mainCube.moveDownLayer();
	mainCube.moveRightLayer();
	mainCube.moveRightLayer();
}
void PLLCases::AbPerm() {
	cout << "Ab Perm - " << "l U' R D2 R' U R D2 R2";

	mainCube.moveLw();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayer();
	mainCube.moveDownLayer();
	mainCube.moveDownLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveUpLayer();
	mainCube.moveRightLayer();
	mainCube.moveDownLayer();
	mainCube.moveDownLayer();
	mainCube.moveRightLayer();
	mainCube.moveRightLayer();
}
void PLLCases::EPerm() {
	cout << "E Perm - " << "X' R U' R' D R U R' D' R U R' D R U' R' D'";

	mainCube.moveXPrime();
	mainCube.moveRightLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayer();
	mainCube.moveRightLayer();
	mainCube.moveUpLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayerPrime();
	mainCube.moveRightLayer();
	mainCube.moveUpLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayer();
	mainCube.moveRightLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayerPrime();
	mainCube.moveDownLayerPrime();
}
void PLLCases::TPerm() {
	cout << "T Perm - " << "R U R' U' R' F R2 U' R' U' R U R' F'";

	mainCube.moveRightLayer();
	mainCube.moveUpLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayerPrime();
	mainCube.moveFrontLayer();
	mainCube.moveRightLayer();
	mainCube.moveRightLayer();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayerPrime();
	mainCube.moveUpLayerPrime();
	mainCube.moveRightLayer();
	mainCube.moveUpLayer();
	mainCube.moveRightLayerPrime();
	mainCube.moveFrontLayerPrime();
}
void PLLCases::FPerm() {

}
void PLLCases::JaPerm() {

}
void PLLCases::JbPerm() {

}
void PLLCases::RaPerm() {

}
void PLLCases::RbPerm() {

}
void PLLCases::VPerm() {

}
void PLLCases::YPerm() {

}
void PLLCases::NaPerm() {

}
void PLLCases::NbPerm() {

}
void PLLCases::GaPerm() {

}
void PLLCases::GbPerm() {

}
void PLLCases::GcPerm() {

}
void PLLCases::GdPerm() {

}

void PLLCases::permuteLastLayer() {
	switch (mCaseIndex) {
	case 1:
		UaPerm();

	case 2:
		UbPerm();

	case 3:
		ZPerm();

	case 4:
		HPerm();

	case 5:
		AaPerm();

	case 6:
		AbPerm();

	case 7:
		EPerm();

	case 8:
		TPerm();

	case 9:
		FPerm();

	case 10:
		JaPerm();

	case 11:
		JbPerm();

	case 12:
		RaPerm();

	case 13:
		RbPerm();

	case 14:
		VPerm();

	case 15:
		YPerm();

	case 16:
		NaPerm();

	case 17:
		NbPerm();

	case 18:
		GaPerm();

	case 19:
		GbPerm();

	case 20:
		GcPerm();

	case 21:
		GdPerm();

	}
}
