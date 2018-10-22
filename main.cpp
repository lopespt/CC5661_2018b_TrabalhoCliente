#include "json.hpp"
#include "communications.hpp"
#include <iostream>
#include "matrixAdapter.hpp"
#include <vector>
#include "utils.hpp"
#include "matriz.hpp"
#include "grafo.hpp"

using namespace std;
using namespace nlohmann;
bool corretor;
json probGrafo(json m){
    
    ProbGrafo g(m);
    vector<vector<int>> menores = g.MenorCaminho();

    json resp;
    if (corretor == true)
        resp["solucoes"] = json(menores);
    else
        resp = json(menores[0]);

    return resp;
}


json probMatriz(json m){
    ProbMatriz p(m);
    
    MatrixAdapter<float> mat2 = p.doProduct();

    return toJson<float>(mat2); //retorna a matriz resultante
}   

json probOrdenacao(json m){
    vector<float> v = jsonToVector<float>(m["elementos"]);
    sort(v.begin(), v.end());
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
    corretor = argc>1;
    if(argc>1)
        connect("http://localhost:5000/correction",-1, trata_problema);
    else
        connect("http://localhost:5000",123, trata_problema);
    
    /* code */
    return 0;
}
