#ifndef NAIVEBAYES_CPP
#define NAIVEBAYES_CPP
#include "NaiveBayes.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std; 

NaiveBayes::NaiveBayes(){
    numClasses = 0;
    numFeatures = 0;    
    classes = NULL;
    features = NULL;    
}

NaiveBayes::~NaiveBayes(){
    delete classes;
    delete features;   
    delete mapSubFeatuByFeatu;
    delete tabClassFreq;
    delete tabClassXFeatXSubFeaFreq;
    delete fetDadaClaseProb;
    delete priorProb;
    delete eviProb; 
}

bool NaiveBayes::loadFileClassFeature(string nameFile){    
    ifstream myFile(nameFile.c_str());
    if(!myFile.is_open()) {
    	printf("No se encontro el archivo indicado");
    	return false;
    }
    printf("Program: ===== Loading classes and features ==== \n");
    string line, token, key;
    int i = 0;
    istringstream iss;
    getline(myFile, line);
    iss.str(line);
    iss>>numClasses;

    classes = new string[numClasses];
    cantSubFeatByFeat = new map<string, int>();    
    tabClassFreq = new map<string, int>();    
    tabFeatXSubFeatFreq = new map<string, map<string, int>* >();
    mapSubFeatuByFeatu = new map<string, string*>();    

    getline(myFile, line);
    iss.str(line);
    istringstream iss1(line);    
    while(getline(iss1, token, ',')){
        classes[i++] = token;
        (*tabClassFreq)[token] = 0;
    }
        
    getline(myFile, line);
    istringstream iss2(line);
    iss2>>numFeatures;
    features = new string[numFeatures];

    for(i = 0; i < numFeatures; i++){
        getline(myFile, line);
        int cantSubFeat = 1;
        for(int k = 0; k <  (int)line.length(); k++)
            cantSubFeat += line[k] == ',';
        istringstream iss3(line);
        getline(iss3,key,':');
        features[i] = key;
        string str1;
        iss3 >> str1;
        string* arrFeat = splitString(str1, cantSubFeat);
        (*cantSubFeatByFeat)[key] = cantSubFeat;
        (*mapSubFeatuByFeatu)[key] = arrFeat;
    }
    myFile.close();

    //generando tabla de frecuencias de features x subFeatures
    tabFeatXSubFeatFreq = new map<string, map<string, int>* >();
    for(int j = 0; j < numFeatures; j++){
        string* arraySubFeat = (*mapSubFeatuByFeatu)[features[j]];
        map<string, int>* mapTemp = new map<string, int>();
        for(int k = 0; k < (*cantSubFeatByFeat)[features[j]]; k++)
            (*mapTemp)[arraySubFeat[k]] = 0;        
        (*tabFeatXSubFeatFreq)[features[j]] = mapTemp;
    }

    // generando tabla de frecuencias de classes x features x subFeatures
    tabClassXFeatXSubFeaFreq = new map<string, map<string, map<string, int>* >* >();
    for(i = 0; i < numClasses; i++){
        map<string, map<string, int>* >* mapFeatToSubFeat = new map<string, map<string, int>* >();        
        for(int j = 0; j < numFeatures; j++){
            string* arraySubFeat = (*mapSubFeatuByFeatu)[features[j]];
            map<string, int>* mapSubFeatToFreq = new map<string, int>();
            for(int k = 0; k < (*cantSubFeatByFeat)[features[j]]; k++)
                (*mapSubFeatToFreq)[arraySubFeat[k]] = 0;
            (*mapFeatToSubFeat)[features[j]] = mapSubFeatToFreq;    
        }
        (*tabClassXFeatXSubFeaFreq)[classes[i]] = mapFeatToSubFeat;
    }    
    return true;
}

bool NaiveBayes::loadDataSetTraining(string nameFile){
    ifstream myFile(nameFile.c_str());
    if(!myFile.is_open()) {
    	printf("No se encontro el archivo indicado");
    	return false;
    }    
    printf("Program: ===== Loading data set ===== \n");
    numTraining = 0;
    string line;

    while(getline(myFile, line)){
        string* arrFeat = splitString(line, numFeatures + 1);
        for(int i = 0; i < numFeatures; i++){
            (*(*tabFeatXSubFeatFreq)[features[i]])[arrFeat[i]]++;
            (*(*(*tabClassXFeatXSubFeaFreq)[arrFeat[numFeatures]])[features[i]])[arrFeat[i]]++;
        }           
        (*tabClassFreq)[arrFeat[numFeatures]]++;
        numTraining++;     
    }
    myFile.close();
    return true;
}

void NaiveBayes::genProbabilities(){
    // generate Prior Probabilities
    priorProb = new map<string, double>();
    for(int i = 0; i < numClasses; i++)
        priorProb->insert(pair<string, double>(classes[i],((double)(*tabClassFreq)[classes[i]])/numTraining));
    
    //genetare Evidence Probabilities
    eviProb = new map<string, double>();
    for(int i = 0; i < numFeatures; i++){
        string* arrSubFeat = (*mapSubFeatuByFeatu)[features[i]]; 
        for(int j = 0; j < (*cantSubFeatByFeat)[features[i]]; j++){
            double x = ((double)(*(*tabFeatXSubFeatFreq)[features[i]])[arrSubFeat[j]])/numTraining;
            eviProb->insert(pair<string,double>(concStr(features[i], arrSubFeat[j]), x ));
        }
    }

    //generate conditionals Probabilities
    fetDadaClaseProb = new map< pair<string, string> , double>();
    for(int i = 0; i < numClasses; i++){
        for(int j = 0; j < numFeatures; j++){
            string* arrSubFeat = (*mapSubFeatuByFeatu)[features[j]];
            for(int k = 0; k < (*cantSubFeatByFeat)[features[j]]; k++){
                double x = ((double)(*(*(*tabClassXFeatXSubFeaFreq)[classes[i]])[features[j]])[arrSubFeat[k]])/(*tabClassFreq)[classes[i]];
                fetDadaClaseProb->insert(pair< pair<string,string>, double>( make_pair(concStr(features[j], arrSubFeat[k]), classes[i]), x));
            }
        }
    }    
}

bool NaiveBayes::testDataSetTest(string nameFile){
    ifstream myFile(nameFile.c_str());
    if(!myFile.is_open()) {
    	printf("No se encontro el archivo indicado");
    	return false;
    }    
    printf("Program: ===== Test data set ===== \n");
    string line;
    while(getline(myFile, line)){
        int cantSubFeat = 1;
        for(int k = 0; k <  (int)line.length(); k++)
            cantSubFeat += line[k] == ',';
        string* arrFeat = splitString(line, cantSubFeat);
        int index = -1;
        double prob = -1.1; 
        for(int i = 0; i < numClasses; i++){
            double x = applyTest(arrFeat, features, classes[i], cantSubFeat);
            printf("P(%s|%s)=%lf ", classes[i].c_str(), line.c_str(), x);
            if(x > prob){
                prob = x;
                index = i;
            }
        }
        printf("== resultado => %s\n", classes[index].c_str());
    }
    return true;
}

double NaiveBayes::applyTest(string* subFeat, string* feat, string nameClass, int cant){
    double num = (*priorProb)[nameClass];
    double den = 1.0;
    for(int i = 0; i < cant; i++){        
        string str = concStr(feat[i], subFeat[i]);
        pair<string, string> keyMap = make_pair(str,nameClass);
        num *= (*fetDadaClaseProb)[keyMap];
        den *= (*eviProb)[str];
    }
    return num / den;
}


string* NaiveBayes::splitString(string str, int cantSubFeat){
    string* res = new string[cantSubFeat];
    string token;
    istringstream iss(str);
    int k = 0;
    while(getline(iss, token, ','))
        res[k++] = token;        
    return res;
}

string NaiveBayes::concStr(string a, string b){
    return a + "$" + b;
}

void NaiveBayes::imprimirAtributos(){
    printf("Program: ===== IMPRIMIENDO ATRIBUTOS === \n");
    printf("=> Número de clases: %d\n", numClasses);
    int i = 0;
    for(i = 0; i < numClasses; i++)
        printf("%s%c", classes[i].c_str(), i + 1 != numClasses ? ' ' : '\n' );
    printf("=> Número de caracteristicas: %d\n", numFeatures);
    map<string, string*>::iterator it;
    for(it = (*mapSubFeatuByFeatu).begin(), i = 0; it != (*mapSubFeatuByFeatu).end(); it++ ){
        string key = it->first;
        string* value = it->second;
        printf("%d %s:", ++i, key.c_str());
        int n = (*cantSubFeatByFeat)[key];
        for(int j = 0; j < n; j++){
            printf("%s%c", value[j].c_str(), j+1 != n ? ',': '\n');
        }       
    }    
    printf("Número de datos de entrenamiento: %d\n", numTraining);
    printf("Frecuencias por clase:\n");
    for(i = 0; i < numClasses; i++)
        printf("%s(%d)%c", classes[i].c_str(), (*tabClassFreq)[classes[i]], i+1 != numClasses ? ' ' : '\n');
    printf("Frecuencias por features\n");
    for(i = 0; i < numFeatures; i++){        
        printf("%d %s: ",i+1, features[i].c_str());
        string* arrSubFeat = (*mapSubFeatuByFeatu)[features[i]];
        for(int k = 0; k < (*cantSubFeatByFeat)[features[i]]; k++){
            printf("%s(%d) ", arrSubFeat[k].c_str(), (*(*tabFeatXSubFeatFreq)[features[i]])[arrSubFeat[k]] );
        }
        printf("\n"); 
    }    
    printf("Frecuencias classesXsubFeaturse\n");
    for(i = 0; i < numClasses; i++){
        printf("%d %s: ", i + 1, classes[i].c_str());
        for(int j = 0; j < numFeatures; j++){
            string* arrSubFeat = (*mapSubFeatuByFeatu)[features[j]];
            printf("%s{", features[j].c_str());
            for(int k = 0; k < (*cantSubFeatByFeat)[features[j]]; k++){
                printf("%s(%d) ", arrSubFeat[k].c_str(), (*(*(*tabClassXFeatXSubFeaFreq)[classes[i]])[features[j]])[arrSubFeat[k]]);
            }
            printf("} || ");
        }
        printf("\n");
    }
}

void NaiveBayes::imprimirProbabilidades(){
    printf("Program: ===== IMPRIMIENDO PROBABILIDADES === \n");
    printf("Probabilides por clase:\n");
    for(int i = 0; i < numClasses; i++)
        printf("P(%s)=%lf%c", classes[i].c_str(), (*priorProb)[classes[i]], i+1 != numClasses ? ' ' : '\n');
    printf("Probabilidades de la evidencia\n");
    for(int i = 0; i < numFeatures; i++){        
        printf("%d %s: ",i+1, features[i].c_str());
        string* arrSubFeat = (*mapSubFeatuByFeatu)[features[i]];
        for(int k = 0; k < (*cantSubFeatByFeat)[features[i]]; k++){
            string str = concStr(features[i],arrSubFeat[k]);             
            printf("P(%s)=%lf ", str.c_str(), (*eviProb)[str]);
        }
        printf("\n"); 
    }    
    printf("Probabilidades condicionales\n");
    for(int i = 0; i < numFeatures; i++){
        string* arrSubFeat = (*mapSubFeatuByFeatu)[features[i]];
        printf("=>%s ", features[i].c_str());
        for(int j = 0; j < (*cantSubFeatByFeat)[features[i]]; j++){
            string str = concStr(features[i],arrSubFeat[j]);
            for(int k = 0; k < numClasses; k++){
                pair<string,string> keyMap = make_pair(str, classes[k]);
                printf("P(%s|%s)=%lf ", str.c_str(), classes[k].c_str(), (*fetDadaClaseProb)[keyMap]);
            }
        }
        printf("\n");
    }

}

#endif /* NAIVEBAYES_H */
