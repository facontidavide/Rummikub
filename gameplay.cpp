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

bool GameEngine::play(int player_number, const GameState &prev_state, GameState &new_state)
{
    new_state = prev_state;
    Player& player = new_state.players[player_number];

    bool res = dropCombinationsInYourHand(player, new_state.combinations_on_table ); // todo change this
    if (res == false)
    {
       player.ownedPieces().push_back( new_state.piece_stack.pop_back() );
       return false;
    }

    return true;
}

bool GameEngine::dropCombinationsInYourHand(Player &player, std::vector<PieceCombination> &combinations_on_table)
{
    std::vector<Piece> playerHand(player.ownedPieces());
    std::vector<PieceCombination> validCombinations;
    validCombinations.reserve(3); // improbable more than 3 combinations.

    // step1 dropColors --> dropNum
    dropColorCombinations(playerHand, validCombinations);
    dropNumericalCombinations(playerHand, validCombinations);

    int oldCombinationsPieces = 0;
    int oldCombinationsValue = 0;
    for( PieceCombination comb : validCombinations)
    {
        oldCombinationsPieces+=comb.size();
        oldCombinationsValue+=comb.getCombinationSumedValue();
    }

    std::vector<PieceCombination> oldValidCombinations;   // store the convinations cretaed by dropColors --> dropnumerical
    oldValidCombinations = std::move (validCombinations);

    // step2 dropNum --> dropColors
    std::vector<Piece> oldPlayerHand(playerHand);
    playerHand.clear();
    playerHand = player.ownedPieces(); // TODO check this

    dropNumericalCombinations(playerHand, validCombinations);
    dropColorCombinations(playerHand, validCombinations);

    int combinationsPieces = 0;
    int combinationsValue = 0;

    for( PieceCombination comb : validCombinations)
    {
        combinationsPieces+=comb.size();
        combinationsValue+=comb.getCombinationSumedValue();
    }

    // select the best combination

    if(player.isFirstPlay()) // see if any combinantion reaches 30 points
    {
        bool OldcombinationIsValid = false;
        bool combinationIsValid = false;

        if (oldCombinationsValue>=30)
            OldcombinationIsValid = true;
        if (combinationIsValid>=30)
            combinationIsValid = true;
        if (combinationIsValid && OldcombinationIsValid) // both valid select the best one
        {

            (oldCombinationsPieces > combinationsPieces) ? ( pushAllCombinations(oldValidCombinations, combinations_on_table)) : (pushAllCombinations(validCombinations, combinations_on_table));
            player._owned_pieces.clear();
            (oldCombinationsPieces > combinationsPieces) ? ( player._owned_pieces = oldPlayerHand ) : (player._owned_pieces = playerHand);
            player._first_play = false; // firts turn is no more  TODo implement setfirtsvalue
            return true;
        }
        else if(combinationIsValid || OldcombinationIsValid)
        {

             (OldcombinationIsValid) ?( pushAllCombinations(oldValidCombinations, combinations_on_table)) : (pushAllCombinations(validCombinations, combinations_on_table));
             player._owned_pieces.clear();
             (OldcombinationIsValid) ?( player._owned_pieces = oldPlayerHand ) : (player._owned_pieces = playerHand);
              player._first_play = false; // firts turn is no more
              return true;
        }
    }
    else
    {
        if(oldCombinationsPieces>0 || combinationsPieces>0)
        {
            (oldCombinationsPieces > combinationsPieces) ? ( pushAllCombinations(oldValidCombinations, combinations_on_table)) : (pushAllCombinations(validCombinations, combinations_on_table));
            player._owned_pieces.clear();
            (oldCombinationsPieces > combinationsPieces) ? ( player._owned_pieces = oldPlayerHand ) : (player._owned_pieces = playerHand);
            return true;
        }
    }

    return false;
}

void dropColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
    int validCombFirstPiece = -1;
    int validCombLastPiece = -1;

    for(int i=0; i< playerHand.size(); i++)
    {
        const auto& currNumber = playerHand[i].number();
        const auto& nextNumber = playerHand[i+1].number();

        if(currNumber == nextNumber) // posible convinantion keep iterating
            if(validCombFirstPiece == -1)
                validCombFirstPiece = i;
            else
                validCombLastPiece = i;
        else if((validCombLastPiece - validCombFirstPiece) >= 2) // valid convination
        {
            PieceCombination validComb;
            for(i=validCombFirstPiece; i <=validCombFirstPiece; i++ )
            {
                validComb.push_back(playerHand[i]);
            }
            playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin() + validCombLastPiece +1); // delete used pieces

            assert(validComb.isValidCombination()); validCombinations.push_back(validComb); //msoler @davide so cool right?

            // playerHand has been shrinked iteratiors no longer valid, reset
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
            i=0;  //start again look for more valid combinations
        }
        else  // not valid convinantion
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
    }
}

void dropNumericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color
    int validCombFirstPiece = -1;
    int validCombLastPiece = -1;

    for(int i=0; i< playerHand.size(); i++)
    {
        const auto& currNumber = playerHand[i].number();
        const auto& nextNumber = playerHand[i+1].number();

        if(currNumber + 1 == nextNumber) // posible convinantion keep iterating
            if(validCombFirstPiece==-1)
                validCombFirstPiece = i;
            else
                validCombLastPiece = i + 1;
        else if((validCombLastPiece - validCombFirstPiece) >= 2) // valid convination
        {
            PieceCombination validComb;
            for(i=validCombFirstPiece; i <=validCombLastPiece; i++ )
            {
                validComb.push_back(playerHand[i]);
            }

            playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin()+ validCombLastPiece + 1); // delete used pieces

            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb); //msoler @davide so cool right?
            validCombinations.push_back(validComb);

            // playerHand has been shrinked iteratiors no longer valid, reset
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
            i=0;  //start again look for more valid combinations
        }
        else  // not valid convinantion
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
    }
}







