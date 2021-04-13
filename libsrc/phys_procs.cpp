#include <phys_procs.hpp>

physproc::TimeStepper::TimeStepper(double time_step_size, double charge,
                                   double mass)
    : p(charge, mass) {
  dt = time_step_size;
}
void physproc::TimeStepper::AddProcess(PhysicsProcess *process) {
  physics_list.push_back(process);
}
void physproc::TimeStepper::Setup(iopdat::Vector3 position,
                                  iopdat::Vector3 Velocity) {
  p.position = position;
  p.velocity = Velocity;
}
void physproc::TimeStepper::Step() {
  iopdat::Vector3 F(0, 0, 0);
  for (int i = 0; i < physics_list.size(); i++) {
    F += physics_list.at(i)->Force(p);
  }
  // F = m * a
  // F = m * v * dt
  // dv = F / m
  iopdat::Vector3 dv(dt * F.x / p.mass, dt * F.y / p.mass, dt * F.z / p.mass);
  p.velocity += dv;

  // x = v * dt
  p.position +=
      iopdat::Vector3(p.velocity.x * dt, p.velocity.y * dt, p.velocity.z * dt);
}
void physproc::TimeStepper::Print() {
  std::cout << "[ " << p.position.x << " " << p.position.y << " "
            << p.position.z << " ]\n";
}
