#include "kNN.hpp"

void tc1(){
int nRows, nCols;
Dataset dataset;
dataset.loadFromCSV("mnist.csv");
Dataset dataset_extract = dataset.extract(30, 20, 0, -1);
Dataset dataset_extract2 = dataset_extract;
dataset_extract2.getShape(nRows, nCols);
cout << "Dataset extract shape: " << nRows << "x" << nCols << endl;
}

int main() {
     tc1();
  
}