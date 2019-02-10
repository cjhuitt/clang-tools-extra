// RUN: %check_clang_tidy %s smells-data-clump %t

double sqrt(double);

struct Point
{
  double x = 0.0;
  double y = 0.0;
};

struct Vector
{
  double x = 0.0;
  double y = 0.0;

  Vector() = default;
  Vector( Point, Point) { }

  double magnitude() const { return sqrt(x*x + y*y); }
  Vector operator-() const { return Vector(); }
};

bool valid(Point) { return true; }

double dist(Point a, Point b)
{
  return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}

bool inBounds(Point pt)
{
  return valid(pt);
}

Point* intersects_line( Point ray_a_origin, Vector ray_a_direction, Point line_begin, Point line_end);

Point* intersects_ray( Point ray_a_origin, Vector ray_a_direction, Point ray_b_origin, Vector ray_b_direction);

Point* intersects_line( Point ray_a_origin, Vector ray_a_direction, Point line_begin, Point line_end)
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: Data clump detected in intersects_line [smells-data-clump]
{
  auto ray_b_direction = Vector( line_begin, line_end );
  auto pt = intersects_ray( ray_a_origin, ray_a_direction, line_begin, ray_b_direction );
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: ray_a_origin and ray_a_direction passed directly to intersects_ray [smells-data-clump]
  if(!pt) return pt;
  return (dist(line_begin, line_end) < ray_b_direction.magnitude()) ? pt : nullptr;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: line_begin and line_end passed directly to dist [smells-data-clump]
}

Point* closeBehind( Point ray_a_origin, Vector ray_a_direction, Point line_begin, Point line_end)
{
  auto ray_b_direction = Vector( line_begin, line_end );
  auto pt = intersects_ray( ray_a_origin, -ray_a_direction, line_begin, ray_b_direction );
  if(!pt) return pt;
  return (dist(line_begin, line_end) < ray_b_direction.magnitude()) ? pt : nullptr;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: line_begin and line_end passed directly to dist [smells-data-clump]
}

Point* intersects_ray( Point, Vector, Point, Vector)
{
  return nullptr;
}
