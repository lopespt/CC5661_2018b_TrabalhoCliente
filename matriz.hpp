#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include <vector>
#include "matrixAdapter.hpp"
#include <json.hpp>
#include <iostream>
#include "utils.hpp"
using namespace nlohmann;
using namespace std;




class ProbMatriz{
    public:
        typedef vector<MatrixAdapter<float> > Matrices;
        Matrices mm;
        vector<vector<unsigned long int>> c;
        vector<vector<unsigned int>> s;
        

        void init_din(){
            c.resize(mm.size());
            s.resize(mm.size());
            for(int i=0;i<mm.size();i++){
                c[i].resize(mm.size());
                s[i].resize(mm.size());
            }

            
        }

        ProbMatriz(json j) {
            int num_matrizes = j["n_matrizes"];
            
            
            for(int i=0;i<num_matrizes;i++){
                mm.push_back(jsonToMatrix<float>(j["matrizes"][i]["matriz"]));
            }
            init_din();

            doParentization();
        }
        ProbMatriz(Matrices mm): mm(mm)  {
            init_din();
        }

        void doParentization(){
            vector<int> dimensions;
            dimensions.push_back(mm[0].getRows());
            for(auto m : mm){
                dimensions.push_back(m.getColumns());
            }

            
            
            for(int i=0;i<mm.size();i++)
                c[i][i] = s[i][i] = 0;
            
            
            for(int dist=1;dist<mm.size();dist++){

                for(int i=0;i<mm.size()-dist;i++){
                    int f = i + dist;
                    
                    c[i][f] = dimensions[i]*dimensions[i+1]*dimensions[f+1]+c[i+1][f];
                    s[i][f] = i;
                    
                    for(int st=i+1; st < f; st++){
                        int ct = c[i][st] + dimensions[i]*dimensions[st+1]*dimensions[f+1]+c[st+1][f];
                        if(ct < c[i][f]){
                            c[i][f] =  ct;
                            s[i][f] = st;
                        }
                    }
                }

            }

        }



        MatrixAdapter<float> doProduct(int mi, int mf){
            
            if(mf==mi)
                return mm[mi];
            else
                return doProduct(mi,s[mi][mf]) * doProduct(s[mi][mf]+1, mf);
        }

        MatrixAdapter<float> doProduct(){
                return doProduct(0, mm.size()-1);
        }

    
    private:
        Matrices ms;
};


#endif