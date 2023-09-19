/* ----------------------------------------------------------------------
   compute position is a child class of "compute", developed based on the
   "compute msd", provided by LAMMPS.
   This command is distributed under the GNU General Public License.
------------------------------------------------------------------------- */

/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifdef COMPUTE_CLASS
  ComputeStyle(position,ComputePosition)
#else
  #ifndef LMP_COMPUTE_POSITION_H   
      
    #define LMP_COMPUTE_POSITION_H
    #include "compute.h"

    namespace LAMMPS_NS
    {
      class ComputePosition : public Compute 
      {
      public:
        ComputePosition(class LAMMPS *, int, char **);
        virtual ~ComputePosition();
        
        virtual void compute_vector();
        void init();
 
      protected:
        int me;           // this is for the definition of rank of processor in LAMMPS
        int nprocs;       // number of processors
        int *tmprecvcnts; // number of atoms per processor
        int *recvcnts;    // number of atoms per processor
        int *displs;      // displacement array for receiving the correct data set from each processor
        double *sendbuff; // sending array from all processors
      };
    }

  #endif
#endif


/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.  Check the input script syntax and compare to the
documentation for the command. 

*/
