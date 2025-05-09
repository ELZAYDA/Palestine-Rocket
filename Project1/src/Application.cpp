#include <GL/glut.h>
#include <math.h>

// Rocket parameters
float rocketPos[3] = { 0.0f, 0.0f, -5.0f };
float rocketRot[3] = { 0.0f, 0.0f, 0.0f };
float rocketScale = 1.0f;
bool reflectX = false, reflectY = false;
float shellingScaleZ = 1.0f;  // بدلاً من shellingScale القديم

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void applyTransformations() {
    glTranslatef(rocketPos[0], rocketPos[1], rocketPos[2]);

    // Apply reflection before rotation and scaling
    if (reflectX) glScalef(-1.0f, 1.0f, 1.0f);
    if (reflectY) glScalef(1.0f, -1.0f, 1.0f);

    glRotatef(rocketRot[0], 1.0f, 0.0f, 0.0f); // X-axis
    glRotatef(rocketRot[1], 0.0f, 1.0f, 0.0f); // Y-axis

    // Scaling with shelling effect only on Z-axis
    glScalef(rocketScale, rocketScale, rocketScale * shellingScaleZ);
}

void drawRocket() {
    glPushMatrix();
    applyTransformations();

    GLUquadricObj* quadric = gluNewQuadric();

    // Black section (bottom)
    glColor3f(0.0f, 0.0f, 0.0f);
    gluCylinder(quadric, 0.5, 0.5, 2.0, 32, 32);
    gluDisk(quadric, 0.0, 0.5, 32, 32);

    // White section (middle)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(quadric, 0.5, 0.5, 2.0, 32, 32);
    gluDisk(quadric, 0.0, 0.5, 32, 32);
    glPopMatrix();

    // Green section (top)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 4.0f);
    glColor3f(0.0f, 0.5f, 0.0f);
    gluCylinder(quadric, 0.5, 0.5, 2.0, 32, 32);
    gluDisk(quadric, 0.0, 0.5, 32, 32);
    glPopMatrix();

    // Red nose cone
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 6.0f);
    glColor3f(0.8f, 0.0f, 0.0f);
    gluCylinder(quadric, 0.5, 0.0, 1.5, 32, 32);
    glPopMatrix();

    // Fins (4 fins)
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(90 * i, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.0f, 0.5f, 1.5f);
        glVertex3f(0.0f, 1.2f, 0.0f);
        glEnd();
        glPopMatrix();
    }

    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw ground
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, -1.0f, -10.0f);
    glVertex3f(-10.0f, -1.0f, 10.0f);
    glVertex3f(10.0f, -1.0f, 10.0f);
    glVertex3f(10.0f, -1.0f, -10.0f);
    glEnd();

    drawRocket();

    // Display controls
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.9f, 0.9f);
    const char* controls = "Arrows: Move  W/S: RotX  A/D: RotY  +/-: Scale";
    for (const char* c = controls; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    glRasterPos2f(-0.9f, 0.8f);
    const char* controls2 = "Hold X: Shell Effect  1/2: Reflect  ESC: Exit";
    for (const char* c = controls2; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeys(int key, int x, int y) {
    float step = 0.1f;

    switch (key) {
    case GLUT_KEY_UP: rocketPos[1] += step; break;
    case GLUT_KEY_DOWN: rocketPos[1] -= step; break;
    case GLUT_KEY_LEFT: rocketPos[0] -= step; break;
    case GLUT_KEY_RIGHT: rocketPos[0] += step; break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float angleStep = 5.0f;
    float scaleStep = 0.1f;

    switch (key) {
        // Rotation
    case 'w': rocketRot[0] += angleStep; break;
    case 's': rocketRot[0] -= angleStep; break;
    case 'a': rocketRot[1] += angleStep; break;
    case 'd': rocketRot[1] -= angleStep; break;

        // Scaling
    case '+': rocketScale += scaleStep; break;
    case '-': rocketScale = fmax(0.1f, rocketScale - scaleStep); break;

        // Reflection
    case '1': reflectX = !reflectX; break;
    case '2': reflectY = !reflectY; break;

        // Shelling effect on Z-axis
    case 'x': shellingScaleZ = fmax(0.5f, shellingScaleZ - 0.1f); break;
    case 'z': shellingScaleZ += 0.1f; break;

    case 27: exit(0); break; // ESC
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Palestinian Rocket with Scaling Shelling");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Register the new key press/release functions

    glutMainLoop();
    return 0;
}