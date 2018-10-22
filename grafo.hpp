#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "matrixAdapter.hpp"
#include <vector>
#include "json.hpp"
#include "utils.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <deque>

using namespace std;
using namespace std::placeholders;
using namespace nlohmann;




class ProbGrafo{

    private:
        typedef pair<int, float> aresta;
        vector<int> passarEm;
        vector<vector<aresta>> grafo;

        static bool compara_aresta(const vector<float> &d, int a, int b){
            if(d[a] >= 0 && d[b] >= 0){
                return d[a] > d[b];
            }else{
                return d[a] < 0;
            }
        }

        

    public:
        struct ArvDijkstra{
            vector<int> arvore;
            vector<float> pesos;
        };

        ProbGrafo(json j){
            MatrixAdapter<float> m=  jsonToMatrix<float>(j["mat_pesos"]);
            grafo.resize(m.getRows());
            for(int i=0;i<m.getRows();i++){
                for(int j=0;j<m.getColumns();j++){
                    if(m[i][j] > 0.0001){
                        grafo[i].push_back(make_pair(j,m[i][j]));
                    }
                }
            }

            passarEm = jsonToVector<int>(j["passar_em"]);
        }

        
        vector<int> caminho(vector<ArvDijkstra>& arv, const vector<int>& ordem){
            vector<int> sequencia;

            for(int i=ordem.size()-1; i>0; i--)
            {
                int de = ordem[i-1];
                int para = passarEm[ordem[i]];
                int atual = para;
                while(atual != -1 && arv[de].arvore[atual] != -1){
                    sequencia.push_back(atual);
                    atual =  arv[de].arvore[atual];
                }
            }
            sequencia.push_back(passarEm[ordem.front()]);
            reverse(sequencia.begin(), sequencia.end());

            return sequencia;
        }

        vector<int> fromOrderToVertices(const vector<int>& ordem){
            vector<int> r;
            
             transform(ordem.begin(), ordem.end(), back_inserter(r), 
                [&](int i){
                    return passarEm[i];
                } 
             );

             return r;
        }


        vector<vector<int>> MenorCaminho(){
            vector<ArvDijkstra> resps;
            resps.push_back(dijkstra(passarEm[0]));
            resps.push_back(dijkstra(passarEm[1]));
            resps.push_back(dijkstra(passarEm[2]));


            /*for(int i=0;i<grafo.size(); i++){
                for(int j=0; j<grafo[i].size(); j++){
                    cout << i << "->" << grafo[i][j].first << " = " << grafo[i][j].second << endl ;
                }
            }*/


            vector<int> order(passarEm.size());
            iota(order.begin(), order.end(),0);
            
            vector<vector<int>> best_perm;
            float best_dist = -1;
            
                        
            while(next_permutation(order.begin(), order.end())){
                float dist = 0;
                bool cancel = false;
                for(int st=order.size()-1; !cancel && st>0; st--){
                    float d = resps[order[st-1]].pesos[passarEm[order[st]]];
                    
                    if(d < 0)
                        cancel=true;
                    
                    
                    dist += d;
                }

                if(cancel){
                    continue;
                }
                
                if(best_dist < 0){
                    best_perm.push_back(order);
                    best_dist = dist;
                }else if(dist < best_dist){    
                    best_perm.clear();
                    dist = best_dist;
                }

                if( fabs(best_dist - dist) < 0.00001 ){
                    best_perm.push_back(order);
                }
            }

            
            vector<vector<int>> sequencias;
            transform( best_perm.begin(), best_perm.end(), back_inserter(sequencias), [&](const vector<int>& i){
                vector<int> x = caminho(resps, i);
                //reverse(x.begin(), x.end());
                return x;
            });
            
            cout << "==============" << endl;
            cout << passarEm << endl;
            cout << sequencias[1] << " - " << best_dist << endl;
            
           //cout << best_dist << endl;
            return sequencias;

        }

        ArvDijkstra dijkstra(int origem){
            vector<int> arvore(grafo.size(), -1);
            vector<float> d(grafo.size(),-1);
            vector<int> h(grafo.size());
            iota(h.begin(), h.end(),0);
            
            

            d[origem] = 0;
            auto cmp = bind(ProbGrafo::compara_aresta, d, _1,_2);
            
            make_heap(h.begin(),h.end(), cmp);
            
            while(!h.empty()){
                
                int n = h[0];
                pop_heap(h.begin(), h.end());
                h.pop_back();

                for(auto viz : grafo[n]){
                    if( d[n] >= 0 && find(h.begin(), h.end(), viz.first) != h.end() ){
                        float dt = d[n] + viz.second;
                        if(d[viz.first] == -1 || dt < d[viz.first]){
                            d[viz.first] = dt;
                            arvore[viz.first] = n;
                        }
                    }
                }



            }
            
            cout << "origem: " << origem << " arv: " << d << endl;

            return ArvDijkstra{arvore, d};

        }

};

#endif
