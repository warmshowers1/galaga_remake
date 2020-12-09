#include "Game.h"
// #include "Rect.h"
// #include "TexRect.h"
#include<iostream>
#include<fstream>
#include<math.h>
#include<deque>
#include<time.h>
#include<stdlib.h>

#if defined WIN32
#include <freeglut.h>
#elif defined __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using namespace std;

bool dirMove = false;

Game::Game(float width){
    text = new Text(width);
    ship = new Ship(0, -0.75, 0.1, 0.1);
    background = new TexRect("images/background.jpg", -1, 1, 2, 2);
    for(int i = 0; i < 2; i++){ // Always starts with 2 lives
        lives.push_back(new Ship((-0.98 + (0.09 * i)), 
            -0.9, 0.08, 0.08));
    }
    for(int i = 0; i < 2; i++){ // 2 rows of 
        for(int j = 0; j < 10; j++){ // 10 minions
            aliens.push_back(new Minion(-0.9 + (j * 0.18), 
                0.25 + (i * 0.15), 0.1, 0.1));
            if(i == 0 && j == 0)
                bounds.push_back(new Rect(-0.9 + (j * 0.18), 0.8, 0.1, 0.1, 1, 1, 1));
            else if(i == 1 && j == 9)
                bounds.push_back(new Rect(-0.9 + (j * 0.18), 0.8, 0.1, 0.1, 1, 1, 1));
        }
    }
    for(int i = 0; i < 6; i++)
        aliens.push_back(new MegaAlien(-0.54 + (i * 0.18), 
                0.6, 0.1, 0.1));

    seed = time(NULL);
    score = 0;
}

void Game::draw(){
    // Always have these render first!
    background->draw();
    ship->draw();

    text->renderText(-0.96, 0.9, "1UP");
    text->renderText(-0.25, 0.9, "HIGH SCORE");
    text->renderText(-0.96, 0.8, to_string(score), 1, 1, 1);

    // Draw in num lives
    for(auto it = lives.cbegin(); it != lives.cend(); ++it)
        (*it)->draw();

    // Draws bounds for testing
    for(auto it = bounds.cbegin(); it != bounds.cend(); ++it){
        // Comment out when done testing
        // (*it)->draw();
        if(dirMove)
            (*it)->moveRight();
        else
            (*it)->moveLeft();
    }

    // Draws bullets and keeps them moving
    for(auto it = bullets.cbegin(); it != bullets.cend(); ++it){
        (*it)->draw();
        (*it)->move();
    }

    // Draws in aliens and switches 
    // travel direction if need be
    for(auto it = aliens.cbegin(); it != aliens.cend(); ++it){
        (*it)->draw();
        if(dirMove)
            (*it)->moveRight();
        else
            (*it)->moveLeft();
    }

    // Determine which directions aliens need to travel
    if(bounds.front()->getX() <= -1)
        dirMove = true;
    if( (bounds.back()->getX() + aliens.back()->getW()) >= 1)
        dirMove = false;

}

void Game::moveShip(bool left) const { 
    if(left)
        ship->moveLeft();
    else
        ship->moveRight();
}

bool Game::noLives(){
    // Probably should add a ship death here
    return lives.empty();
}

void Game::shoot(){
    bullets.push_back(new Bullet(ship->getX() + (ship->getW()/2), ship->getY()));
}

void Game::check(){
    for(auto jt = bullets.begin(); jt != bullets.end();){
        if(!( (*jt)->inBounds() )){
            jt = bullets.erase(jt);
        }
        else{
            ++jt;
        }
    }
    bullets.shrink_to_fit();

    // cout << bullets.size() << endl;
    for(auto it = aliens.begin(); it != aliens.end(); ++it){
        for(auto jt = bullets.begin(); jt != bullets.end();){
            if((*it)->contains((*jt)->gettX(), (*jt)->gettY())){
                cout << "HIT!" << endl;
                it = aliens.erase(it);
                jt = bullets.erase(jt);
                break;
            }
            else{
                ++jt;
            }
        }
    }
    aliens.shrink_to_fit();
    bullets.shrink_to_fit();
}

Game::~Game(){
    delete text;
    delete background;
    delete ship;
    aliens.clear();
    lives.clear();
    bullets.clear();
    // Add delete for aliens deque
}
