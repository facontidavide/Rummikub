#include "base_types.h"


bool isValidCombination(const PieceGroup &group)
{
    return group.size() >=3 && (isValidColorSequence(group) || isValidNumberSequence(group) );
}

bool isValidColorSequence(const PieceGroup &gr)
{
    if( gr.size() <=2 ||  gr.size()>=5 ) return false;

    bool color_found[4] = { false, false, false, false };
    uint8_t first_number = 0;

    for(const Piece& piece: gr )
    {
        if( piece.isJolly() ) continue;

        const uint8_t index = static_cast<uint8_t>( piece.color() );
        if( color_found[ index ] ){
            return false;
        }
        else{
           color_found[ index ] = true;
        }

        if( first_number == 0){
            first_number = piece.number();
        }
        else{
            if (first_number != piece.number() ) {
                return false;
            }
        }
    }
    return true;
}

bool isValidNumberSequence(const PieceGroup &gr)
{
    if( gr.size() <=2 ) return false;

    Color first_color = UNDEFINED;

    // check first the colors
    for(const Piece& piece: gr )
    {
        if( piece.isJolly() ) continue;

        if( first_color == UNDEFINED){
            first_color = piece.color();
        }
        else{
            if (first_color != piece.color() ) {
                return false;
            }
        }
    }

    //TODO check the numbers
    throw std::runtime_error("NOT implemented yet");
    return false;
}
