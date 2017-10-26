#include "gameplay.h"


GameEngine::GameEngine()
{

}

GameState GameEngine::startGame(int num_players)
{
    GameState state;
    state.players.resize(num_players);

    for (Player& player: state.players)
    {
        for(int i=0; i<14; i++)
        {
            player.ownedPieces().push_back( state.piece_stack.pop_back() ) ;
        }
    }
    return state;
}

bool GameEngine::play(int player_number, GameState &state)
{
    Player& player = state.players[0];

    if ( player.isFirstTurn() )
    {
        bool res = dropCombinationsInYourHand(player, state.combinations_on_table );
        if (res == false)
        {
            player.ownedPieces().push_back( state.piece_stack.pop_back() );
            return false;
        }
    }

    // not implemented yet
    return false;
}

bool GameEngine::dropCombinationsInYourHand(Player &player, std::vector<PieceCombination> &combinations_on_table)
{
    // not implemented yet
    return false;
}
