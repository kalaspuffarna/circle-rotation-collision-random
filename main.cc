#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <stdexcept>

using namespace sf;

bool collision(CircleShape const & c1, CircleShape const & c2)
{
    return static_cast<int>(std::sqrt(std::pow(static_cast<int>(c1.getPosition().x - c2.getPosition().x), 2) + std::pow(static_cast<int>(c1.getPosition().y - c2.getPosition().y), 2))) <= c1.getRadius() * 2;
}

void setweaponpos(CircleShape & wpn, std::vector<Vector2i> & plr_movement)
{
    int speed{5};
    Vector2f movedir;
    for(auto i : plr_movement)
    {
        wpn.move(i.x,i.y);
    }
    movedir.x = sin((M_PI/180)*wpn.getRotation()) * speed;
    movedir.y = cos((M_PI/180)*wpn.getRotation()) * speed;
    wpn.move(movedir.x, movedir.y);
    wpn.rotate(2);
    plr_movement.clear();
}

int main()
{
    try{
    RenderWindow window(sf::VideoMode(1000, 500), "Circle Rotation collisions test", sf::Style::Titlebar | sf::Style::Close);
    double r{50};
    double sr{25};
    
    std::ifstream ifs{"seed.txt"};
    if(!ifs.is_open()) throw std::logic_error("ingen seed.txt fil");
    unsigned int seed{};
    ifs >> seed;
    int random;
    srand(seed);
    
    CircleShape weapon(sr);
    CircleShape plr(r);
    CircleShape npc(r);

    plr.setOrigin(r,r);
    npc.setOrigin(r,r);
    weapon.setOrigin(sr,sr);

    plr.setPosition(100, 200);
    npc.setPosition(200, 200);
    weapon.setPosition(plr.getPosition().x - r*3, plr.getPosition().y);  //vet inte hur man kan sätta så man får mittpunkten i rotationen att vara spelaren >:(
    std::vector<std::pair<Keyboard::Key, Vector2i>> v
    {
        {Keyboard::Right, {1,0}},
        {Keyboard::Left, {-1,0}},
        {Keyboard::Up, {0,-1}},        
        {Keyboard::Down, {0,1}}
    };
    std::vector<Vector2i> plr_movement;
    
    while(window.isOpen())
    {
        if(Keyboard::isKeyPressed(Keyboard::Escape))window.close();
        if(collision(weapon, npc))
        {
            npc.setPosition(rand() % 1000, rand() % 500);
        }
        for(auto dir : v)
        {
            if(Keyboard::isKeyPressed(dir.first))
            {
                plr.move(dir.second.x, dir.second.y);
                plr_movement.push_back(dir.second);
            }
        }
        setweaponpos(weapon, plr_movement);

        window.clear();

        window.draw(plr);
        window.draw(npc);
        window.draw(weapon);
        window.display();
        sleep(seconds(0.004));

    }
    ifs.close();
    std::ofstream ofs;
    ofs.open("seed.txt");
    if(!ofs.is_open()) return 1;
    ofs << ++seed;
    }
    catch(std::logic_error const & e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}