// --------------------------------------------------------------------------
//
// This file is part of the RemoteLab software package.
//
// Version 1.0 - September 2020
//
//
// The RemoteLab package is free software; you can use it, redistribute it,
// and/or modify it under the terms of the GNU General Public License
// version 3 as published by the Free Software Foundation. The full text
// of the license can be found in the file LICENSE.txt at the top level of
// the package distribution.
//
// Authors:
//		Alessio Perinelli and Leonardo Ricci
//		Department of Physics, University of Trento
//		I-38123 Trento, Italy
//		alessio.perinelli@unitn.it
//		leonardo.ricci@unitn.it
//		nse.physics.unitn.it
//		https://github.com/LeonardoRicci/RemoteLab
//
// --------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int GnuplotInterface(FILE*, const char*, const char*, const char*, std::vector< std::vector<double> >&);
int pclose2(FILE *, pid_t);
FILE * popen2(int &);
