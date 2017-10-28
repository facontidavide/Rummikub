
#include "catch.hpp"
#include "base_types.h"
#include "gameplay.h"

TEST_CASE( "Initial state", "[Rummikub]" ) {

    GameEngine engine;

    GameState state = engine.startGame(2);

    CHECK( state.players.size() == 2);
    for( Player& player: state.players )
    {
        CHECK( player.ownedPieces().size() == 14 );
    }

    CHECK( state.combinations_on_table.size() == 0 );
}

TEST_CASE( "First game", "[Rummikub]" ) {

    GameEngine engine;

    // I will create the state manually, not from the stack of pieces
    GameState state, next_state;

    state.players.resize(1);
    std::vector<Piece>& owned_pieces = state.players[0].ownedPieces();

    owned_pieces.clear();

    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 8});

    // this is a valid solution, but it is not 30
    owned_pieces.push_back( {RED, 2});
    owned_pieces.push_back( {RED, 3});
    owned_pieces.push_back( {RED, 4});

    owned_pieces.push_back( {YELLOW, 1});
    owned_pieces.push_back( {YELLOW, 5});
    owned_pieces.push_back( {YELLOW, 7});
    owned_pieces.push_back( {YELLOW, 13});

    owned_pieces.push_back( {BLUE, 1});
    owned_pieces.push_back( {BLUE, 5});
    owned_pieces.push_back( {BLUE, 11});
    owned_pieces.push_back( {BLUE, 12});

     CHECK( state.players[0].ownedPieces().size() == 14 );

    bool ret = engine.play( 0, state, next_state );

    CHECK( ret == false );
    CHECK( next_state.players[0].ownedPieces().size() == 15 );
    CHECK( next_state.combinations_on_table.size() == 0);

    owned_pieces[11] = {BLUE, 10}; // substitute {BLUE, 5} with {BLUE, 10}

    ret = engine.play( 0, state, next_state );
    CHECK( ret == true );
    CHECK( next_state.players[0].ownedPieces().size() == 8 );
    CHECK( next_state.combinations_on_table.size() == 2);

    //-------------------------------------------------

    state.combinations_on_table.clear();
    owned_pieces.clear();
    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 8});

    owned_pieces.push_back( {RED, 1});
    owned_pieces.push_back( {RED, 3});
    owned_pieces.push_back( {RED, 4});
    owned_pieces.push_back( {RED, 6});

    owned_pieces.push_back( {YELLOW, 1});
    owned_pieces.push_back( {YELLOW, 5});
    owned_pieces.push_back( {YELLOW, 7});
    owned_pieces.push_back( {YELLOW, 13});

    owned_pieces.push_back( Jolly );
    owned_pieces.push_back( {BLUE, 10});
    owned_pieces.push_back( {BLUE, 11});

    ret = engine.play( 0, state, next_state );

    // thanks to the Jolly, multiple combinations are possible, but only one is more than 30
    CHECK( ret == true );
    CHECK( next_state.players[0].ownedPieces().size() == 11 );

    CHECK( next_state.combinations_on_table.size() == 1);
    {
        PieceCombination& comb = next_state.combinations_on_table[0];
        CHECK( comb.isValidNumberSequence(true) == true ); // reorder too
        CHECK( comb[0] == Piece(BLUE, 10) );
        CHECK( comb[1] == Piece(BLUE, 11) );
        CHECK( comb[2] == Jolly );
    }
    //---------------------------------------------
    state.combinations_on_table.clear();
    owned_pieces.clear();
    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 2});
    owned_pieces.push_back( {BLACK, 8});

    owned_pieces.push_back( {RED, 1});
    owned_pieces.push_back( {RED, 3});
    owned_pieces.push_back( {RED, 4});
    owned_pieces.push_back( {RED, 9});

    owned_pieces.push_back( {YELLOW, 1});
    owned_pieces.push_back( {YELLOW, 2});
    owned_pieces.push_back( {YELLOW, 8});
    owned_pieces.push_back( {YELLOW, 9});

    owned_pieces.push_back( {BLUE, 9} );
    owned_pieces.push_back( {BLUE, 10});
    owned_pieces.push_back( {BLUE, 11});

    ret = engine.play( 0, state, next_state );

    // two combination are possible but one is better than the other
    CHECK( ret == true );
    CHECK( next_state.players[0].ownedPieces().size() == 11 );

    CHECK( next_state.combinations_on_table.size() == 1);
    {
        PieceCombination& comb = next_state.combinations_on_table[0];
        CHECK( comb.isValidNumberSequence(true) == true ); // reorder too
        CHECK( comb[0] == Piece(BLUE, 9) );
        CHECK( comb[1] == Piece(BLUE, 10) );
        CHECK( comb[2] == Piece(BLUE, 11) );
    }
}
