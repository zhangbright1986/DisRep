//-----------------------------------------------------------------------------------
// eOn is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// A copy of the GNU General Public License is available at
// http://www.gnu.org/licenses/
//-----------------------------------------------------------------------------------

// This Lanczos algorithm is implemented as described in this paper:
// R. A. Olsen, G. J. Kroes, G. Henkelman, A. Arnaldsson, and H. Jónsson, 
// Comparison of methods for finding saddle points without knowledge of the final states, 
// J. Chem. Phys. 121, 9776-9792 (2004).

#include "Lanczos.h"
#include <cmath>

Lanczos::Lanczos(Matter const *matter, Parameters *params)
{
    parameters = params;
    lowestEv.resize(matter->numberOfAtoms(),3);
    lowestEv.setZero();
    lowestEw = 0.0;
}

Lanczos::~Lanczos()
{
}

// The 1 character variables in this method match the variables in the
// equations in the paper given at the top of this file.
void Lanczos::compute(Matter const *matter, AtomMatrix direction)
{
    int size = 3*matter->numberOfFreeAtoms();
    MatrixXd T(size,size), Q(size,size);
    T.setZero();
    VectorXd u(size), r(size);

    // Convert the AtomMatrix of all the atoms into
    // a single column vector with just the free coordinates.
    int i,j;
    for (i=0,j=0;i<matter->numberOfAtoms();i++) {
        if (!matter->getFixed(i)) {
            r.segment<3>(j) = direction.row(i);
            j+=3;
        }
    }
    r.normalize();

    double alpha, beta=r.norm();
    double ew=0, ewOld=0, ewAbsRelErr;
    double dr = parameters->lanczosFiniteDiff;
    VectorXd evEst, evOldEst, evT;

    VectorXd force1, force2;
    Matter *tmpMatter = new Matter(parameters);
    *tmpMatter = *matter;
    force1 = tmpMatter->getForcesFreeV();

    for (i=0;i<size;i++) {
        Q.col(i) = r/beta;

        // Finite difference force in the direction of the ith Lanczos vector
        tmpMatter->setPositionsFreeV(matter->getPositionsFreeV()+dr*Q.col(i));
        force2 = tmpMatter->getForcesFreeV();

        u = -(force2-force1)/dr;

        if (i==0) {
            r = u;
        }else{
            r = u-beta*Q.col(i-1);
        }
        alpha = Q.col(i).dot(r);
        r = r-alpha*Q.col(i);

        //Add to Tridiagonal Matrix
        T(i,i) = alpha;
        if (i>0) {
            T(i-1,i) = beta;
            T(i,i-1) = beta;
        }

        beta = r.norm();

        //Check Eigenvalues
        if (i>1) {
            Eigen::SelfAdjointEigenSolver<MatrixXd> es(T.block(0,0,i+1,i+1));
            ew = es.eigenvalues()(0); 
            evT = es.eigenvectors().col(0);
            ewAbsRelErr = fabs((ew-ewOld)/ewOld);
            ewOld = ew;

            //Convert eigenvector of T matrix to eigenvector of full Hessian
            evEst = Q.block(0,0,size,i+1)*evT;
            evEst.normalize();

            //printf("rotation angle: %f ewdiff: %f\n", acos(fabs(evEst.dot(evOldEst)))*(180/M_PI), ewChange);
            if (ewAbsRelErr < parameters->lanczosTolerance) {
                break;
            }
            evOldEst = evEst;
        }else{
            ewOld = -alpha/dr;
            ewAbsRelErr = ewOld;
            evOldEst = Q.col(0);
        }

        if (i >= parameters->lanczosMaxIterations) {
            break;
        }
    }

    lowestEw = ew;

    // Convert back from free atom coordinate column vector
    // to AtomMatrix style.
    lowestEv.resize(matter->numberOfAtoms(),3);
    for (i=0,j=0;i<matter->numberOfAtoms();i++) {
        if (!matter->getFixed(i)) {
            lowestEv.row(i) = evEst.segment<3>(j);
            j+=3;
        }
    }

    delete tmpMatter;
}

double Lanczos::getEigenvalue()
{
    return lowestEw;
}

AtomMatrix Lanczos::getEigenvector()
{
    return lowestEv;
}