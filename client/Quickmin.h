
#ifndef QUICKMIN_H
#define QUICKMIN_H

#include "Optimizer.h"
#include "Matter.h"
#include "Parameters.h"

class Quickmin : public Optimizer
{

    public:
        double dt;

        Quickmin(ObjectiveFunction *objf, Parameters *parameters);
        ~Quickmin();

        bool step(double maxMove);
        bool run(int maxIterations, double maxMove);

    private:
        ObjectiveFunction *objf;
        Parameters *parameters;
        VectorXd velocity;
        int iteration;
};

#endif
