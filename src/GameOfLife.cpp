#include "GameOfLife.hpp"

GameOfLife::GameOfLife(size_t width, size_t height)
{
    //TODO: Remove this
    srand(time(NULL));

    this->width = width;
    this->height = height;

    //Initialize toCheck vectors
    this->currentToCheck = new std::vector<struct Point>();
    this->nextToCheck = new std::vector<struct Point>();

    //Initialize grid 1
    this->currentGrid = new struct Cell*[width];
    for(int i = 0; i < width; i++)
    {
        this->currentGrid[i] = new struct Cell[height];
    }

    //TODO: Dont set all of them to alive
    //      and allow setting custom patterns
    //      Remember to edit toCheck as well
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            struct Cell c;
            this->currentGrid[i][j] = c;
            struct Point p;
            p.x = i;
            p.y = j;
            this->currentGrid[i][j].alive = true;
            if(rand() % 3 == 0)
                this->currentGrid[i][j].alive = false;
            currentToCheck->push_back(p);
        }
    }
    
    //Initialize grid 2
    //This grid will be the grid used on the next iteration
    this->nextGrid = new struct Cell*[width];
    for(int i = 0; i < width; i++)
    {
        this->nextGrid[i] = new struct Cell[height];
    }

    //Initialize all cells in next grid
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            struct Cell c;
            this->nextGrid[i][j] = c;
        }
    }

    //All Cells in next grid start as dead
    this->clearNextGridThread = std::thread(&GameOfLife::clearNextGrid, this);
}

/*
 * This function should be called in its own thread.
 * Making sure all the spots are dead before calculating the 
 * next iteration is much faster.
 */
void GameOfLife::clearNextGrid()
{
    for(int i = 0; i < this->width; i++)
    {
        for(int j = 0; j < this->height; j++)
        {
            this->nextGrid[i][j].alive = false;
            this->nextGrid[i][j].inVector = false;
        }
    }
}

void GameOfLife::nextGeneration()
{
    //Make sure clearNextGridThread has finished before writing to
    //nextGrid
    this->clearNextGridThread.join();
    while(this->currentToCheck->size() > 0)
    {
        struct Point* p = &this->currentToCheck->back();
        size_t neighborCount = this->getNeighborCount(p);

        if(neighborCount < 2)
            this->nextGrid[p->x][p->y].alive = false;
        else if(this->currentGrid[p->x][p->y].alive)
        {
            if(neighborCount == 2 || neighborCount == 3)
            {
                this->nextGrid[p->x][p->y].alive = true;
                this->populateNextToCheck(p);
            }
            else if(neighborCount > 3)
                this->nextGrid[p->x][p->y].alive = false;
        }
        else
        {
            if(neighborCount == 3)
            {
                this->nextGrid[p->x][p->y].alive = true;
                this->populateNextToCheck(p);
            }
        }

        this->currentToCheck->pop_back();
    }
    
    this->swapCurrentAndNextPointers();
    this->clearNextGridThread = std::thread(&GameOfLife::clearNextGrid, this);
    //this->clearCurrentCellInVector();
    //Make sure the nextGrid is clear before swapping pointers
    
}

void GameOfLife::clearCurrentCellInVector()
{
    for(int i = 0; i < this->width; i++)
    {
        for(int j = 0; j < this->height; j++)
        {
            this->nextGrid[i][j].inVector = false;
        }
    }
}

void GameOfLife::populateNextToCheck(struct Point* p)
{

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int x = p->x + i;
            int y = p->y + j;
            
            //Check to make sure x and y do not go out of bounds
            if(x < 0 || x > this->width - 1 || y < 0 || y > this->height - 1)
                continue;
            else
            {
                if(!this->nextGrid[x][y].inVector)
                {
                    struct Point pToCheck;
                    pToCheck.x = x;
                    pToCheck.y = y;
                    this->nextToCheck->push_back(pToCheck);
                    this->nextGrid[x][y].inVector = true;
                }
            }
        }
    }
}

size_t GameOfLife::getNeighborCount(struct Point* p)
{
    size_t neighborCount = 0;

    //Get locations up, down, left, right, and diagonally of the location provided (Point p).
    //After getting the locations, check if the Cell is alive and increase neighborCount.
    //
    //   * * *
    //    \|/
    //   *-p-*
    //    /|\
    //   * * *
    //

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            int x = p->x + i;
            int y = p->y + j;
            
            //Check to make sure x and y do not go out of bounds
            //i and j cannot be 0 because a Cell cannot be its own neighbor
            if(x < 0 || x > this->width - 1 || y < 0 || y > this->height - 1 || (i == 0 && j == 0))
                continue;
            else if(this->currentGrid[x][y].alive)
                neighborCount++;
            
            //TODO: Maybe revise this and allow wrapping?
            //if(x < 0)
            /*     x = this->width - 1; */
            /* else if(x > this->width - 1) */
            /*     x = 0; */
            
            /* //Wrap y coordinate around the grid */
            /* if(y < 0) */
            /*     y = this->height - 1; */
            /* else if(y > this->height - 1) */
            /*     y = 0; */
        }
    }
    
    return neighborCount;
}

void GameOfLife::printGrid(bool current)
{
    if(current)
    {
        for(int i = 0; i < this->height; i++)
        {

            for(int j = 0; j < this->width; j++)
            {
                if(this->currentGrid[j][i].alive) 
                    std::cout <<  "O";
                else
                    std::cout <<  "-";
            }
            std::cout << "\n";
        }
    }
    else
    {
        for(int i = 0; i < this->height; i++)
        {

            for(int j = 0; j < this->width; j++)
            {
                if(this->nextGrid[j][i].alive) 
                    std::cout <<  "O";
                else
                    std::cout <<  "-";
            }
            std::cout << "\n";
        }

    }
}

Cell** GameOfLife::getGrid()
{
    return this->currentGrid;
}

void GameOfLife::swapCurrentAndNextPointers()
{
    std::swap(this->currentGrid, this->nextGrid);
    std::swap(this->currentToCheck, this->nextToCheck);
}
