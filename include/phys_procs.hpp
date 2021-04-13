#ifndef PHYS_PROCS_H
#define PHYS_PROCS_H

#include "data_types/iop_particle.hpp"
#include "data_types/iop_vec.hpp"

#include <cmath>
#include <vector>

namespace physproc {
class PhysicsProcess {
public:
  virtual iopdat::Vector3 Force(iopdat::Particle p) = 0;
};

class Gravity : public PhysicsProcess {
private:
  double g = -9.81;

public:
  Gravity() {}
  iopdat::Vector3 Force(iopdat::Particle p) {
    // F = m * g
    return iopdat::Vector3(0, p.mass * g, 0);
  }
};

class TimeStepper {
private:
  std::vector<PhysicsProcess *> physics_list;
  iopdat::Particle p;
  double dt;

public:
  TimeStepper(double time_step_size, double charge, double mass);
  void AddProcess(PhysicsProcess *process);
  void Setup(iopdat::Vector3 position, iopdat::Vector3 Velocity);
  void Step();
  void Print();
  double getX();
  double getY();
  double getZ();
};

class UniformB : public PhysicsProcess {
private:
  iopdat::Vector3 B;

public:
  UniformB(iopdat::Vector3 Field) : B(Field.x, Field.y, Field.z) {}
  iopdat::Vector3 Force(iopdat::Particle p) {
    // F = qV x B
    return iopdat::Vector3(p.charge * (p.velocity.y * B.z - p.velocity.z * B.y),
                           p.charge * (p.velocity.z * B.x - p.velocity.x * B.z),
                           p.charge *
                               (p.velocity.x * B.y - p.velocity.y * B.x));
  }
};

class PointCharge : public PhysicsProcess {
private:
  double Q;
  iopdat::Vector3 origin;

public:
  PointCharge(int Charge, iopdat::Vector3 position) {
    // Q = C*V
    Q = 1.60217646E-19 * Charge;
    origin.x = position.x;
    origin.y = position.y;
    origin.z = position.z;
  }
  iopdat::Vector3 Force(iopdat::Particle p) {
    // F = (Ke * Q * q / r^3) * R
    iopdat::Vector3 R(p.position.x - origin.x, p.position.y - origin.y,
                      p.position.z - origin.z);
    double r = std::sqrt(R.x * R.x + R.y * R.y + R.z * R.z);
    double r3 = r * r * r;
    double F = -8.988E9 * Q * p.charge / r3;
    return iopdat::Vector3(F * R.x, F * R.y, F * R.z);
  }
};

class electrode : public PhysicsProcess {
private:
  std::vector<PointCharge> points;

public:
  electrode(int Charge, double radius, double z) {
    for (int phi = 0; phi < 360; phi += 10) {
      double dx = radius * std::sin((double)phi * M_PI / 180);
      double dy = radius * std::cos((double)phi * M_PI / 180);
      // std::cout<<dx<<"\t"<<dy<<std::endl;
      points.push_back(PointCharge(Charge, iopdat::Vector3(dx, dy, z)));
    }
  }
  iopdat::Vector3 Force(iopdat::Particle p) {
    iopdat::Vector3 Force = {0., 0., 0.};
    for (PointCharge &point : points) {
      Force += point.Force(p);
    }
    // std::cout<<"F"<< Force.x<<"\t"<<Force.y<<std::endl;
    return Force;
  }
};

} // namespace physproc

#endif /* PHYS_PROCS_H */
