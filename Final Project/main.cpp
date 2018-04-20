#include <iostream>
#include <string>
#include "Node.h"
#include "ExcelReader.h"
#include "KNN.h"
#include "Dataprocess.h"
#include "DecisionTree.h"
#include "Evaluation.h"
#define SPLITWEIGHT 0.8
using namespace std;

int main() {
	//Read from csv file
	string fileName("dataSet.csv");
	ExcelReader reader(fileName);
	set dataSet = reader.readFile();

	//Coyp DataSet for KNN and normalize it
	set dataSetCopy = dataSet;
	DATARPOCESS::normalizeAllFactorsExcept(dataSetCopy, 11);


	//Seperate data into trainingSet and testSet
	std::tuple<set, set> setPair = DATARPOCESS::seperateSet(dataSetCopy, SPLITWEIGHT);
	set trainningSet = get<0>(setPair);
	set testSet = get<1>(setPair);
	vector<int> originalLabel = DATARPOCESS::getLabels(testSet);




	//Rebalance trainingSet
	double balanceWeights[3] = { 1,5,1 };
	vector<Node> balanceSet = DATARPOCESS::rebalanceSet(trainningSet, balanceWeights);


	double weights[12] = {1,1,1,1,1,1,1,1,1,1,1,1};

	KNN cif;
	cif.fit(balanceSet, balanceSet.size());
	vector<int> predictLabel = cif.predict(testSet);
	double accuracy = Evaluation::accuracy(originalLabel, predictLabel);

	cout << "Accuracy: " << accuracy << endl;
	Evaluation::confusionMatrix(originalLabel, predictLabel);
	
	// Decision Tree Implementation
	// Tree Construction using trainningSet
	Tree tree(0.8);
	Tree * DT = &tree;
	int factorNum = trainningSet[0].getFactorNum();
	vector<int> attributes;
	for (int i = 0; i < factorNum - 1; ++i) { attributes.push_back(i); }
	DT = DT->buildTree(DT, dataSet,0,attributes);
	// Display the tree structure
	vector<string> attributeName;
	DT->display(attributeName);

	// Label Prediction using testSet
	vector<int> predictLabel_DT = DT->predict(testSet);
	double accuracy_DT = Evaluation::accuracy(originalLabel, predictLabel_DT);
	cout << "Accuracy: " << accuracy_DT << endl;
	Evaluation::confusionMatrix(originalLabel, predictLabel_DT);

}
