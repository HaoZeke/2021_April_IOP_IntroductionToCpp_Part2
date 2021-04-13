#include <catch2/catch.hpp>
#include <data_types/iop_particle.hpp>
#include <data_types/iop_vec.hpp>
#include <phys_procs.hpp>

#include <iostream>

TEST_CASE("Basic Vector class tests", "Vector3") {
  iopdat::Vector3 a{1, 2, 3};
  REQUIRE(sizeof(a) == 24);
  REQUIRE(sizeof(a) / sizeof(1) == 6);
  REQUIRE(sizeof(a) / sizeof(1.0) == 3);
  iopdat::Vector3 b{4, 5, 6};
  b += a;
  REQUIRE(b.x == 5);
  REQUIRE(b.y == 7);
  REQUIRE(b.z == 9);
  std::cout << b.x << "\t" << b.y << "\t"
            << "\n";
}

TEST_CASE("Particle printers", "Particle") {
  iopdat::Particle electron(0.1, 0.5);
  std::cout << "electron:";
  electron.Print();
  REQUIRE(electron.charge == 0.1);
  REQUIRE(electron.mass == 0.5);
}
