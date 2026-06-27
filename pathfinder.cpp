#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>


using namespace std;

const int ROWS = 25;
const int COLS = 25;
const int CELL_SIZE = 30;

enum CellState { EMPTY, WALL, START, END, VISITED, PATH };

struct Node {
    int row, col;
    float g = numeric_limits<float>::infinity();
    float h = 0;
    float f = numeric_limits<float>::infinity();
    Node* parent = nullptr;
    CellState state = EMPTY;
};

vector<vector<Node>> grid(ROWS, vector<Node>(COLS));
Node* startNode = nullptr;
Node* endNode = nullptr;
bool running = false;

float heuristic(Node* a, Node* b) {
    return abs(a->row - b->row) + abs(a->col - b->col);
}

vector<Node*> getNeighbors(Node* node) {
    vector<Node*> neighbors;
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    for (int i = 0; i < 4; i++) {
        int r = node->row + dr[i];
        int c = node->col + dc[i];
        if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
            if (grid[r][c].state != WALL)
                neighbors.push_back(&grid[r][c]);
        }
    }
    return neighbors;
}

void reconstructPath(Node* end) {
    Node* cur = end->parent;
    while (cur && cur != startNode) {
        cur->state = PATH;
        cur = cur->parent;
    }
}

void runAStarStep(priority_queue<pair<float, Node*>,
    vector<pair<float, Node*>>,
    greater<pair<float, Node*>>>& openSet) {

    if (openSet.empty()) return;

    Node* current = openSet.top().second;
    openSet.pop();

    if (current == endNode) {
        reconstructPath(endNode);
        running = false;
        return;
    }

    if (current != startNode)
        current->state = VISITED;

    for (Node* neighbor : getNeighbors(current)) {
        float tempG = current->g + 1;
        if (tempG < neighbor->g) {
            neighbor->parent = current;
            neighbor->g = tempG;
            neighbor->h = heuristic(neighbor, endNode);
            neighbor->f = neighbor->g + neighbor->h;
            openSet.push({neighbor->f, neighbor});
        }
    }
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned>(COLS * CELL_SIZE),
                       static_cast<unsigned>(ROWS * CELL_SIZE)}),
        "A* Pathfinding Visualizer");

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            grid[i][j].row = i;
            grid[i][j].col = j;
        }

    priority_queue<pair<float, Node*>,
        vector<pair<float, Node*>>,
        greater<pair<float, Node*>>> openSet;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                int r = mouse->position.y / CELL_SIZE;
                int c = mouse->position.x / CELL_SIZE;

                if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                        startNode = &grid[r][c];
                        startNode->state = START;
                        startNode->g = 0;
                    } 
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                        endNode = &grid[r][c];
                        endNode->state = END;
                    } 
                    else {
                        grid[r][c].state = WALL;
                    }
                }
            }

            if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::A && startNode && endNode) {
                    running = true;
                    startNode->h = heuristic(startNode, endNode);
                    startNode->f = startNode->h;
                    openSet.push({startNode->f, startNode});
                }

                if (key->code == sf::Keyboard::Key::R) {
                    grid.assign(ROWS, vector<Node>(COLS));
                    startNode = endNode = nullptr;
                    running = false;
                }
            }
        }

        if (running)
            runAStarStep(openSet);

        window.clear(sf::Color::White);

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));

                switch (grid[i][j].state) {
                    case EMPTY: cell.setFillColor(sf::Color::White); break;
                    case WALL: cell.setFillColor(sf::Color::Black); break;
                    case START: cell.setFillColor(sf::Color::Red); break;
                    case END: cell.setFillColor(sf::Color::Yellow); break;
                    case VISITED: cell.setFillColor(sf::Color::Green); break;
                    case PATH: cell.setFillColor(sf::Color::Blue); break;
                }

                window.draw(cell);
            }
        }

        window.display();
    }
    return 0;
}
