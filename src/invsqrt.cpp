#include <cstdint>

//I did not write this code (source: https://stackoverflow.com/questions/11644441/fast-inverse-square-root-on-x64). It is possible that the target CPU 
//also has this function in its instruction set. in this case using this function is a terrible idea.
double invsqrtQuake( double number )
  {
      double y = number;
      double x2 = y * 0.5;
      std::int64_t i = *(std::int64_t *) &y;
      // The magic number is for doubles is from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
      i = 0x5fe6eb50c7b537a9 - (i >> 1);
      y = *(double *) &i;
      y = y * (1.5 - (x2 * y * y));   // 1st iteration
      //      y  = y * ( 1.5 - ( x2 * y * y ) );   // 2nd iteration, this can be removed
      return y;
  }