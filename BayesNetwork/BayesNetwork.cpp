#ifndef BAYESNETWORK_CPP
#define BAYESNETWORK_CPP

#include "BayesNetwork.h"
#include "NodeBayes.h"
#include <stack>

 using namespace std;

template<typename K>
BayesNetwork<K>::BayesNetwork(Ba_Word name, Ba_Int cant){
    cantNodes = cant;
    nameNetwork = name;
    root = NULL;
}

template<typename K>
BayesNetwork<K>::BayesNetwork(){
    cantNodes = 0;
    nameNetwork = "";
    root = NULL;
}

template<typename K>
BayesNetwork<K>::~BayesNetwork(){
    delete root;
}

template<typename K>
void BayesNetwork<K>::recProfundidad(){
    stack<NodeBayes<K>* >* pila = new stack<NodeBayes<K>*>();
    NodeBayes<K>* temp = root;
    pila.push(temp);
    while(!pila.empty()){
        temp = pila.top();
        
        printf("Nombre nodo: %s | Cantidad Estados: %d ==> ", temp->nameNode.c_str(), temp->cant);
        for(int j = 0; j < temp->cant; j++)
            printf("%s(%lf)%c", temp->names[j].c_str(), temp->probabi[j], j+1 != temp->cant ? ' ' : '\n');
        

        pila.pop();
        for(int i = 0; i < (int)temp->verAdj->size(); i++){
            pila.push((*temp->verAdj)[i]);
        }        
    }
    delete pila;
}

#endif