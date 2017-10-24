#include <iostream>

#include "base_types.h"

using namespace std;

int main()
{
    Piece p(BLACK, 7);
    printf("size: %d\n", static_cast<int>(sizeof(p)) );
    printf("color: 0x%02x\n", static_cast<int>(p.color()) );
    printf("number: %d\n", static_cast<int>(p.number()) );
    return 0;
}
