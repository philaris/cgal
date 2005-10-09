#ifndef CGAL_CARTESIAN_INSTANTANEOUS_KERNEL_H
#define  CGAL_CARTESIAN_INSTANTANEOUS_KERNEL_H
#include <CGAL/KDS/basic.h>
#include <CGAL/KDS/internal/Instantaneous_adaptor.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>
#include <map>
#include <iostream>
#include <CGAL/KDS/Ref_counted.h>
#include <CGAL/KDS/internal/To_static.h>
//#include <CGAL/KDS/Cartesian_static_converter.h>

#define MSA(Pred, pred, d) typedef Instantaneous_adaptor<typename Static_kernel::Pred##_##d, This, Point_2> Pred##_##d;\
Pred##_##d pred##_##d##_object() const {\
typename Static_kernel::Pred##_##d sp= rep_->static_kernel().pred##_##d##_object();\
return Pred##_##d(*this, sp);\
}

#define TSO(name) typedef typename Static_kernel::name name;


CGAL_KDS_BEGIN_NAMESPACE

template <class MovingObjectTable, class StaticKernel> 
class Cartesian_instantaneous_kernel_rep: public Ref_counted<Cartesian_instantaneous_kernel_rep<MovingObjectTable, StaticKernel> > {
public: 
  typedef StaticKernel Static_kernel;
  typedef To_static<typename MovingObjectTable::Data, StaticKernel> Converter;
  typedef MovingObjectTable Object_table;
  typedef typename Converter::Time Time;
  Cartesian_instantaneous_kernel_rep(typename Object_table::Const_pointer mot,
				     Static_kernel sk): skernel_(sk),
							convert_(sk),
							mot_(mot){
  }
  void set_time(const Time &t) const {
    convert_.set_time(t);
    cache_.clear();
  }
  const Time & time() const {
    return convert_.time();
  }
  
  const typename Converter::result_type& static_object(typename Object_table::Key k) const {
    if (cache_.find(k) == cache_.end()){
      cache_.insert(std::pair<typename Object_table::Key, 
		    typename Converter::result_type>(k, convert_(mot_->at(k))));
      //std::cout << "Object " << k << " is " << cache_[k] << std::endl;
    }
    return cache_[k];
  }

  const Static_kernel& static_kernel() const {
    return skernel_;
  }
protected:
  mutable Static_kernel skernel_;
  mutable Converter convert_;
  typename Object_table::Const_pointer mot_;
  mutable std::map<typename Object_table::Key,
		   typename Converter::result_type> cache_;
};

//! A Kernel that makes a snapshot of a Kinetic motion look like a normal Kernel
/*!  Unfortunately, you have to pick a particular sort of object to
  use and pass a MovingObjectTable of that type of object to the
  Kernel. This is due to the need for caching which is hard to do
  efficiently otherwise.
  
  This currently only supports operations involving points. 
*/
template <class MPT, class SK = CGAL::Simple_cartesian<typename MPT::Data::Coordinate::NT> > 
class Cartesian_instantaneous_kernel {
  typedef Cartesian_instantaneous_kernel<MPT, SK> This;
  typedef Cartesian_instantaneous_kernel_rep< MPT, SK>  Rep;
  typedef typename Rep::Pointer  Rep_pointer;
public:
  //! The table of the objects
  typedef MPT Object_table;
  //! The CGAL kernel
  typedef SK Static_kernel;
  //! The type used to represent time.
  typedef typename Object_table::Data::Coordinate::NT Time;
  
  Cartesian_instantaneous_kernel(typename Object_table::Const_pointer mot,
				 const Static_kernel &sk= Static_kernel()):
    rep_(new Rep(mot, sk)) {
  }
  //! To get around the last of non-const geom_traits() function in triangulation
  void set_time(const Time &cur_time) const {
    rep_->set_time(cur_time);
    //Ptr()->cache_.clear();
  }
    
  //! The current time.
  const Time &time() const {
    return rep_->time();
  }

  /*const Static_kernel &static_kernel() const {
    return rep_->skernel_;
    }*/

  //! The CGAL Kernel RT
  typedef typename SK::RT RT;
  //! The CGAL Kernel FT
  typedef typename SK::FT FT;

  typedef typename Object_table::Key Point_1;

  //! 2D point
  /*!  Only one of this or Point_3 really works for a given
    instantiation.
  */
  typedef typename Object_table::Key Point_2;
  //! 3D point
  /*!  Only one of this or Point_2 really works for a given
    instantiation.
  */
  typedef typename Object_table::Key Point_3;

  //! Compute the static object correspinding to a particular kinetic object.
  const typename Rep::Converter::result_type &
  static_object(const typename MPT::Key &k) const {
    return rep_->static_object(k);
  } 


  typedef Instantaneous_adaptor<std::less<RT>, This, Point_1> Less_x_1;
  Less_x_1 less_x_1_object() const {
    typename std::less<RT> sp;
    return Less_x_1(*this, sp);
  }


  MSA(Side_of_oriented_circle,side_of_oriented_circle, 2);
  MSA(Orientation,orientation, 2);
  MSA(Compare_x, compare_x, 2);
  MSA(Compare_y,compare_y, 2);
  MSA(Less_x, less_x, 2);
  MSA(Less_y, less_y, 2); 
  TSO(Segment_2);
  TSO(Triangle_2);

  MSA(Side_of_oriented_sphere,side_of_oriented_sphere, 3);
  MSA(Orientation,orientation, 3);
  //MSA(Power_test,power_test, 3);
  MSA(Compare_x,compare_x, 3);
  MSA(Compare_y,compare_y, 3);
  MSA(Compare_z,compare_z, 3);
  MSA(Compare_xyz,compare_xyz, 3);
  MSA(Less_x, less_x, 3);
  MSA(Less_y, less_y, 3); 
  MSA(Less_z, less_z, 3); 
  MSA(Coplanar_orientation, coplanar_orientation, 3); 
  MSA(Coplanar_side_of_bounded_circle, coplanar_side_of_bounded_circle, 3); 
  TSO(Segment_3);
  TSO(Triangle_3);
  TSO(Tetrahedron_3);
  TSO(Line_3);
  TSO(Ray_3);
  TSO(Object_3);
  TSO(Plane_3);
protected:
  Rep_pointer rep_;
};
#undef MSA
#undef TSO
CGAL_KDS_END_NAMESPACE
#endif
