#include <random>
#include <algorithm>
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

bool PieceCombination::isValidNumberSequence(bool reorder_jolly )
{
    if( size() <=2 ) return false;

    std::sort( begin(), end(), numberCompare);

    int jolly_count = 0;

    for( int i = size()-1 ; i >=0 && _piece[i].isJolly(); i-- )
    {
        jolly_count++;
    }
    //--------------------------------------
    int expected_number  = _piece[0].number() +1;
    Color expected_color = _piece[0].color();


    for (int i=1; i<size() && !_piece[i].isJolly(); i++ )
    {
        if( _piece[i].color() != expected_color)
        {
            return false;
        }

        if( _piece[i].number() != expected_number)
        {
            //try using a Jolly
            if( jolly_count > 0 )
            {
                jolly_count--;
                if( reorder_jolly )
                {
                    std::rotate( &_piece[i], end()-1, end() );
                }
                else{
                    i--;
                }
            }
            else{ return false; }
        }
        expected_number++;
    }
    return true;
}
