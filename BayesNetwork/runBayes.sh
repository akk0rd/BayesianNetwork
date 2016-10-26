for contador in `seq 1 5`; do echo ''; done
rm Bayes.exe
echo 'Compilando'
g++ -Wall KernelBayes.h NodeBayes.h NodeBayes.cpp BayesNetwork.h BayesNetwork.cpp Main.cpp -o Bayes.exe
echo 'Comienza la ejecución del programa'
./Bayes.exe