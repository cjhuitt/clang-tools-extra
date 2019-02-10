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
  Vector( const Point&, const Point&) { }

  double magnitude() const { return sqrt(x*x + y*y); }
  Vector operator-() const { return Vector(); }
};

bool valid(const Point&) { return true; }

double dist(const Point& a, const Point& b)
{
  return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}

bool inBounds(const Point& pt)
{
  return valid(pt);
}

Point* intersects_line( const Point& ray_a_origin, const Vector& ray_a_direction, const Point& line_begin, const Point& line_end);

Point* intersects_ray( const Point& ray_a_origin, const Vector& ray_a_direction, const Point& ray_b_origin, const Vector& ray_b_direction);

Point* intersects_line( const Point& ray_a_origin, const Vector& ray_a_direction, const Point& line_begin, const Point& line_end)
// CHECK-MESSAGES: :[[@LINE-1]]:8: warning: Data clump detected in intersects_line [smells-data-clump]
{
  auto ray_b_direction = Vector( line_begin, line_end );
  auto pt = intersects_ray( ray_a_origin, ray_a_direction, line_begin, ray_b_direction );
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: ray_a_origin, ray_a_direction, and line_begin passed directly to intersects_ray [smells-data-clump]
  if(!pt) return pt;
  return (dist(line_begin, line_end) < ray_b_direction.magnitude()) ? pt : nullptr;
  // CHECK-MESSAGES: :[[@LINE-1]]:11: warning: line_begin and line_end passed directly to dist [smells-data-clump]
}

Point* intersects_ray( const Point&, const Vector&, const Point&, const Vector&)
{
  return nullptr;
}
