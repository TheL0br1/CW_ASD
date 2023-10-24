#include <iostream>
#include<bits/stdc++.h>
#include<SFML/Graphics.hpp>
#include <sys/time.h>

#define GAME_WINDOW_SIZE_X 1500
#define GAME_WINDOW_SIZE_Y 800
#define INTERFACE_WINDOW_SIZE_Y 100

int size_x, size_y;
int apples;
int n;
int relative_x;
int relative_y;
long long start_time;

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
        rectangle.move(relative_x*this->GetPosition().first, relative_y*this->GetPosition().second +INTERFACE_WINDOW_SIZE_Y);
        window.draw(rectangle);
    }
};
int Apple::counter = 0;


class Crawler: public Entity{
private:
    int steps=0;
public:

    explicit Crawler(std::pair<int, int> position) : Entity(position) {

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
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000/n));
                }else{
                    crawler->move(window, -1, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000/n));
                }
            }
            while(crawler->GetPosition().second != x->GetPosition().second){
                if(crawler->GetPosition().second < x->GetPosition().second) {
                    crawler->move(window, 0, 1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000/n));
                }else{
                    crawler->move(window, 0, -1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(5000/n));

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
    int getCrawlerSteps(){
        return this->crawler->getSteps();
    }
};
Map* Map::pMapInstance = nullptr;




class Solution{
private:
    std::map<std::pair<std::shared_ptr<Entity>,std::shared_ptr<Entity>>, int> dist;
    std::vector<std::vector<int>> graph;
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
                if(x->GetPosition().first == y->GetPosition().first && x->GetPosition().second == y->GetPosition().second) continue;
                dist[std::make_pair(x,y)] = std::abs(x->GetPosition().first - y->GetPosition().first) + std::abs(x->GetPosition().second - y->GetPosition().second);
            }
        }

        std::set<std::shared_ptr<Entity>> UsedEntities;
        auto start = std::chrono::high_resolution_clock::now();

        dfs(firstEntity, UsedEntities,path, min);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = duration_cast<std::chrono::milliseconds>(end - start);

        //std::reverse(path.begin()+1, path.end());
        std::cout << "Algorightm time: " << duration.count() << " ms\n";


    }
    std::vector<std::shared_ptr<Entity>> dfs(std::shared_ptr<Entity> cEntity, std::set<std::shared_ptr<Entity>>& UsedEntities, std::vector<std::shared_ptr<Entity>> path1, int counter) {
        UsedEntities.insert(cEntity);
        std::set<std::shared_ptr<Entity>> TempUsed(UsedEntities);
        if(path1.size()>=Map::getInstance()->getEntities().size()){
            return path1;
        }
        path1.emplace_back(cEntity); // Додаємо поточну вершину в шлях
        for (auto& x : Map::getInstance()->getEntities()) {
            if (!TempUsed.contains(x) && (counter + dist[std::make_pair(cEntity, x)]) <= n) {
                TempUsed.insert(x);
                std::vector<std::shared_ptr<Entity>> subPath = dfs(x, UsedEntities, path1, counter + dist[std::make_pair(cEntity, x)]);
                if (subPath.size()> path.size()) {
                    path = subPath;
                }
            }
        }
        return path1;
    }
    std::vector<int> findLongestPath(std::vector<std::vector<int>>& graph, int start, int n) {
        int numVertices = graph.size();
        std::vector<std::vector<std::pair<int, int>> > paths(numVertices); // Масив, який зберігає максимальну відстань та попередню вершину.
        std::vector<bool> visited(numVertices, false);

        paths[start].push_back({0, -1}); // Початкова вершина з відстанню 0 та без попередньої вершини.

        for (int i = 0; i < numVertices; ++i) {
            for (int v = 0; v < numVertices; ++v) {
                if (!visited[v] && !paths[v].empty()) {
                    std::pair<int, int> current = paths[v].back();
                    int currentDistance = current.first;
                    int previousVertex = current.second;

                    for (int u = 0; u < numVertices; ++u) {
                        if (!visited[u] && graph[v][u] > 0) { // Якщо існує ребро
                            int newDistance = currentDistance + graph[v][u];

                            if (newDistance <= n) {
                                paths[u].push_back({newDistance, v});
                            }
                        }
                    }

                    visited[v] = true; // Позначаємо вершину, яку вже врахували.
                }
            }
        }

        int longestPathLength = 0;
        int endVertex = -1;

        // Знаходимо найдовший шлях
        for (int v = 0; v < numVertices; ++v) {
            if (!paths[v].empty()) {
                int currentDistance = paths[v].back().first;
                if (currentDistance > longestPathLength) {
                    longestPathLength = currentDistance;
                    endVertex = v;
                }
            }
        }

        // Побудова найдовшого шляху
        std::stack<int> pathStack;
        while (endVertex != -1) {
            pathStack.push(endVertex);
            endVertex = paths[endVertex].back().second;
        }

        std::vector<int> longestPath;
        while (!pathStack.empty()) {
            longestPath.push_back(pathStack.top());
            pathStack.pop();
        }

        return longestPath;
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
    sf::Font font;
    sf::Text n_text;
    sf::Text apples_text;

    if(!font.loadFromFile("../fonts/OpenSans-Regular.ttf")) {
        assert("Error loading font");
    }
    n_text.setFont(font);
    apples_text.setFont(font);
    window->setActive(true);
    relative_y = (window->getSize().y-INTERFACE_WINDOW_SIZE_Y)/size_y;
    relative_x = (window->getSize().x)/size_x;

    apples_text.move(300,0);

    while (window->isOpen())
    {
        window->clear(sf::Color::White);
        wchar_t temp[100];
        swprintf(temp, 100, L"Кількість ходів: %d", Map::getInstance()->getCrawlerSteps());
        n_text.setString(temp);
        n_text.setCharacterSize(25);
        n_text.setFillColor(sf::Color::Red);
        window->draw(n_text);
        wchar_t temp2[100];
        swprintf(temp2, 100, L"З'їв яблук: %d", apples-Map::getInstance()->getEntities().size());
        apples_text.setString(temp2);
        apples_text.setCharacterSize(25);
        apples_text.setFillColor(sf::Color::Red);
        window->draw(apples_text);
        Map::getInstance()->draw(*window);

        for (float y = INTERFACE_WINDOW_SIZE_Y; y < window->getSize().y; y += relative_y)
        {
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(0, y), sf::Color::Black),
                            sf::Vertex(sf::Vector2f(window->getSize().x, y),sf::Color::Black)
                    };
            window->draw(line, 2, sf::Lines);
        }

        for (float x = 0; x < window->getSize().x; x += relative_x)
        {
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(x, INTERFACE_WINDOW_SIZE_Y),sf::Color::Black),
                            sf::Vertex(sf::Vector2f(x, window->getSize().y),sf::Color::Black)
                    };
            window->draw(line, 2, sf::Lines);
        }




        window->display();
    }
}


int main() {
    input();
    sf::RenderWindow window(sf::VideoMode(GAME_WINDOW_SIZE_X, GAME_WINDOW_SIZE_Y+INTERFACE_WINDOW_SIZE_Y), "CW_ASD");
    window.setFramerateLimit(20);
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
