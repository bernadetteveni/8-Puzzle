#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#define tiles 3

using namespace std;

// Struct representation of the states (path)
struct State {
    State *parentNode;
    int puzzle[tiles][tiles];
    // coordinates
    int X, Y;
    int heuristic;
    int move;
    int cost;
};

typedef struct State state;

void initializePuzzle(state* currentState) {
    // root tile
    currentState->parentNode = NULL;
    // Go through all tiles in the puzzle
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++) { 
             // extract empty slot position (-1) x,y
            if (currentState->puzzle[i][j] == -1) {
                currentState->X = i;
                currentState->Y = j;
                return;
            }
        }
    }
    // Set moves and heuristic to 0 for initialization
    currentState->move = 0;
    currentState->heuristic = 0;
}

// Move empty slot up
state* Up(state currentState) {
    // Top row of table, cannot move up
    if (currentState.X == 0)
        return NULL;
    // Store current state datum in temporary state
    state *tempState = (state*)malloc(sizeof(state));
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++) {
            tempState->puzzle[i][j] = currentState.puzzle[i][j];
        }
    }

    // Swap blank tile position with tile above it, and update blank tile position
    tempState->X = currentState.X;
    tempState->Y = currentState.Y;
    tempState->puzzle[tempState->X][tempState->Y] = tempState->puzzle[tempState->X - 1][tempState->Y];
    tempState->puzzle[tempState->X - 1][tempState->Y] = -1;
    tempState->X = tempState->X - 1;
    return tempState;
}

// Move empty slot down
state* Down(state currentState) {
    // Bottom row of table, cannot move down
    if (currentState.X == tiles - 1)
        return NULL;

    // Store current state datum in temporary state
    state* tempState = (state*)malloc(sizeof(state));
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++)
            tempState->puzzle[i][j] = currentState.puzzle[i][j];
    }

    // Swap blank tile position with tile below it, and update blank tile position
    tempState->X = currentState.X;
    tempState->Y = currentState.Y;
    tempState->puzzle[tempState->X][tempState->Y] = tempState->puzzle[tempState->X + 1][tempState->Y];
    tempState->puzzle[tempState->X + 1][tempState->Y] = -1;
    (tempState->X)++;
    return tempState;
}

// Move empty slot left
state* Left(state currentState) {
    // Leftmost column of table, cannot move left
    if (currentState.Y == 0)
        return NULL;

    // Store current state datum in temporary state
    state* tempState = (state*)malloc(sizeof(state));
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++)
            tempState->puzzle[i][j] = currentState.puzzle[i][j];
    }

    // Swap blank tile position with tile to left of it, and update blank tile position
    tempState->X = currentState.X;
    tempState->Y = currentState.Y;
    tempState->puzzle[tempState->X][tempState->Y] = tempState->puzzle[tempState->X][tempState->Y - 1];
    tempState->puzzle[tempState->X][tempState->Y - 1] = -1;
    tempState->Y = tempState->Y - 1;
    return tempState;
}

// Move empty slot right
state* Right(state currentState) {
    // Rightmost column of table, cannot move right
    if (currentState.Y == tiles - 1)
        return NULL;

    // Store current state datum in temporary state
    state* tempState = (state*)malloc(sizeof(state));
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++)
            tempState->puzzle[i][j] = currentState.puzzle[i][j];
    }

    // Swap blank tile position with tile to right of it, and update blank tile position
    tempState->X = currentState.X;
    tempState->Y = currentState.Y;
    tempState->puzzle[tempState->X][tempState->Y] = tempState->puzzle[tempState->X][tempState->Y + 1];
    tempState->puzzle[tempState->X][tempState->Y + 1] = -1;
    tempState->Y = tempState->Y + 1;
    return tempState;
}

state* nextMove(state* currentState, int move) {
    // From move passed by parameter, the current state will move up, down, left, or right and return the new state
    state *newState;
    switch (move) {
    case 1:
        newState = Up(*currentState);
        break;
    case 2:
        newState = Down(*currentState);
        break;
    case 3:
        newState = Left(*currentState);
        break;
    case 4:
        newState = Right(*currentState);
        break;
    default:
        newState = NULL;
    }
    return newState;
}

int compareStates(state* firstState, state* secondState) {
    // Go through each tile of first and second states
    for (int i = 0; i < tiles; i++)
        for (int j = 0; j < tiles; j++) { 
            // If firstState and secondState are not the same 2D array
            if (firstState->puzzle[i][j] != secondState->puzzle[i][j])
                // return false
                return 0; 
        }
    // If firstState and secondState are the same 2D array return true
    return 1; 
}

// Print function
void printPath(state currentState) {
    cout << endl;
    // Go through each tile in the state
    for (int i = 0; i < tiles; i++) {
        for (int j = 0; j < tiles; j++)
            // Print each tile
            cout << currentState.puzzle[i][j] << setw(5);
        cout << endl;
    }
    cout << endl;
}

// Print states from final state to the root
void printFinalState(state* FinalState) {
    // Store final state in temporary state
    state *tempState = FinalState;
    // While we are within the bounds of the final state and it is not empty
    while (tempState != NULL) {
        // Print and traverse backwards to parent node
        printPath(*tempState);
        tempState = tempState->parentNode;
    }
    cout << endl << endl;
}

//==========================Begin BFS=========================

struct QueueNode {
    // Datum pointer which can point to objects of any data type (int or full state etc.)
    void* datum;
    // Pointer to next queue node
    QueueNode* next;
};

struct Queue {
    // Head and tail queue node pointers
    QueueNode* head;
    QueueNode* tail;
};

// Initialize Queue to null
void newQueue(Queue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

QueueNode *createQueueNode(void* datum) {
    // Create new newNode pointer of size and type QueueNode
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    // Set datum to datum passed as parameter
    newNode->datum = datum;
    // initialize to point to null (no next node)
    newNode->next = NULL;
    return newNode;
}

void enqueue(Queue* queue, void* datum) {
    // If the queue is empty
    if (queue->head == NULL)
        // Create new node and set it as head and tail (only one element in queue) with datum passed by parameter
        queue->head = queue->tail = createQueueNode(datum);
    else {
        // Create new node with datum passed by parameter, and set it as the tail
        queue->tail->next = createQueueNode(datum);
        queue->tail = queue->tail->next;
    }
}

QueueNode* dequeue(Queue* queue) {
    // If the queue is empty, nothing to dequeue
    if (queue->head == NULL)
        return NULL;
    // Create node to be dequeued and set it as the head of the list
    else {
        QueueNode *dequeued = queue->head;
        // Set new head
        queue->head = queue->head->next;
        // return dequeued node
        return dequeued;
    }
}

int EmptyQueue(Queue* queue) {
    // If no head of list then queue is empty
    if (queue->head == NULL)
        // return true
        return 1;
    else
        // return false
        return 0;
}

int inQueue(Queue* queue, state* datum) {
    QueueNode *p = queue->head;
    // Go through datum of the 2D array and compare to queue
    while (p != NULL) {
        // If they are the same
        if (compareStates((state *)p->datum, datum) == 1)
            // return true
            return 1;
        p = p->next;
    }
    // If they are not the same return false
    return 0;
}

void solveByBFS(state startState, state finalState) {
    // The set of all nodes in the queue at the end of all visited paths is unvisitedState
    Queue unvisitedStates;
    // All visited paths in the queue
    Queue visitedStates;
    // Initialize these new queues with head and tail of null
    newQueue(&unvisitedStates);
    newQueue(&visitedStates);
    // create state pointers
    state *newState;
    state *currentState;
    // root node initial state
    int numOfGeneratedNodes = 1; 

    // putting finalState 2D array into unvisitedStates queue
    enqueue(&unvisitedStates, &finalState);
    // While unvisitedStates queue is not empty
    while (EmptyQueue(&unvisitedStates) != 1) { 
        // dequeue finalState datum from unvisitedState queue and enqueue it to visitedState queue
        currentState = (state *)(dequeue(&unvisitedStates)->datum);
        enqueue(&visitedStates, currentState);
        // if dequeued datum matches startState datum
        if (compareStates(currentState, &startState) == 1) {
            // Found solution
            printFinalState(currentState);
            break;
        }

        // 4 children -> 4 moves (Up, Down, Left, Right)
        for (int i = 1; i < 5; i++) {
            // Return new table state after doing operation i on previous table state
            newState = nextMove(currentState, i);

            // if operation is successful
            if (newState != NULL) {
                // Keep track of the number of generated nodes
                numOfGeneratedNodes++;
                // Record successful operation: 1 = up, 2=down, 3 = left. 4 = right.
                newState->move = i;
                // If it is not visited
                if (inQueue(&visitedStates, newState) == 0) {
                    // Put in unvisitedStates
                    newState->parentNode = currentState;
                    enqueue(&unvisitedStates, newState);
                } else
                    // delete the unused state
                    free(newState);
            }
        }
    }

    cout << "BFS generated " << numOfGeneratedNodes << " nodes." << endl;

    // If the unvisited is empty and we did not find initial state then no solution
    if (EmptyQueue(&unvisitedStates))
        cout << "No final state possible." << endl;
}

//==========================End BFS===========================

//==========================Begin DFS=========================
struct StackNode {
    // Datum pointer which can point to objects of any data type (int or full state etc.)
    void* datum;
    // Pointer to next queue node
    StackNode* next;
};

struct Stack {
    // top of stack queue node pointers
    StackNode* top;
};

// Initialize Queue to null
void newStack(Stack* stack) {
    stack->top = NULL;
}

StackNode* createStackNode(void* datum) {
    // create new newNode pointer of size and type StackNode
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    // Set datum to datum passed as parameter
    newNode->datum = datum;
    // initialize to point to null (no next node)
    newNode->next = NULL;
    return newNode;
}

void push(Stack* stack, void* datum) {
    // If the stack is empty
    if (stack->top == NULL)
    // Create new node and set it as top (only one element in stack) with datum passed by parameter
        stack->top = createStackNode(datum);
    else {
        // Create new node with datum passed by parameter, and set it as the new top
        StackNode* newNode = createStackNode(datum);
        newNode->next = stack->top;
        stack->top = newNode;
    }
}

StackNode* pop(Stack* stack) {
    // If the stack is empty, nothing to pop
    if (stack->top == NULL)
        return NULL;
    // Create node to be popped and set it as the top of the list
    else {
        StackNode* popped = stack->top;
        // Set new top
        stack->top = stack->top->next;
        // return popped node
        return popped;
    }
}

int EmptyStack(Stack* stack) {
    // If no top of list then stack is empty
    if (stack->top == NULL)
        // return true
        return 1;
    else
        // return false
        return 0;
}

void solveByDFS(state startState, state finalState) {
    // The set of all nodes in the stack at the end of all visited paths is unvisitedStack
    Stack unvisitedStack;
    // All visited paths in the queue
    Queue visitedQueue;
    // Initialize new stack with top of null and queue with head and tail of null
    newStack(&unvisitedStack);
    newQueue(&visitedQueue);
    // create state pointers
    state* newState;
    state* currentState;
    // root node initial state
    int numOfGeneratedNodes = 1;

    // putting finalState 2D array into unvisitedStack
    push(&unvisitedStack, &finalState);
    // While unvisitedStack is not empty
    while (EmptyStack(&unvisitedStack) != 1) {
         // pop finalState datum from unvisitedStack and enqueue it to visitedQueue
        currentState = (state *)(pop(&unvisitedStack)->datum);
        enqueue(&visitedQueue, currentState);
        // if popped datum matches startState datum
        if (compareStates(currentState, &startState) == 1) {
            // Found solution
            printFinalState(currentState);
            break;
        }

        // 4 children -> 4 moves (Up, Down, Left, Right)
        for (int i = 1; i < 5; i++) {
            // Return new table state after doing operation i on previous table state
            newState = nextMove(currentState, i);
            // if operation is successful
            if (newState != NULL) {
                // Keep track of the number of generated nodes
                numOfGeneratedNodes++;
                // Record successful operation: 1 = up, 2=down, 3 = left. 4 = right.
                newState->move = i;
                // If it is not visited
                if (inQueue(&visitedQueue, newState) == 0) {
                    // Put in unvisitedStack
                    newState->parentNode = currentState;
                    push(&unvisitedStack, newState);
                } else
                    // delete the unused state
                    free(newState);
            }
        }
    }

        cout << "DFS generated " << numOfGeneratedNodes << " nodes." << endl;

    // If the unvisited is empty and we did not find initial state then no solution
    if (EmptyStack(&unvisitedStack))
       cout << "No final state possible." << endl;
}

//==========================End DFS===========================

//======================Begin Best-First======================

// the "sequence score" that measures the degree to which the tiles are already ordered in the current position with respect to the order required in the goal configuration.
int seq(state* firstState, state* secondState) {
    int count = 0;
    // Go through each tile in the states
    for (int i = 0; i < tiles; i++)
        for (int j = 0; j < tiles; j++)
            // if left array does not match right array, then increment
            if (firstState->puzzle[i][j] != secondState->puzzle[i][j])
                count++;
    return count;
}

// To make |-x| = x
int makePos(int num){
    if(num < 0)
        return -num;
    else
        return num;
}

// The "total distance" of the eight tiles in board position from their "home squares".
int totdist(state* firstState, state* secondState) {
    int manhattanDist = 0;
    // Find the distance between first and second state in the horizontal direction plus the distance between S1 and S2 in the vertical direction.
    for (int i = 0; i < tiles; i++)
        for (int j = 0; j < tiles; j++) {
            for (int x = 0; x < tiles; x++)
                for (int y = 0; y < tiles; y++)
                    if (firstState->puzzle[i][j] == secondState->puzzle[x][y])
                        // |horizontal distance + vertical distance|
                        manhattanDist += makePos(i - x) + makePos(j - y);
        }
    return manhattanDist;
}

//H = totdist + 3*seq
int H(state *currentState, state *finalState) {
    return totdist(currentState, finalState) + 3 * seq(currentState, finalState);
}

struct HeapNode {
    void *datum;
    int element;
};

struct Heap {
    // Pointer to array of tiles
    HeapNode *puzzle;
    int heapSize;
};

void createMinHeap(Heap* heap, int size) {
    // create heap with puzzle pointer to array of tiles of size and type HeapNode
    heap->puzzle = (HeapNode *)calloc(sizeof(HeapNode *), size);
    // initialize heap size to -1
    heap->heapSize = -1;
}

void BubbleUp(Heap* heap, int heapSize) {
    // Heap is empty
    if (heapSize == 0)
        return;
    // if puzzle[heapsize].datum has a parent and it is less than its parent
    else if (heap->puzzle[heapSize].element < heap->puzzle[(heapSize - 1) / 2].element) {
        // swap child with parent and bubble up on the parent
        HeapNode temp = heap->puzzle[heapSize];
        heap->puzzle[heapSize] = heap->puzzle[(heapSize - 1) / 2];
        heap->puzzle[(heapSize - 1) / 2] = temp;
        BubbleUp(heap, (heapSize - 1) / 2);
    }
}

void BubbleDown(Heap* heap, int i) {
    int lowerIndex = i;
    // Already heapified (right children are greater than or equal to parent)
    if ((2 * i) >= heap->heapSize)
        return;
    else {
        // Right child is less than lower index
        if (heap->puzzle[2 * i + 1].element < heap->puzzle[lowerIndex].element)
            // lower index becomes index of right child
            lowerIndex = 2 * i + 1;
        // Right grandchild is less than lower index
        if (heap->puzzle[2 * i + 2].element < heap->puzzle[lowerIndex].element)
            // lower index becomes index of right grandchild
            lowerIndex = 2 * i + 2;
        // Only one element in array
        if (lowerIndex == i)
            return;
        // Swap smaller element with element at i and then bubble down on index of lower element
        else {
            HeapNode temp = heap->puzzle[i];
            heap->puzzle[i] = heap->puzzle[lowerIndex];
            heap->puzzle[lowerIndex] = temp;
            BubbleDown(heap, lowerIndex);
        }
    }
}

void *getMinimum(Heap* heap) {
    // Minimum is at the root, therefore first element in array
    void *minimum = heap->puzzle[0].datum;
    // Heap is empty
    if (heap->heapSize == -1)
        return NULL;
    // Make minimum element last element of array/heap
    else {
        heap->puzzle[0] = heap->puzzle[heap->heapSize];
        // extract min and decrement array
        heap->heapSize--;
        // Bubble down on new root
        BubbleDown(heap, 0);
    }
    return minimum;
}

void Insert(Heap* heap, void* datum, int element) {
    // Increase heap size
    heap->heapSize++;
    // For array at new heapsize, new last element will have datum and element passed by parameter
    heap->puzzle[heap->heapSize].datum = datum;
    heap->puzzle[heap->heapSize].element = element;
    // Bubble up on new heap
    BubbleUp(heap, heap->heapSize);
}

void solveByBestFS(state startState, state finalState, int heuristic) {
    // The set of all nodes in the heap at the end of all visited paths is unvisitedState
    Heap unvisitedStates;
    // All visited paths in the queue
    Queue visitedStates;
    // Initialize new heap with size tiles squared (size will be overridden)
    createMinHeap(&unvisitedStates, tiles*tiles);
    // Initialize new queue with head and tail of null
    newQueue(&visitedStates);
    // create state pointers
    state *newState;
    state *currentState;
    // root node initial state
    int numOfGeneratedNodes = 1;

    // initialize cost to 0
    finalState.cost = 0;

    // Switch case to change heuristic
    switch (heuristic) {
    case 1:
        // Heuristic is number of tiles out of place
        finalState.heuristic = seq(&startState, &finalState);
        break;
    case 2:
        // Heuristic is number of moves to reach goal state
        finalState.heuristic = totdist(&startState, &finalState);
        break;
    case 3:
        // Heuristic is H
        finalState.heuristic = H(&startState, &finalState);
        break;
    default:
        cout << "ERROR";
    }

    // Create heap putting finalState 2D array into unvisitedStates heap, with determined heuristic
    Insert(&unvisitedStates, &finalState, finalState.heuristic);
    // While unvisitedStates heap is not empty
    while (unvisitedStates.heapSize != -1) {
        // extract finalState datum from unvisitedState heap and enqueue it to visitedState queue
        currentState = (state *)(getMinimum(&unvisitedStates));
        enqueue(&visitedStates, currentState);
        // if extracted datum matches startState datum
        if (compareStates(currentState, &startState) == 1) {
            // Found solution
            printFinalState(currentState);
            break;
        }

        // 4 children -> 4 moves (Up, Down, Left, Right)
        for (int i = 1; i < 5; i++) {
            // Return new table state after doing operation i on previous table state
            newState = nextMove(currentState, i);
            // if operation is successful
            if (newState != NULL) {
                // Keep track of the number of generated nodes
                numOfGeneratedNodes++;
                // Record successful operation: 1 = up, 2=down, 3 = left. 4 = right.
                newState->move = i;
                // If it is not visited
                if (inQueue(&visitedStates, newState) == 0) {
                    // newState will be placed after currentState (as child node)
                    newState->parentNode = currentState;
                    // Current state parent node cost will be added to new state
                    newState->cost = 1 + newState->parentNode->cost;
                    // Switch case from determined heuristic
                    switch (heuristic) {
                    case 1:
                        // depth in the search space + number of tiles out of place.
                        newState->heuristic = newState->cost + seq(newState, &startState);
                        break;
                    case 2:
                        // depth in the search space + minimum number of moves to reach the goal state. 
                        newState->heuristic = newState->cost + totdist(newState, &startState);
                        break;
                    case 3:
                        // depth in the search space + the heuristic H
                        newState->heuristic = newState->cost + H(newState, &finalState);
                        break;
                    default:
                        cout << "ERROR";
                    }
                    // Create heap putting newState 2D array into unvisitedStates heap, with determined heuristic
                    Insert(&unvisitedStates, newState, newState->heuristic);
                } else
                    // Delete unused state
                    free(newState);
            }
        }
    }

    // If the unvisited heap is empty and we did not find initial state then no solution
    if (unvisitedStates.heapSize == -1)
        cout << "No final state possible." << endl;

    // Print the number of nodes for each heuristic
    switch (heuristic) {
        case 1:
            cout << "Best-First + number of tiles out of place generated " << numOfGeneratedNodes << " nodes." << endl;
            break;
        case 2:
            cout << "Best-First + minimum number of moves to reach the goal generated " << numOfGeneratedNodes << " nodes." << endl;
            break;
        case 3:
            cout << "Best-First + defined heuristic generated " << numOfGeneratedNodes << " nodes." << endl;
            break;
        default:
            cout << "ERROR";
    }
}

//======================End Best-First========================

int main()
{
    // set start and final state variables
    state startState;
    state finalState;

    cout << "Enter the initial state (-1 for empty tile): ";
    for (int i = 0; i < tiles; i++)
        for (int j = 0; j < tiles; j++)
            cin >> startState.puzzle[i][j];
    // Send user inputted start state 2D array datum to be initialized
    initializePuzzle(&startState);

    cout << "Enter the final state (-1 for empty tile): ";
    for (int i = 0; i < tiles; i++)
        for (int j = 0; j < tiles; j++)
            cin >> finalState.puzzle[i][j];
    // Send user inputted final state 2D array datum to be initialized
    initializePuzzle(&finalState);

    cout << "Starting state: " << endl;
    cout << setw(5);
    // Print start state
    printPath(startState);

    cout << endl;
    cout << "Expected final state: " << endl;
    cout << setw(5);
    printPath(finalState);

    cout << endl << endl;

    cout << "PATHS: " << endl;
    // Go through operations for BFS, DFS and Best First Search
    cout << "BREADTH FIRST SEARCH:" << endl;
    cout << setw(5);
    solveByBFS(startState, finalState);
    cout << endl << endl;
    
// ************ UNCOMMENT BELOW FOR DFS AND BEST FIRST SEARCH **************
    // cout << "DEPTH FIRST SEARCH:" << endl;
    // cout << setw(5);
    // solveByDFS(startState, finalState);
    // cout << endl << endl;

    // /* For Best first search:
    //     1 = depth in the search space + number of tiles out of place.
    //     2 = depth in the search space + minimum number of moves to reach the goal state. 
    //     3 = depth in the search space + the heuristic
    // */

    // cout << "BEST FIRST SEARCH:" << endl;
    // cout << "Depth in the search space + number of tiles out of place:" << endl;
    // cout << setw(5);
    // solveByBestFS(startState, finalState, 1);
    // cout << endl << endl;

    // cout << "Depth in the search space + minimum number of moves to reach the goal state:" << endl;
    // cout << setw(5);
    // solveByBestFS(startState, finalState, 2);
    // cout << endl << endl;

    // cout << "Depth in the search space + the heuristic:" << endl;
    // cout << setw(5);
    // solveByBestFS(startState, finalState, 3);
    // cout << endl << endl;

    return 0;
}
