#include <data_types/iop_vec.hpp>
#include <data_types/iop_particle.hpp>

#include <cmath>
#include <iostream>
#include <vector>


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

class PhysicsProcess
{
    public:
    virtual iopdat::Vector3 Force(iopdat::Particle p) = 0;
};


class Gravity: public PhysicsProcess
{
private:
    double g = -9.81;
public:
    Gravity()
    {

    }
    iopdat::Vector3 Force(iopdat::Particle p)
    {
       // F = m * g
       return iopdat::Vector3( 0, p.mass*g, 0 );
    }
};

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


class TimeStepper
{
private:
    std::vector<PhysicsProcess*> physics_list;
    iopdat::Particle p;
    double dt;
public:
    TimeStepper(double time_step_size, double charge, double mass):
       p(charge,mass)
    {
       dt = time_step_size;
    }
    void AddProcess(PhysicsProcess* process)
    {
       physics_list.push_back(process);
    }
    void Setup(iopdat::Vector3 position, iopdat::Vector3 Velocity)
    {
        p.position = position;
        p.velocity = Velocity;
    }
    void Step()
    {
       iopdat::Vector3 F(0,0,0);
       for (int i =0 ; i< physics_list.size(); i++)
       {
           F += physics_list.at(i)->Force(p);
       }
       //F = m * a
       //F = m * v * dt
       //dv = F / m
       iopdat::Vector3 dv(
           dt * F.x / p.mass, 
           dt * F.y / p.mass, 
           dt * F.z / p.mass
           );
       p.velocity += dv;

       //x = v * dt
       p.position += iopdat::Vector3(
           p.velocity.x * dt,
           p.velocity.y * dt,
           p.velocity.z * dt
           );
    }
    void Print()
    {
        std::cout<<"[ "<<p.position.x<<" "<<p.position.y<<" "<<p.position.z<<" ]\n";
    }
};

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


class UniformB: public PhysicsProcess
{
    private:
    iopdat::Vector3 B;
    public:
    UniformB(iopdat::Vector3 Field):
        B( Field.x,
            Field.y,
            Field.z )
    {
    }
    iopdat::Vector3 Force(iopdat::Particle p)
    {
        //F = qV x B
        return iopdat::Vector3( 
            p.charge*( p.velocity.y*B.z - p.velocity.z*B.y),
            p.charge*( p.velocity.z*B.x - p.velocity.x*B.z),
            p.charge*( p.velocity.x*B.y - p.velocity.y*B.x)
        );
    }
};

class PointCharge: public PhysicsProcess
{
    private:
    double Q;
    iopdat::Vector3 origin;
    public:
    PointCharge(int Charge, iopdat::Vector3 position )
    {
        //Q = C*V
        Q = 1.60217646E-19*Charge;
        origin.x = position.x;
        origin.y = position.y;
        origin.z = position.z;
    }
    iopdat::Vector3 Force(iopdat::Particle p)
    {
        // F = (Ke * Q * q / r^3) * R
        iopdat::Vector3 R(
            p.position.x - origin.x,
            p.position.y - origin.y,
            p.position.z - origin.z
            );
        double r = 
            std::sqrt(R.x*R.x + R.y*R.y + R.z*R.z);
        double r3 = r*r*r;
        double F = -8.988E9*Q*p.charge/r3;
        return iopdat::Vector3(F*R.x,F*R.y,F*R.z);
    }
};

class electrode: public PhysicsProcess
{
    private:
    std::vector<PointCharge> points;
    public:
    electrode(int Charge, double radius, double z)
    {
        for (int phi = 0; phi< 360; phi+=10 )
        {
            double dx=radius*std::sin((double)phi * M_PI / 180);
            double dy=radius*std::cos((double)phi * M_PI / 180);
            //std::cout<<dx<<"\t"<<dy<<std::endl;
            points.push_back(
                PointCharge(
                    Charge,
                    iopdat::Vector3(dx,dy,z)
                    )
                );
        }
    }
    iopdat::Vector3 Force(iopdat::Particle p)
    {
        iopdat::Vector3 Force = {0.,0.,0.};
        for (PointCharge& point: points)
        {
            Force += point.Force(p);
        }
        //std::cout<<"F"<< Force.x<<"\t"<<Force.y<<std::endl;
        return Force;
    }
};





int main()
{
   TimeStepper ProtonStepper(1E-9,-1.60217646E-19, 1.67262192369E-27);
   //TimeStepper ElectronStepper(1E-9,-1.60217646E-19, 9.1093837015E-31);
//   iopdat::Particle proton(  +1.60217646E-19, 1.67262192369E-27);
   ProtonStepper.Setup(iopdat::Vector3(0,0.01,0), iopdat::Vector3(0,10,20));
   ProtonStepper.AddProcess(new Gravity());
   ProtonStepper.AddProcess(new electrode(10,0.02,0.02));
   ProtonStepper.AddProcess(new electrode(10,0.02,-0.02));
   ProtonStepper.AddProcess(new UniformB(iopdat::Vector3(0,0,0.0001)));
   std::cout<<"[ x y z ]"<<"\n";
   for (int i=0; i<5E7; i++)
   {
       ProtonStepper.Step();
       if (i%700 == 0)
          ProtonStepper.Print();
   }
   ProtonStepper.Print();
}
