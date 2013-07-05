
/*!
\ingroup PkgInterpolation2Concepts
\cgalConcept

\ref PkgInterpolationSibsonGradientFitting are parameterized by a 
traits class that defines the primitives used by the algorithm. The 
concept `GradientFittingTraits` defines this common set of requirements. 

\cgalHasModel `CGAL::Interpolation_gradient_fitting_traits_2<K>` 

\sa `InterpolationTraits` 
\sa `CGAL::Interpolation_traits_2<K>` 
\sa \ref PkgInterpolationSibsonGradientFitting
\sa `CGAL::sibson_c1_interpolation()`
\sa `CGAL::farin_c1_interpolation()`
\sa `CGAL::quadratic_interpolation()` 

*/

class GradientFittingTraits {
public:

/// \name Types 
/// @{

/*! 
The number type must follow the model 
`FieldNumberType`. 
*/ 
typedef unspecified_type FT; 

/*! 
The point type on 
which the function is defined and interpolated. 
*/ 
typedef unspecified_type Point_d; 

/*! 
The corresponding vector type. 
*/ 
typedef unspecified_type Vector_d; 

/*! 
defines a 
matrix type. 
Must provide the following member functions : 

`Aff_transformation tr.inverse ()` which gives the inverse 
transformation, and 

`Aff_transformation tr.transform( Vector v)` which returns the 
multiplication of `tr` with `v`. 

*/ 
typedef unspecified_type Aff_transformation_d; 

/*! 
A constructor object for 
`Vector_d`. 
Provides : 

`Vector_d operator() (Point_d a, Point_d b)` which produces the 
vector `b - a` and 

`Vector_d operator() (Null_vector NULL_VECTOR)` which introduces 
the null vector. 
*/ 
typedef unspecified_type Construct_vector_d; 

/*! 
Constructor object for 
`Vector_d`. 
Provides : 

`Vector_d operator() (Vector_d v,FT scale)` which produces the 
vector `v` scaled by a factor `scale`. 
*/ 
typedef unspecified_type Construct_scaled_vector_d; 

/*! 
Constructor object for 
`Aff_transformation_d`. Provides : 

`Aff_transformation_d operator()()` which introduces an affine 
transformation whose matrix has only zero entries. 
*/ 
typedef unspecified_type Construct_null_matrix_d; 

/*! 
Constructor object for 
`Aff_transformation_d`. Provides : 

`Aff_transformation_d operator()(FT scale)` which introduces a 
scaling by a scale factor `scale`. 
*/ 
typedef unspecified_type Construct_scaling_matrix_d; 

/*! 
Constructor object for 
`Aff_transformation_d`. Provides : 

`Aff_transformation_d operator()(Aff_transformation_d tr1, Aff_transformation_d tr2)` which returns the sum of the two matrices 
representing `tr1` and `tr2`. 
*/ 
typedef unspecified_type Construct_sum_matrix_d; 

/*! 
Constructor object for 
`Aff_transformation_d`. Provides : 

`Aff_transformation_d operator()(Vector v)` which returns the 
outer product, i.e.\ the quadratic matrix `v`\f$ ^t\f$`v`. 
*/ 
typedef unspecified_type Construct_outer_product_d; 

/// @} 

/// \name Creation 
/// @{

/*! 
default constructor. 
*/ 
GradientFittingTraits(); 

/// @} 

/// \name Operations 
/// The following functions that create instances of the above
/// constructor object types must exist.
/// @{

/*! 

*/ 
Construct_vector_d construct_vector_d_object(); 

/*! 

*/ 
Construct_scaled_vector_d 
construct_scaled_vector_d_object(); 

/*! 

*/ 
Construct_null_matrix_d construct_null_matrix_d_object(); 

/*! 

*/ 
Construct_sum_matrix_d 
construct_sum_matrix_d_object(); 

/*! 

*/ 
Construct_outer_product_d 
construct_outer_product_d_object(); 

/// @}

}; /* end GradientFittingTraits */

