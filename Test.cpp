#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cmath>

#define right 1
#define left -1
#define up -132
#define down 132
#define startingPos 5508

std::unordered_set<std::streamoff> visitedPositions;
int count = 0;

bool inBounds(std::ifstream &file){
    if(file.tellg() == -1){
        return false;
    }
    else{
        return true;
    }
}

int rotate(int direction){
        switch(direction){
            case up:
                return right;
                break;
            case right:
                return down;
                break;
            case down:
                return left;
                break;
            case left:
                return up;
                break;
            default:
                return direction;
                break;
        }
}

bool move(std::streamoff direction, std::ifstream &file){
    std::streampos currPos = file.tellg();
    std::streampos newPos = currPos + direction;
    file.seekg(newPos, std::ios::beg); //newpos
    char ch = file.peek();
    if(ch == '#'){
        file.seekg(currPos, std::ios::beg);
        return false;
    }
    return true;
}

int main() {
    std::ifstream file("test.txt", std::ios::binary);
    std::streamoff direction = up;
    int count = 0;
    if(!file.is_open()){
        return 1;
    }
    file.seekg(startingPos, std::ios::beg);
    visitedPositions.insert(file.tellg());
    count++;
    while(true){
        int position = file.tellg();
         if (visitedPositions.find(file.tellg()) == visitedPositions.end()) {
            visitedPositions.insert(file.tellg());
            count++;
        }
        if (!move(direction,file)){
            direction = rotate(direction);
        }
        if(!inBounds(file)){
            break;
        }
    }
    std::cout<<count;

    file.close();
    return 0;
}