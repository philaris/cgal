// Copyright (c) 1997-2001  ETH Zurich (Switzerland).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
// 
//
// Author(s)     : Sven Schoenherr <sven@inf.fu-berlin.de>
//                 Bernd Gaertner <gaertner@inf.ethz.ch>
//                 Franz Wessendorp <fransw@inf.ethz.ch>
//                 Kaspar Fischer <fischerk@inf.ethz.ch>

#ifndef CGAL_QP_PARTIAL_EXACT_PRICING_H
#define CGAL_QP_PARTIAL_EXACT_PRICING_H

// includes
#include <CGAL/QP_solver/QP__partial_base.h>

CGAL_BEGIN_NAMESPACE

// =================
// class declaration
// =================
template < typename Q, typename ET, typename Tags >
class QP_partial_exact_pricing;

// ===============
// class interface
// ===============
template < typename Q, typename ET, typename Tags >
class QP_partial_exact_pricing : public QP__partial_base<Q,ET,Tags> {

    // self
    typedef  QP_pricing_strategy<Q,ET,Tags>       Base;
    typedef  QP__partial_base<Q,ET,Tags>          Partial_base;
    typedef  QP_partial_exact_pricing<Q,ET,Tags>  Self;

    // types from the pricing base class
    typedef  typename Tags::Is_nonnegative           Is_nonnegative;
    typedef  typename Partial_base::Index_iterator        Index_iterator;
    typedef  typename Partial_base::Index_const_iterator  Index_const_iterator;

  public:

    // creation
    QP_partial_exact_pricing( bool     randomize = false,
			       Random&  random    = default_random);

    // operations
    int  pricing(int& direction );
    
    // creation
    ~QP_partial_exact_pricing(){ };

  private:
    int pricing_helper(int& direction, Tag_true  /*is_in_standard_form*/);
    int pricing_helper(int& direction, Tag_false /*is_in_standard_form*/);
};

// ----------------------------------------------------------------------------

// =============================
// class implementation (inline)
// =============================

// construction
template < typename Q, typename ET, typename Tags >  inline
QP_partial_exact_pricing<Q,ET,Tags>::
QP_partial_exact_pricing( bool  randomize, Random&  random)
    : Base( "partial exact"),
      Partial_base( randomize, random)
{ }
    
// operations
template < typename Q, typename ET, typename Tags >
int  QP_partial_exact_pricing<Q,ET,Tags>::
pricing(int& direction )
{
  return (pricing_helper(direction, Is_nonnegative()));
}

template < typename Q, typename ET, typename Tags >
int  QP_partial_exact_pricing<Q,ET,Tags>::
pricing_helper(int& /*direction*/, Tag_true /*is_in_standard_form*/)
{
    Index_const_iterator  it, min_it;
    ET mu, min_mu = this->et0;

    // loop over all active non-basic variables
    CGAL_qpe_debug {
	this->vout() << "active variables:" << std::endl;
    }
    for ( it = this->active_set_begin(); it != this->active_set_end(); ++it) {

        // don't price artificial variables
	if (this->solver().is_artificial( *it) ||
	    this->solver().is_basic( *it))  // added by kf
	  continue;

	// compute mu_j
	mu = mu_j( *it);

	CGAL_qpe_debug {
	    this->vout() << "  mu_" << *it << ": " << mu << std::endl;
	}

	// new minimum?
	if ( mu < min_mu) { min_it = it; min_mu = mu; }
    }

    // no entering variable found so far?
    if ( ( min_mu == this->et0) && ( this->inactive_set_begin() <
                                     this->inactive_set_end())) {

	// loop over all inactive non-basic variables
	CGAL_qpe_debug {
	    this->vout() << "inactive variables:" << std::endl;
	}
	Index_const_iterator  active_it;
	for ( it = this->inactive_set_begin(); it != this->inactive_set_end(); ++it) {

	    // don't price artificial variables
	    if (this->solver().is_artificial( *it)) continue;
	    
	    // compute mu_j
	    mu = mu_j( *it);

	    CGAL_qpe_debug {
		this->vout() << "  mu_" << *it << ": " << mu << std::endl;
	    }

	    // candidate for entering?
	    if ( mu < this->et0) {

		// make variable active
		active_it = it;
		activating( active_it);

		// new minimum?
		if ( mu < min_mu) { min_it = active_it; min_mu = mu; }
	    }
	}
    }
    this->vout() << std::endl;

    // return index of entering variable, if any
    if ( min_mu < this->et0) {
	int  j = *min_it;
	entering_basis( min_it);
	return j;
    }

    // no entering variable found
    return -1;
}
template < typename Q, typename ET, typename Tags >
int  QP_partial_exact_pricing<Q,ET,Tags>::
pricing_helper(int& direction, Tag_false /*is_in_standard_form*/)
{
    Index_const_iterator  it, min_it;
    int                   min_j = -1;
    ET                    mu, min_mu =  this->et0;

    // loop over all active non-basic variables
    CGAL_qpe_debug {
	this->vout() << "active variables:" << std::endl;
    }
    for ( it = this->active_set_begin(); it != this->active_set_end(); ++it) {

        // don't price artificial variables
	if (this->solver().is_artificial( *it) ||
	    this->solver().is_basic( *it))  // added by kf
	  continue;

	// compute mu_j
	mu = this->mu_j( *it);

	if (price_dantzig (*it, mu, this->et0, min_j, min_mu, direction))
	  min_it = it;
    }

    // no entering variable found so far?
    if ( ( min_j == -1) && ( this->inactive_set_begin() <
                             this->inactive_set_end())) 
      {

	// loop over all inactive non-basic variables
	CGAL_qpe_debug {
	    this->vout() << "inactive variables:" << std::endl;
	}
	Index_const_iterator  active_it;
	for ( it = this->inactive_set_begin(); 
	      it != this->inactive_set_end(); ++it) {

	  // don't price basics/artificials
	  CGAL_qpe_assertion (!this->solver().is_basic(*it));
	  if (this->solver().is_artificial( *it)) continue;
	    
	  // compute mu_j
	  mu = mu_j( *it);

	  CGAL_qpe_debug {
	    this->vout() << "  mu_" << *it << ": " << mu << std::endl;
	  }

	  // candidate for entering?
	  if ( is_improving(*it, mu, this->et0)) {

	    // make variable active
	    active_it = it;
	    activating( active_it);

	    // new minimum?
	    if (price_dantzig (*active_it, mu, this->et0, 
			       min_j, min_mu, direction))
	      min_it = active_it;
	  }
	}
      }
    this->vout() << std::endl;

    // return index of entering variable, if any
    if ( min_j >= 0) {
      CGAL_qpe_assertion(min_j == *min_it);
      entering_basis( min_it);
      return min_j;
    }

    // no entering variable found
    return -1;
}

CGAL_END_NAMESPACE

#endif // CGAL_QP_PARTIAL_EXACT_PRICING_H

// ===== EOF ==================================================================
