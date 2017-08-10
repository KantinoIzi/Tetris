////////////////////////////////////////////////////////////////////
///                       TETRIS GAME                            ///
/// Developed with Microsoft Visual Studio 2013 and SFML library ///
////////////////////////////////////////////////////////////////////

#include "block.h"
using namespace std;

sf::Event event;
vector<int> path_opt;
vector<Piece> P_ingame;

////////////// Size of the window //////////////
const int w = 500;
const int h = 500;

////////////// Parameters for the game //////////////
int speed = 500;
const int size = 15;
int const nb_line = 20;
int blocks_per_line[nb_line];
int score_table[nb_line];
int nb_line_done = 0;
bool quit = false;

////////////// Borders of the game //////////////
int lim_left = w / 4;
int lim_right = w / 4 + 10 * size;
int lim_inf = h / 4 + 20 * size;
int lim_sup = h / 4;

////////////// Timers //////////////
sf::Time Timer = sf::milliseconds(speed);
sf::Time Timer_delay = sf::milliseconds(200);
sf::Time Timer_Game_Over = sf::milliseconds(5000);


////////////// Functions to initialize parameters //////////////
void ini_score_tab(int score_tab[nb_line]){
	for (int i = 0; i < nb_line; i++){
		score_tab[i] = 100*i;
	}
}

void initialize_lines(int tab[nb_line]){
	for (int i = 0; i < nb_line; i++){
		tab[i] = 0;
	}
}



//////////////////////////////////////////////
///           Draw functions               ///
//////////////////////////////////////////////

void game_borders(sf::RenderWindow & window){
	sf::Vertex borders[] =
	{
		sf::Vertex(sf::Vector2f(w / 4, h / 4)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, h / 4)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, h / 4)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, h / 4 + 20 * size)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, h / 4 + 20 * size)),
		sf::Vertex(sf::Vector2f(w / 4, h / 4 + 20 * size)),
		sf::Vertex(sf::Vector2f(w / 4, h / 4 + 20 * size)),
		sf::Vertex(sf::Vector2f(w / 4, h / 4))
	};
	for (int i = 0; i < 8; i++){
		borders[i].color = sf::Color(0, 0, 0);
	}
	window.draw(borders, 8, sf::Lines);
}

void draw_game_place(sf::RenderWindow & window){
	sf::RectangleShape rectangle(sf::Vector2f(10*size, 20*size));
	rectangle.setFillColor(sf::Color(255,255,255));
	rectangle.setPosition(w/4, h/4);
	window.draw(rectangle);
	rectangle.setSize(sf::Vector2f(10 * size, 6 * size));
	rectangle.setPosition(w / 4, size);
	window.draw(rectangle);
}

void border_next_piece(sf::RenderWindow & window){
	sf::Vertex borders[] =
	{
		sf::Vertex(sf::Vector2f(w / 4, size)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, size)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, size)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, 7 * size)),
		sf::Vertex(sf::Vector2f(w / 4 + 10 * size, 7 * size)),
		sf::Vertex(sf::Vector2f(w / 4, 7 * size)),
		sf::Vertex(sf::Vector2f(w / 4, 7 * size)),
		sf::Vertex(sf::Vector2f(w / 4, size))
	};
	for (int i = 0; i < 8; i++){
		borders[i].color = sf::Color(0, 0, 0);
	}
	window.draw(borders, 8, sf::Lines);
}


//////////////////////////////////////////////
///           Functions for lines          ///
//////////////////////////////////////////////

void update_lines(int tab[nb_line], Piece P){
	for (int i = 0; i < P.blocks.size(); i++){
		int line = (P.blocks[i].get_y() - h / 4) / size;
		tab[line]++;
	}
}

void clear_line(int line, sf::RenderWindow & window){
	sf::RectangleShape rectangle(sf::Vector2f(10*size, size +1));
	rectangle.setFillColor(sf::Color(255, 255, 255));
	rectangle.setPosition(w/4, size*line + h/4-1);
	window.draw(rectangle);
}

void process_line(vector<Piece> &  P_list, int tab[nb_line], sf::RenderWindow & window, int &line_done){
	for (int i = 0; i < nb_line; i++){
		if (tab[nb_line - 1 - i] == 10){//line completed
			line_done++;
			clear_line(nb_line - 1 - i, window);
			for (int j = 0; j < P_list.size(); j++){//we delete each block of the line
				bool block_erase = false;
				for (int k = 0; k < P_list[j].blocks.size(); k++){
					if (P_list[j].blocks[k].get_y() == h / 4 + (nb_line - 1 - i)*size){
						block_erase = true;
						P_list[j].blocks.erase(P_list[j].blocks.begin() + k);
						k--;
					}
				}

				if (P_list[j].blocks.size() == 0){//if the size of P_list[j].blocks is 0 we remove it
					P_list.erase(P_list.begin() + j);
					j--;
				}
				else if (block_erase){//we split the piece in two if it is necessary
					if (P_list[j].blocks.size() > 1){
						for (int k = 0; k < P_list[j].blocks.size(); k++){
							double distance_min = 10.*size;
							for (int l = 0; l < P_list[j].blocks.size(); l++){
								if (k != l){
									double distance = P_list[j].blocks[k].distance(P_list[j].blocks[l]);
									if (distance < distance_min){
										distance_min = distance;
									}
								}
							}
							if (distance_min > size){
								Piece Single = P_list[j].Single(size, P_list[j].blocks[k]);
								P_list.push_back(Single);
								P_list[j].blocks.erase(P_list[j].blocks.begin() + k);
								break;
							}
						}
					}
				}
			}
			bool change = true;
			while (change){//the line has benn deleted, the pieces are falling
				change = false;
				for (int j = 0; j < P_list.size(); j++){
					P_list[j].fall = true;
					int move = 0;
					P_list[j].undraw(window);
					while (P_list[j].fall){
						move++;
						P_list[j].down(size, P_list, lim_left, lim_right, lim_inf, j);
					}
					P_list[j].draw(window, true);
					if (move > 1){
						change = true;
					}
				}
			}
			//we display the scene
			game_borders(window);
			window.display();
			initialize_lines(tab);
			for (int j = 0; j < P_list.size(); j++){
				update_lines(tab, P_list[j]);
			}
			sf::Clock delay;
			while (delay.getElapsedTime() < Timer_delay){

			}
			i = -1;
		}
	}
}


//////////////////////////////////////////////
///           Functions for AI             ///
//////////////////////////////////////////////

void AI_move(int side, int nb_rotation, int & score, int & score_max, Piece P, vector<Piece> P_list, int tab[nb_line],
					vector<int> & path, vector<int> & path_opt){
	int tab_copy[nb_line];
	int tab_copy_after[nb_line];
	ini_score_tab(score_table);
	int nb_translation_max = 5;
	if (side == 1){//max translation on the right
		nb_translation_max = 7;
	}

	int miny_list = 100000;
	for (int i = 0; i < P_list.size(); i++){//we are looking for the highest block in P_list
		int miny = P_list[i].min_y();
		if (miny < miny_list){
			miny_list = miny;
		}
	}

	int maxy_P = P.max_y(); //lowest block of P

	for (int g = 0; g < nb_rotation; g++){
		int rotation = 0;
		int nb_do_rotation = 0;
		Piece P_copy_rotation = P.Copy(size);
		while ((rotation < g) && (P_copy_rotation.fall)){//Rotation(s)
			if (miny_list > maxy_P + 2 * size){//if the distance between miny_list and max_P is high we don't need to test the rotation
				P_copy_rotation.rotation(0, size);
				nb_do_rotation++;
			}
			else if (P_copy_rotation.test_move(0, 0, P_list, 1, size, lim_left, lim_right, lim_inf, -1)){//here we test if the rotation can be done
				P_copy_rotation.rotation(0, size);
				nb_do_rotation++;
			}
			maxy_P = P_copy_rotation.max_y(); //update of max_P
			if (miny_list > maxy_P + size){//the piece falls and we don't need to test
				P_copy_rotation.translation(0, size);
			}
			else{//the piece falls and we need to test
				P_copy_rotation.down(size, P_list, lim_left, lim_right, lim_inf, -1);
			}
			rotation++;
		}
		for (int i = 0; i < nb_translation_max; i++){//we write the actions in path
			for (int h = 0; h < nb_do_rotation; h++){
				path.push_back(2);
			}
			for (int h = 0; h < rotation - nb_do_rotation; h++){
				path.push_back(-1);
			}
			score = 0;
			for (int j = 0; j < nb_line; j++){
				tab_copy[j] = tab[j];
				tab_copy_after[j] = tab[j];
			}

			Piece P_copy = P_copy_rotation.Copy(size); //we copy P_copy_rotation after the rotation(s)

			//Translation
			int nb_translation = 0;
			if (i == 0){//special case for 0 translation
				while (P_copy.fall){
					path.push_back(-1);
					maxy_P = P_copy.max_y();
					if (miny_list > maxy_P + size && maxy_P < lim_inf - size){
						P_copy.translation(0, size);
					}
					else{
						P_copy.down(size, P_list, lim_left, lim_right, lim_inf, -1);
					}
				}
			}
			else{//for more than 0 translation
				while ((nb_translation < i) && (P_copy.fall)){
					if (P_copy.test_move(-size, 0, P_list, 0, size, lim_left, lim_right, lim_inf, -1) && side == 0){//try left translation
						P_copy.translation(-size, 0);
						path.push_back(0);
					}
					else if (P_copy.test_move(size, 0, P_list, 0, size, lim_left, lim_right, lim_inf, -1) && side == 1){//try right translation
						P_copy.translation(size, 0);
						path.push_back(1);
					}
					else{//no translation
						path.push_back(-1);
					}
					maxy_P = P_copy.max_y();
					if (miny_list > maxy_P + size && maxy_P < lim_inf - size){//the piece falls and we don't test
						P_copy.translation(0, size);
					}
					else{//the piece falls and we test
						P_copy.down(size, P_list, lim_left, lim_right, lim_inf, -1);
					}
					nb_translation++;
				}
				while (P_copy.fall){//we have done all the translations we need, now the piece falls
					path.push_back(-1);
					maxy_P = P_copy.max_y();
					if (miny_list > maxy_P + size && maxy_P < lim_inf - size){//we don't test
						P_copy.translation(0, size);
					}
					else{//we test
						P_copy.down(size, P_list, lim_left, lim_right, lim_inf, -1);
					}
				}
			}
			update_lines(tab_copy_after, P_copy);

			//Compute score
			int score1 = 0;
			for (int j = 0; j < nb_line; j++){
				score += (tab_copy_after[j] - tab_copy[j])*score_table[j]; //Height of each block
				if (tab_copy_after[j] < 10){
					score += tab_copy_after[j] * score_table[j]; //nb blocks per line
				}
				else{
					score += 100000; //line completed
				}
			}
			for (int j = 0; j < P_copy.blocks.size(); j++){//we don't want to have holes
				if (P_copy.blocks[j].get_y() < lim_inf - size){
					bool hole = true;
					for (int k = 0; k < P_copy.blocks.size(); k++){ //we compare the position of the blocks in P_copy with each other
						if (P_copy.blocks[k].get_x() == P_copy.blocks[j].get_x() && P_copy.blocks[k].get_y() == P_copy.blocks[j].get_y() + size){
							hole = false;
							break;
						}
					}
					if (hole){
						for (int k = 0; k < P_list.size(); k++){//we compare the position of the blocks in P_list with the blocks of P_copy
							for (int l = 0; l < P_list[k].blocks.size(); l++){
								if (P_list[k].blocks[l].get_x() == P_copy.blocks[j].get_x() &&
									P_list[k].blocks[l].get_y() == P_copy.blocks[j].get_y() + size){
									hole = false;
									break;
								}
							}
							if (!hole){
								break;
							}
						}
					}
					if (hole){
						score -= 1000;
					}
				}
			}

			if (score > score_max){//if the path is the best for now we keep it in path_opt
				path_opt.erase(path_opt.begin(), path_opt.end());
				for (int j = 0; j < path.size(); j++){
					path_opt.push_back(path[j]);
				}
				score_max = score;
			}
			path.erase(path.begin(), path.end());
			P_copy.blocks.erase(P_copy.blocks.begin(), P_copy.blocks.end());
		}
	}
}

void AI(Piece P, vector<Piece> P_list, int tab[nb_line], vector<int> & path_opt){
	vector<int> path; //0 : left translation, 1 : right translation, 2 : left rotation, -1 : no move
	int score = 0;
	int score_max = -1000000000;

	//the number of rotation depends of the shape of the piece
	int nb_rotation = 4;
	if (P.get_shape() == 1){
		nb_rotation = 1;
	}
	else if (P.get_shape() == 2 || P.get_shape() == 5 || P.get_shape() == 6){
		nb_rotation = 2;
	}

	AI_move(0, nb_rotation, score, score_max, P, P_list, tab, path, path_opt); //AI with left translation
	AI_move(1, nb_rotation, score, score_max, P, P_list, tab, path, path_opt); //AI with right translation
}

////////////// Game Over //////////////
bool Test_Game_Over(Piece P, int lim_sup){
	bool GO = false; //Game over
	for (int i = 0; i < P.blocks.size(); i++){
		if (P.blocks[i].get_y() < lim_sup + 2 * size){ // if height is < lim_sup + 2*size, game over
			GO = true;
			break;
		}
	}
	return GO;
}

//////////////////////////////////////////////
///                MAIN                    ///
//////////////////////////////////////////////

int main(){
	srand(time(NULL));
	sf::Clock delay;
	initialize_lines(blocks_per_line);

	sf::RenderWindow window(sf::VideoMode(w, h), "Tetris");
	while (window.isOpen()){
		delay.restart();
		bool Game_Over = false;
		bool First_Piece = true;

		//for the window
		sf::Texture texture;
		texture.loadFromFile("title_screen_choice.png");
		sf::Sprite sprite(texture);
		sprite.setPosition(0, 0);
		window.draw(sprite);
		window.display();

		bool Player = true;
		bool game_choice = false;

		while (!sf::Mouse::isButtonPressed(sf::Mouse::Left) || !game_choice){ //choice between player and AI
			sf::Vector2i Mouse_pos = sf::Mouse::getPosition(window);
			if (Mouse_pos.x < w / 2 && Mouse_pos.x >= 0 && Mouse_pos.y <= h && Mouse_pos.y >= 0){
				Player = true;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					game_choice = true;
				}
			}
			else if (Mouse_pos.x > w / 2 && Mouse_pos.x <= w && Mouse_pos.y <= h && Mouse_pos.y >= 0){
				Player = false;
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					game_choice = true;
				}
			}
		}

		//update the window
		texture.loadFromFile("title_screen.png");
		window.draw(sprite);
		draw_game_place(window);
		game_borders(window);
		border_next_piece(window);
		window.display();

		//variables to have the pieces randomly selected
		int alea;
		int alea2;

		while (!Game_Over){
			//Random creation of Piece P
			if (First_Piece){
				alea = rand() % (6) + 1;
				First_Piece = false;
			}
			else{
				alea = alea2;
			}
			alea2 = rand() % (6) + 1;
			Piece P = Piece(alea, size, w / 4 + 4 * size, h / 4);
			Piece P_next = Piece(alea2, size, w / 4 + 4 * size, 2 * size);
			P.draw(window, true);
			P_next.draw(window, true);
			window.display();
			if (!Player){//The AI plays
				AI(P, P_ingame, blocks_per_line, path_opt);
			}
			int index_move = 0;//just for the AI
			while (P.fall){
				while (window.pollEvent(event)){//we can close the game if we click on the cross oh the window
					if (event.type == sf::Event::Closed){
						window.close();
						Game_Over = true;
						quit = true;
					}
				}
				if (Game_Over){
					break;
				}

				Piece P_undraw = P.Copy(size);
				delay.restart();
				while (delay.getElapsedTime() < Timer_delay){

				}
				if (Player){//The player plays
					P.move(size, P_ingame, Timer, lim_left, lim_right, lim_inf);
				}
				else{//The computer plays with the moves got with the function AI
					if (path_opt[index_move] == 0){
						P.translation(-size, 0);
					}
					else if (path_opt[index_move] == 1){
						P.translation(size, 0);
					}
					else if (path_opt[index_move] == 2){
						P.rotation(0, size);
					}
					index_move++;
					//path_opt.erase(path_opt.begin());
				}
				P.down(size, P_ingame, lim_left, lim_right, lim_inf, -1); //the piece falls
				//update the window
				P_undraw.undraw(window);
				game_borders(window);
				P.draw(window, true);
				for (int i = 0; i < P_ingame.size(); i++){
					P_ingame[i].draw(window, true);
				}
				window.display();
			}
			if (quit){
				break;
			}
			//process the piece after it stops falling
			update_lines(blocks_per_line, P);
			P_ingame.push_back(P);
			process_line(P_ingame, blocks_per_line, window, nb_line_done);
			Game_Over = Test_Game_Over(P, lim_sup);
			P_next.undraw(window);
			P_next.blocks.erase(P_next.blocks.begin(), P_next.blocks.end());
			if (nb_line_done > 5){
				speed = 0.95*speed;
				Timer = sf::milliseconds(speed);
				nb_line_done = 0;
			}
		}
		if (quit){
			break;
		}
		//Game Over
		P_ingame.erase(P_ingame.begin(), P_ingame.end());
		cout << "You loose !" << endl;
		delay.restart();
		while (delay.getElapsedTime() <= Timer_Game_Over){

		}
	}
	return 0;
}
