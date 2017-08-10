#include "block.h"

Block::Block(int a, int b, int s, sf::Color c){
    x = a;
    y = b;
    size = s;
    color = c;

}

int Block::get_x(){
    return x;
}

int Block::get_y(){
    return y;
}

void Block::set_x(int a){
	x = a;
}
void Block::set_y(int b){
	y = b;
}

void Block::move(int a, int b){
    x += a;
	y += b;
}

int Block::get_size(){
    return size;
}

void Block::draw(sf::RenderWindow & window, bool todraw){
	sf::RectangleShape rectangle(sf::Vector2f(size-1, size-1));
	rectangle.setFillColor(color);
	rectangle.setPosition(get_x(), get_y());
	if (todraw){
		rectangle.setOutlineColor(sf::Color(0, 0, 0));
		rectangle.setOutlineThickness(1.f);
	}
	else{
		rectangle.setFillColor(sf::Color(255, 255, 255));
		rectangle.setOutlineColor(sf::Color(255, 255, 255));
		rectangle.setOutlineThickness(1.f);
	}
	window.draw(rectangle);
}

Block Block::Copy(){
	Block B_copy(get_x(), get_y(), get_size(), sf::Color(255, 255, 255));
	return B_copy;
}

double Block::distance(Block B){
	double dist = sqrt(((B.get_x() - get_x())*(B.get_x() - get_x())) + ((B.get_y() - get_y())*(B.get_y() - get_y())));
	return dist;
}

Piece::Piece(int s, int size, int pos_x, int pos_y){
    shape = s;
	fall = true;
    if (shape == 1) { //square
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(255,0,0)));
		blocks.push_back(Block(pos_x + size, pos_y, size, sf::Color(255, 0, 0)));
		blocks.push_back(Block(pos_x, pos_y + size, size, sf::Color(255, 0, 0)));
		blocks.push_back(Block(pos_x + size, pos_y + size, size, sf::Color(255, 0, 0)));
    }
    else if (shape == 2){ // I
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(0, 255, 0)));
		blocks.push_back(Block(pos_x, pos_y + size, size, sf::Color(0, 255, 0)));
		blocks.push_back(Block(pos_x, pos_y + 2 * size, size, sf::Color(0, 255, 0)));
		blocks.push_back(Block(pos_x, pos_y + 3 * size, size, sf::Color(0, 255, 0)));
    }
    else if (shape == 3){ // L
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(0, 0, 255)));
		blocks.push_back(Block(pos_x, pos_y + size, size, sf::Color(0, 0, 255)));
		blocks.push_back(Block(pos_x, pos_y + 2 * size, size, sf::Color(0, 0, 255)));
		blocks.push_back(Block(pos_x + size, pos_y + 2 * size, size, sf::Color(0, 0, 255)));
    }
    else if (shape == 4){ // L inverse
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(0, 255, 255)));
		blocks.push_back(Block(pos_x, pos_y + size, size, sf::Color(0, 255, 255)));
		blocks.push_back(Block(pos_x, pos_y + 2 * size, size, sf::Color(0, 255, 255)));
		blocks.push_back(Block(pos_x - size, pos_y + 2 * size, size, sf::Color(0, 255, 255)));
    }
    else if (shape == 5){ // Z
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(255, 255, 0)));
		blocks.push_back(Block(pos_x + size, pos_y, size, sf::Color(255, 255, 0)));
		blocks.push_back(Block(pos_x + size, pos_y + size, size, sf::Color(255, 255, 0)));
		blocks.push_back(Block(pos_x + 2 * size, pos_y + size, size, sf::Color(255, 255, 0)));
    }

    else if (shape == 6){ // Z inverse
		blocks.push_back(Block(pos_x, pos_y, size, sf::Color(255, 0, 255)));
		blocks.push_back(Block(pos_x - size, pos_y, size, sf::Color(255, 0, 255)));
		blocks.push_back(Block(pos_x - size, pos_y + size, size, sf::Color(255, 0, 255)));
		blocks.push_back(Block(pos_x - 2 * size, pos_y + size, size, sf::Color(255, 0, 255)));
    }
}

Piece Piece::Single(int size, Block B){//Create a piece with a single block
	Piece Single(shape, size, 0, 0);
	Single.blocks.erase(Single.blocks.begin(), Single.blocks.end());
	Single.blocks.push_back(B);
	return Single;
}

int Piece::get_shape(){
    return shape;
}

void Piece::translation(int a, int b){
    for(int i = 0;i<blocks.size();i++){
        blocks[i].move(a, b);
    }
}

void Piece::rotation(int way, int size){
	if (get_shape() != 1){ // not a square
		double center_x = blocks[1].get_x() + size / 2.;
		double center_y = blocks[1].get_y() + size / 2.;
		double r = size*sqrt(2) / 2.;
		if (way == 0){ // Left Rotation
			for (int i = 0; i < blocks.size(); i++){
				int new_x = -(blocks[i].get_y() - center_y) + center_x - size; //-size : for plotting
				int new_y = (blocks[i].get_x() - center_x) + center_y;
				blocks[i].set_x(new_x);
				blocks[i].set_y(new_y);
			}
		}
		else if (way == 1){ // Right Rotation
			for (int i = 0; i < blocks.size(); i++){
				int new_x = (blocks[i].get_y() - center_y) + center_x;
				int new_y = -(blocks[i].get_x() - center_x) + center_y - size; //-size : for plotting
				blocks[i].set_x(new_x);
				blocks[i].set_y(new_y);
			}
		}
	}
}

Piece Piece::Copy(int size){
	Piece P_copy(get_shape(), size, 0, 0);
	for (int i = 0; i < blocks.size(); i++){
		P_copy.blocks[i] = blocks[i].Copy();
	}
	if (P_copy.blocks.size() > blocks.size()){
		P_copy.blocks.erase(P_copy.blocks.begin()+blocks.size(), P_copy.blocks.end());
	}
	return P_copy;
}

bool Piece::test_move(int a, int b, vector<Piece> P_ingame, int move_type, int size, int lim_left, int lim_right, int lim_inf, int me){
	bool move_allowed = true;
	Piece P_copy = Copy(size);
	if (move_type == 0){ // Translation
		P_copy.translation(a, b);
	}
	else if (move_type == 1){ // Left Rotation
		P_copy.rotation(0, size);
	}
	else if (move_type == 2){ // Right Rotation
		P_copy.rotation(1, size);
	}

	//Test if the block is inside the lateral borders
	for (int k = 0; k < P_copy.blocks.size(); k++){
		if (b == 0){ // move from the player
			if (P_copy.blocks[k].get_x() < lim_left || P_copy.blocks[k].get_x() > lim_right - size){ // the piece cannot cross the 
																									 //left and right borders
				move_allowed = false;
				break;
			}
		}
		else{ //the piece goes down
			if (P_copy.blocks[k].get_y() > lim_inf - size){ //the piece cannot go under the low border
				move_allowed = false;
				break;
			}
		}
		//Test collision with other blocks
		for (int i = 0; i < P_ingame.size(); i++){
			if (i != me){
				for (int j = 0; j < P_ingame[i].blocks.size(); j++){
					if (P_copy.blocks[k].get_x() == P_ingame[i].blocks[j].get_x() && P_copy.blocks[k].get_y() == P_ingame[i].blocks[j].get_y()){
						move_allowed = false;
						break;
					}
				}
				if (move_allowed == false){
					break;
				}
			}
			if (move_allowed == false){
				break;
			}
		}
	}
	return move_allowed;
}

void Piece::move(int size, vector<Piece> P_ingame, sf::Time timer, int lim_left, int lim_right, int lim_inf){
	sf::Clock clock;
	while (clock.getElapsedTime() < timer){//we have to play before time timer
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){ //we always check if the move is possible before doing it
			bool left = test_move(-size, 0, P_ingame, 0, size, lim_left, lim_right, lim_inf,-1);
			if (left){
				translation(-size, 0);
				break;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			bool right = test_move(size, 0, P_ingame, 0, size, lim_left, lim_right, lim_inf,-1);
			if (right){
				translation(size, 0);
				break;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			timer = sf::milliseconds(100);
			break;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			bool left_rot = test_move(0, 0, P_ingame, 1, size, lim_left, lim_right, lim_inf,-1);
			if (left_rot){
				rotation(0,size);
				break;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			bool right_rot = test_move(0, 0, P_ingame, 2, size, lim_left, lim_right, lim_inf,-1);
			if (right_rot){
				rotation(1,size);
				break;
			}
		}
	}
}

void Piece::draw(sf::RenderWindow & window, bool todraw){
	for (int i = 0; i < blocks.size(); i++){
		blocks[i].draw(window, todraw);
	}
}

void Piece::undraw(sf::RenderWindow & window){
	draw(window, false);
}

void Piece::down(int size, vector<Piece> P_ingame,int lim_left, int lim_right, int lim_inf, int me){
	bool godown = test_move(0, size, P_ingame, 0, size, lim_left, lim_right, lim_inf, me);
	if (godown){
		translation(0, size);
	}
	else{
		fall = false;
	}
}

int Piece::min_y(){
	int miny = 100000;
	for (int i = 0; i < blocks.size(); i++){
		if (blocks[i].get_y() < miny){
			miny = blocks[i].get_y();
		}
	}
	return miny;
}

int Piece::max_y(){
	int maxy = -5;
	for (int i = 0; i < blocks.size(); i++){
		if (blocks[i].get_y() > maxy){
			maxy = blocks[i].get_y();
		}
	}
	return maxy;
}