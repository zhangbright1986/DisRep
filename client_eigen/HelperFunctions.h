/*
 *===============================================
 *  EON HelperFunctions.h
 *===============================================
 */
#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

// Random number generator constants

#define IM 2147483647
#define AM (1.0/IM)
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)         
#define IM1 2147483563
#define IM2 2147483399
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791

/* Collection of supporting functions that handle arrays of doubles as vectors and different random number generators */
namespace helper_functions {

    double random(long newSeed=0); // random number generator from numerical recipies
    double randomDouble(); // random value between 0 and 1
    double randomDouble(int max); // random value between 0 and max
    double randomDouble(long max); // random value between 0 and max
    double randomDouble(double max); // random value between 0 and max
    double guaRandom(double avg,double std);//Guassion Random Number with avg and std;
    /* the dot product of the two (v1 v2) double arrays being passed in of length size */
    double dot(const double *v1, const double *v2, long size);

    /* the length of (v1) array being passed in of length size */
    double length(const double *v1, long size);

    /* the sum of the two (v1 v2) double arrays being passed in of length size in result */
    void add(double *result, const double *v1, const double *v2, long size);

    /* the difference of the two (v1 v2) double arrays being passed in of length size in result */
    void subtract(double *result, const double *v1, const double *v2, long size);

    /* the product of the (v1) double array being passed in of length size and scalar in result */
    void multiplyScalar(double *result, const double *v1, double scalar, long size);

    /* Calculate the ratio of the (v1) double array being passed in of length size and scalar in result */
    void divideScalar(double *result, const double *v1, double scalar, long size);

    /* copy v2 into v1, double arrays of length size */
    void copyRightIntoLeft(double *result, const double *v1, long size);

    /* normalize the double array v1 of length size. */
    void normalize(double *v1, long size);

    /* the orthogonal part of v1 to v2, of length size and store in result */
    void makeOrthogonal(double *result, const double *v1, const double *v2, long size);

    /* the projection of v1 on v2, of length size and store in result */
    void makeProjection(double *result, const double *v1, const double *v2, long size);
}
#endif