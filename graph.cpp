#include <iostream>
#include <list>
#include <GL/glut.h>

#define KEY_ESC 27
#define KEY_E 'e'       // show edge
#define KEY_V 'v'       // show vertex
#define KEY_R 'r'       // remove into 2 point
#define KEY_S 's'       // get node source
#define KEY_T 't'       // get node target
#define KEY_A 'a'

#include "src/graph.h"

typedef traits<float, double, double>	box;	// type box
typedef cpoint<box>						point;	// point
typedef cnode<box>                      node;  // point
typedef cgraph<box>						graph;	// graph

graph *tmp;								// graph

double row_size = 500;					// screen row size
double col_size = 1000;					// screen col size
double unit_size = 10;					// unit min

double ts = 10;

point pmin(0-ts,-ts);					// min point 
point pmax(col_size+ts,row_size+ts);	// max point

point minr(0,0);
point maxr(0,0);

node *node_source = new node(point(0,0));
node *node_target = new node(point(0,0));

bool state_edge = true;
bool state_vertex = true;
bool state_resize = true;

bool state_source = false;
bool state_target = false;

void gldraw(){
	if(state_vertex){
	    glPointSize(2);
	    glBegin(GL_POINTS);
	        glColor3d(255,0,0);
	        for(unsigned i=0; i<vpoints.size(); i++)
	            glVertex2d(vpoints[i].first,vpoints[i].second);
	    glEnd();
	}

	if(state_edge){
	    glLineWidth(1);    
	    glBegin(GL_LINES);
			for(unsigned i=0; i<vlines.size(); i+=2){
				glColor3d(0,255,0);
				glVertex2d(vlines[i].first,vlines[i].second);
				glVertex2d(vlines[i+1].first,vlines[i+1].second);
			}		
		glEnd();
	}

    if(state_source){
        glPointSize(5);
        glBegin(GL_POINTS);
            glColor3d(255,255,255);
            glVertex2d(node_source->m_data.get_x(),
                node_source->m_data.get_y());
        glEnd();
    }

    if(state_target){
        glPointSize(5);
        glBegin(GL_POINTS);
            glColor3d(255,255,0);
            glVertex2d(node_target->m_data.get_x(),
                node_target->m_data.get_y());
        glEnd();
    }
}

void glpaint(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glOrtho(pmin.get_x(),pmax.get_x(),pmin.get_y(),pmax.get_y(),-1.0f,1.0f);
    gldraw();
    glutSwapBuffers();
}

void idle(){
	glutPostRedisplay();
}

void on_click(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if(state_resize){
			minr.set_x(((x-ts)*col_size)/(col_size-(2*ts)));
			minr.set_y(row_size-(((y-ts)*row_size)/(row_size-(2*ts))));
		}
		else{
			maxr.set_x(((x-ts)*col_size)/(col_size-(2*ts)));
			maxr.set_y(minr.get_y());
			minr.set_y(row_size-(((y-ts)*row_size)/(row_size-(2*ts))));
		}
		state_resize = !state_resize;
	}
}


void init_GL(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// frame color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid window_redraw(GLsizei width, GLsizei height){
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(pmin.get_x(),pmax.get_x(),pmin.get_y(),pmax.get_y(),-1.0f,1.0f);
}

GLvoid window_key(unsigned char key, int x, int y) {
    switch (key) {
        case KEY_ESC:
            exit(0);
            break;
        case KEY_E:
        	if(state_edge){		state_edge = false;	state_vertex = true;	}
        	else{	state_edge = true;	state_vertex = true;	}
        	break;
        case KEY_V:
        	if(state_vertex){		state_vertex = false;	state_edge = true;	}
        	else{	state_edge = true;	state_vertex = true;	}
        	break;
        case KEY_R:        	
        	tmp->remove_node_in(minr, maxr);
        	// std::cout << minr << "\t" << maxr << "\n";
        	break;
        case KEY_S:{
            if(!state_resize){
                state_resize = true;
                node_source = tmp->near_to(minr);
                std::cout << node_source->m_data << "\n";
                state_source = true;
            }            
            break;
        }
        case KEY_T:{
            if(!state_resize){
                state_resize = true;
                node_target = tmp->near_to(minr);
                std::cout << node_target->m_data << "\n";
                state_target = true;
            }            
            break;
        }
        case KEY_A:
            tmp->a_asterisk(node_source, node_target);
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]){
	tmp = new graph(row_size,col_size,unit_size);
	tmp->create_regular_graph();
	tmp->save_to_opengl();

	std::cout << "min: " << pmin << "\n";
	std::cout << "max: " << pmax << "\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(col_size, row_size);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Regular Graph");
    init_GL();

    glutDisplayFunc(glpaint);

    glutReshapeFunc(&window_redraw);
    glutKeyboardFunc(&window_key);
    glutMouseFunc(&on_click);
    glutIdleFunc(&idle);

    glutMainLoop();

	delete tmp;
	return 0;
}