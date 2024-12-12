#include <fstream> //file pointers to read and write
#include <iostream> //output to console
#include <unordered_map> //used to count number of times a position was visited
#include <unordered_set> //used to store number of unique positions

//global constants dependant on the file dimensions
#define startingPosition 5508
#define up -132
#define down 132
#define right 1
#define left -1
#define lineLength 130 //number of readable bytes per line
#define lineEnd 132 //number bytes per line (includes \r\n)
#define fileSize 17160 //total number of bytes in file

//struct definition to hold a position and a direction
struct PositionDirectionPair {
    std::streamoff position;
    std::streamoff direction;

    bool operator==(const PositionDirectionPair &other)const{
        return position == other.position && direction == other.direction;
    }
};

//hash is required to use unordered map datatype
namespace std {
    template <>
    struct hash<PositionDirectionPair> {
        size_t operator()(const PositionDirectionPair &pair)const {
            return hash<std::streamoff>()(pair.position)^
                hash<std::streamoff>()(pair.direction);
        }
    };
}


std::fstream file("puzzle.txt", std::ios::binary | std::ios::in | std::ios::out);
bool escaped = false;
char obstacle = '#';
std::streamoff direction = up;
std::unordered_set<std::streamoff> uniquePositions;
std::unordered_set<std::streamoff> validObstaclePositions;


void getFileSize(){
    std::ifstream file("puzzle.txt", std::ios::binary | std::ios::ate);
     std::streamoff size = file.tellg();
    file.close();

    std::cout<<"File size: " << size << "bytes";
}

//guard rotates 90 degrees right if met with an obstacle
std::streamoff rotate(std::streamoff direction){
        switch(direction){
            case up:
                return right;
            case right:
                return down;
            case down:
                return left;
            case left:
                return up;
            default:
                return direction;
        }
}

bool canMove(std::streamoff direction){
    std::streampos originalPosition = file.tellg();
    std::streampos newPosition = originalPosition + direction;

    //if new position is out of bounds, seeking to it will throw an error
    file.seekg(newPosition, std::ios::beg);
    
    //errors are handled here, if the pointer leaves the file, the guard left the maze
    if(file.fail()){
        file.clear();
        escaped = true;
        return true;
    }

    //another check to see if the new position is out of bounds of the file
    if(newPosition < 0 || newPosition >= fileSize){
        file.clear();
        escaped = true;
        return true;
    }

    //if not out of bounds, read the character
    char c = file.peek();

    //character escaped on the left or right side
    if(c == '\001' || c == '\r'){
        escaped = true;
        return true;
    }

    //move pointer back
    file.seekg(originalPosition);
    
    return (c != obstacle);
}

void move(std::streamoff direction){
    file.seekg(direction, std::ios::cur);
}

void restartMaze(){
    escaped = false;
    direction = up;
    file.seekg(startingPosition);
}

void solvePartOne(){
     if(file.fail()){
        file.close();
        std::cout<<"Failure to open file.\n";
    }
    
    //move pointer to guard's position
    file.seekg(startingPosition);
    //add the starting position to the unordered map
    uniquePositions.insert(file.tellg());

    while(!escaped){
        if(file.fail()){
            std::cout<<"Failure during maze run. Exiting...\n";
            break;
        }

        if(canMove(direction)){
            move(direction);
        } else {
            direction = rotate(direction);
        }
        if(!escaped){
            //if still in the maze, add the new position direction pair to the list
            //if escaped squares were added, errors would arise
            uniquePositions.insert(file.tellg());
        }
    }

    std::cout<<"Answer for part one: " << uniquePositions.size() << "\n\n";
}

void solvePartTwo(){
    
    std::cout<<"Running the maze "<< uniquePositions.size() - 1 << " times. Please wait...\n\n";

    int numOfObstacles = 0;

    //remove the starting position from the list, no need to add an obstacle on top of the guard
    uniquePositions.erase(5508);
    
    //for loop to go through every position in the previous list
    for(const auto &entry : uniquePositions){

        //list to keep track of visited positions, the direction, and how many times visited
        std::unordered_map<PositionDirectionPair, int> loopTestPositions;

        //move write pointer to the new position
        file.seekp(entry);
        //replace the open space with an obstacle
        file.put(obstacle);
        file.flush();

        //reset escaped,direction, and read pointer
        restartMaze();
        //add position to list
        loopTestPositions[{file.tellg(), direction}]++;

        while(!escaped){
            if(file.fail()){
            std::cout<<"Failure during maze run. Exiting...\n";
            break;
            }

            if(canMove(direction)){
                //after moving, break if escaped. Do not want to add the position to the list
                if(escaped){
                    break;
                }
                move(direction);
            } else {
                direction = rotate(direction);
            }

            loopTestPositions[{file.tellg(), direction}]++;

            //if guard visits a position facing the same direction twice, he is in a loop
            if(loopTestPositions[{file.tellg(), direction}] > 1){
                //add position to the list
                validObstaclePositions.insert(entry);
                break;
            }

            if(file.tellg() >= fileSize){
                std::cout<<"Exceeding filesize. Exiting...\n";
                break;
            }
        }

        //replace the object back to it's original state
        file.seekp(entry);
        file.put('.');
        file.flush();
    }

    std::cout<<"Answer for part two: "<<validObstaclePositions.size()<<"\n";
    file.close();
}

int main(){
    solvePartOne();
    solvePartTwo();
    return 0;
}