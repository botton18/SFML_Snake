#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int N = 30;
int M = 20;
int size = 16;

int w = 16 * N;
int h = 16 * M;

class Body
{
private:
	sf::Sprite body;
	sf::Texture green;
	float body_x;
	float body_y;
	int prev_x;
	int prev_y;
public:
	Body()
	{
		green.loadFromFile("images/green.png");

		body = sf::Sprite(green);
		//body.setPosition(240.f, 160.f);
		//body.setPosition(body_x, body_y);
	}
	Body(float x, float y)
	{
		green.loadFromFile("images/green.png");

		body = sf::Sprite(green);

		body.setPosition(x, y);
	}

	Body(int h)
	{
		green.loadFromFile("images/red.png");
		body = sf::Sprite(green);
	}
	float getX()
	{
		return body_x;
	}
	float getY()
	{
		return body_y;
	}
	void setNewPos( float x, float y )
	{
		body_x = x;
		body_y = y;
		body.setPosition(x, y);
	}
	sf::Sprite sprite()
	{
		return body;
	}
	void setPre(int x, int y)
	{
		prev_x = x;
		prev_y = y;
	}
	int getPrex()
	{
		return prev_x;
	}
	int getPrey()
	{
		return prev_y;
	}
};

class Fruit
{
private:
	sf::Sprite fruit;
	sf::Texture fruit_texture;
	float x;
	float y;
public:
	Fruit()
	{
		fruit_texture.loadFromFile("images/red.png");
		fruit = sf::Sprite(fruit_texture);
		fruit.setPosition(16.f * 4, 16.f * 7);
	}

	void SetRandomPos()
	{
		srand(time(NULL));
		x = rand() % 30;
		y = rand() % 20;
		fruit.setPosition(x * 16, y * 16);
	}
	void drawFruit(sf::RenderWindow & win)
	{
		win.draw(fruit);
	}
	sf::Sprite getFruitSprite()
	{
		return fruit;
	}
};



class Snake
{
private:
	vector<Body> snake;
	int direction_x;
	int direction_y;
public:
	Snake()
	{
		for (int i = 0; i < 5; ++i)
		{
			Body *temp = new Body();
			snake.push_back(*temp);
		}
		snake.at(0).setNewPos(240.f, 160.f);
		direction_x = 0;
		direction_y = -1;
	}
	Body getHead()
	{
		return snake.at(0);
	}
	void move()
	{
		snake.at(0).setNewPos(snake.at(0).getX() + (direction_x*16.f), snake.at(0).getY() + (direction_y * 16.f));
	}
	void setDir(int x, int y)
	{
		direction_x = x;
		direction_y = y;
	}
	void update()
	{
		for (int i = snake.size() - 1; i > 0; --i)
		{	
			snake.at(i).setNewPos(snake.at(i - 1).getX(), snake.at(i - 1).getY());	
		}
	}
	void expand()
	{
		Body *temp = new Body();
		snake.push_back(*temp);
	}
	void drawSnake(sf::RenderWindow & win)
	{
		for (int i = 0; i < snake.size(); ++i)
		{
			win.draw(snake.at(i).sprite());
		}
		cout << snake.size() << endl;
	}
	bool Collided(Fruit fruit)
	{
		if (snake.at(0).sprite().getGlobalBounds().intersects(fruit.getFruitSprite().getGlobalBounds()))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool isGameOver()
	{
		for (int i = 1; i < snake.size(); ++i)
		{
			if (snake.at(0).sprite().getGlobalBounds().intersects(snake.at(i).sprite().getGlobalBounds()))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
};



int main()
{
	sf::RenderWindow window(sf::VideoMode(w, h), "SFML works!");

	sf::Texture white;
	sf::Texture red;
	//sf::Texture green;

	float speed = 0.1;

	Snake my_snake;
	Fruit fruit;
	white.loadFromFile("images/white.png");

	int xdir = 0;
	int ydir = -1;

	sf::Sprite tiles(white);

	//sf::Sprite apple(red);

	//Body body;

	int direction = 1;

	sf::Clock clock;

	float timer = 0;
	float delay = 0.2;

	while (window.isOpen())
	{
		
		
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (timer > delay)
		{
			timer = 0;
			my_snake.move();
			//my_snake.expand()
			my_snake.update();
			if (my_snake.Collided(fruit))
			{
				fruit.SetRandomPos();
				my_snake.expand();
			}
		
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			
			xdir = 0;
			ydir = -1;
			my_snake.setDir(0, -1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			
			xdir = -1;
			ydir = 0;
			my_snake.setDir(-1, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			
			xdir = 0;
			ydir = 1;
			my_snake.setDir(0, 1);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			
			xdir = 1;
			ydir = 0;
			my_snake.setDir(1, 0);
			
		}

		window.clear();
		
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				tiles.setPosition(i * 16, j * 16);
				window.draw(tiles);
			}
		}

		my_snake.drawSnake(window);
		fruit.drawFruit(window);
		//window.draw(body.sprite());
		window.display();
	}

	return 0;
}