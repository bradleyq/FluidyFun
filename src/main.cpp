#define OUTPUT_ANIMATION 0

#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Simulator.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#if OUTPUT_ANIMATION
#include <opencv2/opencv.hpp>
#endif


inline float clip(const float& n, const float& lower, const float& upper)
{
    return glm::max(lower, glm::min(n, upper));
}

float theta = M_PI/8;
float phi = -M_PI/8+M_PI_2;
float dist = 2.5;
int width = 800;
int height = 800;
float zoom = 0.0f;
int iszoom = 0;
int frame = 0;
float rotationX = 0.0f;
float rotationY = 0.0f;
int isrotatedX = 0;
int isrotatedY = 0;
glm::dmat4 m = glm::dmat4(1.0f);
const int render_step = 3;
int mx, my;
const glm::dvec3 u = glm::dvec3(0, 1, 0);
glm::dvec3 w = glm::dvec3(0, 0, -1);
Simulator* simulator = new Simulator();
/*
void display(void);

void reshape(int width, int height);

void idle(void)
{
    particles.step();
    glutPostRedisplay();
    if(frame/render_step >= 300)
        return;
    if(frame%render_step == 0)
    {
        #if OUTPUT_ANIMATION
        cv::Mat3b image(height, width);
        glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        cv::flip(image, image, 0);
        char fn[512];
        sprintf(fn, "result/%04d.png", frame/render_step);
        cv::imwrite(fn, image);
        #endif
    }
    frame++;
}

void mouse(int button, int state, int x, int y);

void motion(int x, int y);

void keyboard(unsigned char c, int x, int y)
{
    switch(c)
    {
    case 'o' :
        break;
    }
}
*/


void glLookAt(double x, double y, double z, double dx, double dy, double dz, double d) // look at x,y,z along vector dx,dy,dz from distance -d.
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    float fov = 0.01;
    glFrustum(-fov, fov, fov, -fov, 0.01f, 10.0f);


    glMatrixMode( GL_MODELVIEW );
    w = glm::normalize(glm::dvec3(dx, dy, dz));
    glm::dvec3 r = glm::normalize(glm::cross(w, u));
    glm::dvec3 ul = glm::cross(r, w);
  //  GLdouble foo[16] = {
    //      1.0,  0.0,  0.0,  0.0,
      //    0.0,  1.0,  0.0,  0.0,
        //  0.0,  0.0,  1.0,  0.0,
          //0.0,  0.0,  -1.0,  1.0};
    
    GLdouble foo[16] = {
          r.x,  r.y,  r.z,  0.0,
          ul.x,  ul.y,  ul.z,  0.0,
         -w.x, -w.y, -w.z,  0.0,
         -x+d*w.x, -y+d*w.y, -z+d*w.z, 1.0};
    
    glLoadMatrixd(foo);
    //SDL_GL_SwapBuffers();  
    //glLoadIdentity();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glRotatef(0, 1.0f, 0.0f, 0.0f);
    //glRotatef(0, 0.0f, 1.0f, 0.0f);
    //glRotatef(0, 0.0f, 0.0f, 1.0f);
    //glTranslated(-x - d * dx, -y - d * dy, -z - d * dz);
}

void keyboardFunc(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_UP:
                //m = glm::rotate(m, 5.0, glm::dvec3(1.0f, 0.0f, 0.0f));
                rotationX += 5.0f;
                isrotatedX = 1;
                break;
            case GLFW_KEY_DOWN:
                //m = glm::rotate(m, -5.0, glm::dvec3(1.0f, 0.0f, 0.0f));
                rotationX -= 5.0f;
                isrotatedX = 1;
                break;
            case GLFW_KEY_LEFT:
                //m = glm::rotate(m, 5.0, glm::dvec3(0.0f, 1.0f, 0.0f));
                rotationY += 5.0f;
                isrotatedY = 1;
                break;
            case GLFW_KEY_RIGHT:
                //m = glm::rotate(m, -5.0, glm::dvec3(0.0f, 1.0f, 0.0f));
                rotationY -= 5.0f;
                isrotatedY = 1;
                break;
            default:
                break;
        }
    }
}

void scrollFunc(GLFWwindow* win, double x, double y) {
    zoom += 0.4f*y;
    iszoom = 1;
}

int main(int argc, char** argv)
{
    /**glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);

    (void)glutCreateWindow("GLUT Program");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    glutKeyboardFunc(keyboard);

    return EXIT_SUCCESS;**/
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Fluidy Fun", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboardFunc);
    glfwSetScrollCallback(window, scrollFunc);
    //glfwSetCursorPosCallback(window, mouseFunc);
    //glfwSetMouseButtonCallback(window, clickFunc);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    /* Loop until the user closes the window */
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        //start
        glLoadIdentity();

        /*if (iszoom) {
            glTranslatef(0.0f, 0.0f, zoom);
            zoom = 0.0f;
            iszoom = 0;
        }
        glMatrixMode(GL_MODELVIEW);
        if (isrotatedX) {
            glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
            rotationX = 0;
            isrotatedX = 0;
        }
        if (isrotatedY) {
            glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
            rotationY = 0;
            isrotatedY = 0;
        }*/
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 1.5);
        glTranslatef(0.0f, 0.0f, zoom);
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        simulator->step();

        simulator->render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/*
void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // your drawing code goes here
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.01, 100);
    gluLookAt(dist*sin(phi)*cos(theta), dist*cos(phi), dist*sin(phi)*sin(theta),
            0, 0, 0,
            0, 1, 0);

    particles.render();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mx = x;
        my = y;
    }
}

void motion(int x, int y)
{
    int dx = x - mx;
    int dy = y - my;
    mx = x;
    my = y;
    if(abs(dx) > abs(dy))
        theta += dx*0.005;
    else
        phi -= dy*0.005;
    if(theta > 2*M_PI)
        theta -= 2*M_PI;
    if(theta < -2*M_PI)
        theta += 2*M_PI;
    phi = clip(phi, M_PI/12, M_PI*11/12);
    glutPostRedisplay();
}*/
