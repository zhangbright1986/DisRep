/*
 *===============================================
 *  EON Eigenvalues.h
 *-----------------------------------------------
 *  Taken from Numerical Recipies
 *===============================================
*/

#ifndef _EIG_H
#define _EIG_H

#include <iostream>
#include <cmath>
#include <cassert>

#define Sign(a) ((a)>0?1:-1)
#define Abs(a) ((a)>0?(a):-(a))

void eigenValues(const int n,double *d, double **H);
void MakeSymmetric(const int n, double **H);
void TriDiagonalize(const int n,double *d,double *e,double **H);
void Diagonalize(const int n,double *d,double *e);
double scaling(double a,double b);

#endif