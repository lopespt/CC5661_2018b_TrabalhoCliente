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
                return data[thisRow*totalCols+column];
            }
    };
    class ConstMatrixAdapterRow{
        private:
            const vector<T>& data;
            unsigned int thisRow;
            unsigned int totalCols;
        public:
            ConstMatrixAdapterRow(const vector<T>& data, unsigned int row, unsigned int totalCols): data(data), thisRow(row), totalCols(totalCols){}
            T operator[](unsigned int column) const {
                return data[thisRow*totalCols+column];
            }
    };

public:
    unsigned int getColumns() const{
        return cols;
    }

    unsigned int getRows() const{
        return data.size()/cols;
    }
    MatrixAdapter(): cols(0){

    }
    
    MatrixAdapter(vector<T> data, unsigned int cols): cols(cols), data(data){}

    MatrixAdapter(unsigned int rows, unsigned int cols): cols(cols), data(rows*cols){}

    MatrixAdapterRow operator[](unsigned int row){
        return MatrixAdapterRow(data, row, cols);
    }
    ConstMatrixAdapterRow operator[](unsigned int row) const {
        return ConstMatrixAdapterRow(data, row, cols);
    }

    MatrixAdapter<T> operator*(const MatrixAdapter<T>& other){
            MatrixAdapter<T> res(this->getRows(), other.getColumns());

            for(int i=0;i<this->getRows();i++){
                for(int j=0;j<other.getColumns();j++){
                    res[i][j] = 0;

                    for(int k=0; k< this->getColumns(); k++){
                        res[i][j] += (*this)[i][k]*other[k][j];
                    }
                }
            }   

            return res;
        }

};
template <typename T>
ostream& operator<<(ostream& out, const MatrixAdapter<T>& m){
    for(int i=0;i<m.getRows();i++){
        for(int j=0;j<m.getColumns();j++){
            out << m[i][j] << "\t";
        }    
        out << endl;
    }
    return out;
}


#endif