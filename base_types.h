#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <algorithm>    // std::sort

enum Color{
    YELLOW = 0,
    RED    = 1,
    BLUE   = 2,
    BLACK  = 3,
    JOLLY  = 4,
    UNDEFINED = 15
};

class Piece{
public:
    Piece(): _data({UNDEFINED, 15}) {} // number 15 makes easier to sort if is jolly
    Piece(Color col, uint8_t value): _data( { static_cast<uint8_t>(col),value} ) {}

    Color color() const    { return static_cast<Color>(_data.col); }

    uint8_t number() const { return _data.num; }

    bool isInitialized() const { return _data.num != 0 || _data.col == UNDEFINED; }

    bool isJolly() const       { return _data.col == JOLLY; }

    bool operator ==( const Piece& other) const;

    bool operator !=( const Piece& other) const;

private:
    // both the color and the number can be stored in a single byte.
    // it is important to keep this small to store as many pieces as possible in
    // a single cache line.
    //
    // http://en.cppreference.com/w/cpp/language/bit_field
    struct Storage {
     uint8_t col : 4;
     uint8_t num : 4;
    };
    Storage _data;
};


const Piece Jolly(JOLLY, 0XFF);


//-------------------------------
//TODO:
// - add size check in debug mode?
// - implement isValidCombination()
// unit test everything

//This custom container is much faster then a std::vector<Piece>,
// because it fits better in the cache and does not require heap allocations.
class PieceGroup{

public:
    typedef Piece* iterator;
    typedef const Piece* const_iterator;

    PieceGroup(): _size(0) {}
    PieceGroup(const_iterator it_front, const_iterator it_back);

    const_iterator begin() const { return &_data[0]; }
    const_iterator end()   const { return &_data[_size]; }

    iterator begin()  { return &_data[0]; }
    iterator end()    { return &_data[_size-1]; }

    bool operator() (Piece a, Piece b) { return (a.number() < b.number());}

    void push_back(Piece val);
    void pop_back();

    void sort();

    void insert(int pos, Piece value);

    void clear() { _size = 0; }

    uint8_t size() const { return _size; }

    const Piece& operator[](int index) const { return _data[index]; }
    Piece& operator[](int index)             { return _data[index]; }

    static uint8_t maxSize() { return 13; }

    // split this in two groups. The first containing the indexes [0, pos-1]
    // and the second the indexes  [pos, size()-1]
    std::pair<PieceGroup,PieceGroup> split(int pos);

private:
    //sizeof(PieceSet) == 14 bytes
    // Note that size of an EMPTY std::vector<Piece> would be 24 bytes
   Piece _data[13];
   uint8_t _size;
};


class RandomPiecesStack
{
public:
    //generates a random sequence
    RandomPiecesStack();

    size_t size() const { return _pieces_stack.size(); }

    // pick a piece from the stack. Size will be decremented by 1.
    Piece pop_back();

    // remove a specific piece from the stack. usually used in testing and debugging.
    bool removePiece( Piece p);

private:
    std::vector<Piece> _pieces_stack;
};

bool isValidCombination(const PieceGroup& group);

bool isValidColorSequence(const PieceGroup& group);

bool isValidNumberSequence(const PieceGroup& group);
//-----------------------------------------------------------------------------

inline bool Piece::operator ==(const Piece &other) const
{
    return (_data.col == other._data.col) && (_data.num == other._data.num);
}

inline bool Piece::operator !=(const Piece &other) const
{
    return (_data.col != other._data.col) || (_data.num != other._data.num);
}

inline PieceGroup::PieceGroup(PieceGroup::const_iterator it_front, PieceGroup::const_iterator it_back): _size(0)
{
    while( it_front != it_back) { _data[_size++] = *(it_front++); }
}

inline void PieceGroup::push_back(Piece val){
    _data[_size++] = val;
    sort();
}

inline void PieceGroup::pop_back() {
    if(_size >0) _size--;
}

inline void PieceGroup::sort(){
    std::sort(begin(), end(), PieceGroup()); // TODO fix bug with 5 -> 15jolly -> 15jolly -> 3
    bool numbers_are_consecutive= false;
    while(end()->isJolly() && size()> 2 && !numbers_are_consecutive )
    {
        int i=0;  // change by iterator num
        iterator it_front = begin();
        iterator it_back = end();

        while( it_front != it_back && ((it_front->number() == (it_front->isJolly() || (it_front+1)->number() -1)) || (it_front+1)->isJolly()))
        {
            i++;
            it_front++;
            if (it_front == it_back)  // all numbers are consecutives jolly will be moved at the beggining
            {
                numbers_are_consecutive = true;
            }
        }
        if (!numbers_are_consecutive)
        {
            insert(i,*end());     //insert joly at the corresponding position
            this->pop_back();     // remove joly at the end
        }
    }
}

inline void PieceGroup::insert(int pos, Piece value)
{
    if( _size < maxSize() && _size > 0){
        for( int i=(_size-1); i>=pos; i--) { _data[i+1] = _data[i]; }
    }
    _data[pos] = value;
    _size++;
}

inline std::pair<PieceGroup, PieceGroup> PieceGroup::split(int pos)
{
    return { PieceGroup( &_data[0],   &_data[pos-1]),
             PieceGroup( &_data[pos], &_data[_size-1]) };
}

#endif // BASE_TYPES_H

