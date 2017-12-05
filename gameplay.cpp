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
    std::vector<PieceCombination> validCombinations[4];

    int combinations_pieces[4] = {0,0,0,0};
    int combinations_value[4] = {0,0,0,0};

    dropColorCombinations(playerHand, validCombinations[0]);
    dropNumericalCombinations(playerHand, validCombinations[1]);
    dropJollyColorCombinations(playerHand, validCombinations[2]);
    dropJollyNumbericalCombinations(playerHand, validCombinations[3]);

    for(int i=0; i<4; i++)
    {
        for( PieceCombination comb : validCombinations[i])
        {
            combinations_pieces[i] += comb.size();
            combinations_value[i] += comb.getCombinationSumedValue();
        }
    }

    // select the best combination

    if(player.isFirstPlay()) // see if any combinantion reaches 30 points
    {
        bool comb_is_valid_0 = (combinations_value[0]>=30);
        //bool comb_is_valid_1 = (combinations_value[1]>=30);


        return true;
    }
    else if(combinations_pieces[0]>0 || combinations_pieces[1]>0)  // any valid solution, after firts play
    {
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
    int number_of_colors=0;
    //bool two_valid_posible_combinations= false; TODO
    PieceCombination validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        combFirstPiece = i;
        const auto& currNumber = playerHand[i].number();
        validComb.clear();
        std::array<uint,4> yrub_counter = {0,0,0,0};
        number_of_colors=0;

        while (playerHand[i].number()==currNumber && i < (playerHand.size() - jollyPieces))
        {
            const uint8_t index = static_cast<uint8_t>( playerHand[i].color() );

            if(yrub_counter[index] == 0) {number_of_colors++;}
            yrub_counter[index]++;

            i++;
        }

        if(number_of_colors<3) continue; // no valid convination skip the rest

        combLastPiece= i;
        //TODO implement two valid convinations
        //        if(combLastPiece-combFirstPiece>5)
        //        {
        //            for(int i=0; i<3; i++)
        //            {
        //                yrub_counter[i]>
        //            }
        //            two_valid_posible_combinations=true; }

        std::array<uint,4> yrub_counter_copy (yrub_counter);
        for(int j=combFirstPiece; j <=combLastPiece; j++ )
        {
            const uint8_t index = static_cast<uint8_t>( playerHand[j].color() );
            if(yrub_counter[index]>=1 && yrub_counter[index]== yrub_counter_copy[index] )
            {
                yrub_counter[index]--;
                validComb.push_back(playerHand[j]);
            }
        }
        assert(validComb.isValidCombination()); validCombinations.push_back(validComb);
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


/* selectValidCombFromCounter
 *
 * I: playerHand, counter, combFirstPiece, numPiecesTofirst, firstPieceCounter, numPiecesToLast
 * O: validComb
 *
 * Example:
 **************************************************************************
 * combFirstPiece     *
 *                    1 3 5 5 5 6 7 8 10 13 ...         playerHand
 * firstValidPiece        *
 * lastValidPiece                   *
 *
 * numberOfPiecesToValid  2
 * firstPieceCounter          *
 *                    0 1 1 0 3 1 1 1 0       counter
 *
 *
 * (firstValidPiece = combFirstPiece + numberOfPiecesToValid)
 ****************************************************************************
 */

void selectValidCombFromCounter(const std::vector<Piece>& playerHand, PieceCombination& validComb,const std::array<uint,13>& counter,
const int combFirstPiece,const int numPiecesTofirst, int firstPieceCounter,const int numPiecesToLast)
{
    int firstValidPiece = combFirstPiece + numPiecesTofirst;
    for(int i=firstValidPiece; i< (combFirstPiece + numPiecesToLast); i++)
    {
        assert(counter[firstPieceCounter] > 0); validComb.push_back(playerHand[i]);
        if(counter[firstPieceCounter]>1)
        {
            i+= (counter[firstPieceCounter] - 1); // skip repeated pieces go to next valid
        }
        firstPieceCounter++;
    }
}


/* TODO implement this
**************************************************************************
* combFirstPiece     *
*                    1 3 5 5 5 7 10 13 ...         playerHand
* firstValidPiece        *
* lastValidPiece                   *
*
* numberOfPiecesToValid  3
* firstPieceCounter          *
*                    0 1 1 0 3 0 1 0 0       counter
*
*
* (firstValidPiece = combFirstPiece + numberOfPiecesToValid)
****************************************************************************
*/
void selectValidCombFromCounterJolly(const std::vector<Piece>& playerHand, PieceCombination& validComb, const std::array<uint,13>& counter,
                const int combFirstPiece, const int numPiecesTofirst, int firstPieceCounter, const int numPiecesToLast, const int jollyPieces)
{
    int firstValidPiece = combFirstPiece + numPiecesTofirst;
    for(int i=firstValidPiece; i< (combFirstPiece + numPiecesToLast); i++)
    {
        assert(counter[firstPieceCounter] > 0); validComb.push_back(playerHand[i]);
        if(counter[firstPieceCounter]>1)
        {
            i+= (counter[firstPieceCounter] - 1); // skip repeated pieces go to next valid
        }
        firstPieceCounter++;
    }
}

void dropNumericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color
    int jollyPieces = countJolly(playerHand);
    int combFirstPiece = 0;
    int valid_numbers=0;
    PieceCombination validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        const auto& currColor = playerHand[i].color();
        std::array<uint,13> counter={0,0,0,0,0,0,0,0,0,0,0,0,0};
        validComb.clear();
        valid_numbers=0;
        combFirstPiece = i;

        while (playerHand[i].color()==currColor && i < (playerHand.size() - jollyPieces))
        {
            const uint8_t index = playerHand[i].number() -1;
            if(counter[index]==0){valid_numbers++;}
            counter[index]++;
            i++;
        }

        if(valid_numbers<3) continue; // no valid convination skip the rest

        int numPiecesTofirst = 0;
        int firstValid=0;
        int last = 0;
        int numPiecesToLast = 0;
        for(int j=0; j<13; j++)
        {
            if(counter[j]==0 || j == 12)
            {
                if( (last - firstValid) >= 3)
                {
                    selectValidCombFromCounter(playerHand, validComb, counter, combFirstPiece, numPiecesTofirst, firstValid, numPiecesToLast);
                    assert(validComb.isValidCombination()); validCombinations.push_back(validComb);
                    validComb.clear();
                }

                last=-1;
                firstValid =-1;
                numPiecesTofirst=numPiecesToLast;
            }
            else if(counter[j]>0)
            {
                if(firstValid==-1)
                {
                    numPiecesTofirst+=counter[j];
                    firstValid=j;
                }
                else{ last=j;}
                numPiecesToLast+=counter[j];
            }
        }
    }
}


void dropJollyColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return; // not jolly pieces.

    int combFirstPiece = 0;
    int combLastPiece = 0;
    int number_of_colors=0;
    //bool two_valid_posible_combinations= false; TODO
    PieceCombination validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        combFirstPiece = i;
        const auto& currNumber = playerHand[i].number();
        validComb.clear();
       std::array<uint,4> yrub_counter={0,0,0,0};
        number_of_colors=0;

        while (playerHand[i].number()==currNumber && i < (playerHand.size() - jollyPieces))
        {
            const uint8_t index = static_cast<uint8_t>( playerHand[i].color() );

            if(yrub_counter[index] == 0) {number_of_colors++;}
            yrub_counter[index]++;

            i++;
        }

        if(number_of_colors<(3-jollyPieces)) continue; // no valid convination skip the rest

        combLastPiece= i;
        //TODO implement two valid convinations
        //        if(combLastPiece-combFirstPiece>5)
        //        {
        //            for(int i=0; i<3; i++)
        //            {
        //                yrub_counter[i]>
        //            }
        //            two_valid_posible_combinations=true; }

        std::array<uint,4> yrub_counter_copy = yrub_counter;
        for(int j=combFirstPiece; j <=combLastPiece; j++ )
        {
            const uint8_t index = static_cast<uint8_t>( playerHand[j].color() );
            if(yrub_counter[index]>=1 && yrub_counter[index]== yrub_counter_copy[index] )
            {
                yrub_counter[index]--;
                validComb.push_back(playerHand[j]);
            }
        }
        while(validComb.size()<3)
        {
            validComb.push_back(Jolly);
        }
        assert(validComb.isValidCombination()); validCombinations.push_back(validComb);
    }
}



void dropJollyNumbericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color

    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return ; // not jolly pieces.

    int combFirstPiece = 0;
    int valid_numbers=0;
    PieceCombination validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        const auto& currColor = playerHand[i].color();
        std::array<uint,13> counter = {0,0,0,0,0,0,0,0,0,0,0,0,0};
        validComb.clear();
        valid_numbers=0;
        combFirstPiece = i;

        while (playerHand[i].color()==currColor && i < (playerHand.size() - jollyPieces))
        {
            const uint8_t index = playerHand[i].number() -1;
            if(counter[index]==0){valid_numbers++;}
            counter[index]++;
            i++;
        }

        if(valid_numbers < ( 3 - jollyPieces)) continue; // no valid convination skip the rest

        int numPiecesTofirst = 0;
        int firstValid=0;
        int last = 0;
        int numPiecesToLast = 0;
        for(int j=0; j<13; j++)
        {
            if(counter[j]==0 || j == 12)
            {
                if( (last - firstValid) >= ( 3 - jollyPieces))
                {
                    selectValidCombFromCounterJolly(playerHand, validComb, counter, combFirstPiece, numPiecesTofirst, firstValid, numPiecesToLast, jollyPieces);
                    while( validComb.size() < 3)
                    {
                        validComb.push_back(Jolly);
                    }
                    assert(validComb.isValidCombination()); validCombinations.push_back(validComb);
                    validComb.clear();
                }

                last=-1;
                firstValid =-1;
                numPiecesTofirst=numPiecesToLast;
            }
            else if(counter[j]>0)
            {
                if(firstValid==-1)
                {
                    numPiecesTofirst+=counter[j];
                    firstValid=j;
                }
                else{ last=j;}
                numPiecesToLast+=counter[j];
            }
        }
    }
}













