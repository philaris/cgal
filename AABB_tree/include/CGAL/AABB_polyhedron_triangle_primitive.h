// Copyright (c) 2009 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
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
// Author(s)     : Stéphane Tayeb, Pierre Alliez
//

#ifndef CGAL_AABB_POLYHEDRON_TRIANGLE_PRIMITIVE_H_
#define CGAL_AABB_POLYHEDRON_TRIANGLE_PRIMITIVE_H_

#define CGAL_DEPRECATED_HEADER "<CGAL/AABB_polyhedron_triangle_primitive.h>"
#define CGAL_REPLACEMENT_HEADER "<CGAL/AABB_FaceGraph_triangle_primitive.h>"
#include <CGAL/internal/deprecation_warning.h>

namespace CGAL {
    /// \ingroup PkgAABB_tree
    /// \deprecated This class is deprecated since \cgal 4.3, the class
    /// `AABB_HalfedgeGraph_triangle_primitive` should be used instead.
    ///
    /// Primitive type that wraps a facet handle of a polyhedron,
    /// which is used as id, and allows the construction of the datum on
    /// the fly. Since only the facet handle is stored in this primitive,
    /// the polyhedron from which the AABB tree is built should not be
    /// deleted while the AABB tree is in use.
    ///
    /// \cgalModels `AABBPrimitive`
    /// \tparam GeomTraits must provides a \c %Point_3
    /// type, used as \c Point, and a \c %Triangle_3 type, used as \c
    /// Datum and constructible from three arguments of type \c
    /// Point.
    /// \tparam  Polyhedron must be a
    /// \c CGAL::Polyhedron_3 whose points have type \c Point.
    ///
    /// \sa `AABBPrimitive`
    /// \sa `AABB_polyhedron_segment_primitive`
    template<typename GeomTraits, typename Polyhedron>
    class AABB_polyhedron_triangle_primitive
    {
    public:
        typedef typename GeomTraits::Point_3 Point;
        /// \name Types
        /// @{

        /// Id type.
        typedef typename Polyhedron::Facet_handle Id;
        /// Geometric data type.
        typedef typename GeomTraits::Triangle_3 Datum;

        /// @}

        // Self
        typedef AABB_polyhedron_triangle_primitive<GeomTraits, Polyhedron> Self;

        // Constructors
        AABB_polyhedron_triangle_primitive() {}
        AABB_polyhedron_triangle_primitive(const AABB_polyhedron_triangle_primitive& primitive)
        {
            m_facet_handle = primitive.id();
        }
        AABB_polyhedron_triangle_primitive(const Id& handle)
            : m_facet_handle(handle)  { };
        AABB_polyhedron_triangle_primitive(const Id* ptr)
            : m_facet_handle(*ptr)  { };
        template <class Iterator>
        AABB_polyhedron_triangle_primitive( Iterator it,
                                            typename boost::enable_if<
                                                       boost::is_same<Id,typename Iterator::value_type>
                                            >::type* =0
        ) : m_facet_handle(*it)  { }


        // Default destructor, copy constructor and assignment operator are ok

        // Returns by constructing on the fly the geometric datum wrapped by the primitive
        Datum datum() const
        {
          const Point& a = m_facet_handle->halfedge()->vertex()->point();
          const Point& b = m_facet_handle->halfedge()->next()->vertex()->point();
          const Point& c = m_facet_handle->halfedge()->next()->next()->vertex()->point();
          return Datum(a,b,c);
        }

        // Returns a point on the primitive
        Point reference_point() const
        {
          return m_facet_handle->halfedge()->vertex()->point();
        }

        // Returns the identifier
        const Id& id() const { return m_facet_handle; }
        Id& id() { return m_facet_handle; }

    private:
        /// The id, here a polyhedron facet handle
        Id m_facet_handle;
    };  // end class AABB_polyhedron_triangle_primitive

    template<typename GeomTraits, typename Polyhedron>
    class AABB_const_polyhedron_triangle_primitive
    {
    public:
        /// AABBPrimitive types
        typedef typename GeomTraits::Point_3 Point;
        typedef typename GeomTraits::Triangle_3 Datum;
        typedef typename Polyhedron::Facet_const_handle Id;

        /// Constructors
        AABB_const_polyhedron_triangle_primitive(const Id& handle)
            : m_facet_handle(handle)  { };

        // Default destructor, copy constructor and assignment operator are ok

        /// Returns by constructing on the fly the geometric datum wrapped by the primitive
        Datum datum() const
        {
          const Point& a = m_facet_handle->halfedge()->vertex()->point();
          const Point& b = m_facet_handle->halfedge()->next()->vertex()->point();
          const Point& c = m_facet_handle->halfedge()->next()->next()->vertex()->point();
          return Datum(a,b,c);
        }

        /// Returns a point on the primitive
        Point reference_point() const
        {
          return m_facet_handle->halfedge()->vertex()->point();
        }

        /// Returns the identifier
        Id id() const { return m_facet_handle; }

    private:
        /// The id, here a polyhedron facet handle
        Id m_facet_handle;
    };  // end class AABB_polyhedron_triangle_primitive



}  // end namespace CGAL


#endif // CGAL_AABB_POLYHEDRON_TRIANGLE_PRIMITIVE_H_
