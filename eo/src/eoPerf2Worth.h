/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

   -----------------------------------------------------------------------------
   eoPerf2Worth.h 
   (c) Maarten Keijzer, Marc Schoenauer, 2001
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Contact: todos@geneura.ugr.es, http://geneura.ugr.es
             Marc.Schoenauer@polytechnique.fr
             mkeijzer@dhi.dk
 */
//-----------------------------------------------------------------------------

#ifndef eoPerf2Worth_h
#define eoPerf2Worth_h

#include <utils/eoStat.h>

/** Base class to transform raw fitnesses into fitness for selection

It is an eoStat so
- it is updated inside a checkpoint (i.e. at start of every generation)
- it can be monitored or whatever else you wish through its value()

@see eoSelectFromWorth
*/
template <class EOT, class WorthT = typename EOT::Fitness>
class eoPerf2Worth : public eoStat<EOT, vector<WorthT> >
{
  public:
  eoPerf2Worth(std::string _description = "Worths"):eoStat<EOT, vector<WorthT> >(vector<WorthT>(0),
					      _description) {}
};

#endif