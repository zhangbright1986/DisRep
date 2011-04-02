//-----------------------------------------------------------------------------------
// eOn is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// A copy of the GNU General Public License is available at
// http://www.gnu.org/licenses/
//-----------------------------------------------------------------------------------

#ifndef NEBJOB_H
#define NEBJOB_H

#include "Matter.h"
#include "Parameters.h"
#include "NudgedElasticBand.h"
#include "Job.h"

class NudgedElasticBandJob : public Job {
    public:
        NudgedElasticBandJob(Parameters *params);
        ~NudgedElasticBandJob(void);
        std::vector<std::string> run(void);

    private:
        int  doNudgedElasticBand();
        void printEndState(int status);
        void saveData(int status);

        std::vector<std::string> returnFiles;

        Parameters *parameters;
        NudgedElasticBand *NudgedElasticBand; 
        Matter *initial;      // initial configuration.
        Matter *saddle;       // configuration used during the saddle point search.
        Matter *displacement; // configuration used during the saddle point search.

        int fCallsSaddle;
};

#endif