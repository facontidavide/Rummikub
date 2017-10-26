#include <random>
#include <algorithm>
#include <iterator>
#include "base_types.h"


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


void PieceCombination::sortByNumber()
{
    std::sort( begin(), end(), [](const Piece& a, const Piece& b)
    {
       if( a.number() < b.number() ) return true;
       else{
           if( a.number() == b.number())
           {
               return a.color()< b.color();
           }
       }
       return false;
    }
    );
}

void PieceCombination::sortByColor()
{
    std::sort( begin(), end(), [](const Piece& a, const Piece& b)
    {
       if( a.color() < b.color() ) return true;
       else{
           if( a.color() == b.color())
           {
               return a.number()< b.number();
           }
       }
       return false;
    }
    );
}


bool PieceCombination::isValidColorSequence() const
{
    if( size() <=2 ||  size()>=5 ) return false;

    bool color_found[4] = { false, false, false, false };
    uint8_t first_number = 0;

    for(int i=0; i< size(); i++)
    {
        Piece piece = _piece[i];

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

bool PieceCombination::isValidNumberSequence() const
{
    if( size() <=2 ) return false;

    Color first_color = UNDEFINED;

    // check first the colors
    for(int i=0; i< size(); i++)
    {
        Piece piece = _piece[i];

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
