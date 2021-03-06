/*
 * Copyright (C) 1998, 2000-2007, 2010, 2011, 2012, 2013 SINTEF ICT,
 * Applied Mathematics, Norway.
 *
 * Contact information: E-mail: tor.dokken@sintef.no                      
 * SINTEF ICT, Department of Applied Mathematics,                         
 * P.O. Box 124 Blindern,                                                 
 * 0314 Oslo, Norway.                                                     
 *
 * This file is part of GoTools.
 *
 * GoTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. 
 *
 * GoTools is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with GoTools. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public
 * License, a covered work must retain the producer line in every data
 * file that is created or manipulated using GoTools.
 *
 * Other Usage
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the GoTools library without
 * disclosing the source code of your own applications.
 *
 * This file may be used in accordance with the terms contained in a
 * written agreement between you and SINTEF ICT. 
 */

#include "GoTools/intersections/Intersector.h"
#include "GoTools/utils/Point.h"

using std::vector;

namespace Go
{
//===========================================================================
    // Default constructor
SingularityInfo::SingularityInfo()
    : nmb_iter_(0), nmb_success_(0), nmb_simple1_(0), nmb_simple2_(0),
      iteration_done_(false), iteration_succeed_(false), high_pri_type_(NO_SING)
{
}

//===========================================================================
    // Inheritance constructor
SingularityInfo::SingularityInfo(shared_ptr<SingularityInfo> previous,
				 bool use_previous)
    : nmb_iter_(0), nmb_success_(0), nmb_simple1_(0), nmb_simple2_(0),
      iteration_done_(false), iteration_succeed_(false), high_pri_type_(NO_SING)
{
    if (previous.get())
    {
	nmb_iter_ = previous->nmb_iter_;
	nmb_success_ = previous->nmb_success_;
	nmb_simple1_ = previous->nmb_simple1_;
	nmb_simple2_ = previous->nmb_simple2_;
	param_sing_ = previous->param_sing_;
	high_pri_sing_ = previous->high_pri_sing_;
	high_pri_type_ = (previous->high_pri_type_ == INIT_SING) ? DIVIDED_SING :
	    ((previous->high_pri_type_ == KEEP_SING) ? INIT_SING : NO_SING);

	// Iteration at this level is not performed yet
	if (use_previous)
	{
	    iteration_done_ = previous->iteration_done_;
	    iteration_succeed_ = previous->iteration_succeed_;
	}
    }
}

//===========================================================================
    // Inheritance constructor
SingularityInfo::SingularityInfo(shared_ptr<SingularityInfo> previous,
				 int missing_dir)
    : nmb_iter_(0), nmb_success_(0), nmb_simple1_(0), nmb_simple2_(0),
      iteration_done_(false), iteration_succeed_(false), high_pri_type_(NO_SING)
{
    if (previous.get())
    {
	nmb_iter_ = previous->nmb_iter_;
	nmb_success_ = previous->nmb_success_;
	nmb_simple1_ = previous->nmb_simple1_;
	nmb_simple2_ = previous->nmb_simple2_;
	if (previous->param_sing_.size() > 0)
	{
	    param_sing_ = previous->param_sing_;
	    param_sing_.erase(param_sing_.begin()+missing_dir, 
			      param_sing_.begin()+missing_dir+1);
	}

	// Iteration at this level is not performed yet
    }
}

//===========================================================================
double SingularityInfo::getParam(int dir)
{
    if (dir < 0 || dir >= int(param_sing_.size()))
	return -1.e6;  // Not an often used parameter value
    else
	return param_sing_[dir];
}

//===========================================================================
void SingularityInfo::setSingularPoint(double* par, int nmb_par)
{
    int ki;
    
    if (nmb_par != int(param_sing_.size()))
	param_sing_.resize(nmb_par);
    for (ki=0; ki<nmb_par; ki++)
	param_sing_[ki] = par[ki];
    nmb_success_++;
    iteration_succeed_ = true;
    
}

//===========================================================================
void SingularityInfo::setHighPriSing(double* par, int nmb_par)
{
    int ki;
    
    if (nmb_par != int(high_pri_sing_.size()))
	high_pri_sing_.resize(nmb_par);
    for (ki=0; ki<nmb_par; ki++)
	high_pri_sing_[ki] = par[ki];
    high_pri_type_ = INIT_SING;
    
}

//===========================================================================
void SingularityInfo::addSingularPoint(double* par, int nmb_par)
{
    for (int ki=0; ki<nmb_par; ki++)
	param_sing_.push_back(par[ki]);
    iteration_succeed_ = true;
    
}

//===========================================================================
Point SingularityInfo::getPoint(int idx, int nmb_par)
{
    Point curr(param_sing_.begin()+idx*nmb_par, 
	       param_sing_.begin()+(idx+1)*nmb_par);
    return curr;
}

//===========================================================================
void SingularityInfo::cleanUp(vector<double> start,
			      vector<double> end,
			      double tol)
{
    int nmbpar = (int)start.size();
    int nmbpoint = (int)param_sing_.size()/nmbpar;
    for (int ki=0; ki<nmbpoint; ki++)
    {
	int kj;
	for (kj=0; kj<nmbpar; kj++)
	{
	    if (param_sing_[ki*nmbpar+kj] < start[kj]-tol ||
		param_sing_[ki*nmbpar+kj] > end[kj]+tol)
		break;
	}
	if (kj < nmbpar)
	{
	    // Singularity outside of domain. Erase
	    param_sing_.erase(param_sing_.begin()+ki*nmbpar, 
			      param_sing_.begin()+(ki+1)*nmbpar);
	    ki--;
	    nmbpoint--;
	}
    }
}

} // namespace Go

