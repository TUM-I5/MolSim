import numpy as np
from typing import List

#produce hand calculated values
#for comparison with our implementation


#no need for forces or velocities, just the position 
class Particle:
    def __init__(self, position: np.ndarray):
        self.position = position

    def getPosition(self) -> np.ndarray:
        return self.position
    



p1 = Particle(np.array([2.0,3,4]))
p2 = Particle(np.array([5.0,6,7]))


p3 = Particle(np.array([5.0,3.0,4.5]))
p4 = Particle(np.array([5.0,6.3,7.4]))


def lenn_jones(p1,p2,epsilon=5.0, sigma=1.0):
    x_i = p1.getPosition()
    x_j = p2.getPosition()
    print(x_i)
    print(x_j)

    norm = np.linalg.norm(x_i - x_j) # Calculate L2 norm using NumPy

    print(norm)

    prefactor = (-24 * epsilon) / (norm ** 2)

    prefactor *= (pow(sigma / norm, 6) - 2 * pow(sigma / norm, 12))

    print(x_i - x_j)

    return prefactor * (x_i - x_j)



print("Force between p1 and p2: " + str(lenn_jones(p1,p2)))

print("Force between p1 and p2: " + str(lenn_jones(p3,p4)))

