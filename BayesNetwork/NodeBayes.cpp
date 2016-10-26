#ifndef NODEBAYES_CPP
#define NODEBAYES_CPP

#include "NodeBayes.h"
#include <vector>

using namespace std;

template<typename K>
NodeBayes<K>::NodeBayes(Ba_Int cantNo, Ba_Word nameVa, Ba_Word* stateName, Ba_Dou* valoPro){
    cant = cantNo;
    nameNode = nameVa;
    names = stateName;    
    probabi = valoPro;    
    verAdj = new vector< NodeBayes<K>* >();
}

template<typename K>
NodeBayes<K>::~NodeBayes(){
    delete names;
    delete probabi;
    delete verAdj;
}

template<typename K>
void NodeBayes<K>::insertVec(NodeBayes* temp){
    verAdj->push_back(temp);
}

#endif