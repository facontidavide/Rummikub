#include <random>
#include <algorithm>
#include <iterator>
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

RandomPiecesStack::RandomPiecesStack()
{
    _pieces_stack.reserve( 3*( 13*4 + 1) );

    for(uint8_t j=0; j<3; j++)
    {
        for(uint8_t num=1; num<=13; num++)
        {
            for( Color col: {BLACK, YELLOW, RED, BLUE} )
            {
                _pieces_stack.push_back( Piece(col, num) );
            }
        }
        _pieces_stack.push_back(Jolly);
    }

    // http://en.cppreference.com/w/cpp/algorithm/random_shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle( _pieces_stack.begin(), _pieces_stack.end(), g);
}

Piece RandomPiecesStack::pop_back()
{
    if( _pieces_stack.empty() ) return Piece();
    Piece out = _pieces_stack.back();
    _pieces_stack.pop_back();
    return out;
}

bool RandomPiecesStack::removePiece(Piece p)
{
    for (size_t i=0; i< _pieces_stack.size(); i++)
    {
        if( p == _pieces_stack[i] )
        {
            _pieces_stack.erase( _pieces_stack.begin() + i );
            return true;
        }
    }
    return false;
}