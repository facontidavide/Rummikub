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

    // unsigned counter[13];
    //1,2,1,2,1,0,0,0,0,0,0,1,0

    unsigned yrub_counter[3]={0,0,0,0}; //yellow, red, blue, black;   counter
    int combFirstPiece = 0;
    int combLastPiece = 0;
    bool color_found[4] = { false, false, false, false };
    int number_of_colors=0;
    bool two_valid_posible_combinations= false;
    PieceCombination validComb;

    for(int i=0; i< playerHand.size() - jollyPieces; i++) // posible combinations without jolly
    {
        combFirstPiece = i;
        const auto& currNumber = playerHand[i].number();

        possibleComb.clear();
        yrub_counter[3]={0,0,0,0};
        color_found[4] = { false, false, false, false };
        number_of_colors=0;
        int e = i;   // TODO preguntar com funciona referencia si con i funcionaria...
        while (playerHand[e].number()==currNumber && e < (playerHand.size() - jollyPieces))
        {
            const uint8_t index = static_cast<uint8_t>( playerHand[i].color() );
            // YELLOW = 0, RED = 1, BLUE = 2, BLACK = 3
            switch(index)
            {
            case 0 :
                yrub_counter[0]++;
                if(!color_found[0]){number_of_colors++;}
                color_found[0]=true;
                break;
            case 1 :
                yrub_counter[1]++;
                if(!color_found[1]){number_of_colors++;}
                color_found[i]=true;
                break;
            case 2 :
                yrub_counter[2]++;
                if(!color_found[2]){number_of_colors++;}
                color_found[2]=true;
                break;
            case 3 :
                yrub_counter[3]++;
                if(!color_found[3]){number_of_colors++;}
                color_found[3]=true;
                break;
            }
            e++;
        }
        i = e; // to be deleted

        if(number_of_colors<3) continue; // no valid convination skip the rest

        combLastPiece= e;
//        if(combLastPiece-combFirstPiece>5)
//        {
//            for(int i=0; i<3; i++)
//            {
//                yrub_counter[i]>
//            }
//            two_valid_posible_combinations=true; }

        unsigned yrub_counter_copy= yrub_counter;
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
            isValidCombination = false;
            i=combFirstPiece -1;
        }
    }
}



void dropJollyColorCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), numberCompare ); // sort by number
    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return; // not jolly pieces.

    uint lastNonJollyNumber = playerHand.size() - 1 - jollyPieces ;

    int combFirstPiece = 0;
    int combLastPiece = 0;
    bool isValidCombination = false;
    PieceCombination possibleComb, validComb;

    for(uint i=0; i < lastNonJollyNumber; i++)
    {
        combFirstPiece = i;
        combLastPiece = i+ (2 - jollyPieces);

        possibleComb.clear();
        if (combFirstPiece >= combLastPiece)    // corner case 2 or more jollys
        {
             possibleComb.push_back(playerHand[i]);
             combLastPiece = combFirstPiece;
        }
        for(int e=combFirstPiece; e <=combLastPiece; e++ )
        {
            possibleComb.push_back(playerHand[e]);
        }

        for(int e = 0; e< 2; e++) // never want more than 2 jollys in one combination
        {
            possibleComb.push_back(playerHand[playerHand.size()-1]); //jolly
        }

        uint j = combLastPiece + 1;
        while( possibleComb.isValidColorSequence())
        {
            isValidCombination = true;
            validComb=possibleComb;
            combLastPiece=j-1;
            if(j<playerHand.size() - jollyPieces)
            {
                possibleComb.push_back(playerHand[j]);
                j++;
            }
            else
                break;
        }
        if (isValidCombination)
        {
            validCombinations.push_back(validComb);
            isValidCombination = false;
            i = combLastPiece;
        }
    }
}

void dropJollyNumbericalCombinations( std::vector<Piece>& playerHand, std::vector<PieceCombination>& validCombinations)
{
    std::sort( playerHand.begin(), playerHand.end(), colorCompare ); // sort by color

    int jollyPieces = countJolly(playerHand);

    if (jollyPieces==0)
        return ; // not jolly pieces.

    uint lastNonJollyNumber = playerHand.size() - 1 - jollyPieces ;

    int combFirstPiece = 0;
    int combLastPiece = 0;
    bool isValidCombination = false;
    PieceCombination possibleComb, validComb;

    for(uint i=0; i < lastNonJollyNumber; i++)
    {
        combFirstPiece = i;
        combLastPiece = i+1;

        possibleComb.clear();
        for(int e=combFirstPiece; e <=combLastPiece; e++ )
        {
            possibleComb.push_back(playerHand[e]);
        }
        possibleComb.push_back(playerHand[playerHand.size()-1]); //jolly
        uint j = combLastPiece + 1;
        while( possibleComb.isValidNumberSequence(true) && j<playerHand.size() - jollyPieces )
        {
            isValidCombination = true;
            validComb=possibleComb;
            combLastPiece = j;
            possibleComb.push_back(playerHand[j]);
            j++;
        }
        if (isValidCombination)
        {
            validCombinations.push_back(validComb);
            isValidCombination = false;
            i = combLastPiece-1;
        }
    }
}









