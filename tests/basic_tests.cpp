#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "base_types.h"

TEST_CASE( "Piece type", "[Rummikub]" ) {
    Piece a(BLACK,7);

    REQUIRE( a.color() == BLACK );
    REQUIRE( a.number() == 7 );

    Piece b(RED,1);

    REQUIRE( b.color() == RED );
    REQUIRE( b.number() == 1 );

    REQUIRE( a != b );

    // copy
    a = b;
    REQUIRE( a.color() == RED );
    REQUIRE( a.number() == 1 );

    // operator ==
    REQUIRE( a == b );
}

TEST_CASE( "Is Valid Color Sequence", "[Rummikub]" ) {

    PieceGroup gr;
    REQUIRE( isValidColorSequence(gr) == false); // size == 0

    gr.push_back( Piece(BLACK,2) );
    REQUIRE( isValidColorSequence(gr) == false); // size == 1

    gr.push_back( Piece(RED,2) );
    REQUIRE( isValidColorSequence(gr) == false); // size == 2

    gr.push_back( Piece(BLUE,2) );
    REQUIRE( isValidColorSequence(gr) == true); // size == 3

    gr.push_back( Piece(YELLOW,2) );
    REQUIRE( isValidColorSequence(gr) == true); // size == 4

    gr.push_back( Piece(YELLOW,2) );
    REQUIRE( isValidColorSequence(gr) == false); // size == 5
    gr.pop_back();

    gr[3] = Piece( YELLOW, 3);
    REQUIRE( isValidColorSequence(gr) == false); // colors are different but one number is not 2

    gr.clear();
    gr.push_back( Piece(RED,7) );
    gr.push_back( Piece(YELLOW,7) );
    gr.push_back( Piece(BLACK,7) );
    REQUIRE( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Piece(RED,6) );
    gr.push_back( Piece(YELLOW,7) );
    gr.push_back( Piece(BLACK,7) );
    REQUIRE( isValidColorSequence(gr) == false);

    gr.clear();
    gr.push_back( Piece(RED,6) );
    gr.push_back( Piece(YELLOW,6) );
    gr.push_back( Jolly );
    REQUIRE( isValidColorSequence(gr) == true);
    gr.push_back( Piece(BLACK,6) );
    REQUIRE( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Jolly );
    gr.push_back( Piece(YELLOW,9) );
    gr.push_back( Jolly );
    REQUIRE( isValidColorSequence(gr) == true);
}
