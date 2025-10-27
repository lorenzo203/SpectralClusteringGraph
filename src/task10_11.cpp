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
    
    // Load the symmetrized adjacency matrix A_s
    SparseMatrix<double> A_s;
    if(!loadMarket(A_s, "As.mtx")){
        cerr << "Error: failed to load As.mtx" << endl;
        return 1;
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

    //group the nodes as connected by recording the indexes of the positive and negative elements in the fiedler's vector
    vector<int> positive_nodes;
    vector<int> negative_nodes;
    int side = FiedlerVec.rows();
    for(int k = 0; k < side; k++){
        if(FiedlerVec(k) > tol_sign){
            positive_nodes.push_back(k);
        } else if(FiedlerVec(k) < -tol_sign){
            negative_nodes.push_back(k);
        }
    }
    
    //build a permutation vector: first positive nodes, then negative nodes
    vector<int> P_vec;
    P_vec.insert(P_vec.end(), positive_nodes.begin(), positive_nodes.end());
    P_vec.insert(P_vec.end(), negative_nodes.begin(), negative_nodes.end());
    
    // Create permutation matrix
    PermutationMatrix<Dynamic, Dynamic> P(side);
    for(int i = 0; i < side; i++){
        P.indices()(i) = P_vec[i];
    }
    
    // Compute reordered adjacency matrix: A_ord = P * A_s * P^T
    SparseMatrix<double> A_ord = P.transpose() * A_s * P;
    
    out << "----------------------------------------------" << "\n";
    out << "\nTask 11: " << "\n";
    //out << "A_ord size: " << A_ord.rows() << "x" << A_ord.cols() << "\n";
    
    // Count non-zeros in off-diagonal blocks
    int np_int = static_cast<int>(np);
    int nn_int = static_cast<int>(nn);
    long long nnz_A12_ord = 0;
    for (int col = np_int; col < side; ++col) {                 // colonne nel gruppo 2
        for (SparseMatrix<double>::InnerIterator it(A_ord, col); it; ++it) {
            if (it.row() < np_int) ++nnz_A12_ord;               // righe nel gruppo 1
        }
    }

    //same for A_s
    long long nnz_A12_s = 0;
    for (int col = np_int; col < side; ++col) {                 // colonne nel gruppo 2
        for (SparseMatrix<double>::InnerIterator it(A_s, col); it; ++it) {
            if (it.row() < np_int) ++nnz_A12_s;               // righe nel gruppo 1
        }
    }    
    
    out << "Non-zeros in off-diagonal blocks for A_ord: " << nnz_A12_ord << "\n";
    out << "Non-zeros in off-diagonal blocks for A_ord: " << nnz_A12_s << "\n";
    //out << "As nnz: " << A_s.nonZeros() << "\n";
    report.close();
    return 0;
}
