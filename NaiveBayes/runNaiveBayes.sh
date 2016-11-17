for contador in `seq 1 3`; do echo ''; done
rm NaiveBayes.exe
echo 'Compilando...'
g++ -Wall NaiveBayes.h NaiveBayes.cpp Main.cpp -o NaiveBayes.exe
echo '======Comienza la ejecución del programa===='
./NaiveBayes.exe