#ifndef NODEBAYES_H
#define NODEBAYES_H

#include "KernelBayes.h"
#include <vector>

using namespace std;

template<typename K>
class NodeBayes{
    protected:
        typedef typename K::Ba_Int Ba_Int;
        typedef typename K::Ba_Word Ba_Word;
        typedef typename K::Ba_Dou Ba_Dou;    
    public:                
        Ba_Word nameNode;        
        Ba_Int cant;
        vector< NodeBayes<K>* >* verAdj;
        Ba_Word* names;
        Ba_Dou* probabi;

        NodeBayes(Ba_Int,Ba_Word,Ba_Word*,Ba_Dou*);
        ~NodeBayes();
        void insertVec(NodeBayes*);        
};

#include "NodeBayes.cpp"
#endif