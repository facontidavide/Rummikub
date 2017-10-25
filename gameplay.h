#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "base_types.h"

// any player has a set of Pieces. Whn the size is zero, the player wins
typedef std::vector<Piece> PiecesOwned;

// This structure should store the entire state of the game.
// Should be sufficient to Load and Save the entire state of the game to file.

struct GameState{
    // combination already dropped on the table
    std::vector<PieceCombination> combinations_on_table;

    //pieces that can be picked by a player
    RandomPiecesStack piece_stack;

    // each player has its own set of pieces
    std::vector<PiecesOwned> pieces_by_player;

    size_t numPlayers() const { return pieces_by_player.size(); }
};


bool SaveToFile(const GameState& source_state,const char* filename);

bool LoadFromFile(const char* filename, GameState& destination_state);


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
