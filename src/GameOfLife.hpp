#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>

struct Cell
{
    size_t age;
    bool alive;
    bool inVector;
};

/**
 * Here the Point struct is only being used to store an x and y coordinate
 * for the vector toCheck to know coordinates of cells in the grids.
 *
 */
struct Point
{
    int x;
    int y;
};

class GameOfLife
{
    public:
        GameOfLife(size_t width, size_t height);
        ~GameOfLife();

        void nextGeneration();
        void printGrid(bool current);
        struct Cell** getGrid();

    private:
        int width;
        int height;

        struct Cell** currentGrid;
        struct Cell** nextGrid;

        std::vector<struct Point>* currentToCheck;
        std::vector<struct Point>* nextToCheck;

        void populateNextToCheck(struct Point* p);

        size_t getNeighborCount(struct Point* p);
        void swapCurrentAndNextPointers();

        void clearCurrentCellInVector(); 

        //To be called in its own thread
        void clearNextGrid();
};
