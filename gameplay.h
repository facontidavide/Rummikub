#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "base_types.h"

class GameEngine;

// any player has a set of Pieces. Whn the size is zero, the player wins
class Player{
public:
    Player(): _first_turn(true) {}

    const std::vector<Piece>& ownedPieces() const { return _owned_pieces; }

    std::vector<Piece>& ownedPieces()             { return _owned_pieces; }

    bool isFirstTurn()                      const { return _first_turn; }
private:

    std::vector<Piece> _owned_pieces;
    // the first time the player play, it must
    // do at least 30 points and can not attach to anything until
    // he did.
    bool _first_turn;

    friend class GameEngine;
};


// This structure should store the entire state of the game.
// It should be sufficient to Load and Save the entire state of the game to file.

struct GameState{
    // combination already dropped on the table
    std::vector<PieceCombination> combinations_on_table;

    //pieces that can be picked by a player
    RandomPiecesStack piece_stack;

    // each player has its own set of pieces
    std::vector<Player> players;
};


bool SaveToFile(const GameState& source_state,const char* filename);

bool LoadFromFile(const char* filename, GameState& destination_state);


// I propose to not store GameState in GameEngine.
// at least for now. It might make it easier to tru different strategies in parallel
// using multithread.
// Any thread should have its own copy of GameState to avoid concurrency problems.
class GameEngine{
public:
    GameEngine(int num_players);

    /// create the initial state.
    GameState startGame();

    /**
     * @brief play   Give a state, a player try to find a solution.
     *               All the intelligence of the algorithm is implemented here.
     *
     * @param player_number    ID of the player. Must be less than state.numPlayers();
     * @param state            It is both the input and oputput of the funtion.
     * @return                 true if the player did something and the state was changed.
     */
    bool play(int player_number, GameState& state);

};

#endif // GAMEPLAY_H
