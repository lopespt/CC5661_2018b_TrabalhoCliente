#ifndef MatrixAdapter_HPP
#define MatrixAdapter_HPP

#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;


template<typename T>
class MatrixAdapter
{
private:
    unsigned int cols;
    vector<T> data;
    class MatrixAdapterRow{
        private:
            vector<T>& data;
            unsigned int thisRow;
            unsigned int totalCols;
        public:
            MatrixAdapterRow(vector<T>& data, unsigned int row, unsigned int totalCols): data(data), thisRow(row), totalCols(totalCols){}
            T& operator[](unsigned int column){
                if(thisRow*totalCols + column >= data.size())
                    throw invalid_argument("rows and coluns overflow Matrix data");
                return data[thisRow*thisRow+column];
            }
    };
public:
    unsigned int getColumns() const{
        return cols;
    }

    unsigned int getRows() const{
        return data.size()/cols;
    }
    MatrixAdapter(vector<T> data, unsigned int cols): cols(cols), data(data){}

    MatrixAdapterRow operator[](unsigned int row){
        return MatrixAdapterRow(data, row, cols);
    }

};

#endif