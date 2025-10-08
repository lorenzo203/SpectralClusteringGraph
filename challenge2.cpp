#include<iostream>
#include<Eigen/Dense>
#include<Eigen/Sparse>
#include<unsupported/Eigen/SparseExtra>
#include<string>
#include<fstream>

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
    //Frobenius norm of Ag
    cout << "----------------------------------------------" << endl;
    cout << "Task 1: Frobenius norm of Ag: ||Ag||_F = " << Ag.norm() << endl;
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
    VectorXi x = VectorXi::Ones(9);
    //Compute y 
    VectorXi y = Lg * x;
    cout << "----------------------------------------------" << endl;
    cout << "Task 2: Euclidean norm of y: ||y||_2 = " << y.norm() << endl;
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
        cout << "Task 2: Lg is symmetric positive definite" << endl;
    } else {
        cout << "Task 2: Lg is not symmetric positive definite" << endl;
    } 
    //Use SelfAdjointEigenSolver to compute the eigenvalues and eigenvectors of Lg
    SelfAdjointEigenSolver<MatrixXd> eigensolver(LgDouble);
    cout << "----------------------------------------------" << endl;
    if (eigensolver.info() != Success) {
        cerr << "Error: Eigenvalue computation failed" << endl;
        return -1;
    }
    // Extract the smallest and largest eigenvalues
    double smallestEigenvalue = eigensolver.eigenvalues()(0); // First element
    double largestEigenvalue = eigensolver.eigenvalues()(eigensolver.eigenvalues().size() - 1); // Last element
    // Results
    cout << "Task 3: Smallest eigenvalue of Lg:" << smallestEigenvalue << endl;
    cout << "Task 3: Largest eigenvalue of Lg: " << largestEigenvalue << endl;
    
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
    cout << "----------------------------------------------" << endl;
    cout << "Task 4: Smallest non-zero eigenvalue of Lg: " << smallestNonZeroEigenvalue << endl;
    //Use the index to get the corresponding eigenvector
    VectorXd correspondingEigenvector = eigensolver.eigenvectors().col(index);
    cout << "Task 4: Eigenvector associated with the smallest non-zero eigenvalue: (" << correspondingEigenvector.transpose() << ")^T" << endl;
    //cout<<eigensolver.eigenvalues()<<endl;

    SparseMatrix<int> As;
    // Load the matrix from the file
    string filename = "social.mtx";
    loadMarket(As, filename);
    //Compute the Frobenius norm of As
    cout << "----------------------------------------------" << endl;
    cout << "Task 5: Frobenius norm of As: ||As||_F = " << As.norm() << endl;
    
    //Create the vector vs containing the sum of each row of As
    VectorXi vs = VectorXi::Zero(As.rows());
    for(int i=0;i<As.cols(); i++){
        vs(i) = As.row(i).sum();
    }
    
    MatrixXi Ds = vs.asDiagonal();
    
    MatrixXi Ls = Ds - As;
    
    cout<<"----------------------------------------------"<<endl;
    if(Ls==Ls.transpose()){
        cout << "Task 6: Is the matrix Ls symmetric? YES" << endl;
    }else{
        cout << "Task 6: Is the matrix Ls symmetric? NO" << endl;
    }
    cout << "Task 6: Number of non-zero entries in Ls: " << Ls.nonZeros() << endl;
    
    //Add perturbation to the first diagonal entry of the laplacian Lg
    MatrixXd LsDouble = Ls.cast<double>(); //the matrix Ls initially is int
    LsDouble(0,0) = LsDouble(0,0) + 0.2;
    saveMarket(LsDouble, "./lis-2.1.10/test/Ls_double.mtx");
    //saveMarket(Ls, "Outputs/Ls.mtx");

    return 0; 
}