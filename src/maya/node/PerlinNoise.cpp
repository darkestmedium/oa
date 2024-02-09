#include "PerlinNoise.hpp"




double PerlinNoise::gradient(int hashID, double valX, double valY, double valZ) {
  /* Gradient function.

  Ken's new function to return gradient values based on bit operations.

  Args:
    hashID (int):
    valX (double): X input value
    valY (double): Y input value
    valZ (double): Z input value

  Returns:
    double: Result of hshU plus hshV

  */
  int hshID = hashID & 15;
  double hshU, hshV;

  if (hshID < 8) {
    hshU = valX;
  } else {
    hshV = valZ;
  }

  if (hshID < 4) {
    hshV = valZ;
  } else if (hshID == 12 || hshID == 14) {
    hshV = valX;
  } else {
    hshV = valZ;
  }

  if ((hshID & 1) != 0) {
    hshU = -hshU;
  }
  if ((hshID & 2) != 0) {
    hshV = -hshV;
  }

  return hshU + hshV;
}


double PerlinNoise::gradNoise(double valXYZ) {   
  /* Improved Perlin Noise.

  The X, Y and Z input values are entered as the same input as the 3D noise
  creates much more natural and organic results in comparison to the 1D noise.

  Args:
    valXYZ (double): XYZ input

  Returns:
    double: Interpolated noise output

  */
  double valX = valXYZ;
  double valY = valXYZ;
  double valZ = valXYZ;

  // Get integer lattice values for sample point position
  int intX = (int) floor(valX) & 255;
  int intY = (int) floor(valY) & 255;
  int intZ = (int) floor(valZ) & 255;

  // Fractional part of point position
  valX -= floor(valX);
  valY -= floor(valY);
  valZ -= floor(valZ);

  // Interpolate fractional part of point position
  double valU = fade(valX);
  double valV = fade(valY);
  double valW = fade(valZ);

  // New hash integer lattice cell coords onto perm array
  int A = permutation[intX] + intY;
  int B = permutation[intX + 1] + intY;
  int AA = permutation[A] + intZ;
  int BA = permutation[B] + intZ;
  int AB = permutation[A + 1] + intZ;
  int BB = permutation[B + 1] + intZ;

  // New hash onto gradients
  double gradAA = gradient(permutation[AA], valX, valY, valZ);
  double gradBA = gradient(permutation[BA], valX - 1.0, valY, valZ);
  double gradAB = gradient(permutation[AB], valX, valY - 1.0, valZ);
  double gradBB = gradient(permutation[BB], valX - 1.0, valY - 1.0, valZ);
  double gradAA1 = gradient(permutation[AA + 1], valX, valY, valZ - 1.0);
  double gradBA1 = gradient(permutation[BA + 1], valX - 1.0, valY, valZ - 1.0);
  double gradAB1 = gradient(permutation[AB + 1], valX, valY - 1.0, valZ - 1.0);
  double gradBB1 = gradient(permutation[BB + 1], valX - 1.0, valY - 1.0, valZ - 1.0);

  // Trilinear interpolation of resulting gradients to sample point position
  double firstPassA = lerp(valU, gradAA, gradBA);
  double firstPassB = lerp(valU, gradAB, gradBB);
  double firstPassesCombined = lerp(valV, firstPassA, firstPassB);
  double secondPassA = lerp(valU, gradAA1, gradBA1);
  double secondPassB = lerp(valU, gradAB1, gradBB1);
  double secondPassesCombined = lerp(valV, secondPassA, secondPassB);
  double trilinear = lerp(valW, firstPassesCombined, secondPassesCombined);

  return trilinear;
}


double PerlinNoise::calculateNoise(double weight, double time, double seed, double frequency, double strength, double fractal, double rough) {
  /* Calculates the noise based on the given arguments.

  Combines the base and secondary noise. Takes the nodes input attributes as parameters.

  Args:
    weight (double): Overall weight / multiplier of the noise
    time (double): Time input
    seed (int): Pseudo random initializer
    frequency (double): Base frequency
    strength (double): Base strength
    fractal (double): Secondary noise weight / multiplier
    rough (double): Secondary noise frequency

  Returns:
    double: Combined end result

  */
  double baseNoise = gradNoise(time * (frequency * 0.078) + seed);
  double fractalNoise = gradNoise(time * (2 * (frequency + 0.067)) + seed);
  double combinedNoise = weight * (strength * baseNoise + fractal * ((rough + 0.084) * 3.3 * fractalNoise));

  return combinedNoise;
}
