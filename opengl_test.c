#include <GL/gl.h>
#include <GL/glu.h>       // For perspective projection
#include <GLFW/glfw3.h>   // For window and context management
#include <math.h>

void drawCube() {
    glBegin(GL_QUADS);  // Start drawing the cube with quads

    // Front face (z = 1.0)
    glColor3f(1.0, 0.0, 0.0);  // Red
    glVertex3f(-1.0, -1.0, 1.0);
    glVertex3f( 1.0, -1.0, 1.0);
    glVertex3f( 1.0,  1.0, 1.0);
    glVertex3f(-1.0,  1.0, 1.0);

    // Back face (z = -1.0)
    glColor3f(0.0, 1.0, 0.0);  // Green
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);

    // Left face (x = -1.0)
    glColor3f(0.0, 0.0, 1.0);  // Blue
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f(-1.0,  1.0,  1.0);
    glVertex3f(-1.0,  1.0, -1.0);

    // Right face (x = 1.0)
    glColor3f(1.0, 1.0, 0.0);  // Yellow
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0,  1.0, -1.0);
    glVertex3f(1.0,  1.0,  1.0);
    glVertex3f(1.0, -1.0,  1.0);

    // Top face (y = 1.0)
    glColor3f(0.0, 1.0, 1.0);  // Cyan
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0,  1.0);
    glVertex3f( 1.0, 1.0,  1.0);
    glVertex3f( 1.0, 1.0, -1.0);

    // Bottom face (y = -1.0)
    glColor3f(1.0, 0.0, 1.0);  // Magenta
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f(-1.0, -1.0,  1.0);

    glEnd();  // Done drawing the cube
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "3D Scene", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);

    float angle = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, 640.0/480.0, 0.1, 100.0);

        // Move the "camera" back so we can see the object
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -5.0);

        // Rotate the cube over time
        glRotatef(angle, 1.0, 1.0, 0.0);
        angle += 0.5f;  // Adjust to change rotation speed

        // Draw the cube
        drawCube();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
