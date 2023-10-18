#include <iostream>
#include<bits/stdc++.h>

class Entity {
protected:
    std::pair<int, int> position;
public:
    explicit Entity(std::pair<int, int> position_) : position(std::move(position_)) {
        //add to drawer later
    }

    std::pair<int, int> GetPosition() {
        return position;
    }
    virtual void draw() = 0;
};

class Apple : public Entity {
public:
    static int counter;
    //sfml texture
    explicit Apple(std::pair<int, int> position) : Entity(position) {
        counter++;
    }
    ~Apple() {
        //delete from drawer later
        counter--;
    }
    void draw() override {
        //  implement the draw method for Apple
    }
};
int Apple::counter = 0;


class Crawler: public Entity{
public:
    void move(std::pair<int,int> dir) {
        this->position.first += dir.first;
        this->position.second += dir.second;
    }
    void draw() override{

    }
};
class Map {
private:
    std::set<std::shared_ptr<Entity>> Entities;
    static Map* pMapInstance;
    explicit Map(std::pair<int, int> size) {
    }
public:
    static Map* getInstance(std::pair<int, int> size = {0, 0}) {
        if (pMapInstance != nullptr) {
            return pMapInstance;
        }
        pMapInstance = new Map(size);
        return pMapInstance;
    }
    void addEntity(const std::shared_ptr<Entity>& e) {
        Entities.insert(e);
    }
    std::set<std::shared_ptr<Entity>> getEntities(){
        return Entities;
    }
};


class Solution{

public:
    std::map<std::pair<std::shared_ptr<Entity>,std::shared_ptr<Entity>>, int> dist;


    explicit Solution(){
        for(auto &x : Map::getInstance()->getEntities()){
            for(auto &y: Map::getInstance()->getEntities()){
                if(x->GetPosition().first == y->GetPosition().first && y->GetPosition().second == y->GetPosition().second) continue;
                dist[std::make_pair(x,y)] = std::abs(x->GetPosition().first - y->GetPosition().first) + std::abs(x->GetPosition().second - y->GetPosition().second);
            }
        }
        std::cout << " ";
    }
    void dfs();
                void print(){
            std::cout << "DISTS:\n";
            for(auto& x: Map::getInstance()->getEntities()) {
            for (auto &y: Map::getInstance()->getEntities()) {
                if(x==y) { continue;}
                std::cout << x->GetPosition().first << " " << x->GetPosition().second << ";";
                std::cout << y->GetPosition().first << " " << y->GetPosition().second << ";";
                std::cout << dist[std::make_pair(x,y)] << "\n";
            }
        }
    }
};

Map* Map::pMapInstance = nullptr;

int size_x, size_y;
int apples;
int n;


void input(){
    system("chcp 1251");
    std::cout << "Введіть розмір карти\n";
    std::cin >> size_x >> size_y;
    std::cout << "Введіть кількість яблук\n";
    std::cin >> apples;
    std::cout <<"Введіть кількість ходів\n";
    std::cin >> n;

}
void solution(){
    input();
    Map* pMap = Map::getInstance({size_x,size_y});
    srand (time(nullptr));
    for(int i = 0; i < apples; i+=1){
        pMap->addEntity(std::make_shared<Apple>(std::make_pair(rand()%size_x, rand()%size_y)));
    }
    for(auto& x:pMap->getEntities()){
        std::cout << x->GetPosition().first << " " << x->GetPosition().second << "\n";
    }
    Solution a;
}

int main() {
    solution();

    return 0;
}
