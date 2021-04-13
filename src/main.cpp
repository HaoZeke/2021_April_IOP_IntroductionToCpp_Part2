#include <data_types/iop_particle.hpp>
#include <data_types/iop_vec.hpp>
#include <phys_procs.hpp>

#include <iostream>
#include <pybind11/embed.h> // py::scoped_interpreter
#include <pybind11/stl.h>   // bindings from C++ STL containers to Python types

namespace py = pybind11;

int main() {
  physproc::TimeStepper ProtonStepper(1E-9, -1.60217646E-19, 1.67262192369E-27);
  // TimeStepper ElectronStepper(1E-9,-1.60217646E-19, 9.1093837015E-31);
  //   iopdat::Particle proton(  +1.60217646E-19, 1.67262192369E-27);
  ProtonStepper.Setup(iopdat::Vector3(0, 0.01, 0), iopdat::Vector3(0, 10, 20));
  ProtonStepper.AddProcess(new physproc::Gravity());
  ProtonStepper.AddProcess(new physproc::electrode(10, 0.02, 0.02));
  ProtonStepper.AddProcess(new physproc::electrode(10, 0.02, -0.02));
  ProtonStepper.AddProcess(
      new physproc::UniformB(iopdat::Vector3(0, 0, 0.0001)));
  std::vector<double> x, y, z;
  std::cout << "[ x y z ]"
            << "\n";
  for (int i = 0; i < 1E3; i++) {
    ProtonStepper.Step();
    if (i % 7 == 0) {
      ProtonStepper.Print();
      x.push_back(ProtonStepper.getX());
      y.push_back(ProtonStepper.getY());
      z.push_back(ProtonStepper.getZ());
    }
  }
  ProtonStepper.Print();
  py::scoped_interpreter guard{};
  using namespace py::literals;
  py::dict locals = py::dict{
      "x"_a = x,
      "y"_a = y,
      "z"_a = z,
  };
  py::exec(R"(
    from mpl_toolkits.mplot3d import Axes3D
    import matplotlib.pyplot as plt
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(x, y, z, c='r', marker='o')
    ax.set_xlabel('X Axis')
    ax.set_ylabel('Y Axis')
    ax.set_zlabel('Z Axis')
    ax.set_title('Proton Stepper')
    plt.show()
    )",
           py::globals(), locals);
}
