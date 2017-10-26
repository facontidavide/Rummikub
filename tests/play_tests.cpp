
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
    GameState state;

    state.players.resize(1);
    Player& player = state.players[0];

    player.ownedPieces().clear();

    player.ownedPieces().push_back( {BLACK, 2});
    player.ownedPieces().push_back( {BLACK, 2});
    player.ownedPieces().push_back( {BLACK, 8});

    // this is a valid solution
    player.ownedPieces().push_back( {RED, 2});
    player.ownedPieces().push_back( {RED, 3});
    player.ownedPieces().push_back( {RED, 4});

    player.ownedPieces().push_back( {YELLOW, 1});
    player.ownedPieces().push_back( {YELLOW, 5});
    player.ownedPieces().push_back( {YELLOW, 7});
    player.ownedPieces().push_back( {YELLOW, 13});

    player.ownedPieces().push_back( {BLUE, 1});
    player.ownedPieces().push_back( {BLUE, 5});
    player.ownedPieces().push_back( {BLUE, 9});
    player.ownedPieces().push_back( {BLUE, 11});

    bool ret = engine.play( 0, state );
}
