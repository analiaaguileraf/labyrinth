#include <iostream> // entrada y salida de datos
#include <vector> // permite visualizar vectores dinamicos
#include <queue> // Para usar colas, necesarias en la búsqueda en anchura (BFS)
#include <algorithm> // Funciones de manipulación de colecciones, como barajar
#include <random>    // Funciones de generación de números aleatorios

// Estructura para representar un punto con coordenadas x e y
struct Point {  // se utiliza para almacenar coordenadas. No se usan punteros para su manipulación, sino que se pasan por valor o referencia.
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

// Esta funcion recorre e imprime cada celda del laberinto.
void printMaze(const std::vector<std::vector<char>>& maze) { // VECTOR DE VECTORES
    for (const auto& row : maze) { //Un ciclo que pasa por cada fila del laberinto.
        for (const auto& cell : row) { //Otro ciclo dentro del primero, que pasa por cada celda de la fila.
            std::cout << cell << ' '; // Imprime cada celda seguida de un espacio.
        }
        std::cout << '\n';  //Al final de cada fila, imprime un salto de línea para pasar a la siguiente.
    }
}

// Función para generar el laberinto usando Backtracking
void generateMaze(std::vector<std::vector<char>>& maze, int x, int y) {
    int dx[] = { -1, 1, 0, 0 };  // Desplazamientos posibles (arriba, abajo, izquierda, derecha).
    int dy[] = { 0, 0, -1, 1 };
    std::vector<int> directions = { 0, 1, 2, 3 };  // Baraja las direcciones para generar un laberinto aleatorio.
    
    // Creamos un generador de números aleatorios
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(directions.begin(), directions.end(), rng); //Es una función para mezclar aleatoriamente los elementos de un vector.

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[directions[i]] * 2;
        int ny = y + dy[directions[i]] * 2;

        if (nx >= 0 && nx < maze[0].size() && ny >= 0 && ny < maze.size() && maze[ny][nx] == '|') {
            maze[ny][nx] = ' '; // La función marca como camino (' ') las celdas visitadas y llama recursivamente para continuar el proceso.
            maze[y + dy[directions[i]]][x + dx[directions[i]]] = ' ';
            generateMaze(maze, nx, ny);
        }
    }
}

// Verifica si el punto está dentro de los límites y si no es una pared ('|') y no ha sido visitado.
bool isValid(const std::vector<std::vector<char>>& maze, int x, int y, std::vector<std::vector<bool>>& visited) {
    return x >= 0 && x < maze[0].size() && y >= 0 && y < maze.size() && maze[y][x] != '|' && !visited[y][x];
}

// Función para resolver el laberinto usando BFS (busqueda primero en anchura)

bool solveMaze(std::vector<std::vector<char>>& maze) {
    std::queue<Point> q;  // MI COLA: tipo de lista que se caracteriza por conforme sus elementos van llegandO, ejemplo primer elemento en llegar, primer elemento que sera atendido

// se usa para marcar las celdas del laberinto que ya han sido visitadas durante la búsqueda.   
    std::vector<std::vector<bool>> visited(maze.size(), std::vector<bool>(maze[0].size(), false));
    std::vector<std::vector<Point>> parent(maze.size(), std::vector<Point>(maze[0].size(), Point(-1, -1)));

    int dx[] = { -1, 1, 0, 0 };  //array 
    int dy[] = { 0, 0, -1, 1 }; 

    q.push(Point(1, 0)); // Empieza desde la entrada
    visited[0][1] = true;  // marca los puntos visitados

    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        if (p.x == maze[0].size() - 2 && p.y == maze.size() - 1) { // Llegamos a la salida
            while (!(p.x == 1 && p.y == 0)) {
                maze[p.y][p.x] = '*'; // La función recorre el laberinto buscando la salida y marca el camino
                p = parent[p.y][p.x]; // Guarda el padre de cada punto para reconstruir el camino.
            }
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (isValid(maze, nx, ny, visited)) {
                q.push(Point(nx, ny));
                visited[ny][nx] = true;
                parent[ny][nx] = p;
            }
        }
    }

    return false;
}

// Función principal
int main() {
    int width, height;
    std::cout << "Introduce el ancho del laberinto (debe ser un numero impar): ";
    std::cin >> width;
    std::cout << "Introduce la altura del laberinto (debe ser un numero impar): ";
    std::cin >> height;

    if (width % 2 == 0 || height % 2 == 0) {
        std::cerr << "El ancho y la altura deben ser números impares.\n";
        return 1;
    }



    std::vector<std::vector<char>> maze(height, std::vector<char>(width, '|'));
    maze[1][1] = ' ';
    generateMaze(maze, 1, 1);

    maze[0][1] = 'S';
    maze[height - 1][width - 2] = 'E';

    printMaze(maze);
    std::cout << "Resolving maze...\n";

    if (solveMaze(maze)) {
        std::cout << "Maze solved:\n";
        printMaze(maze);
    } else {
        std::cout << "No solution found.\n";
    }

    return 0;
}
