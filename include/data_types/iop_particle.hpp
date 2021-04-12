#ifndef IOP_PARTICLE_H
#define IOP_PARTICLE_H

#include "iop_vec.hpp"

#include <iostream>

namespace iopdat {
/**
 * @brief A basic physical object
 */
class Particle {
public:
  Vector3 position; /*!< Positon vector */
  Vector3 velocity; /*!< Velocity vector */
  double charge;    /*!< Fundamental charge */
  double mass;      /*!< Fundamental mass */

public:
  /**
   * @brief Initializer
   * @param c Charge
   * @param m Mass
   */
  Particle(double c, double m) : charge(c), mass(m) {}
  /**
   * @brief Output function
   * @returns None.
   */
  void Print() {
    std::cout << "Charge: " << charge << " Mass: " << mass << "\n";
  }
};
} // namespace iopdat

#endif /* IOP_PARTICLE_H */
