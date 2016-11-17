#include "NaiveBayes.h"
#include <cstdio>   

int main(){

    NaiveBayes* naiveBayes = new NaiveBayes();
    naiveBayes->loadFileClassFeature("dataSet/dataSetClassFeatures.txt");
    naiveBayes->loadDataSetTraining("dataSet/dataSetTraininigSet.data");
    naiveBayes->genProbabilities();
    // naiveBayes->imprimirAtributos();    
    // naiveBayes->imprimirProbabilidades();
    naiveBayes->testDataSetTest("dataSet/car-prueba.data");

    delete naiveBayes;
    return 0;
}