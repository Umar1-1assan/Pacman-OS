#include<iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<X11/Xlib.h>

using namespace std;
using namespace sf;


///===Semophores and threads==///
pthread_t main_game_thread,ui_thread,ghost_thread[4],pacman_thread;
sem_t s,gh,pal,boost;
sem_t lock,key,permit;

////======///


int ch =10;
int MAX_X=900,MAX_Y=864;


// make BOARD... for game
    	
// x- 22 y-28
int board[28][22] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,2,1,1,2,2,1,1,2,2,1,1,2,1,1,1,2,1},
    {1,3,1,0,1,2,1,1,2,2,1,1,2,2,1,1,2,1,0,1,3,1},
    {1,2,1,1,1,2,1,1,2,2,1,1,2,2,1,1,2,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,2,1,2,1,1,2,2,1,1,2,1,2,1,1,1,2,1},
    {1,2,0,1,0,2,1,2,2,1,2,2,1,2,2,1,2,0,1,0,2,1},
    {1,2,2,2,2,2,1,1,2,2,2,2,2,2,1,1,2,2,2,2,2,1},
    {1,1,1,1,1,2,1,2,2,0,4,4,0,2,2,1,2,1,1,1,1,1},
    {0,0,0,0,1,2,1,2,1,1,1,1,1,1,2,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,2,1,0,0,0,0,1,2,1,2,1,1,1,1,1},
    {0,2,2,2,2,2,2,2,1,0,0,0,0,1,2,2,2,2,2,2,2,0},
    {1,1,1,1,1,2,1,2,2,1,1,1,1,2,2,1,2,1,1,1,1,1},
    {0,0,0,0,1,2,1,2,2,2,2,2,2,2,2,1,2,1,0,0,0,0},
    {0,0,0,0,1,2,1,1,1,1,1,1,1,1,1,1,2,1,0,0,0,0},
    {0,0,0,0,1,2,2,2,2,1,0,0,1,2,2,2,2,1,0,0,0,0},
    {0,0,0,0,1,2,1,1,2,1,0,0,1,2,1,1,2,1,0,0,0,0},
    {1,1,1,1,1,2,1,1,2,1,1,1,1,2,1,1,2,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,2,2,1,1,1,1,2,2,1,1,1,1,1,2,1},
    {1,2,2,2,1,1,1,1,2,2,1,1,2,2,1,1,1,1,2,2,2,1},
    {1,1,1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1,1,1},
    {1,1,1,2,1,2,1,2,1,1,1,1,1,1,2,1,2,1,2,1,1,1},
    {1,2,2,2,2,2,1,2,2,2,1,1,2,2,2,1,2,2,2,2,2,1},
    {1,3,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,3,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// pacman_variables
float sprotate=0;
int x=11;
int y=19;
int old_x;
int old_y;
int score =0;				//250 +1 left max at x==1
int movement=0;	
int pills_count=0;

float pallet_time=-1;
int lives=3;
// render window..  
RenderWindow* window;
bool game_over=0;
// ghosts...
int gh_x[4]={0};
int gh_y[4]={0};

Clock* clock3;
sf::Time *de_t;

void* UI_fnc(void* arg)
{ 	    		
	while(window->isOpen() && !game_over)
    	{
	//---Event Listening Part---//
	
    		Event e;
        	while (window->pollEvent(e))
        	{                    
        		if (e.type == Event::Closed)                   //If cross/close is clicked/pressed
                	window->close();
                             
                //    cout<<"Events...."<<endl;            
            	if (e.type == Event::KeyPressed) 
            	{        
            		sem_wait(&s);     
               		if (e.key.code == Keyboard::Up)            //Check if the other key pressed is UP key
               		{
		               	movement = 1;                                	
		            }
               		else if (e.key.code == Keyboard::Right)    //Check if the other key pressed is RIGHT key
               		{
                		movement = 4;                          
                	}
                	else if(e.key.code==Keyboard::Escape)      //To Exit Window
                	{
					window->close();
					}       
					else if (e.key.code == Keyboard::Left)     //Check if the other key pressed is LEFT key
                    {
                		movement = 3;                          
                	}
                    else if (e.key.code == Keyboard::Down)     //Check if the other key pressed is Down key
                	{
            			movement = 2; 
               		}
              		sem_post(&s);

        		}// end if keypressed
       	      
        	}// End_Events	

			sem_wait(&s);
			// check colllisions with pallets and else..
			if(board[y][x]==2)
			{
				score+=10;
				board[y][x]=0;
				pills_count+=1;
			}
			else if(board[y][x]==3 &&  pallet_time<0)
			{
				// implement pallet
				sem_wait(&pal);
				board[y][x]=0;
				pallet_time=100;
			}
			sem_post(&s);

        }// end_of window
                	
	return NULL;
}

int bost=1;
int boost_time=-1;
int out[4]={0};
sf::Time dl[4]={seconds(0.2),seconds(0.2),seconds(0.2),seconds(0.2)};
sf:: Clock clock2[4];

// function for ghost...
void* ghost_fnc(void* arg)
{
	int gh_ind = *((int*)arg);
	while(window->isOpen() && !game_over)
	{
		if (clock2[gh_ind].getElapsedTime() >= dl[gh_ind])
		{
			if(out[gh_ind]==1 || !((gh_x[gh_ind] > 8 && gh_x[gh_ind] <13) && (gh_y[gh_ind] > 9 && gh_y[gh_ind] <13)))
			{
				out[gh_ind]=1;
				int rnd =rand() %4;
				
				sem_wait(&gh);	

				//cout<<gh_ind<<" "<<dl[gh_ind].asSeconds()<<endl;   			-- for check delay of every ghost..

				// move ghosts
				int old_x=gh_x[gh_ind];
				int old_y=gh_y[gh_ind];
				if(rnd==0)
				{
					//down
					if(gh_y[gh_ind]+1<27 )
					{
						gh_y[gh_ind]+=1;
					}			
				}
				else if(rnd==1)
				{
					//up
					if(gh_y[gh_ind]-1>1 )
					{
						gh_y[gh_ind]-=1;	
					}
						
				}
				else if(rnd==2)
				{
					//left
					if(gh_x[gh_ind]-1>1 )
					{
						gh_x[gh_ind]-=1;	
					}					
				}
				else if(rnd==3)
				{
					//right
					if(gh_x[gh_ind]+1<21)
					{
						gh_x[gh_ind]+=1;	
					}			
				}				

				// check collision with walls	
					sem_wait(&s);
				if(board[gh_y[gh_ind]][gh_x[gh_ind]]==1)
				{
					gh_y[gh_ind]=old_y;
					gh_x[gh_ind]=old_x;
				}
				clock2[gh_ind].restart();
				
				// check ghost with pacman after eating pallet
				if(pallet_time>0 && x==gh_x[gh_ind] && y==gh_y[gh_ind])
				{
					sem_post(&permit);
					sem_post(&key);
					gh_x[gh_ind]=9+gh_ind;
					gh_y[gh_ind]=11;
					out[gh_ind]=0;
					score+=200;
				}
				sem_post(&s);
				// boost part
				if(bost>0)
				{
					sem_wait(&boost);
					boost_time=100;
					bost--;
					dl[gh_ind]=seconds(0.1);								/// change here for change value of boost
					cout<<"BOOST : ghost no."<<gh_ind<<" has boost"<<endl;						// check who  has the boost
				}			
				if(boost_time==0)
				{
					boost_time=-1;
					bost++;
					dl[gh_ind]=seconds(0.2);
					sem_post(&boost);
				}
				if(boost_time>0)
				{
					boost_time--;
				}

				sem_post(&gh);
			}
			else if (out[gh_ind]==0)
			{
				// wait for permit
				sem_wait(&permit);
				sem_wait(&key);
				gh_y[gh_ind]-=2;
				cout<<"PERMIT AND KEY : ghost no."<<gh_ind<<" has KEY AND PERMIT"<<endl;
			}
		}
		
	}
	return NULL;
}

void* pacman_collision(void* arg)
{
	while(window->isOpen() && !game_over)
	{
		sem_wait(&s);
        	if (clock3->getElapsedTime() >= *de_t)
        	{
				old_x=x;
				old_y=y;
				//movement of pacman
      			if(movement)
        		{
        			if(movement==3 && x-1>0)	
        			{
        				x-=1;
			        }	
			        else if(movement==4 && x+1<21)
			        {
			        	x+=1;
			        }
			        else if(movement==1 && y-1>0)
			        {
			        	y-=1;
			        }
			        else if(movement==2 && y+1<28)
			        {
			        	y+=1;
			        }
        		}
				// check collision with walls
				if(board[y][x]==1)
				{
					x=old_x;
					y=old_y;
				}
        								clock3->restart();
				if(pallet_time>0)
				pallet_time--;
				else if(pallet_time==0)
				{
					pallet_time=-1;
					sem_post(&pal);
				}
        	}

			// check collision with ghosts...
			for(int i=0;i<4;i++)
			{
				if(pallet_time<=0 && x==gh_x[i] && y==gh_y[i])
					{
						lives-=1;
						x=11;
						y=19;
						
					}
					if(lives==0)
					break;

			}
        	sem_post(&s);
		
	}
	return NULL;
}



	// main game Engine thread...
int main()
{
    XInitThreads();
    srand(time(0));

		// initialize semophores...
    sem_init(&s,0,1);				// for pacman variables
	sem_init(&gh,0,1);				// for ghosts variables
	sem_init(&pal,0,1);				// for pallets
	sem_init(&boost,0,1);			// for boosts 

	sem_init(&permit,0,2);			// for keys and permits for ghosts
	sem_init(&key,0,2);

	/// recources////
	// load sprites
	Texture obj1, obj2, obj3,obj4,obj5,obj6,obj7,obj8,obj9,obj10,obj11,obj12,obj13,obj14,obj15;  //Declare variables for Textures  
	//pacman
	obj1.loadFromFile("img/pc_up.png");
	obj2.loadFromFile("img/pc_down.png");
	obj3.loadFromFile("img/pc_left.png");
	obj4.loadFromFile("img/pc_right.png");
	//wall
	obj5.loadFromFile("img/wall.png");
	//pills
	obj6.loadFromFile("img/pill.png");
	obj7.loadFromFile("img/big-0.png");
	// ghosts
	obj8.loadFromFile("img/y-0.png");
	obj9.loadFromFile("img/r-0.png");
	obj10.loadFromFile("img/b-0.png");
	obj11.loadFromFile("img/p-0.png");
	// after eating pallet
	obj12.loadFromFile("img/d.png");
	obj13.loadFromFile("img/f-0.png");
	// home screen
	obj14.loadFromFile("img/home_1.png");
	obj15.loadFromFile("img/home_2.png");

	sf::Font font;	
    font.loadFromFile("img/text.otf");

	// rendering Window...
	window = new RenderWindow (VideoMode(MAX_X, MAX_Y), "PACMAN");

	// sprites..
	Sprite pc(obj1),map(obj5),pill(obj6),b_pill(obj7),eye(obj12),red_gh(obj9),y_gh(obj8),pi_gh(obj11),bl_gh(obj10),logo(obj15),image(obj14);
	
	/////=============HOME Screen=============/////
	bool con=0;
	while(window->isOpen() && !con)
    	{	
			Event e;
        	while (window->pollEvent(e))
        	{                    
        		if (e.type == Event::Closed)                   //If cross/close is clicked/pressed
                	window->close();
				if (e.type == Event::KeyPressed) 
            		{        
                		if(e.key.code==Keyboard::Enter)                     //To Exit Window
                		{
							con=1;
						}  
						if(e.key.code==Keyboard::Escape)                     //To Exit Window
                		{
							window->close();
						}       
					}
			}
			
			window->clear(Color::Black);
			// image
			image.setScale(0.4, 0.4);
			image.setPosition(100,100);
			window->draw(image);
			//logo
			logo.setPosition(300,30);
			window->draw(logo);
			// text
			Text t3;  
			t3.setFont(font); 
            t3.setCharacterSize(30);     
            t3.setFillColor(Color::White);
            t3.setString("Press\nEnter for Start...\nEscape for Exit...");
            t3.setPosition(100,600);

			window->draw(t3);
			window->display();
		}
		
	/////============================Main Game======================//////

	// timers
    Clock clock;
    sf::Time delayTime = seconds(0.2);
	clock3=new sf::Clock;
	de_t=new sf::Time;
	*de_t=seconds(0.2);

	// initialize value of ghosts
	for(int i=0;i<4;i++)
	{
		gh_x[i]=9+i;
		gh_y[i]=11;
	}

	/// create threads
    pthread_create(&ui_thread,NULL,UI_fnc,NULL);		// ui thread
	for(int i=0;i<4;i++)
	{
		int *gh_index = new int(i);
		pthread_create(&ghost_thread[i],NULL,ghost_fnc,(void*)gh_index);		// ghosts threads

	}
	pthread_create(&pacman_thread,NULL,pacman_collision,NULL);					// pacman movement, collision with walls and ghosts

    // main game engine loop
	while(window->isOpen() && !game_over)
    	{

    	    //-change sprite's textures-//
        	sem_wait(&s);
        	if (clock.getElapsedTime() >= delayTime)
        	{
      			if(movement)
        		{
        			if(movement==3 && x-1>0)	
        			{
						pc.setTexture(obj3);
			        }	
			        else if(movement==4 && x+1<21)
			        {
						pc.setTexture(obj4);
			        }
			        else if(movement==1 && y-1>0)
			        {
						pc.setTexture(obj1);
			        }
			        else if(movement==2 && y+1<28)
			        {
						pc.setTexture(obj2);
			        }
        		}

        	  clock.restart();
        	}
        	sem_post(&s);

        //---Display sprites Part---//
        window->clear(Color::Black);
        
		// map draw
        for(int i=0;i<28;i++)
        {
        	for(int j=0;j<22;j++)
        	{
        		if(board[i][j] == 1)
        		{
        			map.setPosition(j*30,i*30);
       				window->draw(map);
        		}
				if(board[i][j]==2)
				{
					pill.setPosition(j*30,i*30);
       				window->draw(pill);
				}
				else if(board[i][j]==3)
				{
					b_pill.setPosition(j*30,i*30);
       				window->draw(b_pill);
				}
        	}
        }

	// ghosts
		if(pallet_time>0)
		{
			// after pallet eating
			y_gh.setTexture(obj13);
			red_gh.setTexture(obj13);
			pi_gh.setTexture(obj13);
			bl_gh.setTexture(obj13);
		}
		else
		{
			// natural
			y_gh.setTexture(obj8);
			red_gh.setTexture(obj9);
			pi_gh.setTexture(obj11);
			bl_gh.setTexture(obj10);
		}

		// y ghost
		y_gh.setPosition(gh_x[0]*30,gh_y[0]*30);
		eye.setPosition(gh_x[0]*30,gh_y[0]*30);
		window->draw(y_gh);
		window->draw(eye);

		// red ghost
		red_gh.setPosition(gh_x[1]*30,gh_y[1]*30);
		eye.setPosition(gh_x[1]*30,gh_y[1]*30);
		window->draw(red_gh);
		window->draw(eye);
		// pink ghost
		pi_gh.setPosition(gh_x[2]*30,gh_y[2]*30);
		eye.setPosition(gh_x[2]*30,gh_y[2]*30);
		window->draw(pi_gh);
		window->draw(eye);
		// blure ghost
		bl_gh.setPosition(gh_x[3]*30,gh_y[3]*30);
		eye.setPosition(gh_x[3]*30,gh_y[3]*30);
		window->draw(bl_gh);
		window->draw(eye);

		// scoring....
		Text t,t2,t3,t4;  
		// scores....
            t.setFont(font); 
            t.setCharacterSize(30);     
            t.setFillColor(Color::White);
            t.setString(to_string(score));
            t.setPosition(700,155);

			t2.setFont(font); 
            t2.setCharacterSize(30);     
            t2.setFillColor(Color::Yellow);
            t2.setString("//Scores//");
            t2.setPosition(700,120);

		// lives....
			t3.setFont(font); 
            t3.setCharacterSize(30);     
            t3.setFillColor(Color::White);
            t3.setString(to_string(lives));
            t3.setPosition(700,235);

			t4.setFont(font); 
            t4.setCharacterSize(30);     
            t4.setFillColor(Color::Red);
            t4.setString("//Lives//");
            t4.setPosition(700,200);
            
            window->draw(t);
			window->draw(t2);
			window->draw(t3);
            window->draw(t4);


		// pacman
		pc.setPosition(x*30,y*30);
        window->draw(pc);

        window->display();				
		// game_over Checking...
		sem_wait(&s);
		if(lives==0 || pills_count==253)
		{
			game_over=1;
			break;
		}
		sem_post(&s);
    }	// end if game loop is closed..


	// exiting all threads....	
	pthread_cancel(ghost_thread[0]);
	pthread_cancel(ghost_thread[1]);
	pthread_cancel(ghost_thread[2]);
	pthread_cancel(ghost_thread[3]);
	pthread_cancel(pacman_thread);
	pthread_cancel(ui_thread);

	// game_over Screen...
	while(window->isOpen())
    	{	
			Event e;
        	while (window->pollEvent(e))
        	{                    
        		if (e.type == Event::Closed)                   //If cross/close is clicked/pressed
                	window->close();
				if (e.type == Event::KeyPressed) 
            		{        
                		if(e.key.code==Keyboard::Escape)                     //To Exit Window
                		{
							window->close();
						}       
					}
			}
			// game_over
			window->clear(Color::Black);
			Text t,t2,t3;  
			// scores....
            t.setFont(font); 
            t.setCharacterSize(30);     
            t.setFillColor(Color::White);
            t.setString(to_string(score));
            t.setPosition(400,120);
			t2.setFont(font); 
            t2.setCharacterSize(30);     
            t2.setFillColor(Color::Yellow);
            t2.setString("Your Scores : ");
            t2.setPosition(100,120);

			t3.setFont(font); 
            t3.setCharacterSize(30);     
            t3.setFillColor(Color::White);
            t3.setString("PRESS Escape for Exit...");
            t3.setPosition(100,600);

			window->draw(t);
			window->draw(t2);
			window->draw(t3);
			window->display();
		}

	// releasing heap
    delete window;
	delete clock3;
	delete de_t;

	// destroy semophores..
	sem_destroy(&s);
	sem_destroy(&gh);
	sem_destroy(&pal);
	sem_destroy(&permit);
	sem_destroy(&key);
	sem_destroy(&boost);

	exit(0);
  	return 0;  
}// END_Main




//NOTE :
// Prioriting the threads, key, permit, boost on he basis of CPU's Selection of threads..
