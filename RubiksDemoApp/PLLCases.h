#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "RubiksCube.h"
using namespace std;

class PLLCases : RubiksCube
{
private:
	RubiksCube& mainCube;
	vector<int> mLastLayer;
	vector<vector<int>> mAllCasesVec;
	int mCaseIndex;

	//Edge only permutations
	const vector<int> UaCase = { 1, 2, 3, 8, 5, 4, 7, 6, 9 };
	const vector<int> UbCase = { 1, 2, 3, 6, 5, 8, 7, 4, 9 };
	const vector<int> ZCase = { 1, 4, 3, 2, 5, 8, 7, 6, 9 };
	const vector<int> HCase = { 1, 8, 3, 6, 5, 4, 7, 2, 9 };

	//Corner only permutations
	const vector<int> AaCase = { 1, 8, 7, 6, 5, 4, 9, 2, 3 };
	const vector<int> AbCase = { 1, 6, 3, 2, 5, 8, 9, 4, 7 };
	const vector<int> ECase = { 1, 4, 7, 8, 5, 2, 3, 6, 9 };

	//Corner and Edge swap permutations
	const vector<int> TCase = { 1, 2, 9, 6, 5, 4, 7, 8, 3 };
	const vector<int> FCase = { 1, 4, 9, 2, 5, 8, 3, 6, 7 };
	const vector<int> JaCase = { 1, 6, 9, 4, 5, 8, 3, 2, 7 };
	const vector<int> JbCase = { 1, 2, 9, 4, 5, 8, 7, 6, 3 };
	const vector<int> RaCase = { 1, 6, 9, 2, 5, 4, 3, 8, 7 };
	const vector<int> RbCase = { 1, 8, 9, 2, 5, 6, 3, 4, 7 };
	const vector<int> VCase = { 1, 8, 7, 2, 5, 4, 3, 6, 9 };
	const vector<int> YCase = { 1, 8, 7, 6, 5, 2, 3, 4, 9 };
	const vector<int> NaCase = { 1, 2, 7, 5, 5, 4, 3, 8, 9 };
	const vector<int> NbCase = { 1, 8, 7, 4, 5, 6, 3, 2, 9 };

	//Corner and Edge cycle permutations
	const vector<int> GaCase = { 1, 6, 7, 2, 5, 4, 9, 8, 3 };
	const vector<int> GbCase = { 1, 4, 9, 6, 5, 8, 7, 2, 3 };
	const vector<int> GcCase = { 1, 8, 9, 6, 5, 2, 7, 4, 3 };
	const vector<int> GdCase = { 1, 6, 9, 8, 5, 4, 7, 2, 3 };

public:
	void copyLastLayer();
	void getLastLayerCase();
	void setAllCases();
	void getCaseIndex();

	PLLCases(RubiksCube& cube) : mainCube(cube) {

	}

	//Permutation Functions

	
};

