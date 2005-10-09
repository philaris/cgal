#ifndef CGAL_POLYNOMIAL_INTERNAL_FILTERED_ROOT_MULTIPLICITYR_H
#define CGAL_POLYNOMIAL_INTERNAL_FILTERED_ROOT_MULTIPLICITYR_H

#include <CGAL/Polynomial/basic.h>
#include <CGAL/Polynomial/internal/interval_arithmetic.h>

CGAL_POLYNOMIAL_BEGIN_INTERNAL_NAMESPACE
template <class Kernel, class NTT>
unsigned int filtered_root_multiplicity(const typename Kernel::Function &fh,
			       const NTT &t, const Kernel &k){
  Polynomial_exactness_assertion(k.sign_at_object(fh)(t)==POLYNOMIAL_NS::ZERO);
  //if (k.sign_at_object(fh)(t)!= ::CGAL::ZERO) return 0;
  // need to check if it is an even root
  int interval_deg=1;
  int exact_deg=-1;
  if (0) k.sign_at_object(fh);
  
  POLYNOMIAL_NS::Interval_arithmetic_guard gd;
  
  if (fh.interval_function().is_zero()) return -1;
  //typename POLYNOMIAL_NS::To_interval<NTT> ti;
  
  typename Kernel::Interval_kernel::NT i= k.interval_function_converter_object().nt_converter()(t);
  
  typename Kernel::Interval_kernel::Function cfi= fh.interval_function().derivative();
  typename Kernel::Exact_kernel::Function cfe;
  typename Kernel::Exact_kernel::NT e;
  do {
    typename Kernel::Interval_kernel::NT vali= cfi(i);

    if (vali.sup() < 0 || vali.inf() >0){
      return interval_deg;
    } else if (vali.sup()==0 && vali.inf()==0){
      
    } else {
      gd.set_enabled(false); // Turn off filtering for exact

      // catch up exact and evaluate
      if (exact_deg==-1) {
	cfe= fh.exact_function().derivative();
	exact_deg=1;
	e= k.exact_function_converter_object().nt_converter()(t);
      }
      while (exact_deg < interval_deg){
	cfe= cfe.derivative();
	++exact_deg;
      }
      typename Kernel::Exact_kernel::NT ev= cfe(e);
      if (POLYNOMIAL_NS::sign(ev) == POLYNOMIAL_NS::ZERO) {
	// update interval;

	/*To_interval<typename Kernel::Exact_kernel::Function::NT> ei;
	Polynomial_converter<typename Kernel::Exact_kernel::Function,
	typename Kernel::Interval_kernel::Function, ::CGAL::To_interval<typename Kernel::Exact_kernel::Function::NT> > pei(ei);*/
	
	gd.set_enabled(true); // turn on filtering
	cfi= k.exact_interval_function_converter_object()(cfe);
      } else {
	// filtering is off
	return interval_deg;
      }
    }
    // filtering is on
    cfi=cfi.derivative();
    ++interval_deg;
  } while (true);
}

template <class Kernel>
class Filtered_root_multiplicity {
public:
  
  Filtered_root_multiplicity(){}
  
  Filtered_root_multiplicity(const typename Kernel::Function &fh, Kernel k= Kernel()): h_(fh), kernel_(k){
  }

  ~Filtered_root_multiplicity(){
  }

  typedef unsigned int result_type;
  //typedef Bound_type argument_type;
  typedef typename Kernel::Function::NT argument_type;
  
  template <class NTT>
  result_type operator()(const NTT &t) const {
    return filtered_root_multiplicity(h_, t, kernel_);
  }

protected:
  typename Kernel::Function h_;
  Kernel kernel_;
};

CGAL_POLYNOMIAL_END_INTERNAL_NAMESPACE
#endif
