#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <iostream>
#include <stdint.h>

// see the implementation of isValidColorSequence to understand why I am doing this
enum Color{
    YELLOW = 0X1, // binary 0001
    RED    = 0X2, // binary 0010
    BLUE   = 0X4, // binary 0100
    BLACK  = 0X8, // binary 1000
    JOLLY  = 0x0  // binary 0000
};

class Piece{
public:
    Piece(): _data({0,0}) {}
    Piece(Color col, uint8_t value): _data( { static_cast<uint8_t>(col),value} ) {}

    Color color() const    { return static_cast<Color>(_data.col); }
    uint8_t number() const { return _data.num; }

    bool isInitialized() const { return _data.num != 0; }
    bool isJolly() const       { return _data.col == JOLLY; }

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

const Piece Jolly(JOLLY, 0X0);


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
    const_iterator end()   const { return &_data[_size-1]; }

    iterator begin()  { return &_data[0]; }
    iterator end()    { return &_data[_size-1]; }

    void push_back(Piece val);
    void pop_back();

    void insert(int pos, Piece value);

    void clear() { _size = 0; }

    uint8_t size() const { return _size; }

    const Piece& operator[](uint8_t index) const { return _data[index]; }
    Piece& operator[](uint8_t index)             { return _data[index]; }

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


bool isValidCombination(const PieceGroup& group);

bool isValidColorSequence(const PieceGroup& group);

bool isValidNumberSequence(const PieceGroup& group);
//-----------------------------------------------------------------------------


inline PieceGroup::PieceGroup(PieceGroup::const_iterator it_front, PieceGroup::const_iterator it_back): _size(0)
{
    while( it_front != it_back) { _data[_size++] = *(it_front++); }
}

inline void PieceGroup::push_back(Piece val){
    _data[_size++] = val;
}

inline void PieceGroup::pop_back() {
    if(_size >0) _size--;
}

inline void PieceGroup::insert(int pos, Piece value)
{
    if( _size < maxSize() && _size > 0){
        for( int i=(_size-1); i>=pos; i--) { _data[i+1] = _data[i]; }
    }
    _data[pos] = value;
}

inline std::pair<PieceGroup, PieceGroup> PieceGroup::split(int pos)
{
    return { PieceGroup( &_data[0],   &_data[pos-1]),
             PieceGroup( &_data[pos], &_data[_size-1]) };
}

#endif // BASE_TYPES_H

