#include "BayesNetwork.h"
#include "NodeBayes.h"
#include "KernelBayes.h"
#include <cstdio>
#include <string>

using namespace std; 

typedef BayesNetwork<KernelBayes> BayesNet;
typedef NodeBayes<KernelBayes> NodBayes;

// void imprimirBayesNet(BayesNet* bayesNet){
//     printf("Nombre Red: %s\n", bayesNet->nameNetwork.c_str());
//     printf("Cant Nodos: %d\n", bayesNet->cantNodes);
//     NodBayes* temp = bayesNet->root;
//     for(int i = 0; i < bayesNet->cantNodes; i++){
//         printf("Nombre nodo: %s | Cantidad Estados: %d ==> ", temp->nameNode.c_str(), temp->cant);
//         for(int j = 0; j < temp->cant; j++)
//             printf("%s(%lf)%c", temp->names[j].c_str(), temp->probabi[j], j+1 != temp->cant ? ' ' : '\n');
//         temp = temp->next;
//     }
// }

int main(){
    string nameNode = "Node";
    string nameEstado = "Est";
    int n = 7, cantEst;
    BayesNetwork<KernelBayes>* bayes = new BayesNetwork<KernelBayes>("TestBayes", n);
    NodBayes* temp;
    for(int i = 0; i < n; i++){
        cantEst = (i+2)/2 + 1;
        string* arrayNameEsta = new string[cantEst];
        double* arrayProba = new double[cantEst];
        for(int j = 0; j < cantEst; j++){
            arrayNameEsta[j] = nameEstado + (char)('0' + j);
            arrayProba[j] = 1.0 / cantEst;
        }
        NodBayes* nod = new NodBayes(cantEst, nameNode, arrayNameEsta, arrayProba);
        if(i == 0){
            bayes->root = nod;
            temp = bayes->root;
        }
        for(int j = 0; j < 4; j++){
            NodBayes* nodTemp = new NodBayes(cantEst, nameNode, arrayNameEsta, arrayProba);
            bayes->root->insertVec(nodTemp);
        }          
    }    


    // imprimirBayesNet(bayes);    
    return 0;
}
