/*
 *===============================================
 *  ConjugateGradients.h
 *  eon2
 *-----------------------------------------------
 *  Created by Andreas Pedersen on 10/11/06.
 *-----------------------------------------------
 *  Modified. Name, Date and a small description!
 *
 *-----------------------------------------------
 *  Todo:
 *
 *-----------------------------------------------
 *  Heavily inspired of codes by:
 *      Graeme Henkelmann
 *      Andri Arnaldsson
 *      Roar Olsen
 *===============================================
 */
#ifndef CG_H
#define CG_H

#include "MinimizersInterface.h"
#include "Matter.h"
#include "HelperFunctions.h"
#include "Constants.h"
#include "Parameters.h"

/** Functionality relying on the conjugate gradients algorithm. The object is capable of minimizing an Matter object or modified forces being passed in.*/
class ConjugateGradients : public MinimizersInterface{

public:
    /** Constructor to be used when a structure is minimized.
    @param[in]   *matter        Pointer to the Matter object to be relaxed.
    @param[in]   *parameters    Pointer to the Parameter object containing the runtime parameters.*/
    ConjugateGradients(Matter *matter, Parameters *parameters);

    /** Constructor to be used when modified forces are used.
    @param[in]   *matter        Pointer to the Matter object to be relaxed.
    @param[in]   *parameters    Pointer to the Parameter object containing the runtime parameters.
    @param[in]   *forces        Double array containing the forces acting on the matter.*/
    ConjugateGradients(Matter *matter, Parameters *parameters, double *forces); 

    ~ConjugateGradients();///< Destructor.

    void oneStep();///< Do one iteration.
    void fullRelax();///< Relax the Matter object corresponding to the pointer that was passed with the constructor.
    bool isItConverged(double convergeCriterion);///< Determine if the norm of the force vector is bellow the \a convergeCriterion.

    //----Functions used when forces are modified (saddle point search)----        
    /** Performs and infinitisimal step along the search direction. 
    @param[out]  *posStep  Double array containing the modified positions of the atoms.
    @param[in]   *pos      Double array containing the original positions of the atoms.*/
    void makeInfinitisimalStepModifiedForces(double *posStep, double *pos);

    /** Performs and infinitisimal step along the search direction. 
    @param[out]  *pos              Double array containing the calculated new positions of the atoms.
    @param[in]   *forceBeforeStep  Double array, the forces before ConjugateGradients::makeInfinitisimalStepSaddleSearch was called.
    @param[in]   *forceAfterStep   Double array, the forces returned by ConjugateGradients::makeInfinitisimalStepSaddleSearch.
    @param[in]   maxStep           Double the maximal accepted step. The maximal value of norm of the displacement.*/
    void getNewPosModifiedForces(double *pos, double *forceBeforeStep, double *forceAfterStep, double maxStep);
    
    void setFreeAtomForcesModifiedForces(double *forces);///< Enables the use of modyfied forces. \a Double array containing the forces acting on the atoms.
    
private:
    long nFreeCoord_;///< Number of free coordinates.

    Matter *matter_;///< Pointer to atom object \b outside the scope of the class.    
    Parameters *parameters_;///< Pointer to a structure outside the scope of the class containing runtime parameters. 

    double *tempListDouble_;///< Double array, its size equals the number of atoms times 3.
    double *direction_;///< Double array, its size equals the number of \b free atoms times 3.
    double *directionOld_;///< Double array, its size equals the number of \b free atoms times 3.
    double *directionNorm_;///< Double array, its size equals the number of \b free atoms times 3.
    double *force_;///< Double array, its size equals the number of \b free atoms times 3.
    double *forceOld_; ///< Double array, its size equals the number of \b free atoms times 3.
    
    /** To initialize the object to perform minimization of Matter objebt.
    @param[in]   *matter        Pointer to the Matter object to be relaxed.
    @param[in]   *parameters    Pointer to the Parameter object containing the runtime parameters.*/
    void initialize(Matter *matter, Parameters *parameters);

    void determineSearchDirection();///< Determine the search direction according to Polak-Ribiere, some of the previous direction is mixed in. 

    /** Determine the size step to be performed.
    @param[in]   *forceBeforeStep  Double array, the forces before ConjugateGradients::makeInfinitisimalStepModifiedForces was called.
    @param[in]   *forceAfterStep   Double array, the forces returned by ConjugateGradients::makeInfinitisimalStepModifiedForces.
    @param[in]   maxStep           Double the maximal accepted step. The maximal value of norm of the displacement.*/
    double stepSize(double *forceBeforeStep, double *forceAfterStep, double maxStep);

    double sign(double value){ return(-(value<0)*2+1);};///< Determine the sign of \a value, \return {double being either 1 or -1}.
};

#endif