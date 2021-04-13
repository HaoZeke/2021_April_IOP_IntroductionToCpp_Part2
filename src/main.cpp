#include <data_types/iop_vec.hpp>
#include <data_types/iop_particle.hpp>
#include <phys_procs.hpp>

#include <iostream>

/*
int main()
{
    iopdat::Vector3 a{1,2,3};
    return sizeof(a);
    iopdat::Vector3 b{4,5,6};
    b+=a;
    return b.x;

}
*/
//int main()
//{
//    iopdat::Vector3 v(1.,3.,4.);
//    std::cout<< v.x <<"\t"<<v.y<<"\t"<<"\n";
//    v+=iopdat::Vector3(6,7,8);
//    std::cout<< v.x <<"\t"<<v.y<<"\t"<<"\n";
//    std::cout<<sizeof(v.x) << "\t"<<sizeof(v)<<"\n";
//}

//int main()
//{
//    iopdat::Particle electron(0.1,0.5);
//    iopdat::Particle proton(12,34);
//    std::cout<<"electron:";
//    electron.Print();
//    return 0;
//}


//Test gravity force
//int main()
//{
//   iopdat::Particle electron(-1.60217646E-19, 9.1093837015E-31);
//   iopdat::Particle proton(  +1.60217646E-19, 1.67262192369E-27);
//   Gravity g;
//   iopdat::Vector3 F=g.Force(proton);
//   std::cout<< F.x <<"\t"<<F.y<<"\t"<<"\n";
//   // F = m * a;
//   // a = F / m;
//   std::cout << F.y / proton.mass << "\n";
//}


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

int main()
{
   physproc::TimeStepper ProtonStepper(1E-9,-1.60217646E-19, 1.67262192369E-27);
   //TimeStepper ElectronStepper(1E-9,-1.60217646E-19, 9.1093837015E-31);
//   iopdat::Particle proton(  +1.60217646E-19, 1.67262192369E-27);
   ProtonStepper.Setup(iopdat::Vector3(0,0.01,0), iopdat::Vector3(0,10,20));
   ProtonStepper.AddProcess(new physproc::Gravity());
   ProtonStepper.AddProcess(new physproc::electrode(10,0.02,0.02));
   ProtonStepper.AddProcess(new physproc::electrode(10,0.02,-0.02));
   ProtonStepper.AddProcess(new physproc::UniformB(iopdat::Vector3(0,0,0.0001)));
   std::cout<<"[ x y z ]"<<"\n";
   for (int i=0; i<5E7; i++)
   {
       ProtonStepper.Step();
       if (i%700 == 0)
          ProtonStepper.Print();
   }
   ProtonStepper.Print();
}
