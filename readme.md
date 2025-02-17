# OCTP Plugin for LAMMPS

On-the-fly Calculation of Transport Properties of Fluids (OCTP) using the order-n algorithm in Equilibrium Molecular Dynamics. More information on how this plugin works and how this plugin can be used with [LAMMPS](https://www.lammps.org/) is thoroughly explained in the work by [Jamali et al. J. Chem. Inf. Model. 2019, 59, 4, 1290-1294](https://pubs.acs.org/doi/10.1021/acs.jcim.8b00939). 

This plugin consists of two new compute commands (position and rdf/ext) and one new fix command (ordern) for LAMMPS. For more information on the use of these new commands as well as a LAMMPS input file for the calcualation of transport properties for a water-methanol mixture, the user is referred to the work of Jamali et al. 

## Compatibility

| OCTP branch | LAMMPS | Status | 
|-------------|--------|--------|
| [master](https://github.com/yiquintero/octp/tree/master) | [stable_23Jun2022_update4](https://github.com/lammps/lammps/releases/tag/stable_23Jun2022_update4) | [![Build and Test](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-stable-release.yml/badge.svg?branch=master)](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-stable-release.yml)
| [master](https://github.com/yiquintero/octp/tree/master) | [release branch](https://github.com/lammps/lammps/tree/release) | [![Build and Test](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-release-branch.yml/badge.svg?branch=master)](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-release-branch.yml)
| [develop](https://github.com/yiquintero/octp/tree/develop) | [stable_23Jun2022_update4](https://github.com/lammps/lammps/releases/tag/stable_23Jun2022_update4) | [![Build and Test](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-stable-release.yml/badge.svg?branch=develop)](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-stable-release.yml)
| [develop](https://github.com/yiquintero/octp/tree/develop) | [release branch](https://github.com/lammps/lammps/tree/release) | [![Build and Test](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-release-branch.yml/badge.svg?branch=develop)](https://github.com/yiquintero/octp/actions/workflows/build-and-test-with-lammps-release-branch.yml)

## Installation

It is not possible to use the OCTP plugin with pre-built LAMMPS binaries or installations. To use any of the OCTP commands, the source code from LAMMPS and the plugin need to be compiled together - i.e. LAMMPS needs to be built from source. On Linux:

```bash
# Clone the OCTP plugin repo and checkout the master branch
git clone https://github.com/yiquintero/octp.git
cd octp
git checkout master
cd ..

# Clone the LAMMPS repo and checkout the latest stable release
# A list of releases can be found in https://github.com/lammps/lammps/releases
git clone https://github.com/lammps/lammps.git
cd lammps
git checkout stable_23Jun2022_update4

# Copy the plugin's source code to the LAMMPS source directory
cp ../octp/src/* src/

# Build LAMMPS
cd src/
make mpi
```

More information on how to build LAMMPS from source can be found in the official [LAMMPS documentation](https://docs.lammps.org/Build.html).

## OCTP Commands

- **compute position** computes a global vector of 5*N components, where N is the total number of atoms in the simulation box. This vector includes the position (x,y,z), the atom ID, and the group mask of each atom. This compute can be used by "fix ordern" to calculate the self-diffusion and Maxwell-Stefan diffusion coefficients of a multicomponent mixture (see J. Chem. Theory Comput., 2018, 14 (5), pp 2667–2677). The components of the mixture should be pre-defined via the command group. The only restriction on the calculation of diffusion coefficients is that no atom should belong to more than 1 group. 

- **compute rdf/ext** is an extension of "compute rdf", which is already available in LAMMPS. This compute calculates the RDF up to sqrt(2)/2 of the box length. In addition, RDFs are corrected due to the finite-size of the simulation box are computed. For the finite-size correction, the method of van der Vegt et al. (see J. Chem. Theory Comput., 2013, 9 (3), pp 1347–1355 and J. Phys. Chem. B, 2018, 122 (21), pp 5515–5526) has been implemented. Similar to "compute position", the mixture should be pre-defined via the command group. The restriction on the calculation of RDFs is that no atom should belong to more than 1 group. This compute command directly generates output files; however, in order to invoke it during the simulation, a fix command (e.g., fix ave/time) should be specified.

- **fix ordern** uses the modified order-n algorithm by Dubbeldam et al. (Mol. Simul., 2009, 35 (12–13), pp 1084–1097) to compute mean-square displacement (MSD) for the self-diffusion coefficient, Maxwell-Stefan diffusion coefficient, shear viscosity, bulk viscosity, and thermal conductivity. Each transport property can be calculated from the slope of the time-MSD plot, where a linear relation between MSD and time is found. As the input to this fix command, the ID of "compute position", "compute pressure", and "compute heat/flux" should be assigned. This fix generates output files containing MSD as a function of time. The definition of reported MSD can be found at pages S14, S17, and S19 of the Supporting Information of the article "OCTP: A Tool for On-the-fly Calculation of Transport Properties of Fluids with the Order-n Algorithm in LAMMPS" (J. Chem. Inf. Model., 2019). It should be noted that the reported MSD must be divided by a factor explicitly mentioned at the top of each output file.

The thermal conductivity computation may produce erroneous results in the case of heat flux when applied to systems with many-body interactions. Read the [Wiki page](https://github.com/yiquintero/octp/wiki/Thermal-Conductivity-Computation) for more information.

## Citation

If this package is used, please cite the following article:

Jamali SH, Wolff L, Becker TM, de Groen M, Ramdin M, Hartkamp R, Bardow A, Vlugt TJH, Moultos OA. OCTP: A Tool for On-the-Fly Calculation of Transport Properties of Fluids with the Order-n Algorithm in LAMMPS. Journal of Chemical Information and Modeling. 2019, 59, 4, 1290-1294 (DOI: 10.1021/acs.jcim.8b00939).