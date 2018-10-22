#ifndef UTILS_HPP
#define UTILS_HPP
#include "matrixAdapter.hpp"
#include "json.hpp"
#include <iostream>
using namespace std;
using namespace nlohmann;

template <typename T>
ostream& operator<<(ostream &out, const vector<T>& x){
    out << "[ ";
    for(auto i : x){
        out  << i << ", ";
    }
    out << " ]";
    return out;
}

template <typename T>
MatrixAdapter<T> jsonToMatrix(json m){
    vector<T> v;
    auto it = back_inserter(v);
    
    for( auto x : m.items()){
        auto items = x.value().items();
        transform(items.begin(), items.end(), it, [](auto z){
           return (T)z.value();
        });
    }
    int cols = m[0].size();
    MatrixAdapter<T> mat(v, cols);
    return mat;
}


template <typename T>
vector<T> jsonToVector(json m){
    vector<T> v;
    for(auto x: m){
        v.push_back(x);
    }
    return v;
}

template <typename T>
json toJson(MatrixAdapter<T>& mat){
    json out;
    for(int i=0; i<mat.getRows(); i++){
        json line;
        for(int j=0; j<mat.getColumns(); j++){
            line.push_back(mat[i][j]);
        }
        out.push_back(line);
    }
    return out;
}

#endif