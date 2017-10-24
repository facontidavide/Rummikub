#include "base_types.h"


bool isValidCombination(const PieceGroup &group)
{
    return group.size() >=3 && (isValidColorSequence(group) || isValidNumberSequence(group) );
}

bool isValidColorSequence(const PieceGroup &gr)
{
    // using bitwise operation is easy to see that there must be
    // only a single one for each column of the binary representation
    // therefore if all the color are different, the binary operator &
    // should result in 0.
    // note that this test also take into account the JOLLY
   switch(gr.size())
   {
   case 3: return (gr[0].color() & gr[1].color() & gr[2].color()) == 0;
   case 4: return (gr[0].color() & gr[1].color() & gr[2].color() & gr[3].color()) == 0;
   default: return false;
   }
   // dead code...
   return false;
}

bool isValidNumberSequence(const PieceGroup &group)
{
    //TODO
    return false;
}
