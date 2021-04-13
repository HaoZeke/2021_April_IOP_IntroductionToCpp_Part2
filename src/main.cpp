#include <data_types/iop_vec.hpp>
#include <data_types/iop_particle.hpp>
#include <phys_procs.hpp>

#include <iostream>

// int main()
// {
//    physproc::TimeStepper ProtonStepper(1E-9,-1.60217646E-19, 1.67262192369E-27);
//    //TimeStepper ElectronStepper(1E-9,-1.60217646E-19, 9.1093837015E-31);
// //   iopdat::Particle proton(  +1.60217646E-19, 1.67262192369E-27);
//    ProtonStepper.Setup(iopdat::Vector3(0,0.01,0), iopdat::Vector3(0,10,20));
//    ProtonStepper.AddProcess(new physproc::Gravity());
//    ProtonStepper.AddProcess(new physproc::electrode(10,0.02,0.02));
//    ProtonStepper.AddProcess(new physproc::electrode(10,0.02,-0.02));
//    ProtonStepper.AddProcess(new physproc::UniformB(iopdat::Vector3(0,0,0.0001)));
//    std::cout<<"[ x y z ]"<<"\n";
//    for (int i=0; i<5E7; i++)
//    {
//        ProtonStepper.Step();
//        if (i%700 == 0)
//           ProtonStepper.Print();
//    }
//    ProtonStepper.Print();

// }
#include <pybind11/embed.h>  // py::scoped_interpreter
#include <pybind11/stl.h>    // bindings from C++ STL containers to Python types

namespace py = pybind11;

int main() {
    // Create an exponentially decaying sinusoidal signal as an example
    std::vector<double> signal(1024);
    for (size_t i = 0; i < signal.size(); ++i)
        signal[i] = std::exp(i / -256.0) * std::cos(2 * M_PI * 8 * i / 1024.0);

    // Start the Python interpreter
    py::scoped_interpreter guard{};
    using namespace py::literals;

    // Save the necessary local variables in a Python dict
    py::dict locals = py::dict{
        "signal"_a = signal,
    };

    // Execute Python code, using the variables saved in `locals`
    py::exec(R"(
    
    import matplotlib.pyplot as plt
    plt.plot(signal)
    plt.show()
    
    )",
             py::globals(), locals);
}
