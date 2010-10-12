#ifndef EDIP_POTENTIAL
#define EDIP_POTENTIAL

#include "../../PotentialsInterface.h"

    /** External function implemented in Fortran. Calculate interactions between molecules of water using forcefield EDIP.
    @param[in]	N           Number of atoms.
    @param[in]	R           Array to positions of the atoms in Angstrom.
    @param[out]	F           Array used to return the forces resulting from interactions between molecules. Forces are in eV/Angstrom.
    @param[out]	U           Pointer to energy in eV.
    @param[in]  bx, by, bz  Pointer to box dimensions in Angstrom.
    */
extern "C" {
    void edip_(const long int *N, const double *R, double *F, double *U, const double* bx, const double* by, const double* bz);
}    

/** EDIP potential.*/
class EDIP : public PotentialsInterface{    
public:
// Functions
	// constructor 
    EDIP(void);
	
    // To satify interface
    void initialize(void);    
    void cleanMemory(void);    
    void force(long N, const double *R, const long *atomicNrs, double *F, double *U, const double *box);
};
#endif
