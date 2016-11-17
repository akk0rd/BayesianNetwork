#ifndef NAIVEBAYES_H
#define NAIVEBAYES_H
#include <map>
#include <string>

using namespace std;

class NaiveBayes{
    public:
        int numClasses;
        int numFeatures;     
        int numTraining;   
        string* classes;
        string* features;     
        map<string, string*>* mapSubFeatuByFeatu;
        map<string, int>* cantSubFeatByFeat;

        map<string, int>* tabClassFreq;         
        map<string, map<string, int>* >* tabFeatXSubFeatFreq;
        map<string, map<string, map<string,int>* >* >* tabClassXFeatXSubFeaFreq;
        
        map<string, double>* priorProb;
        map<string, double>* eviProb;
        map< pair<string, string> , double>* fetDadaClaseProb;

        NaiveBayes();
        ~NaiveBayes();
        bool loadFileClassFeature(string);
        bool loadDataSetTraining(string);
        void genProbabilities();        
        bool testDataSetTest(string);
        double applyTest(string*, string*, string, int);
        void imprimirAtributos();
        void imprimirProbabilidades();

    private:
        string concStr(string, string);
        string* splitString(string, int);
                    
};

#endif /* NAIVEBAYES_H */