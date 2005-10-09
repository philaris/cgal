#ifndef CGAL_POLYNOMIAL_INTERNAL_FILTERED_ROOT_MULTIPLICITY_H
#define CGAL_POLYNOMIAL_INTERNAL_FILTERED_ROOT_MULTIPLICITY_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/interval_arithmetic.h>
#include <vector>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE

template <class Kernel>
class Filtered_rational_multiplicity {
  typedef typename Kernel::Function::NT NT;
  typedef typename Kernel::Function Function;
public:
  
  Filtered_rational_multiplicity(){}
  
  Filtered_rational_multiplicity(const Function &fh, const Kernel& k= Kernel()):kernel_(k){
    h_.push_back(fh);
    h_.push_back(k.differentiate_object()(fh));
  }
  

  typedef unsigned int result_type;
  //typedef Bound_type argument_type;
  typedef NT argument_type;
  
  template <class NTT>
  result_type operator()(const NTT &t) const {
    //typename Kernel::Sign_at sa=;
    Polynomial_exactness_assertion( kernel_.sign_at_object(h_[0])(t)==POLYNOMIAL_NS::ZERO);
    //if (kernel_.sign_at_object(fh)(t)!= ::CGAL::ZERO) return 0;
    // need to check if it is an even root
    int deg=1;
    
    
    POLYNOMIAL_NS::Interval_arithmetic_guard gd;
    
    if (h_[0].interval_function().is_zero()) return -1;
    //typename POLYNOMIAL_NS::To_interval<NTT> ti;
    
    typename Kernel::Interval_traits::NT i= kernel_.exact_to_interval_converter_object().nt_converter()(t);
    typename Kernel::Exact_traits::NT e(t);
    do {
      if (h_.size() == static_cast<unsigned int>(deg)) {
	h_.push_back(kernel_.differentiate_object()(h_.back()));
      }
      gd.set_enabled(true); // turn on filtering
      typename Kernel::Interval_traits::NT vali= h_[deg].interval_function()(i);
      gd.set_enabled(false); // turn off filtering
      if (vali.sup() < 0 || vali.inf() >0){
	return deg;
      } else if (vali.sup()==0 && vali.inf()==0){
	
      } else {
	typename Kernel::Exact_traits::NT vale= h_[deg].exact_function()(e);
	// catch up exact and evaluate
	
	if (POLYNOMIAL_NS::sign(vale) == POLYNOMIAL_NS::ZERO) {
	
	} else {
	  // filtering is off
	  return deg;
	}
      }
      ++deg;
    } while (true);
  }

protected:
  Kernel kernel_;
  mutable std::vector<Function> h_;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
