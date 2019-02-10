.. title:: clang-tidy - smells-data-clump

smells-data-clump
===================

Finds data clumps inside functions and notes them.

Data clumps can take many forms.  Right now, this checks the following:
* 2 or more parameters of a method are passed unmodified as arguments to another method.

.. code-block:: c++

   // Foo.cpp
   bool valid(Point pt);

   bool inBounds(Point pt)
   {
     // OK: single parameters aren't a clump by definition
     return valid(pt);
   }

   // OK: only warns at definition of function
   std::optional<Point> intersects( Point ray_a_origin, Vector ray_a_direction, Point line_begin, Point line_end);

   std::optional<Point> intersects( Point ray_a_origin, Vector ray_a_direction, Point ray_b_origin, Vector ray_b_direction);

   // Warning: Will point out potential data clump for function:
   std::optional<Point> intersects( Point ray_a_origin, Vector ray_a_direction, Point line_begin, Point line_end)
   {
     auto ray_b_direction = Vector( line_begin, line_end );
     // Warning: Will warn parameters ray_a_origin, ray_a_direction, and line_begin used directly for intersects
     auto pt = intersects( ray_a_origin, ray_a_direction, line_begin, ray_b_direction );
     if(!pt) return pt;
     // Warning: Will warn parameters line_begin and line_end used directly for dist
     return (dist(line_begin, line_end) < ray_b_direction.magnitude()) ? pt : std::optional<Point>{};
   }
   }

