#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>

enum Color{
    YELLOW = 0X1,
    RED = 0X2,
    BLUE = 0X4,
    BLACK = 0X8,
    JOLLY = 0xF
};

class Piece{
public:
    Piece(): _data(0) {}
    Piece(Color col, uint8_t value):
        _data( ((static_cast<uint8_t>(col)<<4) & 0XF0) | (value & 0xF)) {}

    Color color() const { return static_cast<Color>( (_data>>4) & 0xF); }
    uint8_t number() const { return _data & 0xF; }

    bool isInitialized() const { return _data != 0; }
    bool isJolly() const { return _data == 0xFF; }

private:
    // both the color and the number can be stored in a single byte.
    // it is important to keep this small to store as many pieces as possible in
    // a single cache line.
    // For the records, just 6 bits are sufficent...
    uint8_t _data;
};

const Piece Jolly(JOLLY, 0XF);


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
    PieceGroup(const_iterator it_front, const_iterator it_back): _size(0)
    {
        while( it_front != it_back) { _data[_size++] = *(it_front++); }
    }

    const_iterator begin() const { return &_data[0]; }
    const_iterator end()   const { return &_data[_size-1]; }

    iterator begin()  { return &_data[0]; }
    iterator end()    { return &_data[_size-1]; }

    void push_back(Piece val){
        _data[_size++] = val;
    }
    void pop_back() { if(_size >0) _size--; }

    void insert(int pos, Piece value)
    {
        if( _size < maxSize() && _size > 0){
            for( int i=(_size-1); i>=pos; i--) { _data[i+1] = _data[i]; }
        }
        _data[pos] = value;
    }

    void clear() { _size = 0; }

    uint8_t size() const { return _size; }

    const Piece& operator[](uint8_t index) const { return _data[index]; }
    Piece& operator[](uint8_t index) { return _data[index]; }

    static uint8_t maxSize() { return 13; }

    void isValidCombination() const;

    // split this in two groups. The first containing the indexes [0, pos-1]
    // and the second the indexes  [pos, size()-1]
    std::pair<PieceGroup,PieceGroup> split(int pos)
    {
        return { PieceGroup( &_data[0],   &_data[pos-1]),
                 PieceGroup( &_data[pos], &_data[_size-1]) };
    }

private:
    //sizeof(PieceSet) == 14 bytes
    // Note that size of an EMPTY std::vector<Piece> would be 24 bytes
   Piece _data[13];
   uint8_t _size;
};





#endif // BASE_TYPES_H
