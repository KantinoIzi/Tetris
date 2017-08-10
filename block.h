#pragma once
#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

class Block{
private:
    int x;
    int y;
    int size;
	sf::Color color;
public:
	Block(int a, int b, int s, sf::Color c);
    int get_x();
    int get_y();
	void set_x(int a);
	void set_y(int b);
    int get_size();
    void move(int a, int b);
	void draw(sf::RenderWindow & window, bool todraw);
	Block Copy();
	double distance(Block B);
};

class Piece{
private:
    int shape;
public:
    Piece(int s, int size, int pos_x, int pos_y);
	Piece Copy(int size);
	Piece Single(int size, Block B);
    int get_shape();
	bool fall;
    vector<Block> blocks;
	bool test_move(int a, int b, vector<Piece> P_ingame, int move_type, int size, int lim_left, int lim_right, int lim_inf, int me);
    void translation(int a, int b);
    void rotation(int way, int size);
	void move(int size, vector<Piece> P_ingame, sf::Time timer, int lim_left, int lim_right, int lim_inf);
	void draw(sf::RenderWindow & window, bool todraw);
	void undraw(sf::RenderWindow & window);
	void draw_contour(sf::RenderWindow & window);
	void down(int size, vector<Piece> P_ingame, int lim_left, int lim_right, int lim_inf, int me);
	int max_y();
	int min_y();
};

void undraw(sf::RenderWindow & window, Piece P, int size);