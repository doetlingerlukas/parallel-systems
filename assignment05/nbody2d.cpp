#include <iostream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;

constexpr double G = 1.0;
constexpr double M = 1.0;

double fRand(double min, double max){
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

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

        Particle(int N, int i){
            px = fRand(0, N);
            py = fRand(0, N);
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

        void calculateForce(Particle b){
            double F = G * (m * b.m) / getRadius(b) * getRadius(b);
            fx = F * (px - b.px) / getRadius(b);
            fy = F * (py - b.py) / getRadius(b);
        }

        void update(){
            vx += fx / m;
            vy += fy / M;
            px += vx;
            py += vy;
        }

};

void printParticleVector(vector<Particle> particles){
    for (size_t i = 0; i < particles.size(); i++){
        particles[i].printParticle();
    }
}

int main(){

    int N = 5;
    int timesteps = 5;

    srand(time(NULL));

    vector<Particle> particles;
    particles.reserve(N);
    for (auto i = 0; i < N; i++){
        particles.emplace_back(N, i+100);
    }

    for (auto t = 0; t < timesteps; t++){
        
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i != j) {
                    particles[i].calculateForce(particles[j]);
                }
            }
            particles[i].update();
        }

        cout << "timestep :" << t << endl;
        printParticleVector(particles);
    }

}