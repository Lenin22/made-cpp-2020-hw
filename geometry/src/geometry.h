#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

class Point {
public:
  Point(double xx, double yy) : x(xx), y(yy) {}
  double distance(const Point& other) const;
public:
  double x, y;
};

bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);

class Line {
public:
  Line(const Point& p1, const Point& p2) {
    angle = (p2.y - p1.y) / (p2.x - p1.x);
    shift = (p2.x*p1.y - p1.x*p2.y) / (p2.x - p1.x);
  }
  Line(double angle_, double shift_) : angle(angle_), shift(shift_) {}
  Line(const Point& p1_, double angle_) : angle(angle_), shift(p1_.y - angle*p1_.x) {}
public:
  double angle;
  double shift;
};
bool operator==(const Line& a, const Line& b);
bool operator!=(const Line& a, const Line& b);

class Shape {
public:
  Shape(std::vector<Point> pts) : points(pts) {}
  Shape() {}
  virtual double perimeter() const = 0;
  virtual bool operator==(const Shape& other) = 0;
  virtual bool operator!=(const Shape& other) = 0;
  virtual double area() const = 0;
  virtual void rotate(Point center, double angle) = 0;
  virtual void reflex(Point center) = 0;
  virtual void reflex(Line axis) = 0;
  virtual void scale(Point center, double coefficient) = 0;

protected:
  std::vector<Point> points;
};

class Polygon : public Shape {
public:
  Polygon(std::vector<Point> pts) : Shape(pts) {}
  Polygon() : Shape() {}
  bool operator==(const Shape & other) override;
  bool operator!=(const Shape & other) override;
  double area() const override;
  void rotate(Point center, double angle) override;
  void scale(Point center, double coefficient) override;
  double perimeter() const override;
  const std::vector<Point> getVertices() const;
  void reflex(Line axis) override;
  void reflex(Point center) override;
};

class Ellipse : public Polygon {
public:
  Ellipse(Point f1, Point f2, double sumdist) : Polygon({ f1, f2 }),
                                                large_axe(sumdist / 2) {}
  double eccentricity() const;
  double perimeter() const override;
  double area() const override;
  Point center() const;
protected:
  double large_axe;
};

class Circle : public Ellipse {
public:
  Circle(Point center, double radius) : Ellipse(center, center, 2 * radius) {}
  double radius() const;
};

class Triangle : public Polygon {
public:
  Triangle(Point pt1, Point pt2, Point pt3) : Polygon({pt1, pt2, pt3}) {}
  Circle circumscribedCircle() const;
  Circle inscribedCircle() const;
  Circle ninePointsCircle() const;
  Line EulerLine() const;
  Point centroid() const;
  Point orthocenter() const;
  bool isInside(Point p) const;
};

class Rectangle : public Polygon {
public:
  Rectangle(Point pt1, Point pt2, double term);
  Point center() const;
  std::pair<Line, Line> diagonals() const;
};

class Square : public Rectangle {
public:
  Square(Point pt1, Point pt2) : Rectangle(pt1, pt2, 1) {}
  Circle circumscribedCircle();
  Circle inscribedCircle();
};

double Point::distance(const Point & other) const {
  return sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y));
}

bool operator==(const Point & a, const Point & b) {
  return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point & a, const Point & b) {
  return !(a == b);
}

bool operator==(const Line & a, const Line & b) {
  return a.angle == b.angle && a.shift == b.shift;
}

bool operator!=(const Line & a, const Line & b) {
  return !(a == b);
}

bool Polygon::operator==(const Shape & other) {
  if (this->points.size() == static_cast<const Polygon*>(&other)->points.size()) {
    if (this->points.size() == 0) {
      return true;
    }
    std::vector<Point> poly = static_cast<const Polygon*>(&other)->points;
    std::vector<Point> twiced(this->points);
    twiced.insert(twiced.end(), twiced.begin(), twiced.end());

    std::vector<Point>::iterator iter1 = std::search(twiced.begin(),
                                                     twiced.end(),
                                                     poly.begin(),
                                                     poly.end());
    if (iter1 != twiced.end())
      return true;

    std::vector<Point>::reverse_iterator iter2 = std::search(twiced.rbegin(),
                                                             twiced.rend(),
                                                             poly.begin(),
                                                             poly.end());
    if (iter2 != twiced.rend())
      return true;
  }
  return false;
}

bool Polygon::operator!=(const Shape & other) {
  return !(*this == other);
}

double Polygon::area() const {
  double surf = 0.0;
  // shoelace formula
  int j = this->points.size() - 1;
  for (int i = 0; i < this->points.size(); i++) {
    surf += (this->points[j].x + this->points[i].x) *
            (this->points[j].y - this->points[i].y);
    j = i;
  }
  return fabs(surf / 2.0);
}

void Polygon::rotate(Point center, double angle) {
  angle *= M_PI / 180;
  for (Point& pt : this->points) {
    Point vec(pt.x - center.x, pt.y - center.y);
    pt = { center.x + vec.x * cos(angle) - vec.y * sin(angle),
           center.y + vec.x * sin(angle) + vec.y * cos(angle) };
  }
}

void Polygon::scale(Point center, double coefficient) {
  for (Point& pt : this->points) {
    Point vec(pt.x - center.x, pt.y - center.y);
    pt = { center.x + vec.x*coefficient,
          center.y + vec.y*coefficient };
  }
}

double Polygon::perimeter() const {
  double per = 0;
  for (int i = 0; i < this->points.size(); i++) {
    per += this->points[i].distance(this->points[(i+1)% this->points.size()]);
  }
  return per;
}

const std::vector<Point> Polygon::getVertices() const {
  return this->points;
}

void Polygon::reflex(Line axis) {
  for (Point& pt : this->points) {
    double coef = (pt.x + (pt.y - axis.shift)*axis.angle) / (1 + axis.angle*axis.angle);
    pt = { 2 * coef - pt.x,
           2 * coef*axis.angle - pt.y + 2 * axis.shift };
  }
}

void Polygon::reflex(Point center) {
  for (Point& pt : this->points) {
    Point vec(center.x - pt.x, center.y - pt.y);
    pt = { center.x + vec.x, center.y + vec.y };
  }
}

Rectangle::Rectangle(Point pt1, Point pt2, double term) {
  double len = pt2.distance(pt1);
  Point diff((pt2.x - pt1.x) / len, (pt2.y - pt1.y) / len);

  if (term < 1) {
    term = 1 / term;
  }
  double phi = atan(term);
  Point vec = { cos(phi)*diff.x - sin(phi)*diff.y,
                sin(phi)*diff.x + cos(phi)*diff.y };
  this->points = { pt1, {pt1.x + vec.x, pt1.y + vec.y},
                   pt2, {pt2.x - vec.x, pt2.y - vec.y} };
}

Point Rectangle::center() const {
  return {(this->points[0].x + this->points[2].x) / 2,
          (this->points[0].y + this->points[2].y) / 2 };
}

std::pair<Line, Line> Rectangle::diagonals() const {
  return { {this->points[0], this->points[2]},
           {this->points[1], this->points[3]} };
}

double Ellipse::eccentricity() const {
  double f1f2 = 0.5*sqrt(pow(this->points[0].x - this->points[1].x, 2) +
                         pow(this->points[0].y - this->points[1].y, 2));
  return f1f2/large_axe;
}

double Ellipse::perimeter() const {
	double ecc = eccentricity();
	double s =4*large_axe*std::comp_ellint_2(ecc);
  return s;
}

double Ellipse::area() const {
  double ecc = eccentricity();
  double b = large_axe*sqrt(1 - ecc*ecc);
  return M_PI * large_axe * b;
}

Point Ellipse::center() const {
  return{ (this->points[0].x + this->points[1].x) / 2,
          (this->points[0].y + this->points[1].y) / 2 };
}

double Circle::radius() const {
  return this->large_axe;
}

Circle Square::circumscribedCircle() {
  double diag = this->points[0].distance(this->points[2]);
  Point center = { (this->points[0].x + this->points[2].x) / 2,
                   (this->points[0].y + this->points[2].y) / 2 };
  return Circle(center, diag/2);
}

Circle Square::inscribedCircle() {
  double side = this->points[0].distance(this->points[1]);
  Point center = { (this->points[0].x + this->points[2].x) / 2,
                   (this->points[0].y + this->points[2].y) / 2 };
  return Circle(center, side / 2);
}

Circle Triangle::circumscribedCircle() const {
  Point center1 = { (this->points[0].x + this->points[1].x) / 2,
                   (this->points[0].y + this->points[1].y) / 2 };
  double coef1 = (this->points[0].y - this->points[1].y) /
                (this->points[0].x - this->points[1].x);
  coef1 = -1/coef1;
  double b01 = center1.y - coef1*center1.x;

  Point center2 = { (this->points[0].x + this->points[2].x) / 2,
    (this->points[0].y + this->points[2].y) / 2 };
  double coef2 = (this->points[0].y - this->points[2].y) /
    (this->points[0].x - this->points[2].x);
  coef2 = -1/coef2;
  double b02 = center2.y - coef2*center2.x;

  double x = (b02 - b01) / (coef1 - coef2);
  double y = coef1 * x + b01;
  double R = pow(this->points[0].x - x, 2) + pow(this->points[0].y - y, 2);
  R = sqrt(R);
  return Circle({x, y}, R);
}

Circle Triangle::inscribedCircle() const {
  double p = this->perimeter();
  double s = this->area();
  double a = this->points[0].distance(this->points[1]);
  double b = this->points[1].distance(this->points[2]);
  double c = this->points[2].distance(this->points[0]);
  double R = 2 * s / p;

  Point AB(this->points[1].x - this->points[0].x,
           this->points[1].y - this->points[0].y);
  Point AC(this->points[2].x - this->points[0].x,
           this->points[2].y - this->points[0].y);
  double dot = AB.x * AC.x + AB.y * AC.y;
  double phi = acos(dot / (a*c)) / 2;
  double lenAB = sqrt(AB.x*AB.x + AB.y*AB.y);
  double lenBis = fabs(R / sin(phi));
  AB = {AB.x / lenAB * lenBis, AB.y / lenAB * lenBis};
  Point bis = { this->points[0].x + AB.x * cos(phi) - AB.y * sin(phi),
                this->points[0].y + AB.x * sin(phi) + AB.y * cos(phi) };
  if (!isInside(bis)) {
    phi = -phi;
    bis = { this->points[0].x + AB.x * cos(phi) - AB.y * sin(phi),
            this->points[0].y + AB.x * sin(phi) + AB.y * cos(phi) };
  }
  return Circle(bis, R);
}

Circle Triangle::ninePointsCircle() const {
  return Circle({0, 0}, 1);
}

Line Triangle::EulerLine() const {
  return Line(1, 0);
}

Point Triangle::centroid() const {
  double x = 0, y = 0;
  for (const Point& pt : this->points) {
    x += pt.x;
    y += pt.y;
  }
  x /= this->points.size();
  y /= this->points.size();
  return{x, y};
}

Point Triangle::orthocenter() const {
  return Point(0, 0);
}

bool Triangle::isInside(Point p) const {
  const Point& p1 = this->points[0];
  const Point& p2 = this->points[1];
  const Point& p3 = this->points[2];
  double alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
    ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
  double beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
    ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
  double gamma = 1.0f - alpha - beta;
  return (alpha > 0) && (beta > 0) && (gamma > 0);
}

