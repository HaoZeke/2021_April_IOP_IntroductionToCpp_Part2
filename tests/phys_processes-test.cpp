#include <catch2/catch.hpp>
#include <data_types/iop_particle.hpp>
#include <data_types/iop_vec.hpp>
#include <phys_procs.hpp>

#include <iostream>

TEST_CASE("Gravity Test", "Gravity") {
  iopdat::Particle electron(-1.60217646E-19, 9.1093837015E-31);
  iopdat::Particle proton(+1.60217646E-19, 1.67262192369E-27);
  physproc::Gravity g;
  iopdat::Vector3 F = g.Force(proton);
  REQUIRE(F.x == 0);
}

/*int main()
{
   TimeStepper ElectronStepper(0.001,-1.60217646E-19, 9.1093837015E-31);

   ElectronStepper.Setup(iopdat::Vector3(0,1,2), iopdat::Vector3(0,4,0));
   //TimeStepper ProtonStepper(0.00001,  +1.60217646E-19, 1.67262192369E-27);
   ElectronStepper.AddProcess(new Gravity());
   for (int i=0; i<100000; i++)
   {
       ElectronStepper.Step();
       if (i%1000 == 0)
          ElectronStepper.Print();
   }
}*/
