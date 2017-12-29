#include <iostream>
#include <fstream>
#include <cassert>
#include <tuple>
#include <regex>
#include <algorithm>
#include <sstream>
#include <set>

using namespace std;

using Vector = tuple<int, int, int>;

enum { position, velocity, acceleration, index };
using Particle = tuple<Vector, Vector, Vector, int>;

struct Particle_less
{
    bool operator()(const Particle& a, const Particle& b) const
    { 
        return get<position>(a) < get<position>(b);
    }
};

using Particle_multiset = multiset<Particle, Particle_less>;
using Particle_set = set<Particle, Particle_less>;

Vector operator+(const Vector& a, const Vector& b)
{
    return make_tuple(  get<position>(a) + get<position>(b),
                        get<velocity>(a) + get<velocity>(b),
                        get<acceleration>(a) + get<acceleration>(b));
}


Particle parse_particle(const string& s, int index)
{
    static regex pattern(   "p=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, "
                            "v=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>, "
                            "a=<(-?[0-9]+),(-?[0-9]+),(-?[0-9]+)>");
    smatch sm;
    regex_search(s, sm, pattern);
    assert(sm.size() == 3 * 3 + 1);

    return make_tuple(  make_tuple(stoi(sm[1].str()), stoi(sm[2].str()), stoi(sm[3].str())),
                        make_tuple(stoi(sm[4].str()), stoi(sm[5].str()), stoi(sm[6].str())),
                        make_tuple(stoi(sm[7].str()), stoi(sm[8].str()), stoi(sm[9].str())),
                        index);
}

template <typename Stream>
auto parse_particles(Stream& stream)
{
    Particle_multiset particles;
    string input;
    int counter = 0;
    while (getline(stream, input) && !input.empty()) {
        particles.insert(parse_particle(input, counter++)); 
    } 

    return particles;
}

auto integrate_particle(const Particle& particle)
{
    Particle particle_new(particle);
    get<velocity>(particle_new) = get<velocity>(particle) + get<acceleration>(particle); 
    get<position>(particle_new) = get<position>(particle) + get<velocity>(particle_new); 
    get<acceleration>(particle_new) = get<acceleration>(particle);
    return particle_new; 
}

auto integrate(const Particle_multiset& particles)
{
    Particle_multiset new_particles;
    for_each(particles.cbegin(), particles.cend(), [&new_particles](const Particle& particle) { 
        new_particles.insert(integrate_particle(particle));
    });

    return new_particles;
}

auto integrate(const Particle_set& particles)
{
    Particle_set colliding;
    Particle_set new_particles;
    for_each(particles.cbegin(), particles.cend(), [&new_particles, &colliding](const Particle& particle) { 
        auto new_particle = integrate_particle(particle);
        if (!new_particles.insert(new_particle).second) {
            colliding.insert(new_particle); 
        }
    });

    for_each(colliding.cbegin(), colliding.cend(), [&new_particles](const Particle& p) { new_particles.erase(p);});
    return new_particles;
}

template <typename Particles>
auto integrate(Particles particles, int iterations)
{ 
    while (0 < iterations--) { 
        particles = integrate(particles);
    }

    return particles;
}

unsigned int manhattan_distance(const Vector& a, const Vector& b = make_tuple(0, 0, 0))
{
    return abs(get<0>(a) - get<0>(b)) + abs(get<1>(a) - get<1>(b)) + abs(get<2>(a) - get<2>(b)); 
}

template <typename Particles>
auto find_closest(const Particles& particles, const Vector& point = make_tuple(0, 0, 0))
{ 
    return min_element( particles.cbegin(), 
                        particles.cend(), 
                        [&point](const Particle& a, const Particle& b) { 
                            return manhattan_distance(get<position>(a), point) < manhattan_distance(get<position>(b), point); 
                        });
}

void test()
{
    {
        stringstream ss("p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>\n"
                        "p=<4,0,0>, v=<0,0,0>, a=<-2,0,0>\n");

        auto particles = parse_particles(ss);
        integrate(particles, 3);
        assert(find_closest(particles) == particles.cbegin());
    }

    {
        stringstream ss("p=<-6,0,0>, v=<3,0,0>, a=<0,0,0>\n"
                        "p=<-4,0,0>, v=<2,0,0>, a=<0,0,0>\n"
                        "p=<-2,0,0>, v=<1,0,0>, a=<0,0,0>\n"
                        "p=<3,0,0>, v=<-1,0,0>, a=<0,0,0>\n");
        auto particles = parse_particles(ss);
        Particle_set p_unique(particles.cbegin(), particles.cend());
        p_unique = integrate(p_unique, 3);
        assert(p_unique.size() == 1);
    }
}

int main()
{
    test();

    ifstream file("input/aoc_20.txt");
    if (file) { 
        Particle_multiset particles = parse_particles(file);
        Particle_set particles_unique(particles.cbegin(), particles.cend()); 
        particles = integrate(particles, 500); // Iterations chosen based on a scientifically proven method called trial and error.
        cout << "Part 1: " << get<index>(*find_closest(particles)) << "\n"; 

        particles_unique = integrate(particles_unique, 500);
        cout << "Part 2: " << particles_unique.size() << "\n";
    }

    return 0;
}
