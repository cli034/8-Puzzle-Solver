#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>
#include <cmath>

//added comments to test git on microsoft visual code
const int row = 3; //row size of puzzle
const int col = 3; //column size of puzzle
int defaultPuzzle[row][col] = {{1,0,2},{7,5,4},{8,6,3}}; // default puzzle
//int defaultPuzzle[row][col] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{0,13,14,15}};
int goal[row][col] = {{1,2,3},{4,5,6},{7,8,0}}; //goal state of puzzle
//int goal[row][col] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};

using namespace std;

class Puzzle
{
private:
    int puzzle[row][col];
    int depth;
    int cost;
public:
    //Puzzle constructor
    Puzzle()
    {
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                puzzle[i][j] = defaultPuzzle[i][j];
            }
        }
        depth = 0;
        cost = 0;
    }
    
    //print out current board
    void drawPuzzle()
    {
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                cout << puzzle[i][j] << " ";
            }
            cout << endl;
        }
    }
    
    // change board value if users wanted to
    void changeVal(int arr[][col])
    {
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                puzzle[i][j] = arr[i][j];
            }
        }
    }
    
    // check if two puzzles are equal
    bool isEqual(int arr[][col])
    {
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (puzzle[i][j] != arr[i][j])
                    return false;
            }
        }
        return true;
    }
    
    //moving blank space up
    void moveUp()
    {
        int temp = 0;
        //starts from 1 because 0 row can never go up
        for (int i = 1; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (puzzle[i][j] == 0)
                {
                    temp = puzzle[i-1][j];
                    puzzle[i-1][j] = puzzle[i][j];
                    puzzle[i][j] = temp;
                    return;
                }
            }
        }
    }
    // moving blank space down
    void moveDown()
    {
        int temp = 0;
        //ends at row - 1 because the last row can never go down
        for (int i = 0; i < row - 1; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (puzzle[i][j] == 0)
                {
                    temp = puzzle[i+1][j];
                    puzzle[i+1][j] = puzzle[i][j];
                    puzzle[i][j] = temp;
                    return;
                }
            }
        }
    }
    // moving blank space left
    void moveLeft()
    {
        int temp = 0;
        for (int i = 0; i < row; ++i)
        {
            //starts at second column because first column can never move left
            for (int j = 1; j < col; ++j)
            {
                if (puzzle[i][j] == 0)
                {
                    temp = puzzle[i][j-1];
                    puzzle[i][j-1] = puzzle[i][j];
                    puzzle[i][j] = temp;
                    return;
                }
            }
        }
    }
    // move blank space right
    void moveRight()
    {
        int temp = 0;
        for (int i = 0; i < row; ++i)
        {
            // ends at col - 1 because the last column can never move right
            for (int j = 0; j < col - 1; ++j)
            {
                if (puzzle[i][j] == 0)
                {
                    temp = puzzle[i][j+1];
                    puzzle[i][j+1] = puzzle[i][j];
                    puzzle[i][j] = temp;
                    return;
                }
            }
        }
    }
    
    //evaluation function f(n) = g(n) + h(n) for misplaced tiles
    //increment whenever theres a value not matching the goal state
    int getMisplacedTilesCost()
    {
        int misplacedCnt = 0;
        
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {   
                //does not count blank space
                if (puzzle[i][j] != 0)
                {
                    if (puzzle[i][j] != goal[i][j])
                        misplacedCnt++;
                }
            }
        }
        
        cost = depth + misplacedCnt;
        return cost;
    }
    
    //evaluation function f(n) = g(n) + h(n) for manhattan distance
    // distance = abs(x_f-x_i) + abs(y_f-y_i)
    // finding the final position of the value suppose to be in
    // for example 5, we know that it suppose to be in the 5th box of the 2d matrix
    // from left to right
    // formula x_f = (value - 1) / row
    // formula y_f = (value - 1) % col
    // value = 5, where it should be at box (1,1)
    // x_f = (5 - 1) / 3 = 1, where row = 3 in this case
    // y_f = (5 - 1) % 3 = 1, where col = 3 in this case
    // so we achieved (1,1) for the proper location
    int getManhattanCost()
    {
        int distanceCnt = 0;
        int val = 0;
        int xFinal = 0;
        int yFinal = 0;
        
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                val = puzzle[i][j];
                // no heuristic value for blank space
                if (val != 0)
                {
                    xFinal = (val - 1) / row;
                    yFinal = (val - 1) % col;
                    distanceCnt = distanceCnt + abs(xFinal - i) + abs(yFinal - j);
                }
            }
        }
        
        cost = depth + distanceCnt;
        return cost;
    }
    
    
    int getDepth()
    {
        return depth;
    }
    
    void setDepth(int d)
    {
        depth = d;
    }
    
    int getElement(int i, int j)
    {
        return puzzle[i][j];
    }
    
    Puzzle& operator=(const Puzzle& rhs)
    {
        depth = rhs.depth;
        
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                puzzle[i][j] = rhs.puzzle[i][j];
            }
        }
        return *this;
    }
    
};

// check if two objects contain the same puzzle
bool samePuzzle(Puzzle &p1, Puzzle &p2)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            if (p1.getElement(i,j) != p2.getElement(i,j))
                return false;
        }
    }
    return true;
}

//check if successor state is repeated from any state that has been visited before
bool stateRepeated(vector<Puzzle> &v, Puzzle &p)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (samePuzzle(v.at(i),p))
            return true;
    }
    return false;
}

// sort based on misplaced tiles heuristic
void selectionSortMT(vector<Puzzle> &v)
{
    Puzzle temp;
    int min = 0;
    
    for (int i = 0; i < v.size(); ++i)
    {
        min = i;
        for (int j = i+1; j < v.size(); ++j)
        {
            if (v.at(j).getMisplacedTilesCost() < v.at(min).getMisplacedTilesCost())
            {
                min = j;
            }
        }
        temp = v.at(min);
        v.at(min) = v.at(i);
        v.at(i) = temp;
    }
}

// sort based on manhattan distance heuristic
void selectionSortMD(vector<Puzzle> &v)
{
    Puzzle temp;
    int min = 0;
    
    for (int i = 0; i < v.size(); ++i)
    {
        min = i;
        for (int j = i+1; j < v.size(); ++j)
        {
            if (v.at(j).getManhattanCost() < v.at(min).getManhattanCost())
            {
                min = j;
            }
        }
        temp = v.at(min);
        v.at(min) = v.at(i);
        v.at(i) = temp;
    }
}

//queueing function for uniform cost search
//does not matter the order it queues into since all the cost is the same
queue<Puzzle> uniformCostSearch(queue<Puzzle>& n, Puzzle& curr, vector<Puzzle> &v, int &depth)
{
    //first successor
    Puzzle up = curr;
    up.moveUp();
    up.setDepth(depth);
    if (!stateRepeated(v,up))
    {
        //up.drawPuzzle();
        n.push(up);
    }
    
    //second successor
    Puzzle down = curr;
    down.moveDown();
    down.setDepth(depth);
    if (!stateRepeated(v,down))
    {
        //down.drawPuzzle();
        n.push(down);
    }
    
    //third successor
    Puzzle left = curr;
    left.moveLeft();
    left.setDepth(depth);
    if (!stateRepeated(v,left))
    {
        //left.drawPuzzle();
        n.push(left);
    }
    
    //forth successor
    Puzzle right = curr;
    right.moveRight();
    right.setDepth(depth);
    if (!stateRepeated(v,right))
    {
        //right.drawPuzzle();
        n.push(right);
    }
    
    return n;
}

//queueing function for misplaced tiles
//always queue the state with lowest cost in front
queue<Puzzle> misplacedTiles(queue<Puzzle>& n, Puzzle& curr, vector<Puzzle> &v, int &depth)
{
    vector<Puzzle> toSort;
    
    //add nodes in queue into toSort, so it always check the lowest cost node 
    while(!n.empty())
    {
        toSort.push_back(n.front());
        n.pop();
    }
    
    //first successor
    Puzzle up = curr;
    up.moveUp();
    up.setDepth(depth);

    if (!stateRepeated(v,up))
        toSort.push_back(up);

    //second successors
    Puzzle down = curr;
    down.moveDown();
    down.setDepth(depth);

    if (!stateRepeated(v,down))
        toSort.push_back(down);


    //third successor
    Puzzle left = curr;
    left.moveLeft();
    left.setDepth(depth);

    if (!stateRepeated(v,left))
        toSort.push_back(left);

    //forth successor
    Puzzle right = curr;
    right.moveRight();
    right.setDepth(depth);
    
    if (!stateRepeated(v,right))
        toSort.push_back(right);

    selectionSortMT(toSort);

    for (int i = 0; i < toSort.size(); ++i)
    {
        n.push(toSort.at(i));
    }
    
    return n;
}

//queueing fucntion for manhattan distance
//always queue the state with lowest cost in front
queue<Puzzle> manhattanDistance(queue<Puzzle>& n, Puzzle& curr, vector<Puzzle> &v, int &depth)
{
    vector<Puzzle> toSort;
    
    //add nodes in queue into toSort, so it always check the lowest cost node 
    while(!n.empty())
    {
        toSort.push_back(n.front());
        n.pop();
    }
    
    //first successor
    Puzzle up = curr;
    up.moveUp();
    up.setDepth(depth);

    if (!stateRepeated(v,up))
        toSort.push_back(up);

    //second successors
    Puzzle down = curr;
    down.moveDown();
    down.setDepth(depth);

    if (!stateRepeated(v,down))
        toSort.push_back(down);


    //third successor
    Puzzle left = curr;
    left.moveLeft();
    left.setDepth(depth);

    if (!stateRepeated(v,left))
        toSort.push_back(left);

    //forth successor
    Puzzle right = curr;
    right.moveRight();
    right.setDepth(depth);
    
    if (!stateRepeated(v,right))
        toSort.push_back(right);

    selectionSortMD(toSort);

    for (int i = 0; i < toSort.size(); ++i)
    {
        n.push(toSort.at(i));
    }
    
    return n;
}

// this is the A* algorithm
void general_search(Puzzle init, const int& queueing_function)
{
    //a list that stores nodes that have been generated but not yet visited
    queue<Puzzle> node;
    //store nodes that have been visited 
    vector<Puzzle> visited;
    // depth counter
    int depthCnt = 0;
    int maxNode = node.size();
    node.push(init);
    
    cout << "Expanding state" << endl;
    while (1)
    {
        if (node.empty())
        {
            cout << "Search failure!" << endl;
            return;
        }
        
        Puzzle current = node.front();
        
        if (queueing_function == 1)
            cout << "Expanding state" << endl;
            
        else if (queueing_function == 2)
        {
            cout << "The best state to expand with a g(n) = " << current.getDepth();
            cout << " and h(x) = " << current.getMisplacedTilesCost() - current.getDepth() << endl;
        }
        else if (queueing_function == 3)
        {
            cout << "The best state to expand with a g(n) = " << current.getDepth();
            cout << " and h(x) = " << current.getManhattanCost() - current.getDepth() << endl;
        }
        
        current.drawPuzzle();
        cout << endl;
        
        //if the current state is goal state
        if (current.isEqual(goal))
        {
            cout << "Goal!!" << endl;
            cout << "To solve this problem the search algorithm expanded a total of " << visited.size() << " nodes." << endl;
            cout << "The maximum number of nodes in the queue at one time was " << maxNode << endl;
            cout << "The depth of goal node was " << current.getDepth() << endl;
            return;
        }
        
        visited.push_back(node.front());
        node.pop();
    
        if (queueing_function == 1)
        {
            // when queueing function is called, depth is incremented
            depthCnt = current.getDepth() + 1;
            uniformCostSearch(node, current, visited, depthCnt);
        }
        else if (queueing_function == 2)
        {
            depthCnt = current.getDepth() + 1;
            misplacedTiles(node, current, visited, depthCnt);
        }
        else if (queueing_function == 3)
        {
            depthCnt = current.getDepth() + 1;
            manhattanDistance(node, current, visited, depthCnt);
        }

        if (node.size() > maxNode)
            maxNode = node.size();
    }
}


int main()
{
    int choosePuzzle = 0; //type 1 or 2 to choose puzzle
    int userNum = 0;
    int userArr[row][col];
    
    Puzzle p = Puzzle();
    
    cout << "Welcome to Adrians 8-puzzle solver." << endl;
    cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
    cin >> choosePuzzle;
    
    //default puzzle
    if (choosePuzzle == 1)
    {
        p.drawPuzzle();
    }
    
    //custom puzzle
    else if (choosePuzzle == 2)
    {
        cout << "Enter your puzzle, use a zero to represent the blank" << endl;
        cout << "Enter first row, use space or tabs between numbers" << endl;
        for (int i = 0; i < col; ++i)
        {
            cin >> userNum;
            userArr[0][i] = userNum;
        }
        cout << "Enter second row, use space or tabs between numbers" << endl;
        for (int i = 0; i < col; ++i)
        {
            cin >> userNum;
            userArr[1][i] = userNum;
        }
        cout << "Enter third row, use space or tabs between numbers" << endl;
        for (int i = 0; i < col; ++i)
        {
            cin >> userNum;
            userArr[2][i] = userNum;
        }
        
        /*
        cout << "Enter forth row, use space or tabs between numbers" << endl;
        for (int i = 0; i < col; ++i)
        {
            cin >> userNum;
            userArr[3][i] = userNum;
        }
        */
        cout << endl;
        
        p.changeVal(userArr);
        p.drawPuzzle();
    }
    else
        exit(1);
    
    cout << "Enter your choice of algorithm" << endl;
    cout << "\t 1. Uniform Cost Search" << endl;
    cout << "\t 2. A* with the Misplaced Tile Heuristic" << endl;
    cout << "\t 3. A* with the Manhattan distance Heuristic" << endl;
    cin >> userNum;
    cout << endl;
    
    
    general_search(p,userNum);
    return 0;
}
