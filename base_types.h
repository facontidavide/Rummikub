#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <assert.h>

enum Color{
    YELLOW = 0,
    RED    = 1,
    BLUE   = 2,
    BLACK  = 3,
    JOLLY,
    UNDEFINED
};

class Piece{
public:
    Piece(): _data({UNDEFINED,0}) {}
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
class PieceCombination{

public:
    typedef Piece* iterator;
    typedef const Piece* const_iterator;

    PieceCombination(): _size(0) {}
    PieceCombination(const_iterator it_front, const_iterator it_back);

    const_iterator begin() const { return &_piece[0]; }
    const_iterator end()   const { return &_piece[_size]; }

    iterator begin()  { return &_piece[0]; }
    iterator end()    { return &_piece[_size-1]; }

    void push_back(Piece val);
    void pop_back();

    void insert(int pos, Piece value);

    void clear() { _size = 0; }

    uint8_t size() const { return _size; }

    const Piece& operator[](int index) const { return _piece[index]; }
    Piece& operator[](int index)             { return _piece[index]; }

    static uint8_t maxSize() { return 13; }

    // split this in two groups. The first containing the indexes [0, pos-1]
    // and the second the indexes  [pos, size()-1]
    std::pair<PieceCombination,PieceCombination> split(int pos);


    // Note: it is worth using here the design pattern known as "memoization"

    bool isValidCombination() const;

    bool isValidColorSequence() const;

    bool isValidNumberSequence() const;

private:
    //sizeof(PieceSet) == 14 bytes
    // Note that size of an EMPTY std::vector<Piece> would be 24 bytes
   Piece   _piece[13];
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


//-----------------------------------------------------------------------------
// Inline Declarations
//-----------------------------------------------------------------------------

inline bool Piece::operator ==(const Piece &other) const
{
    return (_data.col == other._data.col) && (_data.num == other._data.num);
}

inline bool Piece::operator !=(const Piece &other) const
{
    return (_data.col != other._data.col) || (_data.num != other._data.num);
}

inline PieceCombination::PieceCombination(PieceCombination::const_iterator it_front, PieceCombination::const_iterator it_back): _size(0)
{
    while( it_front != it_back) {
        assert( _size < maxSize() );
        _piece[_size++] = *(it_front++);
    }
}

inline void PieceCombination::push_back(Piece val){
    assert( _size < maxSize() );
    _piece[_size++] = val;
}

inline void PieceCombination::pop_back() {
    if(_size >0) _size--;
}

inline void PieceCombination::insert(int pos, Piece value)
{
    assert( _size < maxSize() );
    if( _size < maxSize() && _size > 0){
        for( int i=(_size-1); i>=pos; i--) { _piece[i+1] = _piece[i]; }
    }
    _piece[pos] = value;
}

inline std::pair<PieceCombination, PieceCombination> PieceCombination::split(int pos)
{
    return { PieceCombination( &_piece[0],   &_piece[pos-1]),
             PieceCombination( &_piece[pos], &_piece[_size-1]) };
}

inline bool PieceCombination::isValidCombination() const
{
    return size() >=3 && (isValidColorSequence() || isValidNumberSequence() );
}

#endif // BASE_TYPES_H

