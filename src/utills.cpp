#include "utills.h"

double ConvertPCTdegC(double percent) {
    //return (percent - 21.0) / (100 - 21.0) * 100;
    return ((percent/100)*(100 - 21.0))+21.0;
}