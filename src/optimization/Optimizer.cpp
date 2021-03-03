//
// Created by Christopher Jung on 28.02.21.
//

#include "Optimizer.h"

char Optimizer::ID = 0;


ConstantLatticeElement *ConstantLatticeElement::Top = new ConstantLatticeElement(TOP);
ConstantLatticeElement *ConstantLatticeElement::Bottom = new ConstantLatticeElement(BOTTOM);