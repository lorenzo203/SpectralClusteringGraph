#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Sparse>
#include<unsupported/Eigen/SparseExtra>
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include<sstream>

using namespace Eigen;
using namespace std;

int main(){
    //define the matrix
    MatrixXi Ag = MatrixXi::Zero(9,9);
    //add the edges of the graph
    Ag(0,1) = 1;
    Ag(0,3) = 1;
    Ag(1,0) = 1;
    Ag(1,2) = 1;
    Ag(2,1) = 1;
    Ag(2,3) = 1;
    Ag(2,4) = 1;
    Ag(3,0) = 1;
    Ag(3,2) = 1;
    Ag(4,2) = 1;
    Ag(4,5) = 1;
    Ag(4,7) = 1;
    Ag(4,8) = 1;
    Ag(5,4) = 1;
    Ag(5,6) = 1;
    Ag(6,5) = 1;
    Ag(6,7) = 1;
    Ag(6,8) = 1;
    Ag(7,4) = 1;
    Ag(7,6) = 1;
    Ag(7,8) = 1;
    Ag(8,4) = 1;
    Ag(8,6) = 1;
    Ag(8,7) = 1;
    std::ofstream report("../reports/challenge2_report.txt");
    std::ostream& out = report.is_open() ? static_cast<std::ostream&>(report) : std::cout;
    if (!report.is_open()) {
        cerr << "Warning: unable to open report file, using stdout.\n";
    }
    //Frobenius norm of Ag
    MatrixXd Ag_double = Ag.cast<double>(); 
    out << "----------------------------------------------" << "\n";
    out << "Task 1: Frobenius norm of Ag: ||Ag||_F = " << Ag_double.norm() << "\n";
    //Define the vector vg
    VectorXi vg = VectorXi::Zero(9);
    //Sum of each row of Ag into vg
    for(int i=0; i<9; i++){
        vg(i) = Ag.row(i).sum();
    }
    //Build the diagonal matrix Dg from vg
    MatrixXi Dg = vg.asDiagonal();
    //Define the Laplacian matrix Lg
    MatrixXi Lg = Dg - Ag;
    //Build the vector x
    VectorXi x_eigen = VectorXi::Ones(9);
    //Compute y 
    VectorXi y = Lg * x_eigen;
    VectorXd y_d = y.cast<double>();
    out << "----------------------------------------------" << "\n";
    out << "Task 2: Euclidean norm of y: ||y||_2 = " << y_d.norm() << "\n";
    //check simmetry of Lg
    bool LgSymmetric = false;
    if(Lg==Lg.transpose()){
        LgSymmetric = true;
    } 
    //check positive definiteness of Lg
    MatrixXd LgDouble = Lg.cast<double>();
    LLT<MatrixXd> lltOfLg(LgDouble);
    bool LgSPD = LgSymmetric && (lltOfLg.info() == Eigen::Success);
    if(LgSPD){
        out << "Task 2: Lg is symmetric positive definite" << "\n";
    } else {
        out << "Task 2: Lg is not symmetric positive definite" << "\n";
    } 
    //Use SelfAdjointEigenSolver to compute the eigenvalues and eigenvectors of Lg
    SelfAdjointEigenSolver<MatrixXd> eigensolver(LgDouble);
    out << "----------------------------------------------" << "\n";
    if (eigensolver.info() != Success) {
        cerr << "Error: Eigenvalue computation failed" << endl;
        return -1;
    }
    // Extract the smallest and largest eigenvalues
    double smallestEigenvalue = eigensolver.eigenvalues()(0); // First element
    double largestEigenvalue = eigensolver.eigenvalues()(eigensolver.eigenvalues().size() - 1); // Last element
    // Results
    out << "Task 3: Smallest eigenvalue of Lg:" << smallestEigenvalue << "\n";
    out << "Task 3: Largest eigenvalue of Lg: " << largestEigenvalue << "\n";
    
    //Find the smallest non-zero eigenvalue and its eigenvector
    double smallestNonZeroEigenvalue = 0;
    double tol = 1e-10;
    int index = 0;
    for(int i =0; i<eigensolver.eigenvalues().size(); i++){
        if(eigensolver.eigenvalues()(i) > tol){
            smallestNonZeroEigenvalue = eigensolver.eigenvalues()(i);
            index = i;
            break;
        }
    }
    out << "----------------------------------------------" << "\n";
    out << "Task 4: Smallest non-zero eigenvalue of Lg: " << smallestNonZeroEigenvalue << "\n";
    //Use the index to get the corresponding eigenvector
    VectorXd correspondingEigenvector = eigensolver.eigenvectors().col(index);
    out << "Task 4: Eigenvector associated with the smallest non-zero eigenvalue: (" << correspondingEigenvector.transpose() << ")^T" << "\n";
    //cout<<eigensolver.eigenvalues()<<endl;

    SparseMatrix<int> As;
    // Load the matrix from the file
    string filename = "../data/social.mtx";
    loadMarket(As, filename);
    MatrixXd As_d = As.cast<double>();
    //Compute the Frobenius norm of As
    out << "----------------------------------------------" << "\n";
    out << "Task 5: Frobenius norm of As: ||As||_F = " << As_d.norm() << "\n";
    
    //Create the vector vs containing the sum of each row of As
    VectorXi vs = VectorXi::Zero(As.rows());
    for(int i=0;i<As.cols(); i++){
        vs(i) = As.row(i).sum();
    }
    
    MatrixXi Ds = vs.asDiagonal();
    
    MatrixXi Ls = Ds - As;
    SparseMatrix<int> LsSparse = Ls.sparseView();
    
    out<<"----------------------------------------------"<<"\n";
    if(Ls==Ls.transpose()){
        out << "Task 6: Is the matrix Ls symmetric? YES" << "\n";
    }else{
        out << "Task 6: Is the matrix Ls symmetric? NO" << "\bn";
    }
    out << "Task 6: Number of non-zero entries in Ls: " << LsSparse.nonZeros() << "\n";
    
    //Add perturbation to the first diagonal entry of the laplacian Lg
    MatrixXd LsDouble = Ls.cast<double>(); //the matrix Ls initially is int
    LsDouble(0,0) = LsDouble(0,0) + 0.2;
    
    // Save both Ls and As for later tasks
    out << "----------------------------------------------" << "\n";
    if(saveMarket(LsDouble, "../matrices/Ls.mtx")){
        out << "Task 7: Matrix Ls with perturbation saved into ../matrices/Ls.mtx" << "\n";
    } else {
        out << "Error! Matrix Ls could not have been saved properly!"<<"\n";
    }
    
    // Save As for task 10-11
    SparseMatrix<double> AsDouble = As.cast<double>();
    if(saveMarket(AsDouble, "../matrices/As.mtx")){
        out << "Task 7: Matrix As saved into ../matrices/As.mtx" << "\n";
    } else {
        out << "Error! Matrix As could not have been saved properly!"<<"\n";
    }
    
    //find the fiedlers vector by first building the matrix containing the 2 eigenvectors 
    //associated to the 2 smallest eigvals (we need the secodn)
    
    return 0; 
}