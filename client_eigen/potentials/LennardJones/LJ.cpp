#include"LJ.h"

LJ::LJ(){
    // Values from Andri
    this->setParameters(7830.0*8.61738573e-5, 2.2*2.47, 2.47);
}

LJ::LJ(double u0Recieved, double cuttOffRRecieved, double psiRecieved){
    this->setParameters(u0Recieved, cuttOffRRecieved, psiRecieved);
    return;
}

void LJ::cleanMemory(void){
    return;
}

// General Functions
void LJ::setParameters(double u0Recieved, double cuttOffRRecieved, double psiRecieved){
    u0 = u0Recieved;
    psi = psiRecieved;
    
    cuttOffR = cuttOffRRecieved;
    cuttOffU = 4*u0*(pow(psi/cuttOffR,12)-pow(psi/cuttOffR,6));
    return;
}

// pointer to number of atoms, pointer to array of positions	
// pointer to array of forces, pointer to internal energy
// adress to supercell size
void LJ::force(long N, const double *R, const long *atomicNrs, double *F, double *U, const double *box){
    double diffR=0, diffRX, diffRY, diffRZ, dU, a, b;
    double *pos;    
    pos = new double[3*N];
    *U = 0;    
    for(int i=0;i<N;i++){
        F[ 3*i ] = 0;
        F[3*i+1] = 0;
        F[3*i+2] = 0;
    }
    for(int i=0; i<3*N; i++)
        pos[i] = R[i];
    // Initializing end
    
    for(int i=0; i<N-1; i++){
        for(int j=i+1; j<N ;j++){
            diffRX = pos[ 3*i ]-pos[ 3*j ];
            diffRY = pos[3*i+1]-pos[3*j+1];
            diffRZ = pos[3*i+2]-pos[3*j+2];

            diffRX = diffRX-box[0]*floor(diffRX/box[0]+0.5); // floor = largest integer value less than argument 
            diffRY = diffRY-box[1]*floor(diffRY/box[1]+0.5);
            diffRZ = diffRZ-box[2]*floor(diffRZ/box[2]+0.5);
            
            diffR = sqrt(diffRX*diffRX+diffRY*diffRY+diffRZ*diffRZ);
                
            if(diffR<cuttOffR){                
                // 4u0((psi/r0)^12-(psi/r0)^6)
                a = pow(psi/diffR,6);
                b = 4*u0*a;
                
                *U = *U+b*(a-1)-cuttOffU;

                dU=-6*b/diffR*(2*a-1);
                //F is the negative derivative
                F[ 3*i ]=F[ 3*i ] - dU*diffRX/diffR;
                F[3*i+1]=F[3*i+1] - dU*diffRY/diffR;
                F[3*i+2]=F[3*i+2] - dU*diffRZ/diffR;
                
                F[ 3*j ]=F[ 3*j ] + dU*diffRX/diffR;
                F[3*j+1]=F[3*j+1] + dU*diffRY/diffR;
                F[3*j+2]=F[3*j+2] + dU*diffRZ/diffR;
            }
        }
    }
    delete [] pos;
    return;
}