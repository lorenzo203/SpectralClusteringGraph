#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Sparse>
#include<unsupported/Eigen/SparseExtra>
#include<fstream>

using namespace Eigen;
using namespace std;

int main(){
    // Open report file in append mode
    std::ofstream report("../reports/challenge2_report.txt", std::ios::app);
    std::ostream& out = report.is_open() ? static_cast<std::ostream&>(report) : std::cout;
    if (!report.is_open()) {
        cerr << "Warning: unable to open report file, using stdout.\n";
    }
    
    // Load the eigenvector matrix from task 9
    SparseMatrix<double> E_s;
    if(!loadMarket(E_s,"evectors9.mtx")){
        cerr << "Error: failed to load evectors9.mtx" << endl;
        return 1;
    }
    
    MatrixXd E = MatrixXd(E_s);
    
    if(E.cols() < 2) {
        cerr << "Error: evectors9.mtx does not have enough columns" << endl;
        return 1;
    }
    
    // Get the Fiedler vector (second eigenvector)
    VectorXd FiedlerVec = E.col(1);
    
    out << "----------------------------------------------" << "\n";
    out << "TASK 10: Fiedler Vector Analysis" << "\n";
    out << "----------------------------------------------" << "\n";
    
    // Count positive, negative, and zero values
    double tol_sign = 1e-12;
    auto np = (FiedlerVec.array() >  tol_sign).count();
    auto nn = (FiedlerVec.array() < -tol_sign).count();
    auto n0 = FiedlerVec.size() - np - nn;
    
    out << "Task 10: np = " << np << ", nn = " << nn << ", n0 = " << n0 << "\n";

    return 0;
}
