#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;

constexpr double G = 1.0;
constexpr double M = 1.0;

double fRand(double min, double max);

class Particle {
    private:

        double px, py; // position
        double vx, vy; // velocity
        double fx, fy; // force
        double m = M; // mass
        int id = -1;

    public:

        Particle(){}

        Particle(double px, double py, double vx, double vy, double fx, double fy, double m)
            :px(px) ,py(py)
            ,vx(vx) ,vy(vy)
            ,fx(fx) ,fy(fy)
            ,m(m){

        }

        Particle(int Nx, int Ny, int i){
            px = fRand(0, Nx);
            py = fRand(0, Ny);
            vx = fRand(0, 0.2);
            vy = fRand(0, 0.2);
            id = i;
        }

        int getId(){
            return id;
        }

        void printParticle(){
            cout << "position: (" << px << ", " << py << ")" <<endl;;
        }

        double getRadius(Particle b){
            double dx = px - b.px;
            double dy = py - b.py;
            return sqrt(dx*dx + dy*dy);
        }

        int getX(){
            return (int)px;
        }

        int getY(){
            return (int)py;
        }

        void calculateForce(Particle b){
            double F = G * (m * b.m) / getRadius(b) * getRadius(b);
            fx = F * (px - b.px) / getRadius(b);
            fy = F * (py - b.py) / getRadius(b);
        }

        void update(int Nx, int Ny){
            vx += fx / m;
            vy += fy / M;
            px += vx;
            py += vy;

            px = px < 0 ? 0 : px;
            py = py < 0 ? 0 : py;

            px = px > Nx-1 ? Nx-1 : px;
            py = py > Ny-1 ? Ny-1 : py;
        }

};

void printParticleVector(vector<Particle> particles);
void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny);

int main(){

    //room size
    int Nx = 20;
    int Ny = 20;

    //number of particles
    int N = 10;

    int timesteps = 1000;

    srand(time(NULL));

    vector<Particle> particles;
    particles.reserve(N);
    for (auto i = 0; i < N; i++){
        particles.emplace_back(Nx-1, Ny-1, i);
    }

    for (auto t = 0; t < timesteps; t++){
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    particles[i].calculateForce(particles[j]);
                }
            }
            particles[i].update(Nx, Ny);
        }

        cout << "timestep :" << t << endl;
        //printParticleVector(particles);
        printParticleVector2D(particles, N, Nx, Ny);

        // sleep to see movement happen
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }

}

double fRand(double min, double max){
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

void printParticleVector(vector<Particle> particles){
    for (size_t i = 0; i < particles.size(); i++){
        particles[i].printParticle();
    }
}

void printParticleVector2D(vector<Particle> particles, int N, int Nx, int Ny){
    vector<vector<char>> result2D(Nx, vector<char>(Ny, ' '));
    for (auto i = 0; i < N; i++){
        Particle temp = particles[i];
        result2D[temp.getX()][temp.getY()] = 'x';
    }

    for (auto i = 0; i <= Nx+1; i++){
        cout << "-";
    }
    cout << endl;

    for (auto i = 0; i < Nx; i++){
        cout << "|";
        for (auto j = 0; j < Ny; j++){
            cout << result2D[i][j];
        }
        cout << "|" << endl;
    }

    for (auto i = 0; i <= Nx+1; i++){
        cout << "-";
    }
    cout << endl;
}