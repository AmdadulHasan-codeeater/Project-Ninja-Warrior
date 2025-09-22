#include "iGraphics.h"
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

#define SCREEN_WIDTH 960
#define SCREEN_Hight 540
#define BUILDING_WIDTH 400
//#define NUM_BUILDINGS (SCREEN_WIDTH / BUILDING_WIDTH + 2)
#define bgspeed 5
#define Total_Pic 8
#define FRAME_COUNT 6 
#define GAP_WIDTH 150
#define NUM_BUILDINGS 40

int nextPlacementIndex = 0;   



//bg and building
int buildingX[NUM_BUILDINGS];
int buildingH[NUM_BUILDINGS];
int R[NUM_BUILDINGS];
int G[NUM_BUILDINGS];
int B[NUM_BUILDINGS];
int speed = 0;
int gamestate = 0;
int shitmusicplayed = 0;
int introMusicPlayed = 0;
int menuMusicPlayed = 0;
int buildingImg;
int pauseexit;
int restart;
int mainmenu;
int resume;
int pause;
int moveDir = 0;
int dif;

// hero
int X_axis = 280;
int Y_axis = 124;
int img[Total_Pic];
int index = 0;
int isJumping = 0;
int jumpHeight = 274;
int jumpSpeed = 10;
int isRunning = 0;
int D_pressed = 0;
int jumpCount = 0;
int fall = 0;
int life = 3;



//enemy
int frames[FRAME_COUNT];
int currentFrame = 0;
int enemyX = SCREEN_WIDTH;
int enemyY = 240;
int direction = -1;
int enemydir = 0;

//star
int star;
int starX[10], starY[10];
int score = 0;
int starmove = 0;

void loadImages()
{
	pauseexit = iLoadImage("exit.png");
	restart = iLoadImage("restart.png");
	mainmenu = iLoadImage("mainmenu.png");
	pause = iLoadImage("pausegear.png");
	resume = iLoadImage("resume.png");
	star = iLoadImage("star.png");
}

void mainmenubutton()
{
	iShowBMP(50, 450, "play button.bmp");
	iShowBMP(50, 350, "BLUE BUTTON.bmp");
	iShowBMP(50, 250, "ABOUT2.bmp");
	iShowBMP(50, 150, "SETTINGS2.bmp");
	iShowBMP(50, 50, "highscore.bmp");
}

char backgroundimages[200][250] = { "bgpic\\Image-without-mist_01.bmp", "bgpic\\Image-without-mist_02.bmp", "bgpic\\Image-without-mist_03.bmp", "bgpic\\Image-without-mist_04.bmp"
, "bgpic\\Image-without-mist_05.bmp", "bgpic\\Image-without-mist_06.bmp", "bgpic\\Image-without-mist_07.bmp", "bgpic\\Image-without-mist_08.bmp", "bgpic\\Image-without-mist_09.bmp"
, "bgpic\\Image-without-mist_10.bmp", "bgpic\\Image-without-mist_11.bmp", "bgpic\\Image-without-mist_12.bmp", "bgpic\\Image-without-mist_13.bmp", "bgpic\\Image-without-mist_14.bmp",
"bgpic\\Image-without-mist_15.bmp", "bgpic\\Image-without-mist_16.bmp", "bgpic\\Image-without-mist_17.bmp", "bgpic\\Image-without-mist_18.bmp", "bgpic\\Image-without-mist_19.bmp"
, "bgpic\\Image-without-mist_20.bmp" };


struct bg
{
	int x;
	int y;
}; bg background[20];


void setbg()
{
	int sum = 0;
	for (int i = 0; i < 20; i++)
	{
		background[i].y = 0;
		background[i].x = sum;
		sum += 48;
	}
}

void changebg(){
	if (moveDir == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			background[i].x -= bgspeed;
			if (background[i].x <= 0)
			{
				background[i].x = SCREEN_WIDTH;
			}
		}
	}

}



/*int randomHight() {
return 250 + rand() % 101; // 250 to 350
}*/


int randomHight()
{
	int options[3] = { 200, 230, 260 };
	int index = rand() % 3;
	return options[index];
}



/*void initBuildings() {
for (int i = 0; i < NUM_BUILDINGS; i++) {
buildingX[i] = i * BUILDING_WIDTH;
buildingH[i] = randomHight();
}
}*/

/*void initBuildings()
{
for (int i = 0; i < NUM_BUILDINGS; i++) {
buildingX[i] = i * BUILDING_WIDTH;

if (i < 8)  // first 8 buildings
buildingH[i] = 250;
else

buildingH[i] = randomHight();

R[i] = 100 + rand() % 51;
G[i] = 100 + rand() % 51;
B[i] = 100 + rand() % 51;
}
}*/

void initBuildings()
{
	int currentX = 0;   
	nextPlacementIndex = 0;

	for (int i = 0; i < NUM_BUILDINGS; i++) {
		if (nextPlacementIndex > 8 && (nextPlacementIndex - 8) % 6 == 0) {
			currentX += GAP_WIDTH;

		}

		buildingX[i] = currentX;

		if (nextPlacementIndex < 8)  // first 8 are flat
			buildingH[i] = 250;
		else
			buildingH[i] = randomHight();

		R[i] = 100 + rand() % 51;
		G[i] = 100 + rand() % 51;
		B[i] = 100 + rand() % 51;

	
		currentX += BUILDING_WIDTH;


		nextPlacementIndex++;
	}
}


void pausemenu()
{
	iShowImage(400, 400, 150, 60, pauseexit);
	iShowImage(400, 300, 150, 60, restart);
	iShowImage(400, 200, 150, 60, mainmenu);
	iShowImage(400, 100, 150, 60, resume);

	/*iShowBMP(400, 400, "exit.bmp");
	iShowBMP(400, 300, "restart.bmp");
	iShowBMP(400, 200, "mainmenu.bmp");*/
}

void loadCharacterImages(){
	img[0] = iLoadImage("Fighter_run1.png");
	img[1] = iLoadImage("Fighter_run2.png");
	img[2] = iLoadImage("Fighter_run3.png");
	img[3] = iLoadImage("Fighter_run4.png");
	img[4] = iLoadImage("Fighter_run5.png");
	img[5] = iLoadImage("Fighter_run6.png");
	img[6] = iLoadImage("Fighter_run7.png");
	img[7] = iLoadImage("Fighter_run8.png");
}//hero

void saveScore(int score, int dif) {
	FILE *fp = NULL;
	errno_t err;

	if (dif == 0) {
		err = fopen_s(&fp, "easy_scores.txt", "a");
	}
	else {
		err = fopen_s(&fp, "hard_scores.txt", "a");
	}

	if (err == 0 && fp != NULL) {
		fprintf(fp, "%d\n", score);   // store score per line
		fclose(fp);
	}
	else {
		printf("Error opening file to save score.\n");
	}
}




void lifecount()
{
	iSetColor(100, 200, 100);
	if (life == 3)
	{
		iFilledCircle(100, 480, 20, 100);
		iFilledCircle(150, 480, 20, 100);
		iFilledCircle(200, 480, 20, 100);
	}
	else if (life == 2)
	{
		iFilledCircle(100, 480, 20, 100);
		iFilledCircle(150, 480, 20, 100);
	}
	else if (life == 1)
	{
		iFilledCircle(100, 480, 20, 100);
	}

	else if (life == 0)
	{
		gamestate = 69;
		saveScore(score, dif);
	}
}


void setupstars() {
	for (int i = 0; i < 10; i++) {
		starX[i] = 1000 + i * 200;             // keep spacing
		starY[i] = 350 + rand() % 100;        // random height (300–450)
	}
}


void checkCollision() {
	int heroLeft = X_axis + 100;
	int heroRight = X_axis + 150;
	int heroBottom = Y_axis + 120;
	int heroTop = Y_axis + 175;

	for (int i = 0; i < 10; i++) {
		int starLeft = starX[i];
		int starRight = starX[i] + 50;
		int starBottom = starY[i];
		int starTop = starY[i] + 50;

		if (heroRight > starLeft && heroLeft < starRight &&
			heroTop > starBottom && heroBottom < starTop)
		{
			score++;
		
			starX[i] = 900 + rand() % 300;
			starY[i] = 350 + rand() % 100;
		}
	}
}



void resetGame() {
	// Reset hero
	X_axis = 280;
	Y_axis = 124;
	index = 0;
	isJumping = 0;
	isRunning = 0;
	D_pressed = 0;
	jumpCount = 0;
	fall = 0;
	life = 3;

	// Reset background
	setbg();

	// Reset buildings
	srand(time(0));
	initBuildings();

	// Reset enemy
	enemyX = SCREEN_WIDTH;
	enemyY = 240;
	currentFrame = 0;
	enemydir = 0;

	// Reset speed & movement
	speed = 0;
	moveDir = 0;

	//reset stars
	setupstars();
	score = 0;
	starmove = 0;

}


int getHighScore(const char *filename) {
	FILE *fp;
	if (fopen_s(&fp, filename, "r") != 0 || fp == NULL) {
		return 0; 
	}

	int score, high = 0;
	while (fscanf_s(fp, "%d", &score) == 1) {
		if (score > high) {
			high = score;
		}
	}

	fclose(fp);
	return high;
}



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

void iDraw()
{
	iClear();
	if (gamestate == 0)
	{
		if (!shitmusicplayed)
		{
			PlaySound("music0.wav", NULL, SND_FILENAME | SND_ASYNC);
			shitmusicplayed = 1;
		}
		iShowBMP(0, 0, "ninja 2.bmp");
	}


	else if (gamestate == 1){
		if (!introMusicPlayed) {
			PlaySound("music1.wav", NULL, SND_ASYNC | SND_LOOP);
			introMusicPlayed = 1;
			menuMusicPlayed = 0;
		}
		iShowBMP(0, 0, "menubg.bmp");
		mainmenubutton();
	}
	else if (gamestate == 55)
	{
		iShowBMP(0, 0, "difficulty.bmp");
	}

	else if (gamestate == 2)
	{
		
		if (!menuMusicPlayed) {
			PlaySound("music2.wav", NULL, SND_ASYNC | SND_LOOP);
			menuMusicPlayed = 1;
			introMusicPlayed = 0;
		}

		for (int i = 0; i < 20; i++)
		{
			iShowBMP(background[i].x, background[i].y, backgroundimages[i]);
		}

		iShowImage(900, 500, 30, 30, pause);




		if (isRunning){
			iShowImage(X_axis, Y_axis, 250, 250, img[index]);
		}
		else
		{

			iShowImage(X_axis, Y_axis, 250, 250, img[0]);
		}

		for (int i = 0; i < NUM_BUILDINGS; i++)
		{
			iSetColor(R[i], G[i], B[i]);//buildings
			iFilledRectangle(buildingX[i], 0, BUILDING_WIDTH, buildingH[i]);
		}

		iShowImage(enemyX, enemyY, 100, 100, frames[currentFrame]);
		int heroLeft = X_axis+100;
		int heroRight = X_axis + 150;
		int heroBottom = Y_axis+120;
		int heroTop = Y_axis + 175;

		int enemyLeft = enemyX+30;
		int enemyRight = enemyX + 70;
		int enemyBottom = enemyY;
		int enemyTop = enemyY + 65;
		/*iSetColor(0, 255, 0);
		iRectangle(X_axis+100, Y_axis+120, 50, 100);
		iRectangle(enemyX+30, enemyY, 40, 65);*/
		if (heroRight > enemyLeft && heroLeft < enemyRight &&
			heroTop > enemyBottom && heroBottom < enemyTop) {
			// Collision happened
			enemyX = SCREEN_WIDTH - 100;
			life--;
		}

		lifecount();
		
		for (int i = 0; i < 10; i++)
		{
			iShowImage(starX[i], starY[i], 50, 50, star);
		}
		checkCollision();

		char str[20];
		iSetColor(255, 50, 55);
		sprintf_s(str, "Score: %d", score);
		iText(700, 450, str, GLUT_BITMAP_TIMES_ROMAN_24);
		


		
	}

	else if (gamestate == 3)
	{
		pausemenu();
		menuMusicPlayed = 0;
	}

	else if (gamestate == 4)
	{
		iShowBMP(0, 0, "INSIDE ABOUT.bmp");
	}
	else if (gamestate == 5)
	{
		iShowBMP(0, 0, "INSIDE SETTINGS.bmp");
	}
	else if (gamestate == 69)
	{
		iShowBMP(0, 0, "gameover.bmp");
		char str[50];
		iSetColor(255, 255, 255);
		sprintf_s(str, "Your Score: %d", score);
		iText(400, 290, str, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (gamestate = 87)
	{
		iShowBMP(205, 350, "hard.bmp");
		iShowBMP(205, 200, "easy.bmp");

		int hardHigh = getHighScore("hard_scores.txt");
		int easyHigh = getHighScore("easy_scores.txt");

		char str[50];

		sprintf_s(str, "High Score: %d", hardHigh);
		iSetColor(255, 255, 0);
		iText(470, 370, str, GLUT_BITMAP_TIMES_ROMAN_24);

		sprintf_s(str, "High Score: %d", easyHigh);
		iSetColor(0, 255, 0);
		iText(470, 220, str, GLUT_BITMAP_TIMES_ROMAN_24);
	}



}


void jump() {
	if (isJumping == 1) {   // going up
		Y_axis += jumpSpeed;
		if (Y_axis >= jumpHeight)
			isJumping = 2;
	}
	else if (isJumping == 2) {  // coming down
		Y_axis -= jumpSpeed;
		int landed = 0;

		for (int i = 0; i < NUM_BUILDINGS; i++) {
			int heroFeetX = X_axis + 125; 
			if (heroFeetX >= buildingX[i] && heroFeetX <= buildingX[i] + BUILDING_WIDTH) {
				int buildingTop = buildingH[i] - 122;
				if (Y_axis <= buildingTop) {
					Y_axis = buildingTop;
					isJumping = 0;  // 
					landed = 1;
					break;
				}
			}
		}

	}
}



/*void jump() {
if (isJumping == 1) {
Y_axis += 20;
jumpCount += 20;
if (jumpCount >= 120) {
isJumping = 2;
jumpCount = 0;
}
}
else if (isJumping == 2) {
Y_axis -= 20;

// if hero touches ground → stop falling
if (Y_axis <= 0) {
Y_axis = 0;
isJumping = 0;
}
}
}*/



void runAnimation() {
	if (D_pressed) {
		isRunning = 1;
		index++;
		if (index >= Total_Pic) index = 0;
	}
	else {
		isRunning = 0;
		index = 0;
	}

}


void changeFrame()
{
	if (enemydir == 1)
	{
		currentFrame++;
		if (currentFrame >= FRAME_COUNT) currentFrame = 0;
		enemyX += direction * 25+dif;
		if (enemyX < -100) {
			enemyX = SCREEN_WIDTH - 100;
		}
	}
}








/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{

}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (gamestate == 0 || gamestate == 4 || gamestate == 5 || gamestate == 87){
			gamestate = 1;
			shitmusicplayed = 0;
		}
		else if (gamestate == 1)
		{
			PlaySound(NULL, NULL, 0); // music stop
			introMusicPlayed = 0;
			menuMusicPlayed = 0;

			if (mx >= 50 && mx <= 300 && my >= 450 && my <= 464+50) {
				printf("Play Clicked\n");
				resetGame();
				gamestate = 55;
			}

			else if (mx >= 50 && mx <= 300 && my >= 250 && my <= 264+50) {
				printf("About Clicked\n");
				gamestate = 4;
			}

			else if (mx >= 50 && mx <= 300 && my >= 150 && my <= 164+50)
			{
				printf("Settings Clicked\n");
				gamestate = 5;
			}

			else if (mx >= 50 && mx <= 300 && my >= 350 && my <= 364+50) {
				exit(0);
			}
			else if (mx >= 50 && mx <= 300 && my >= 50 && my <= 64 + 50)
			{
				gamestate = 87;

			}

		}

		else if (gamestate == 2)
		{
			if (mx >= 900 && mx <= 930 && my >= 500 && my <= 530)
			{
				gamestate = 3;
				speed = 0;
				moveDir = 0;
				enemydir = 0;

			}
		}

		else if (gamestate == 3)
		{
			if (mx >= 400 && mx <= 550 && my >= 400 && my <= 460)
			{
				exit(0);
			}
			else if (mx >= 400 && mx <= 550 && my >= 300 && my <= 360)
			{
				resetGame();
				gamestate = 55;
			}
			else if (mx >= 400 && mx <= 550 && my >= 200 && my <= 260)
			{
				gamestate = 1;
				resetGame();
			}
			else if (mx >= 400 && mx <= 550 && my >= 100 && my <= 160)
			{
				gamestate = 2;
				speed = 8;
				moveDir = 1;
				enemydir = 1;
				
			}

		}
		else if (gamestate == 69)
		{
			printf("Mouse Clicked at (%d, %d)\n", mx, my);

			if (mx >= 290 && mx <= 680 && my >= 190 && my <= 265)
			{
				resetGame();
				gamestate = 55;
			}

			if (mx >= 290 && mx <= 680 && my >= 80 && my <= 150)
			{
				
				gamestate = 1;
				resetGame();
			}

		}
		else if (gamestate == 55)
		{	
			printf("Mouse Clicked at (%d, %d)\n", mx, my);

			if (mx >= 350 && mx <= 610 && my >= 119 && my <= 167)
			{
			printf("hard clicked");
				resetGame();
				gamestate = 2;
				dif = 5;
			}

			else if (mx >= 350 && mx <= 610 && my >= 230 && my <= 300)
			{
			printf("easy clicked");
			resetGame();
			gamestate = 2;
			dif = 0;
			}
		}

		
	}


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{

	if (key == 's')
	{
		moveDir = 0;
	}
	if (key == ' ' && isJumping == 0) {
		isJumping = 1;
	}

	if (key == 'd' || key == 'D') {
		D_pressed = 1;
		X_axis += 5;              // Move left
		if (X_axis > 960 - 250)
			X_axis = 960 - 250;
	}

	if (key == 'a' || key == 'A') {
		X_axis -= 5;              // Move left
		if (X_axis < 0) X_axis = 0;
	}


}

/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{


	if (key == GLUT_KEY_RIGHT)
	{
		speed = 8;
		moveDir = 1;
		D_pressed = 1;
		enemydir = 1;
		starmove = 1;

	}
	if (key == GLUT_KEY_LEFT)
	{
		speed = 0;
		moveDir = 0;
	}

	if (key == GLUT_KEY_HOME)
	{
		gamestate = 3;
	}

}

void buildingMove() {
	if (speed > 0) {
		for (int i = 0; i < NUM_BUILDINGS; i++) {
			buildingX[i] -= speed+dif;

			if (buildingX[i] + BUILDING_WIDTH <= 0) {
				int maxX = 0;
				for (int j = 0; j < NUM_BUILDINGS; j++) {
					if (buildingX[j] > maxX)
						maxX = buildingX[j];
				}

				// gap after 6 buildings
				if (nextPlacementIndex > 8 && (nextPlacementIndex - 8) % 6 == 0) {
					buildingX[i] = maxX + BUILDING_WIDTH + GAP_WIDTH;
				}
				else {
					buildingX[i] = maxX + BUILDING_WIDTH;
				}

				buildingH[i] = randomHight();
				R[i] = 100 + rand() % 51;
				G[i] = 100 + rand() % 51;
				B[i] = 100 + rand() % 51;

				nextPlacementIndex++;
			}
		}
	}
}


void movestars() {
	if (starmove == 1){
		for (int i = 0; i < 10; i++) {
			starX[i] -= 8 + dif;
			if (starX[i] < -50) {
				starX[i] = 900 + rand() % 300;    
				starY[i] = 350 + rand() % 100;    
			}
		}
	}
}



void updateHeroPosition() {
	if (isJumping == 0) {
		int heroFeetX = X_axis + 125;  

		int found = -1;
		for (int i = 0; i < NUM_BUILDINGS; i++) {
			if (heroFeetX >= buildingX[i] && heroFeetX <= buildingX[i] + BUILDING_WIDTH) {
				found = i;
				break;
			}
		}

		if (found != -1) {
			
			Y_axis = buildingH[found] - 122;
		}
		else {
			
			isJumping = 3;
		}
	}

	
	if (isJumping == 3) {
		Y_axis -= 5;  
		if (Y_axis <= -20) {
			int nextB = -1;
			for (int i = 0; i < NUM_BUILDINGS; i++) {
				if (buildingX[i] > X_axis) {
					nextB = i;
					break;
				}
			}
			if (nextB != -1) {
				Y_axis = buildingH[nextB] - 122;
				isJumping = 0; 
			}
			life--;
		}
	}
}






int main()
{

	setbg();
	srand(time(0));
	initBuildings();
	iSetTimer(20, buildingMove);
	iSetTimer(120, changebg);
	iSetTimer(20, jump);//hero
	iSetTimer(20, updateHeroPosition);
	iSetTimer(100, runAnimation);//hero
	setupstars();
	iSetTimer(50, movestars);
	for (int i = 0; i<FRAME_COUNT; i++){
		char filename[100];
		sprintf_s(filename, sizeof(filename), "frames\\frame%d.png", i + 1);
		frames[i] = iLoadImage(filename);
	}
	iSetTimer(100, changeFrame);
	iInitialize(SCREEN_WIDTH, SCREEN_Hight, "NINJA WARRIOR");
	for (int i = 0; i<FRAME_COUNT; i++){
		char filename[100];
		sprintf_s(filename, sizeof(filename), "frames\\frame%d.png", i + 1);
		frames[i] = iLoadImage(filename);
	}
	loadImages();
	loadCharacterImages();//hero
	iStart();
	return 0;
}


