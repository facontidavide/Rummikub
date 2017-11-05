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
    validCombinations.reserve(5); // improbable to have more than 5 combinations.

    // step1 dropColors --> dropNum
    dropColorCombinations(playerHand, validCombinations);
    dropNumericalCombinations(playerHand, validCombinations);
    int availableJollys = dropJollyColorCombinations(playerHand, validCombinations, player);
    if(availableJollys!=0)
        availableJollys = dropJollyNumbericalCombinations(playerHand, validCombinations, player);

    int oldCombinationsPieces = 0;
    int oldCombinationsValue = 0;
    for( PieceCombination comb : validCombinations)
    {
        oldCombinationsPieces+=comb.size();
        oldCombinationsValue+=comb.getCombinationSumedValue();
    }

    std::vector<PieceCombination> oldValidCombinations;   // store the convinations cretaed by step 1 (dropColors --> dropnumerical)
    oldValidCombinations = std::move (validCombinations);
    std::vector<Piece> oldPlayerHand(playerHand);         // store the resulting player hand if step 1 is used.

    // step2 dropNum --> dropColors
    playerHand.clear();
    playerHand = player.ownedPieces();

    dropNumericalCombinations(playerHand, validCombinations);
    dropColorCombinations(playerHand, validCombinations);
    availableJollys = dropJollyNumbericalCombinations(playerHand, validCombinations, player);
    if(availableJollys!=0)
        availableJollys = dropJollyColorCombinations(playerHand, validCombinations, player);

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
        if (combinationsValue>=30)
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
    else if(oldCombinationsPieces>0 || combinationsPieces>0)  // any valid solution, after firts play
    {
        (oldCombinationsPieces > combinationsPieces) ? ( pushAllCombinations(oldValidCombinations, combinations_on_table)) : (pushAllCombinations(validCombinations, combinations_on_table));
        player._owned_pieces.clear();
        (oldCombinationsPieces > combinationsPieces) ? ( player._owned_pieces = oldPlayerHand ) : (player._owned_pieces = playerHand);
        return true;
    }

    return false;
}

void dropColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
    int jollyPieces = countJolly(playerHand);

    int combFirstPiece = 0;
    int combLastPiece = 0;
    bool isValidCombination = false;
    PieceCombination possibleComb, validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        combFirstPiece = i;
        combLastPiece = i + 2;

        possibleComb.clear();
        for(int e=combFirstPiece; e <=combLastPiece; e++ )
        {
            possibleComb.push_back(playerHand[e]);
        }
        uint j = combLastPiece + 1;
        while( possibleComb.isValidColorSequence() && j<playerHand.size() - jollyPieces )
        {
            isValidCombination = true;
            validComb=possibleComb;
            combLastPiece = j;
            possibleComb.push_back(playerHand[j]);
            j++;
        }
        if(!isValidCombination) // corner case if isvalid but j>playerHand.size() - jollyPieces
        {
            if(possibleComb.isValidColorSequence())
            {
                isValidCombination = true;
                validComb=possibleComb;
                combLastPiece++;
            }
        }

        if (isValidCombination)
        {
            validCombinations.push_back(validComb);
            playerHand.erase (playerHand.begin()+combFirstPiece, playerHand.begin()+combLastPiece); // delete used pieces
            isValidCombination = false;
            // playerHand has been shrinked iteratiors no longer valid, start from beggining
            if(playerHand.size()>2)
                i=0;
            else // if there are less than 3 pieces left no valid convination skip
                return;
        }
    }
}


//void dropColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
//{
//    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
//    int validCombFirstPiece = -1;
//    int validCombLastPiece = -1;

//    for(int i=0; i< playerHand.size(); i++)
//    {
//        const auto& currNumber = playerHand[i].number();
//        const auto& nextNumber = playerHand[i+1].number();
//        const auto& currColor = playerHand[i].color();
//        const auto& nextColor = playerHand[i+1].color();

//        if(currNumber == nextNumber && currColor != nextColor) // posible convinantion keep iterating
//            if(validCombFirstPiece == -1)
//                validCombFirstPiece = i;
//            else
//                validCombLastPiece = i+1;
//        else if((validCombLastPiece - validCombFirstPiece) >= 2) // valid convination
//        {
//            PieceCombination validComb;
//            for(i=validCombFirstPiece; i <=validCombFirstPiece; i++ )
//            {
//                validComb.push_back(playerHand[i]);
//            }
//            playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin() + validCombLastPiece +1); // delete used pieces
//            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb); //TODO implement isValidsequenceNumber
//            validCombinations.push_back(validComb);

//            // playerHand has been shrinked iteratiors no longer valid, reset
//            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
//            i=0;  //start again look for more valid combinations
//        }
//        else  // not valid convinantion
//            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
//    }

//}

//void dropNumericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
//{
//    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color
//    int validCombFirstPiece = -1;
//    int validCombLastPiece = -1;

//    for(int i=0; i< playerHand.size(); i++)
//    {
//        const auto& currNumber = playerHand[i].number();
//        const auto& nextNumber = playerHand[i+1].number();

//        if(currNumber + 1 == nextNumber) // posible convinantion keep iterating
//            if(validCombFirstPiece==-1)
//                validCombFirstPiece = i;
//            else
//                validCombLastPiece = i + 1;
//        else if((validCombLastPiece - validCombFirstPiece) >= 2) // valid convination
//        {
//            PieceCombination validComb;
//            for(i=validCombFirstPiece; i <=validCombLastPiece; i++ )
//            {
//                validComb.push_back(playerHand[i]);
//            }

//            playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin()+ validCombLastPiece + 1); // delete used pieces

//            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb); //TODO implement isValidsequenceNumber
//            validCombinations.push_back(validComb);

//            // playerHand has been shrinked iteratiors no longer valid, reset
//            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
//            i=0;  //start again look for more valid combinations
//        }
//        else  // not valid convinantion
//            resetConvinationSearch(validCombFirstPiece, validCombLastPiece);
//    }
//}


void dropNumericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color

    int jollyPieces = countJolly(playerHand);
    int combFirstPiece = 0;
    int combLastPiece = 0;
    bool isValidCombination = false;
    PieceCombination possibleComb, validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        combFirstPiece = i;
        combLastPiece = i + 2;

        possibleComb.clear();
        for(int e=combFirstPiece; e <=combLastPiece; e++ )
        {
            possibleComb.push_back(playerHand[e]);
        }
        uint j = combLastPiece + 1;
        while( possibleComb.isValidNumberSequence() && j<playerHand.size() - jollyPieces )
        {
            isValidCombination = true;
            validComb = possibleComb;
            combLastPiece = j;
            possibleComb.push_back(playerHand[j]);
            j++;
        }

        if (isValidCombination)
        {
            validCombinations.push_back(validComb);
            playerHand.erase (playerHand.begin()+combFirstPiece, playerHand.begin()+combLastPiece); // delete used pieces
            isValidCombination = false;
            // playerHand has been shrinked iteratiors no longer valid, start from beggining
            if(playerHand.size()>2)
                i=0;
            else // if there are less than 3 pieces left no valid convination skip
                return;
        }
    }
}



int dropJollyColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations, const Player &player)
{
    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return 0; // not jolly pieces.

    uint lastNonJollyNumber = playerHand.size() - 1 - jollyPieces ;
    int validCombFirstPiece = -1;
    int validCombFirstNumber =-1;
    int validCombFirstColor =-1;
    int validCombLastPiece = -1;
    int jollyPose= -1;

    for(uint i=lastNonJollyNumber; i > 0; i--)
    {
        const auto& currNumber = playerHand[i].number();
        const auto& currColor = playerHand[i].color();

        if(currNumber ==  validCombFirstNumber && currColor != validCombFirstColor ) // posible convinantion keep iterating
        {
                validCombLastPiece = i;
        }
        else if((validCombFirstPiece - validCombLastPiece ) >= 2 && validCombLastPiece != -1)        // valid convination
        {
            PieceCombination validComb;
            for(i=validCombLastPiece; i <=validCombFirstPiece; i++ )
            {
                validComb.push_back(playerHand[i]);
            }

            //playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin()+ validCombLastPiece + 1);  // delete used pieces
             playerHand.erase (playerHand.begin()+ validCombLastPiece, playerHand.begin()+ validCombFirstPiece + 1);
            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb);  //TODO implement isValidsequenceNumber
            validCombinations.push_back(validComb);

            // playerHand has been shrinked iteratiors no longer valid, reset
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            validCombFirstColor = -1;
            i= playerHand.size() - 1 - jollyPieces; //start again look for more valid combinations
        }
        else if((validCombFirstPiece - validCombLastPiece ) >= 1 && jollyPieces > 0 && validCombLastPiece != -1) // use jolly at the end
        {
            validCombFirstPiece ++;
            jollyPose = validCombFirstPiece;
            jollyPieces--;
            std::rotate( playerHand.begin() + i, playerHand.end()-1, playerHand.end() );   // move the jolly
            i++; // repeat the loop to save it in valid convination
        }
        else  // not valid convinantion
        {
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            validCombFirstPiece = i;
            validCombFirstNumber = currNumber;
            validCombFirstColor = currColor;
        }
    }

    return jollyPieces;
}

int dropJollyNumbericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations, const Player &player)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color

    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return 0; // not jolly pieces.

    uint lastNonJollyNumber = playerHand.size() - 1 - jollyPieces ;
    int validCombFirstPiece = -1;
    int validCombFirstNumber =-1;
    int validCombLastPiece = -1;
    int initialJollyPieces = jollyPieces;
    int jollyPose= -1;

    // step 1  backwards
    for(uint i=lastNonJollyNumber; i > 0; i--)                          // try to make conbinations with jolly
    {
        const auto& currNumber = playerHand[i].number();

        if(currNumber + validCombFirstPiece ==  validCombFirstNumber + i) // posible convinantion keep iterating
        {
                validCombLastPiece = i;
        }
        else if( (currNumber ==  validCombFirstNumber - i -1) && jollyPieces > 0 )   // use jolly in the middle
        {
            jollyPieces--;
            std::rotate( playerHand.begin() + i, playerHand.end()-1, playerHand.end() );   // move the jolly
            validCombFirstPiece ++;       // when inserting the jolly the first piece has been moved back
        }
        else if((validCombFirstPiece - validCombLastPiece ) >= 2 && validCombLastPiece != -1)        // valid convination
        {
            PieceCombination validComb;
            for(i=validCombLastPiece; i <=validCombFirstPiece; i++ )
            {
                validComb.push_back(playerHand[i]);
            }

            playerHand.erase (playerHand.begin()+ validCombLastPiece, playerHand.begin()+ validCombFirstPiece + 1);  // delete used pieces
            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb);  //TODO implement isValidsequenceNumber
            validCombinations.push_back(validComb);

            // playerHand has been shrinked iteratiors no longer valid, reset
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            i= playerHand.size() - 1 - jollyPieces; //start again look for more valid combinations
        }
        else if((validCombFirstPiece - validCombLastPiece ) >= 1 && jollyPieces > 0 && validCombLastPiece != -1) // use jolly at the end
        {
            validCombFirstPiece ++;
            jollyPose = validCombFirstPiece;
            jollyPieces--;
            std::rotate( playerHand.begin() + jollyPose, playerHand.end()-1, playerHand.end() );   // move the jolly
            i++; // repeat the loop
        }
        else  // not valid convinantion
        {
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            validCombFirstPiece = i;
            validCombFirstNumber = currNumber;
        }
    }

    if(player.isFirstPlay())  // just take to highest possible combinations skip forwards iteration.
        return jollyPieces;

    std::vector<PieceCombination> oldValidCombinations;   // store the convinations cretaed by step 1 (dropColors --> dropnumerical)
    oldValidCombinations = std::move (validCombinations);
    std::vector<Piece> oldPlayerHand(playerHand);         // store the resulting player hand if step 1 is used.
    int oldRemainingJollyPieces =jollyPieces;

    //step 2 forwards
    playerHand.clear();
    playerHand = player.ownedPieces();
    jollyPieces = initialJollyPieces;
    lastNonJollyNumber = playerHand.size() - 1 - jollyPieces ;
    resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);

    for(uint i=0 ; i <= lastNonJollyNumber; i++)                          // try to make conbinations with jolly
    {
        const auto& currNumber = playerHand[i].number();

         if(currNumber + i ==  validCombFirstNumber + validCombFirstPiece)  // posible convinantion keep iterating
        {
                validCombLastPiece = i;
        }
        else if( (currNumber ==  validCombFirstNumber + i+1) && jollyPieces > 0 && validCombLastPiece != -1)   // use jolly in the middle
        {
            std::rotate( playerHand.begin() + i, playerHand.end()-1, playerHand.end() );   // move the jolly
            jollyPieces--;
            validCombLastPiece ++;       // when inserting the jolly the last piece has been moved
        }
        else if((validCombLastPiece - validCombFirstPiece) >= 2)        // valid convination
        {
            PieceCombination validComb;
            for(i=validCombFirstPiece; i <=validCombLastPiece; i++ )
            {
                validComb.push_back(playerHand[i]);
            }

            playerHand.erase (playerHand.begin()+validCombFirstPiece, playerHand.begin()+ validCombLastPiece + 1);  // delete used pieces
            //assert(validComb.isValidCombination()); validCombinations.push_back(validComb); //TODO implement isValidsequenceNumber
            validCombinations.push_back(validComb);

            // playerHand has been shrinked iteratiors no longer valid, reset
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            i = 0;
        }
        else if((validCombLastPiece - validCombFirstPiece) >= 1 && jollyPieces > 0 && validCombLastPiece != -1) // use jolly at the end
        {
            validCombLastPiece ++;
            jollyPose = validCombLastPiece;
            jollyPieces--;
            std::rotate( playerHand.begin() + jollyPose, playerHand.end()-1, playerHand.end() );   // move the jolly
            i++; // repeat the loop
        }
        else  // not valid convinantion
        {
            resetConvinationSearch(validCombFirstPiece, validCombLastPiece, validCombFirstNumber);
            validCombFirstPiece = i;
            validCombFirstNumber = currNumber;
        }
    }

    // select the best combination

    int oldCombinationsPieces = 0;
    int oldCombinationsValue = 0;
    for( PieceCombination comb : oldValidCombinations)
    {
        oldCombinationsPieces+=comb.size();
        oldCombinationsValue+=comb.getCombinationSumedValue();
    }

    int combinationsPieces = 0;
    int combinationsValue = 0;
    for( PieceCombination comb : validCombinations)
    {
        combinationsPieces+=comb.size();
        combinationsValue+=comb.getCombinationSumedValue();
    }

    if(existValidJollyConvinations(oldCombinationsPieces, combinationsPieces, oldRemainingJollyPieces, jollyPieces, initialJollyPieces))
    {
        if(oldCombinationsPieces == combinationsPieces)
        {
            if(oldRemainingJollyPieces!=jollyPieces)
            {
                if(oldRemainingJollyPieces > jollyPieces)
                      validCombinations = std::move (oldValidCombinations);
                if (oldRemainingJollyPieces > jollyPieces)
                    playerHand = std::move(oldPlayerHand);
                if (oldRemainingJollyPieces > jollyPieces)
                    jollyPieces = oldRemainingJollyPieces;
                return jollyPieces;
            }
        }
        else if (oldCombinationsPieces > combinationsPieces)
           validCombinations = std::move (oldValidCombinations);
        if (oldCombinationsPieces > combinationsPieces)
            playerHand = std::move(oldPlayerHand);
        if (oldCombinationsPieces > combinationsPieces)
            jollyPieces = oldRemainingJollyPieces;
        return jollyPieces;
    }
    return initialJollyPieces;
}








