#ifndef BAYESNETWORK_H
#define BAYESNETWORK_H

#include "NodeBayes.h"

template<typename K>
class BayesNetwork{
    protected:
        typedef typename K::Ba_Int Ba_Int;
        typedef typename K::Ba_Word Ba_Word;
        typedef typename K::Ba_Dou Ba_Dou;
        typedef NodeBayes<K> NodeBa;     
    public:          
        NodeBa* root;        
        Ba_Int cantNodes;
        Ba_Word nameNetwork;

        BayesNetwork(Ba_Word,Ba_Int);
        BayesNetwork();        
        ~BayesNetwork();
        void recProfundidad();
};

#include "BayesNetwork.cpp"
#endif