#include<iostream>
#include<Eigen/Dense>
#include<eigen1/Sparse>

using namespace Eigen;
using namespace std;

int main(){
    //define the matrix
    MatrixXi Ag = MatrixXi::Zero(9,9);
    //add the edges of the graph
    Ag(0,1) = 1;
    Ag(0,3) = 1;
    Ag(1,2) = 1;
    Ag(2,3) = 1;
    Ag(2,4) = 1;
    Ag(4,5) = 1;
    Ag(4,7) = 1;
    Ag(4,8) = 1;
    Ag(5,6) = 1;
    Ag(6,7) = 1;
    Ag(6,8) = 1;
    Ag(7,8) = 1;
    //Frobenius norm of Ag
    cout << "---------------------------------------------" << endl;
    cout << "Task 1: Frobenius norm of Ag: ||Ag||_F= " << Ag.norm() << endl;
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
    //Compute y = Lg * x
    VectorXi y = Lg * x;
    cout << "---------------------------------------------" << endl;
    cout << "Task 2: Euclidean norm of y: ||y||_2= " << y.norm() << endl;
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
    
    return 0;
}