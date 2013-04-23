/***************************************************************************
                          humlicek.h  -  description
                             -------------------
    begin                : Tue Jan 21 2003
    copyright            : (C) 2003 by Roman N Tolchenov,A11,,roman@theory.phys.ucl.ac.uk
    email                : roman@phys.ucl.ac.uk.
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef HUMLICEK_H
#define HUMLICEK_H

#include <complex>

namespace Goblin
{

typedef complex<double> double_complex;
//double humlicek(double w,double dl,double dd);
double humlicek(double x,double y);
double_complex cprob(double x,double y); // complex probability function
double galatryG(double x,double y,double z);
double galatryH(double x,double y,double z,double s);

} // Goblin

#endif
