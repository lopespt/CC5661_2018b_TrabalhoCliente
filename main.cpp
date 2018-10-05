#include "json.hpp"
#include "communications.hpp"
#include <iostream>
#include "matrixAdapter.hpp"
#include <vector>
#include "utils.hpp"

using namespace std;
using namespace nlohmann;

json probGrafo(json m){
    vector<float> resp = {1,0,3,6,8,9}; //Índice dos vértices do menor caminho
    return json(resp);
}

json probMatriz(json m){
    MatrixAdapter<float> mat1 = jsonToMatrix<float>(m["matrizes"][0]["matriz"]);
    
    
    return toJson<float>(mat1); //retorna a matriz resultante
}   

json probOrdenacao(json m){
    vector<float> v = jsonToVector<float>(m["elementos"]);
    //ordena o vetor...
    json s(v); //retorna vetor ordenado
    return s;
}

json trata_problema(json j,unsigned int numProb){
    // cout << j.dump() << endl;  // <---imprime o json do problema recebido
    // veja como manipular o json em https://github.com/nlohmann/json
    
    if(numProb % 100 == 0)
        cout << "Quantidade de Problemas Recebidos: " << numProb << endl;
    if(j["tipo"] == "grafo"){
        return probGrafo(j);
    }else if(j["tipo"] == "ordenacao") {
        return probOrdenacao(j);
    }else{
        return probMatriz(j);
    }
    json resp;
    return resp;
}


int main(int argc, char const *argv[])
{
    connect("http://localhost:6000",1534, trata_problema);

    
    /* code */
    return 0;
}
