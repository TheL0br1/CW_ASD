#include <iostream>
#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>

int size_x, size_y;
int apples;
int n;
int relative_x;
int relative_y;


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
    virtual void draw(sf::RenderWindow& window) = 0;
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
    void draw(sf::RenderWindow& window) override {
        sf::RectangleShape rectangle(sf::Vector2f(relative_x, relative_y));
        rectangle.setFillColor(sf::Color::Red);
        rectangle.move(relative_x*this->GetPosition().first, relative_y*this->GetPosition().second);
        window.draw(rectangle);
    }
};
int Apple::counter = 0;


class Crawler: public Entity{

public:
    explicit Crawler(std::pair<int, int> position) : Entity(position) {

    }
    void move(sf::RenderWindow& window, int x=0, int y =0) {
        this->position.first += x;
        this->position.second += y;
    }
    void draw(sf::RenderWindow& window) override{
        sf::RectangleShape rectangle(sf::Vector2f(relative_x, relative_y));
        rectangle.setFillColor(sf::Color::Green);
        rectangle.move(relative_x*this->GetPosition().first, relative_y*this->GetPosition().second);
        window.draw(rectangle);
    }
};
class Map {
private:
    std::set<std::shared_ptr<Entity>> Entities;
    static Map* pMapInstance;
    std::shared_ptr<Crawler> crawler;
    explicit Map(std::pair<int, int> ){
        srand (time(nullptr));
        crawler = std::make_shared<Crawler>(std::make_pair(0,0));

        for(int i = 0; i < apples; i+=1){
            this->addEntity(std::make_shared<Apple>(std::make_pair(rand()%size_x, rand()%size_y)));
        }
    }
public:
    static Map* getInstance(std::pair<int, int> size = {0, 0}) {
        if (pMapInstance != nullptr) {
            return pMapInstance;
        }
        pMapInstance = new Map(size);
        return pMapInstance;
    }
    void draw(sf::RenderWindow& window){
        for(auto& x:Entities){
            x->draw(window);
        }
        crawler->draw(window);
    }
    void drawPath(sf::RenderWindow& window, const std::vector<std::shared_ptr<Entity>>& path){
        for (auto &x : path) {
            while(crawler->GetPosition().first != x->GetPosition().first){
                if(crawler->GetPosition().first < x->GetPosition().first) {
                    crawler->move(window, 1, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }else{
                    crawler->move(window, -1, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            while(crawler->GetPosition().second != x->GetPosition().second){
                if(crawler->GetPosition().second < x->GetPosition().second) {
                    crawler->move(window, 0, 1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }else{
                    crawler->move(window, 0, -1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                }
            }
            this->Entities.erase(x);
        }
    }
    void addEntity(const std::shared_ptr<Entity>& e) {
        Entities.insert(e);
    }
    std::set<std::shared_ptr<Entity>> getEntities(){
        return Entities;
    }
};
Map* Map::pMapInstance = nullptr;




class Solution{
private:
    std::map<std::pair<std::shared_ptr<Entity>,std::shared_ptr<Entity>>, int> dist;
    std::vector<std::shared_ptr<Entity>> path;
public:

    explicit Solution(std::pair<int,int> cPos){
        std::shared_ptr<Entity> firstEntity;
        int min = (1<<30) - 1;
        for(auto &x : Map::getInstance()->getEntities()){
            if(std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second) < min){
                firstEntity = x;
                min = std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second);
            }
        }
        for(auto &x : Map::getInstance()->getEntities()){
            for(auto &y: Map::getInstance()->getEntities()){
                if(x->GetPosition().first == y->GetPosition().first && y->GetPosition().second == y->GetPosition().second) continue;
                dist[std::make_pair(x,y)] = std::abs(x->GetPosition().first - y->GetPosition().first) + std::abs(x->GetPosition().second - y->GetPosition().second);
            }
        }
        std::set<std::shared_ptr<Entity>> UsedEntities;
        this->path = dfs(firstEntity, UsedEntities,path, min);
        std::reverse(path.begin(), path.end());
        std::cout << "N: " << n << "\n";

    }
    std::vector<std::shared_ptr<Entity>> dfs(std::shared_ptr<Entity> cEntity, std::set<std::shared_ptr<Entity>>& UsedEntities, std::vector<std::shared_ptr<Entity>> path1, int counter) {
        UsedEntities.insert(cEntity);

        for (auto& x : Map::getInstance()->getEntities()) {
            if (!UsedEntities.contains(x) && (counter + dist[std::make_pair(cEntity, x)]) <= n) {
                std::cout << "Dist: " << dist[std::make_pair(cEntity, x)] << "\n";
                counter = counter + dist[std::make_pair(cEntity, x)]; // Зберігаємо нове значення локально
                std::vector<std::shared_ptr<Entity>> subPath = dfs(x, UsedEntities, path1, counter);
                if (subPath.size() > path1.size()) {
                    path1 = subPath; // Якщо підшлях довший, оновлюємо шлях
                }
            }
        }
        path1.emplace_back(cEntity); // Додаємо поточну вершину в шлях
        return path1;
    }
    std::vector<std::shared_ptr<Entity>> getPath(){
        return path;
    }
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



void input(){
    system("chcp 1251");
    std::cout << "Введіть розмір карти\n";
    std::cin >> size_x >> size_y;
    std::cout << "Введіть кількість яблук\n";
    std::cin >> apples;
    std::cout <<"Введіть кількість ходів\n";
    std::cin >> n;

}


void renderingThread(sf::RenderWindow* window)
{
    // activate the window's context
    window->setActive(true);
    relative_y = window->getSize().y/size_y;
    relative_x = window->getSize().x/size_x;

    // the rendering loop
    while (window->isOpen())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        window->clear(sf::Color::White);
        Map::getInstance()->draw(*window);
        for (float y = 0; y < window->getSize().y; y += relative_y)
        {
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(0, y), sf::Color::Black),
                            sf::Vertex(sf::Vector2f(window->getSize().x, y),sf::Color::Black)
                    };
            window->draw(line, 2, sf::Lines);
        }

        // Відображення вертикальних ліній сітки
        for (float x = 0; x < window->getSize().x; x += relative_x)
        {
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(x, 0),sf::Color::Black),
                            sf::Vertex(sf::Vector2f(x, window->getSize().y),sf::Color::Black)
                    };
            window->draw(line, 2, sf::Lines);
        }




        // end the current frame
        window->display();
    }
}


int main() {
    input();
    sf::RenderWindow window(sf::VideoMode(1500, 800), "My window");
    window.setFramerateLimit(10);
    auto pMap = Map::getInstance(std::make_pair(size_x,size_y));
    Solution a(std::make_pair(0,0));
    window.setActive(false);
    sf::Thread thread(&renderingThread, &window);
    thread.launch();

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            pMap->drawPath(window, a.getPath());
        }
    }

    return 0;
}
