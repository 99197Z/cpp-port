#include "utills.h"
#include "vex.h"

using namespace vex;

extern motor_group WingGroup;

enum wingsState { retracted, extended };
wingsState WingsPos = wingsState::retracted;


double ConvertPCTdegC(double percent) {
    //(percent)                           = (deg - 21.0) / (100 - 21.0) * 100;
    //(percent/100)                       = (deg - 21.0) / (100 - 21.0)
    //(percent/100) * (100 - 21.0)        = (deg - 21.0)
    //(percent/100) * (100 - 21.0) + 21.0 = deg

    return ((percent/100)*(100 - 21.0))+21.0;
}

void extendWings() {
    if (WingsPos == wingsState::retracted) {
        WingGroup.spinToPosition(135,rotationUnits::deg,false);
        WingsPos = wingsState::extended;
    }
}

void retractWings() {
    if (WingsPos == wingsState::extended) {
        WingGroup.spinToPosition(0,rotationUnits::deg,false);
        WingsPos = wingsState::retracted;
    }
}

void toggleWings() {
    if (WingsPos == wingsState::retracted) {
        extendWings();
        WingsPos = wingsState::extended;
    } else if (WingsPos == wingsState::extended) {
        retractWings();
        WingsPos = wingsState::retracted;
    }
}