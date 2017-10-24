#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "base_types.h"

TEST_CASE( "Piece type", "[Rummikub]" ) {
    Piece a(BLACK,7);

    CHECK( a.color() == BLACK );
    CHECK( a.number() == 7 );

    Piece b(RED,1);

    CHECK( b.color() == RED );
    CHECK( b.number() == 1 );

    CHECK( a != b );

    // copy
    a = b;
    CHECK( a.color() == RED );
    CHECK( a.number() == 1 );

    // operator ==
    CHECK( a == b );
}

TEST_CASE( "Is Valid Color Sequence", "[Rummikub]" ) {

    PieceGroup gr;
    CHECK( isValidColorSequence(gr) == false); // size == 0

    gr.push_back( Piece(BLACK,2) );
    CHECK( isValidColorSequence(gr) == false); // size == 1

    gr.push_back( Piece(RED,2) );
    CHECK( isValidColorSequence(gr) == false); // size == 2

    gr.push_back( Piece(BLUE,2) );
    CHECK( isValidColorSequence(gr) == true); // size == 3

    gr.push_back( Piece(YELLOW,2) );
    CHECK( isValidColorSequence(gr) == true); // size == 4

    gr.push_back( Piece(YELLOW,2) );
    CHECK( isValidColorSequence(gr) == false); // size == 5
    gr.pop_back();

    gr[3] = Piece( YELLOW, 3);
    CHECK( isValidColorSequence(gr) == false); // colors are different but one number is not 2

    gr.clear();
    gr.push_back( Piece(RED,7) );
    gr.push_back( Piece(YELLOW,7) );
    gr.push_back( Piece(BLACK,7) );
    CHECK( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Piece(RED,6) );
    gr.push_back( Piece(YELLOW,7) );
    gr.push_back( Piece(BLACK,7) );
    CHECK( isValidColorSequence(gr) == false);

    gr.clear();
    gr.push_back( Piece(RED,6) );
    gr.push_back( Piece(YELLOW,6) );
    gr.push_back( Jolly );
    CHECK( isValidColorSequence(gr) == true);
    gr.push_back( Piece(BLACK,6) );
    CHECK( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Jolly );
    gr.push_back( Piece(YELLOW,9) );
    gr.push_back( Jolly );
    CHECK( isValidColorSequence(gr) == true);
    gr.push_back( Piece(YELLOW,9) );
    CHECK( isValidColorSequence(gr) == false);
}

TEST_CASE( "Is Valid Number Sequence", "[Rummikub]" ) {

    PieceGroup gr;
    CHECK( isValidColorSequence(gr) == false); // size == 0

    gr.push_back( Piece(BLACK,5) );
    CHECK( isValidColorSequence(gr) == false); // size == 1

    gr.push_back( Piece(BLACK,6) );
    CHECK( isValidColorSequence(gr) == false); // size == 2

    gr.push_back( Piece(BLACK,7) );
    CHECK( isValidColorSequence(gr) == true); // size == 3

    gr.clear();
    gr.push_back( Piece(YELLOW,1) );
    gr.push_back( Piece(YELLOW,3) );
    gr.push_back( Piece(YELLOW,2) );
    gr.push_back( Piece(YELLOW,4) );
    CHECK( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Piece(RED,12) );
    gr.push_back( Piece(RED,11) );
    gr.push_back( Piece(RED,13) );
    CHECK( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Piece(BLUE,5) );
    gr.push_back( Piece(RED, 6) );
    gr.push_back( Piece(BLUE,4) );
    CHECK( isValidColorSequence(gr) == false); // two blue, one red

    gr.clear();
    gr.push_back( Piece(BLUE,5) );
    gr.push_back( Piece(BLUE,4) );
    gr.push_back( Jolly );
    CHECK( isValidColorSequence(gr) == true);
    gr.push_back( Piece(BLUE,7) );
    CHECK( isValidColorSequence(gr) == true);
    gr.push_back( Piece(BLUE,8) );
    CHECK( isValidColorSequence(gr) == true);

    gr.clear();
    gr.push_back( Jolly );
    gr.push_back( Piece(BLUE,5) );
    gr.push_back( Jolly );
    gr.push_back( Piece(BLUE,3) );
    CHECK( isValidColorSequence(gr) == true);

    gr.push_back( Piece(BLUE,6) );
    CHECK( isValidColorSequence(gr) == true);

    gr.push_back( Piece(BLUE,6) );
    CHECK( isValidColorSequence(gr) == false); // 6 repeated twice

    gr.clear();
    gr.push_back( Piece(BLUE,5) );
    gr.push_back( Piece(BLUE,4) );
    gr.push_back( Piece(BLUE,5) );
    CHECK( isValidColorSequence(gr) == false); // 5 repeated twice

    gr.clear();
    gr.push_back( Piece(BLUE,2) );
    gr.push_back( Piece(BLUE,4) );
    gr.push_back( Piece(BLUE,5) );
    CHECK( isValidColorSequence(gr) == false); // missing 3

    gr.clear();
    gr.push_back( Piece(YELLOW,2) );
    gr.push_back( Piece(YELLOW,4) );
    gr.push_back( Jolly );
    gr.push_back( Piece(YELLOW,7) );
    CHECK( isValidColorSequence(gr) == false); // missing 5 or 6
}
