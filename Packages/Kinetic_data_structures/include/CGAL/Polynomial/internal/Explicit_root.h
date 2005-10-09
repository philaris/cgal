#ifndef CGAL_POLYNOMIAL_EXPLICIT_ROOT_H
#define CGAL_POLYNOMIAL_EXPLICIT_ROOT_H
#include <CGAL/Polynomial/basic.h>


CGAL_POLYNOMIAL_BEGIN_NAMESPACE
//! a root that is represented explicitly
/*!
  Not, the number type must have std::numeric_limits defined and 
  infinity() or max() must be something reasonable. So exact types
  won't work at the moment.
  
  Coef_nt is the type of the polynomial coefficients, which can be disjoint from
  the root storage type, if desired. Not sure if this is useful as I am not sure
  you can get too far with an integer type for the coefficients, and anything
  else can be used to store the roots.
*/
template <class NT >
class Explicit_root {
  typedef Explicit_root<NT> This;
public:

  
  //! Set it to an invalid value
  Explicit_root(): value_(0), is_inf_(true){
#ifndef NDEBUG
    approximation_=to_double();
#endif
  }

  template <class CNT>
  Explicit_root(const CNT &v): is_inf_(false){
    // Protect CORE::Expr from initialization with doubles (that is what this whole class is about anyway).
    if (std::numeric_limits<CNT>::has_infinity && CGAL::abs(v) == std::numeric_limits<CNT>::infinity()){
      is_inf_=true;
      if (v > 0) value_=1;
      else value_=1;
    } else {
      value_= NT(v);
    }
#ifndef NDEBUG
    approximation_=to_double();
#endif
  }

  //! Should be protected
  double to_double() const {
    if (is_inf_) {
      if (CGAL::sign(value_)==CGAL::POSITIVE){
	return infinity<double>();
      } else {
	return -infinity<double>();
      }
    }
    return ::CGAL::to_double(value_);
  }

  bool is_even_multiplicity() const {
    return false;
  }

 
  //! Do not use, should be protected
  std::pair<double, double> to_interval() const {
    if (is_inf_){
      return std::pair<double, double>(to_double(), to_double());
    }
    return ::CGAL::to_interval(value_);
  }

  typedef NT Representation;
  const Representation &representation() const {
    return value_;
  }


  template <class OS>
  void write(OS &out) const {
    if (is_inf_){
      if (CGAL::sign(value_)==CGAL::POSITIVE) out << "inf";
      else out << "-inf";
    } else {
      out << value_;
    }
  }

  std::pair<NT, NT> isolating_interval() const {
    return std::pair<NT, NT>(value_, value_);
  }

  bool operator<(const This &o) const {
    return compare(*this, o)==-1;
  }
  bool operator>(const This &o) const {
    return compare(*this, o) ==1;
  }
  bool operator<=(const This &o) const {
    return compare(*this, o)!= 1;
  }
  bool operator>=(const This &o) const {
    return compare(*this, o) != -1;
  }
  bool operator==(const This &o) const {
    return compare(*this, o) ==0;
  }
  bool operator!=(const This &o) const {
    return compare(*this, o) !=0;
  }
  This operator-() const {
    return This(-value_, is_inf_);
  }

  int multiplicity() const {
    if (is_inf_) return -1;
    else return 1;
  }

  /*void write_type() const {
    std::cout << "general" << std::endl;
    }*/

  NT to_rational() const {
    return value_;
  }
  bool is_rational() const {
    // not really true
    return true;
  }
  static This infinity_rep() {
    return This(1, true);
  }
protected:
 

  static int compare(const This &a, const This &b){
    if (a.is_inf_ && b.is_inf_){
      if (CGAL::sign(a.value_)== CGAL::sign(b.value_)) return 0;
      else if (CGAL::sign(a.value_) == CGAL::POSITIVE ) return 1;
      else return -1;
    } else if (b.is_inf_) return -compare(b, a);
    else if (a.is_inf_){
      CGAL_assertion(!b.is_inf_);
      if (CGAL::sign(a.value_)== CGAL::NEGATIVE) return -1;
      else return 1;
    } else {
      if (a.value_ > b.value_) return 1;
      else if (b.value_ == a.value_) return 0;
      else return -1;
    }
  }
  
  Explicit_root(const NT &nt, bool isinf): value_(nt), is_inf_(isinf){
#ifndef NDEBUG
    approximation_=to_double();
#endif
  }


  NT value_;
  bool is_inf_;
#ifndef NDEBUG
  double approximation_;
#endif
};

#if 0

template <>
class Explicit_root<double>  {
  typedef Explicit_root This;
public:

 
  Explicit_root(): value_(infinity_rep()){}

  template <class NT>
  Explicit_root(const NT &v){
    value_= CGAL::to_double(v);
  }

  double to_double() const {
    return value_;
  }
  
  typedef double Representation;
  const Representation &representation() const {
    return value_;
  }

  std::pair<double, double> to_interval() const {
    return std::pair<double,double>(value_,value_);
  }
 
  void write(std::ostream &out) const {
    out << value_;
  }

  bool is_even_multiplicity() const {
    return false;
  }

  /*
  std::pair<double, double> isolating_interval() const {
    return std::pair<double, double>(value_, value_);
    }*/

  bool operator<(const This &o) const {
    return value_ < o.value_;
  }
  bool operator>(const This &o) const {
    return value_ > o.value_;
  }
  bool operator<=(const This &o) const {
    return value_ <= o.value_;
  }
  bool operator>=(const This &o) const {
    return value_ >= o.value_;
  }
  bool operator==(const This &o) const {
    return value_ == o.value_;
  }
  bool operator!=(const This &o) const {
    return value_ != o.value_;
  }
  This operator-() const {
    return This(-value_);;
  }

  int multiplicity() const {
    if (*this == infinity() || *this == -infinity()) return -1;
    else return 1;
  }

  /*void write_type() const {
    std::cout << "double" << std::endl;
    }*/
  double to_rational() const {
    return value_;
  }
  bool is_rational() const {
    return true;
  }
  static double infinity_rep(){
    if (std::numeric_limits<double>::has_infinity){
      return (std::numeric_limits<double>::infinity());
    } else return (std::numeric_limits<double>::max());
  }
protected:
  double value_;

  
};


#endif

template <class NT>
std::ostream &operator<<(std::ostream &out, const Explicit_root<NT> &r){
  r.write(out);
  return out;
}


/*
template <class NT>
double to_double(const Explicit_root<NT> &r){
  return r.to_double();
}



template <class NT>
std::pair<double, double>  to_interval(const Explicit_root<NT> &r){
  return r.to_interval();
}
*/


CGAL_POLYNOMIAL_END_NAMESPACE


CGAL_BEGIN_NAMESPACE


template <class NT>
double to_double(const POLYNOMIAL_NS::Explicit_root<NT> &r){
  return r.to_double();
}



template <class NT>
std::pair<double, double> to_interval(const POLYNOMIAL_NS::Explicit_root<NT> &r){
  return r.to_interval();
}




CGAL_END_NAMESPACE


namespace std {
  template <class Tr>
  struct numeric_limits<POLYNOMIAL_NS::Explicit_root<Tr> >: public numeric_limits<Tr> {
    typedef numeric_limits<Tr> P;
    typedef POLYNOMIAL_NS::Explicit_root<Tr> T;
    static const bool is_specialized = true;
    static T min() throw() {return T(P::min());}
    static T max() throw() {return T(P::max());}
    /*static const int digits =0;
    static const int digits10 =0;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = true;
    static const int radix =0;
    static T epsilon() throw(){return T(0);}
    static T round_error() throw(){return T(0);}
    static const int min_exponent=0;
    static const int min_exponent10=0;
    static const int max_exponent=0;
    static const int max_exponent10=0;*/
    static const bool has_infinity=true;
    /*static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN= false;
    static const float_denorm_style has_denorm= denorm_absent;
    static const bool has_denorm_loss = false;
    */
    static T infinity() throw() {return T::infinity_rep();}
    /*static T quiet_NaN() throw(){return T(0);}
    static T denorm_min() throw() {return T(0);}
    static const bool is_iec559=false;
    static const bool is_bounded =false;
    static const bool is_modulo= false;
    static const bool traps = false;
    static const bool tinyness_before =false;
    static const float_round_style round_stype = round_toward_zero;*/
  };
};

#endif
