//opengl cross platform includes
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include<iostream>
#include<ctime>
#include<cmath>
using namespace std;
/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */

//draw with grid
int draw = 0;
//draw with triangle or quad: 0 - quad, 1 - triangle
int drawQT = 0;
//shading
bool isFlat = true;
//ColorOrGrey
int isColor = 0; // 0 - Grey scale, 1 - Colour scale
//Snowman-character
float pos[] = {0,0,0};
float rot[] = {0, 0, 0};
float headRot[] = {0, 0, 0};

//Lighting
int LightingMode = 0; // 0 - off, 1 - on

float light0x=1;
float light0z=1;
float light1x=0;
float light1z=1;

//Circle {X,Y,Radius,Disp}
float circle[50][3];
// = {{30,20,30},{40,40.5,40},{10,13,45},{20,15,30},{45,45,15}};
float disp[50];
// = {5,-2.2,2,1.5,1};
float pointDis;
	float powerX;
	float powerY;
//Colour
float maxH = 0;
float minH = 0;
float avgHeight= 0;
float ColourScale = 0;
//init map size
extern int MapSizeXY = 50;
float Height[301][301];
float NormalFace[300][300][3];
float NormalVetex[300][300][3];
/*** 
 EYE LOCATION
 ***/
float eye[]={0,100,0};
float lookX = 0;
float lookZ = 0;
float deltaAngle = 0;
float deltaAngle2 = 0;
float Xorigin = 0;
float Yorigin = 0;
float angle = 0;

//cameramouse
float mousex = 0;
float mousey = 0;


float eyeRotate[] = {0,0}; //angle, y - rotation, z - rotaion.



void reset(void){
	mousex = 0;
	mousey = 0;
	angle = 0;
	deltaAngle =0;
	deltaAngle = 0;
	Xorigin = 0;
	Yorigin = 0;
	isFlat = true;
	isColor=0;
	eye[0] = 0;
	eye[1] = 100;
	eye[2] = 0;
	eyeRotate[0] = 0;
	eyeRotate[1] = 0;
	lookX = 0;
	lookZ = 0;
	draw = 0;
	drawQT = 0 ;
	light0x = 1;
	light0z = 1;
	light1x = 0;
	light1z = 1;
	LightingMode = 0;

}


void EnableLight(void){


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1); 	

	float light_pos[]={light0x,0,light0z,0};
	float amb0[4]  = {0.2, 0.2, 0, 1}; 
	float diff0[4] = {0.8, 0.8, 0, 1}; 
	float spec0[4] = {1, 1, 1, 1}; 
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0); 
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


	float light_pos1[]={light1x,0,light1z,0};
	float amb1[4]  = {0, 0.4, 0.4, 1}; 
	float diff1[4] = {0, 0.6, 0.6, 1}; 
	float spec1[4] = {1, 1, 1, 1}; 
	
    	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1); 
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);

	float m_amb[4]  = {0.2, 0.2, 0.2, 1}; 
	float m_diff[4] = {0.8, 0.8, 0.8, 1}; 
	float m_spec[4] = {1, 1, 1, 1}; 
	float shiny = 100;


	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,m_amb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,m_diff);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,m_spec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);



}

void initCircles(void){
	srand(time(0));
	for(int i = 0; i < (sizeof(circle)/4/3);i++){
		
		circle[i][0] = rand()%MapSizeXY;
		circle[i][1] = rand()%MapSizeXY;
		circle[i][2] = rand()%(MapSizeXY-6)+6;
	}
	
	for(int j = 0; j < (sizeof(disp)/4);j++){
		disp[j] = rand()%10;
	}

}


void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);


    /************************************************************************
     
                            PERSPECTIVE SET UP!
     
     modelview moves the eye and objects, projection is for camera type
     frustum / ortho left, right, bottom, top, nearVal, farVal
     
     ************************************************************************/
    	glMatrixMode(GL_PROJECTION);

	glFrustum(-10,10,-10,10,0,100);
	gluPerspective(45,1,1,1000);

}


void initHeightField(void){
	for(int i = 0;i<MapSizeXY+1;i++){
		for(int j = 0;j<MapSizeXY+1;j++){
			Height[i][j]=0;
		}
	}

}


void changeFrame(void){
	if(draw < 2){
		draw++;
	}
	else if(draw ==2 ){
		draw = 0;
	}
}

void changeShade(void){
	if(isFlat){
		isFlat = false;
	}
	else if(!isFlat){
		isFlat = true;
	}
}

void changeLighting(void){
	if(LightingMode == 0){
		LightingMode = 1;
	}
	else if(LightingMode == 1){
		LightingMode = 0;	
	}

}

void changeColor(void){
	if(isColor == 0){
		isColor = 1;
	}
	else if(isColor == 1){
		isColor = 0;	
	}	

}
//keyboard quit 
void keyboard(unsigned char key, int xIn, int yIn){
	switch(key){
		case 'Q':
		case 'q':
		case 27:
			exit(0);
			break;
		case 'R':
			reset();
			break;
		case 'r':
			initCircles();
			break;
		case 'w':
		case 'W':
			changeFrame();
			break;
		case 'f':
		case 'F':
			changeShade();
			break;
		case 't':
		case 'T':
			drawQT = 1;
			draw = 0;
			break;
		case 'y':
		case 'Y':
			drawQT = 0;
			draw = 0;
			break;
		case 'L':
		case 'l':
			changeLighting();
			break;
		case 'c':
		case 'C':
			changeColor();
			break;
		case 'u':
		case 'U':
			light0x -= 0.1;
			break;
		case 'I':
		case 'i':
			light0x += 0.1;
			break;
		case 'O':
		case 'o': 
			light1x -=0.1;
			break;
		case 'P':
		case 'p':
			light1x +=0.1;
			break;

	}
	glutPostRedisplay();
}

void changeCam(int a){
	//turn left
	if(a == 0){
}


}
void mouseButton(int button, int state,int x, int y){
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_UP){
			angle = 0;
			Xorigin = -1;
			Yorigin = -1;

		}
		else{
			Xorigin = x;
			Yorigin = y;
		}
	}


}
void move(int x,int y){
	if(Xorigin >= 0){
		deltaAngle = (x - Xorigin)*0.001f;
		deltaAngle2 = (y - Yorigin)*0.001f; 
		lookX = sin(angle-deltaAngle)*70;
		lookZ = sin(angle-deltaAngle2)*90;


	}

glutPostRedisplay();

}


void special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT: 
			lookX = 0;
			lookZ = 0;
			eyeRotate[1] +=1;

			break; 
		case GLUT_KEY_RIGHT: 
			lookX = 0;
			lookZ = 0;
			//eye[0]+=0.3;
			eyeRotate[1] -=1;

			break;
		case GLUT_KEY_UP:
			//eyemove+=1;
			eyeRotate[0] +=1;
			//cammove += 10;

			break;
		case GLUT_KEY_DOWN:
			eyeRotate[0] -=1;
			//eye[1]+=0.3;
			//cammove -= 10;
			break;
		 
		
    }
	glutPostRedisplay();
}
//---------------------------------ColourMap-----------------------------------
//detect Highest/Lowest RGB
void HLrgb(void){
	for(int i = 0; i < (sizeof(Height[MapSizeXY+1])/4);i++){
		for(int j = 0; j < (sizeof(Height[MapSizeXY+1])/4);j++){
			if(Height[i][j]> maxH)
				maxH = Height[i][j];
			else if(Height[i][j]< minH)
				minH = Height[i][j];
		}
	}
}
//Average the Height
float AverageHeight(float h1,float h2,float h3,float h4){
	float avgH = (h1+h2+h3+h4)/4;
	return avgH;
}
//Setting RGB for each quad
float RGBscale(float height) {
	float scale = 0;
	scale = (height-minH)/(maxH-minH);
	return scale;
}

//---------------------------------HeightMap------------------------------
//Calculate the height
void HeightField(int x, int y){

	for(int i = 0; i < (sizeof(circle)/4/3) ; i ++){
		
		powerX = pow((x-circle[i][0]),2);
		powerY = pow((y-circle[i][1]),2);
		pointDis = sqrt(powerX+powerY)*2/circle[i][2];
		if(fabs(pointDis) <= 1.0){
			Height[x][y] += disp[i]/2 + cos(fabs(pointDis)*3.14)*disp[i]/2;		
		}

	}
}
//Setting the height 
void ChangeHeight(void){

	for(int i = 0; i < MapSizeXY+1;i++){	
		for(int j = 0; j < MapSizeXY+1; j++){
			HeightField(i,j);
		}
	}
	
}
//Calculate normal
void getFaceNormal(int x,int z){
	//vector 1
	float v1x = 1;
	float v1y = Height[x+1][z] - Height[x][z];
	float v1z = 0;
	
	//vector 2
	float v2x = 0;
	float v2y = Height[x][z+1] - Height[x][z];
	float v2z = 1;


	float vx = v1y*v2z - v1z*v2y;
	float vy = v1z*v2x - v1x*v2z;
	float vz = v1x*v2y - v1y*v2x;

	float normal = sqrt(pow(vx,2)+pow(vy,2)+pow(vz,2));
	
	NormalFace[x][z][0] = vx/normal;
	NormalFace[x][z][1] = vy/normal;
	NormalFace[x][z][2] = vz/normal;
	

}
void CalculateNormal(void){
	for(int i = 0; i < MapSizeXY; i++){
		for(int j = 0; j < MapSizeXY; j++){
			getFaceNormal(i,j);

		}
	}

}

//draw the color map (for quads and triangle)
void DrawColor (int a, int b){
	float ColorScale = RGBscale(Height[a][b]);
	if(Height[a][b]>=0 && Height[a][b]<10){
		glColor3f(1*ColorScale,2.5*ColorScale,4*ColorScale);							
	}
	else if(Height[a][b]>=10 && Height[a][b]<25){
		glColor3f(0.8*ColorScale,2*ColorScale,0.8*ColorScale);
	}
	else if(Height[a][b]>=25 && Height[a][b]<30){
		glColor3f(ColorScale*1.8,ColorScale*1.8,ColorScale*1.5);
	}

}


//draw the map with quads
void drawMapQuad(int row, int column){

	glColor3f(0,1,0);
	if(draw == 1 || draw == 2){
		for(int i = 0; i < row+1;i++){	
			for(int k = 1; k < column+1; k++){
		
			glBegin(GL_LINES);

				//previous point
				glVertex3f(k-1,Height[k-1][i],i);
				//current point
				glVertex3f(k,Height[k][i],i);		
			glEnd();
			}	
		}
	

		for(int j = 0; j < column+1;j++){
			for(int l = 1; l < column+1; l++){
			glBegin(GL_LINES);
				//previous point
				glVertex3f(j,Height[j][l-1],l-1);
				//current point
				glVertex3f(j,Height[j][l],l);		
			glEnd();
			}	
		} 
	}

	//draw QUADs
	if(draw == 0 || draw == 2){
		
		for (int i=0; i<row; i++) {
			
			for (int j=0; j<column; j++) {
					glNormal3f(NormalFace[i][j][0],NormalFace[i][j][1],NormalFace[i][j][2]);
					glFrontFace(GL_CCW);
			glBegin(GL_QUAD_STRIP);						
					if(isColor == 0){
						
						glColor3f(Height[i][j+1]/maxH, Height[i][j+1]/maxH, Height[i][j+1]/maxH);
						glVertex3f(i,Height[i][j+1],j+1);
						glColor3f(Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH);				
						glVertex3f(i+1,Height[i+1][j+1],j+1);
						glColor3f(Height[i][j]/maxH, Height[i][j]/maxH, Height[i][j]/maxH);
						glVertex3f(i,Height[i][j],j);
						glColor3f(Height[i+1][j]/maxH, Height[i+1][j]/maxH, Height[i+1][j]/maxH);
						glVertex3f(i+1,Height[i+1][j],j);
					}
					else if(isColor == 1){
						
						for(int a = i ; a< i+2 ; a++){
							for(int b = j; b< j + 2 ; b++){
								DrawColor(a,b);
								glVertex3f(a,Height[a][b],b);
									
							}
						}
					}
					glEnd();
				
					
			}
			
		}
		
	

	}

}

//draw the map with triangle
void drawMapTrig(int row, int column){
	glColor3f(0,1,0);
	
	if(draw == 1 || draw == 2){
		for(int i = 0; i < row+1;i++){	
			for(int k = 1; k < column+1; k++){
		
			glBegin(GL_LINES);

				//previous point
				glVertex3f(k-1,Height[k-1][i],i);
				//current point
				glVertex3f(k,Height[k][i],i);		
			glEnd();
			}	
		}
	

		for(int j = 0; j < column+1;j++){
			for(int l = 1; l < column+1; l++){
			glBegin(GL_LINES);
				//previous point
				glVertex3f(j,Height[j][l-1],l-1);
				//current point
				glVertex3f(j,Height[j][l],l);		
			glEnd();
			}	
		} 
		for(int j = 0; j < column;j++){
			for(int l = 1; l < column; l++){
			glBegin(GL_LINES);
				//previous point
				glVertex3f(j,Height[j][l],l);
				//current point
				glVertex3f(j+1,Height[j+1][l+1],l+1);		
			glEnd();
			}	
		} 
	}

	if(draw == 0 || draw == 2){

		for (int i=0; i<row; i++) {
			for (int j=0; j<column; j++) {
					glNormal3f(NormalFace[i][j][0],NormalFace[i][j][1],NormalFace[i][j][2]);					
					glFrontFace(GL_CCW);
					glBegin(GL_TRIANGLE_STRIP);						
					if(isColor == 0){
						
						glColor3f(Height[i][j]/maxH, Height[i][j]/maxH, Height[i][j]/maxH);
						glVertex3f(i,Height[i][j],j);
						glColor3f(Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH);				
						glVertex3f(i+1,Height[i+1][j+1],j+1);
						glColor3f(Height[i+1][j]/maxH, Height[i+1][j]/maxH, Height[i+1][j]/maxH);
						glVertex3f(i+1,Height[i+1][j],j);
					}
					else if(isColor == 1){
						DrawColor(i,j);
						glVertex3f(i,Height[i][j],j);
						DrawColor(i+1,j+1);			
						glVertex3f(i+1,Height[i+1][j+1],j+1);
						DrawColor(i+1,j);
						glVertex3f(i+1,Height[i+1][j],j);

					}
					
					glEnd();
				
					
					glBegin(GL_TRIANGLE_STRIP);						
					if(isColor == 0){
						glColor3f(Height[i][j]/maxH, Height[i][j]/maxH, Height[i][j]/maxH);
						glVertex3f(i,Height[i][j],j);
						glColor3f(Height[i][j+1]/maxH, Height[i][j+1]/maxH, Height[i][j+1]/maxH);
						glVertex3f(i,Height[i][j+1],j+1);
						glColor3f(Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH, Height[i+1][j+1]/maxH);				
						glVertex3f(i+1,Height[i+1][j+1],j+1);
					}
					else if(isColor == 1){
						DrawColor(i,j);   
						glVertex3f(i,Height[i][j],j);
						DrawColor(i,j+1);
						glVertex3f(i,Height[i][j+1],j+1);
						DrawColor(i+1,j+1);	
						glVertex3f(i+1,Height[i+1][j+1],j+1);
					}
					glEnd();
					
			}
		}
	}
}

//Snowman
void DrawSnowman(float* pos, float* rot)
{	
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[1], 0, 1, 0);

	//draw body
	glColor3f(1,1,1);
	glutSolidSphere(1, 16, 16);

	//draw buttons
	glPushMatrix();
	glTranslatef(0, 0.35, 0.9);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.15, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.05, 0.95);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();


	glPushMatrix();
	//translate relative to body, and draw head
	glTranslatef(0, 1.25, 0);
	glRotatef(headRot[1], 0, 1, 0); //turn the head relative to the body
	glColor3f(1,1,1);
	glutSolidSphere(0.5, 16, 16);
	
	//translate and draw right eye
	glPushMatrix();
	glTranslatef(0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw left eye
	glPushMatrix();
	glTranslatef(-0.2, 0.15, 0.45);
	glColor3f(0,0,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	//translate and draw nose
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glColor3f(1,0.4,0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();

	glPopMatrix();//body
	glPopMatrix();//snowman
}


//display the windows and the content
void display(void)
{	

	glClearColor(0.5,0.5,0.5,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//ignore hidden faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	initHeightField();
	ChangeHeight();
	HLrgb();

	CalculateNormal();

	if(LightingMode == 1){
		EnableLight();
	}
	else if(LightingMode == 0){
		glDisable(GL_LIGHTING);

	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glPushMatrix();
	glRotatef(eyeRotate[0],1,0,0);
	
	gluLookAt(eye[0], eye[1], eye[2], MapSizeXY/2+lookX, 0, MapSizeXY/2+lookZ, 0, 1, 0);

	//Shading
	if(isFlat == 1){
		glShadeModel(GL_FLAT);
		//cout<<"flat"<<endl;
	}
	else if(isFlat == 0){
		glShadeModel(GL_SMOOTH);
		//cout<<"smooth"<<endl;
	}
	glPushMatrix();

	glTranslatef( MapSizeXY/2, 0, MapSizeXY/2);
	
	glRotatef(eyeRotate[1],0,1,0);
	glTranslatef( -MapSizeXY/2, 0, -MapSizeXY/2);

	if(drawQT == 0){
		drawMapQuad(MapSizeXY,MapSizeXY);
	}
	else if(drawQT == 1){
		drawMapTrig(MapSizeXY,MapSizeXY);
	}	
	glPopMatrix();
	
	glutSwapBuffers();
}

 

/* main function - program entry point */
int main(int argc, char** argv)
{		
	printf("enter the dimension of the map");
	scanf("%i",&MapSizeXY);

	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1024,800);
	glutInitWindowPosition(0,0);
		
	glutCreateWindow("Terrain");	//creates the window
	//gluOrtho2D(-1,MapSizeXY+1,-1,MapSizeXY+1);
	initCircles();
	

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(move);
	glutMouseFunc(mouseButton);
	init();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}