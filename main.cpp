
#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include <sys/time.h>
#include <chrono>


#define GAME_WINDOW_SIZE_X 1500
#define GAME_WINDOW_SIZE_Y 800
#define INTERFACE_WINDOW_SIZE_Y 100
#define GAME_TIME 600000.0;
int size_x, size_y;
int apples;
int n;
int relative_x;
int relative_y;
sf::RenderWindow *pWindow;
double n_relative;
double time1 = GAME_TIME;
bool blocked = false;
sf::Clock* clock1;

class Entity {
protected:
    std::pair<int, int> position;
public:
    explicit Entity(std::pair<int, int> position_) : position(std::move(position_)) {
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
        rectangle.move(relative_x*this->GetPosition().first, relative_y*this->GetPosition().second +INTERFACE_WINDOW_SIZE_Y);
        window.draw(rectangle);
    }
};
int Apple::counter = 0;


class Crawler: public Entity{
private:
    int steps=0;
public:

    explicit Crawler(std::pair<int, int> position) : Entity(position), steps(0) {

    }
    void move(sf::RenderWindow& window, int x=0, int y =0) {
        this->position.first += x;
        this->position.second += y;
        steps++;
    }
    void draw(sf::RenderWindow& window) override{
        sf::RectangleShape rectangle(sf::Vector2f(relative_x, relative_y));
        rectangle.setFillColor(sf::Color::Green);
        rectangle.move(relative_x*this->GetPosition().first, relative_y*this->GetPosition().second + INTERFACE_WINDOW_SIZE_Y);
        window.draw(rectangle);
    }
    int getSteps(){
        return steps;
    }
};
class Map {
private:
    std::set<std::shared_ptr<Entity>> Entities;
    static Map* pMapInstance;
    std::shared_ptr<Crawler> crawler;
    int collisionCount =0;
    explicit Map(std::pair<int, int> ){
        collisionCount =0;
        srand (time(nullptr));
        crawler = std::make_shared<Crawler>(std::make_pair(0,0));
        std::ifstream input("1.txt");
        if(input.is_open()){
            std::string line;
            while (std::getline(input, line)) {
                std::istringstream iss(line);
                int x, y;

                if (iss >> x && iss.ignore(1) && iss >> y) {
                    this->addEntity(std::make_shared<Apple>(std::make_pair(x, y)));
                } else {
                    std::cerr << "Error reading line: " << line << std::endl;
                }
            }
            apples = Entities.size();
            n_relative =((double)n/((double)size_x/4.0));


            input.close();
        }
        else {
            std::cerr << "Cannot read file, so just random\n";
            for (int i = 0; i < apples; i += 1) {
                this->addEntity(std::make_shared<Apple>(std::make_pair(rand() % size_x, rand() % size_y)));
            }
            collisionCount=apples-Entities.size();

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
    int getCollisionCount(){
        return collisionCount;
    }
    void
    resetGame(){
        collisionCount =0;
        srand (time(nullptr));
        crawler = std::make_shared<Crawler>(std::make_pair(0,0));
        time1 = GAME_TIME;
        Entities.clear();
        std::ifstream input("1.txt");
        if(input.is_open()){
            std::string line;
            while (std::getline(input, line)) {
                std::istringstream iss(line);
                int x, y;

                if (iss >> x && iss.ignore(1) && iss >> y) {
                    this->addEntity(std::make_shared<Apple>(std::make_pair(x, y)));
                } else {
                    std::cerr << "Error reading line: " << line << std::endl;
                }
            }
            apples = getEntities().size();
            n_relative =((double)n/((double)size_x/4.0));
            input.close();
        }
        else {
            std::cerr << "Cannot read file, so just random\n";
            for (int i = 0; i < apples; i += 1) {
                this->addEntity(std::make_shared<Apple>(std::make_pair(rand() % size_x, rand() % size_y)));
            }
            collisionCount=apples-Entities.size();

        }

    }
    void draw(sf::RenderWindow& window){
        for(auto& x:Entities){
            x->draw(window);
        }
        crawler->draw(window);
    }

    void drawPath(const std::vector<std::shared_ptr<Entity>>& path){

        for (auto &x : path) {
            while(crawler->GetPosition().first != x->GetPosition().first){
                if(crawler->GetPosition().first < x->GetPosition().first) {
                    crawler->move(*pWindow, 1, 0);
                }else{
                    crawler->move(*pWindow, -1, 0);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            }
            while(crawler->GetPosition().second != x->GetPosition().second){
                if(crawler->GetPosition().second < x->GetPosition().second) {
                    crawler->move(*pWindow, 0, 1);

                }else{
                    crawler->move(*pWindow, 0, -1);

                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

                //time1 -= clock1->restart().asMilliseconds();
            }
            this->Entities.erase(x);
        }
    }
    void addEntity(const std::shared_ptr<Entity>& e) {
        for (auto &x : Entities) {
            if(x->GetPosition().first == e->GetPosition().first && x->GetPosition().second == e->GetPosition().second){
                return;
            }
        }
        Entities.insert(e);
    }
    std::set<std::shared_ptr<Entity>> getEntities(){
        return Entities;
    }
    int getCrawlerSteps(){
        return this->crawler->getSteps();
    }
};
Map* Map::pMapInstance = nullptr;


void drawWrap( const std::vector<std::shared_ptr<Entity>>& path){
    Map::getInstance()->drawPath(path);
}


class Solution{
private:
    std::vector<std::shared_ptr<Entity>> pathSol1;
    std::vector<std::shared_ptr<Entity>> path;
    std::map<std::pair<std::shared_ptr<Entity>,std::shared_ptr<Entity>>, int> dist;
    std::set<std::shared_ptr<Entity>> UsedEntities1;


public:

    explicit Solution(){
    }

    std::vector<std::shared_ptr<Entity>> simple() {
        pathSol1.clear();
        std::pair<int,int> cPos(0,0);
        int res=0;
        std::shared_ptr<Entity> currentEntity;
        int min = (1<<30) - 1;
        for(auto &x : Map::getInstance()->getEntities()){
            if(std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second) < min){
                currentEntity = x;
                min = std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second);
            }
        }
        pathSol1.emplace_back(currentEntity);
        res+=min;
        std::set<std::shared_ptr<Entity>> UsedEntities;
        UsedEntities.insert(currentEntity);
        while(UsedEntities.size()!=Map::getInstance()->getEntities().size() && res<=n){
            int min = (1<<30) - 1;
            std::shared_ptr<Entity> temp;
            bool flag = false;
            for(auto &x : Map::getInstance()->getEntities()){
                int dist =std::abs(currentEntity->GetPosition().first - x->GetPosition().first) + std::abs(currentEntity->GetPosition().second - x->GetPosition().second);

                if(dist < min
                    && dist+res < n
                    && !UsedEntities.contains(x)){
                    min =dist;
                    temp = x;
                    flag = true;
                }
            }
            if(!flag){
                break;
            }
            currentEntity = temp;
            res+=min;
            pathSol1.emplace_back(currentEntity);
            UsedEntities.insert(currentEntity);
        }

        return pathSol1;
    }

    std::vector<std::shared_ptr<Entity>> dfs(std::shared_ptr<Entity> cEntity, std::set<std::shared_ptr<Entity>> UsedEntities, std::vector<std::shared_ptr<Entity>> path1, int counter) {
        UsedEntities.insert(cEntity);
        std::set<std::shared_ptr<Entity>> TempUsed(UsedEntities);
        if(path1.size()>=Map::getInstance()->getEntities().size()){
            return path1;
        }
        path1.emplace_back(cEntity); // Додаємо поточну вершину в шлях
        for (auto& x : Map::getInstance()->getEntities()) {
            if (!TempUsed.contains(x) && (counter + dist[std::make_pair(cEntity, x)]) <= (n/n_relative)) {
                TempUsed.insert(x);
                std::vector<std::shared_ptr<Entity>> subPath = dfs(x, UsedEntities, path1, counter + dist[std::make_pair(cEntity, x)]);
                if (subPath.size()> path.size()) {
                    path = subPath;
                }
            }
        }
        return path1;
    }
    std::vector<std::shared_ptr<Entity>> dfsWrap(){

        std::shared_ptr<Entity> firstEntity;
        int min = (1<<30) - 1;
        std::pair<int,int> cPos;
        if(path.empty()){
            cPos = std::make_pair(0,0);
        }else {
            cPos = std::make_pair(path[path.size() - 1]->GetPosition().first,
                                                      path[path.size() - 1]->GetPosition().first);
            path.clear();
        }
        for(auto &x : Map::getInstance()->getEntities()){
            if(!UsedEntities1.contains(x) && std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second) < min){
                firstEntity = x;
                min = std::abs(cPos.first - x->GetPosition().first) + std::abs(cPos.second - x->GetPosition().second);
            }
        }
        for(auto &x : Map::getInstance()->getEntities()){
            for(auto &y: Map::getInstance()->getEntities()){
                if(!UsedEntities1.contains(x) && x->GetPosition().first == y->GetPosition().first && x->GetPosition().second == y->GetPosition().second) continue;
                dist[std::make_pair(x,y)] = std::abs(x->GetPosition().first - y->GetPosition().first) + std::abs(x->GetPosition().second - y->GetPosition().second);
            }
        }
        for (int i = 0; i < n_relative; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            if(i==0) {
                dfs(firstEntity, UsedEntities1, path, min);
            }
            else{
                dfs(firstEntity, UsedEntities1, path, 0);
            }
            for(auto& x : path){
                UsedEntities1.insert(x);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Algorightm time: " << duration.count() << " ms\n";
            sf::Thread thread1(drawWrap, path);
            thread1.launch();
            thread1.wait();
            //Map::getInstance()->drawPath(path);
            firstEntity = path[path.size()-1];
            path.clear();
        }
        return path;

    }

    std::vector<std::shared_ptr<Entity>> getPath(){
        return pathSol1;
    }

    void restartGame(){
        pathSol1.clear();
        path.clear();
        dist.clear();
        UsedEntities1.clear();
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
    n_relative =((double)n/((double)size_x/2.0));

}


void renderingThread(sf::RenderWindow* window)
{
    sf::Font font;
    sf::Text n_text;
    sf::Text apples_text;
    sf::Text time_text;

    if(!font.loadFromFile("../fonts/OpenSans-Regular.ttf")) {
        assert("Error loading font");
    }
    n_text.setFont(font);
    apples_text.setFont(font);
    time_text.setFont(font);

    window->setActive(true);
    relative_y = (window->getSize().y-INTERFACE_WINDOW_SIZE_Y)/size_y;
    relative_x = (window->getSize().x)/size_x;

    apples_text.move(300,0);
    time_text.move(500,0);
    while (window->isOpen())
    {
        if(blocked){
            continue;
        }
        window->clear(sf::Color::White);
        wchar_t temp[100];
        swprintf(temp, 100, L"Кількість ходів: %d", Map::getInstance()->getCrawlerSteps());
        n_text.setString(temp);
        n_text.setCharacterSize(25);
        n_text.setFillColor(sf::Color::Red);
        window->draw(n_text);
        wchar_t temp2[100];
        swprintf(temp2, 100, L"З'їв яблук: %d", apples-Map::getInstance()->getEntities().size()-Map::getInstance()->getCollisionCount());
        apples_text.setString(temp2);
        apples_text.setCharacterSize(25);
        apples_text.setFillColor(sf::Color::Red);
        window->draw(apples_text);
        wchar_t temp3[100];
        swprintf(temp3, 100, L"Час в секундах: %f", time1/1000.0f);
        time_text.setString(temp3);
        time_text.setCharacterSize(25);
        time_text.setFillColor(sf::Color::Red);
        window->draw(time_text);
        Map::getInstance()->draw(*window);

        window->display();
    }
}


int main() {
    input();
    sf::RenderWindow window(sf::VideoMode(GAME_WINDOW_SIZE_X, GAME_WINDOW_SIZE_Y+INTERFACE_WINDOW_SIZE_Y), "CW_ASD");
    pWindow = &window;
    window.setFramerateLimit(10);
    auto pMap = Map::getInstance(std::make_pair(size_x,size_y));
    window.setActive(false);
    sf::Thread thread(&renderingThread, &window);
    thread.launch();
    Solution a;
    clock1 = new sf::Clock();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            clock1->restart().asMilliseconds();
            auto path = a.dfsWrap();
            time1 -= clock1->restart().asMilliseconds();
            std::cout << time1/1000.0f << '\n';

        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            clock1->restart().asMilliseconds();
            auto path = a.simple();
            pMap->drawPath(path);
            time1 -= clock1->restart().asMilliseconds();
            std::cout << time1/1000.0f << '\n';


        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
            blocked = true;
            pMap->resetGame();
            a.restartGame();
            blocked = false;
        }

    }

    return 0;
}
