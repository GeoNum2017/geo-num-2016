#ifndef SIMPLE_VIEWER
#define SIMPLE_VIEWER
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Eigen/Dense> 
#include <typedefs.h>
bool ROTATE = false, RECOMPUTE_VIEWMAT = false;
double XPOS, YPOS;
double CAMRADIUS = 5.0;
glm::vec3 CAMLOOKAT = glm::vec3(0.0,0.0,0.0);
double MODELRADIUS = 1;
unsigned int VIEWPORT_W = 1280;
unsigned int VIEWPORT_H = 800;
bool SHOW_WIREFRAME = true;
class SimpleViewer
{        
    public: 
        GLFWwindow* window;
        // vertex array id
        GLuint vertex_array_id;
        // buffers
        GLuint vbo_vertices;
        GLuint vbo_normals;
        GLuint ibo_elements;
        // shader program id
        GLuint program_id;
        // uniform shader variables
        GLuint model_id, view_id, projection_id, color_id;
        // model, view, projection matrices
        glm::mat4 Model, View, Projection;
        // mesh topology = faces
        MatiX3 F;
        // mesh geometry
        MatdX3 V, N;
        glm::vec3 FACE_COLOR;
        glm::vec3 EDGE_COLOR;
        double az, el; // spherical angles for camera
        void set_facecolor(int r, int g, int b){FACE_COLOR=glm::vec3( (float)r/255.0,(float)g/255.0,(float)b/255.0);}
        void set_edgecolor(int r, int g, int b){EDGE_COLOR=glm::vec3( (float)r/255.0,(float)g/255.0,(float)b/255.0);}
        void set_mesh(
            const MatdX3 &newV,
            const MatiX3 &newF )
        {
            V = newV;
            F = newF;
        }        
        void clear_data()
        {
            V.setZero(0,3);
            N.setZero(0,3);
            F.setZero(0,3);
        }
        void add_patch(
            const MatdXX &X,
            const MatdXX &Y,
            const MatdXX &Z )
        {
            unsigned int countU = X.rows();
            unsigned int countV = X.cols();
            // check if matrices have the correct size
            assert( countU==Y.rows() && countU==Z.rows() &&  countV==Y.cols() && countV==Z.cols() );
            unsigned int numV = countU*countV;
            unsigned int numF = 2*(countU-1)*(countV-1);
            MatdX3 newV;
            MatiX3 newF;
            // V : generate geometry
            newV.resize(numV,3);
            newV.fill(0);
            MatdXX co;
            co = X; co.resize(numV,1); newV.col(0) = co;                        
            co = Y; co.resize(numV,1); newV.col(1) = co;
            co = Z; co.resize(numV,1); newV.col(2) = co;
            // F : generate topology
            newF.resize(numF,3);
            for(unsigned int i=0; i<countU-1; i++) {
                for(unsigned int j=0; j<countV-1; j++) {
                    short A = i*countU+j+0;
                    short B = A+1;
                    short C = (i+1)*countU+j+0;
                    short D = C+1;
                    newF.row( 2* (i*(countU-1) + j) + 0 ) = Veci3(A,B,D);
                    newF.row( 2* (i*(countU-1) + j) + 1 ) = Veci3(D,C,A);
                }
            }
            MatdX3 oldV = V; // oldN = N;
            MatiX3 oldF = F;
            unsigned int shift = 0;
            if (oldF.rows() > 0)
                shift = oldF.maxCoeff();
            if (shift>0)
                newF += (shift+1) * MatiX3::Ones(numF,3);
            V.resize(oldV.rows()+newV.rows(), oldV.cols());
            V << oldV,
                 newV;            
            F.resize(oldF.rows()+newF.rows(), oldF.cols());
            F << oldF,
                 newF;
            double x_mean = V.col(0).array().mean();
            double y_mean = V.col(1).array().mean();
            double z_mean = V.col(2).array().mean();
            CAMLOOKAT = glm::vec3(x_mean,y_mean,z_mean);
        }
        
        void fill_buffers( void )
        {            
            if(V.isZero(0) || F.isZero(0)) {std::cout << "WARNING: vertices or faces are empty." << std::endl;return;}
            std::cout << "setting positions" << std::endl;
            // set vertex positions
            glGenBuffers(1, &vbo_vertices);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
            glBufferData(GL_ARRAY_BUFFER, sizeof(V(0,0))*V.size(), &V(0,0), GL_STATIC_DRAW);
            std::cout << "setting faces" << std::endl;
            // set elements
            glGenBuffers(1, &ibo_elements);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(F(0,0))*F.size(), &F(0,0), GL_STATIC_DRAW);
            double AABBx = V.col(0).maxCoeff() - V.col(0).minCoeff();
            double AABBy = V.col(1).maxCoeff() - V.col(1).minCoeff();
            double AABBz = V.col(2).maxCoeff() - V.col(2).minCoeff();
            MODELRADIUS = std::max(AABBx, std::max(AABBy,AABBz) );
            CAMRADIUS   = 1.5 * MODELRADIUS; 
            RECOMPUTE_VIEWMAT = true;
        }
        
        void compile_shaders( void ) {
            // Logging
            GLint Result = GL_FALSE;
            int InfoLogLength;
            std::string mesh_vertex_shader_string = 
            "#version 330\nlayout(location = 0) in vec3 vertexPosition_modelspace;"
            "uniform mat4 Model;uniform mat4 View;uniform mat4 Projection;"
            "void main(){gl_Position =  Projection * View * Model * vec4(vertexPosition_modelspace,1);}";
            std::string mesh_fragment_shader_string = 
            "#version 330\nuniform vec3 color;out vec4 fragColor;"
            "void main(){fragColor = vec4(color,0);}";
            // Compile vertex shader
            char const * VertexSourcePointer = mesh_vertex_shader_string.c_str();
            GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
            std::cout << "Compiling vertex shader" << std::endl;
            glCompileShader(VertexShaderID);
            // Check vertex shader
            glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                    printf("%s\n", &VertexShaderErrorMessage[0]);
            }
            // Compile fragment shader
            GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
            std::cout << "Compiling fragment shader" << std::endl;
            char const * FragmentSourcePointer = mesh_fragment_shader_string.c_str();
            glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
            glCompileShader(FragmentShaderID);
            // Check Fragment Shader
            glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ) {
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf("%s\n", &FragmentShaderErrorMessage[0]);
            }
            // Link the GPU program
            std::cout << "Linking GPU program" << std::endl;
            //GLuint program_id = glCreateProgram();
            program_id = glCreateProgram();
            glAttachShader(program_id, VertexShaderID);
            glAttachShader(program_id, FragmentShaderID);
            glLinkProgram(program_id);
            // Check the GPU program
            glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ) {
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(program_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }
            // Detach shaders
            glDetachShader(program_id, VertexShaderID);
            glDetachShader(program_id, FragmentShaderID);
            // Delete shaders
            glDeleteShader(VertexShaderID);
            glDeleteShader(FragmentShaderID);
        }
        SimpleViewer()
        {            
            clear_data();
            az = 0.0f; // 0 deg
            el = - 3.14f / 3.0f; // -60 deg;
            set_facecolor(255,163,0); // dark orange
            set_edgecolor(55,55,55); // gray
            if( !glfwInit() )
            {
                fprintf( stderr, "Failed to initialize GLFW\n" );
                getchar();
            }
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            // Open a window and create its OpenGL context
            window = glfwCreateWindow( VIEWPORT_W, VIEWPORT_H, "TP6", NULL, NULL);
            if( window == NULL ){
                    getchar();
                    glfwTerminate();
            }
            glfwMakeContextCurrent(window);
            // Initialize GLEW
            glewExperimental = true; // Needed for core profile
            if (glewInit() != GLEW_OK) {
                    fprintf(stderr, "Failed to initialize GLEW\n");
                    getchar();
                    glfwTerminate();
            }
            glfwSetMouseButtonCallback(window, mouse_button_callback);
            glfwSetScrollCallback( window, mouse_scroll_callback );
            glfwSetWindowSizeCallback( window, window_resize_callback );
            glfwSetKeyCallback( window, option_toggle_callback );
            // Ensure we can capture the escape key being pressed below
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
            // Dark blue background
            glClearColor(1.0f,1.0f,1.0f,0.0f);
            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            // clear depth
            glClearDepth( 1.0f );
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LESS);
            // polygon offset
            glEnable( GL_POLYGON_OFFSET_FILL );
            glPolygonOffset( 1.0, 1.0 );
            // Vertex Array
            glGenVertexArrays(1, &vertex_array_id);
            glBindVertexArray(vertex_array_id);
            // compile shaders
            compile_shaders();
            // Get a handle for MVP matrices(uniform)
            model_id = glGetUniformLocation(program_id, "Model");
            view_id = glGetUniformLocation(program_id, "View");
            projection_id = glGetUniformLocation(program_id, "Projection");
            color_id = glGetUniformLocation(program_id, "color");
            update_camera(0,0);
        }
        void update_camera( double dx, double dy )
        {            
            az -= dx*0.005;
            el += dy*0.005;
            if( CAMRADIUS < 0.1 * MODELRADIUS)
                CAMRADIUS = 0.1 * MODELRADIUS;
            glm::vec3 direction = glm::vec3(
                CAMRADIUS * cos(el) * sin(az),
                CAMRADIUS * sin(el),
                CAMRADIUS * cos(el) * cos(az)
            );
            glm::vec3 zero = glm::vec3(0,0,0);
            glm::vec3 right = glm::vec3(
                sin( az - 3.14f/2.0f ),
                0,
                cos( az - 3.14f/2.0f )
            );
            glm::vec3 up = glm::cross( right, direction );
            // View matrix
            View = glm::lookAt(
                CAMLOOKAT+direction, // Camera is at (x,y,z), in World Space
                CAMLOOKAT,           // and looks at the origin
                up                   // Head is up (set to 0,-1,0 to look upside-down)
            );
            // Projection matrix : 45° Field of View, ratio, display range : 0.1 unit <-> 100 units
            Projection = glm::perspective(45.0f, (GLfloat) VIEWPORT_W / (GLfloat) VIEWPORT_H, 0.1f, 100.0f);
            glViewport(0, 0,VIEWPORT_W, VIEWPORT_H);
            // Model matrix = identity
            Model = glm::mat4(1.0f);
        }
        int show() {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            fill_buffers();
            do {
                // Clear the screen
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                double new_XPOS=XPOS, new_YPOS=YPOS;
                if (ROTATE)
                {
                    glfwGetCursorPos(window, &new_XPOS, &new_YPOS);
                    if (abs(new_XPOS - XPOS) > 0 || abs(new_YPOS - YPOS) > 0)
                        RECOMPUTE_VIEWMAT = true;
                }
                if (RECOMPUTE_VIEWMAT) {
                    update_camera(new_XPOS-XPOS,new_YPOS-YPOS);
                    XPOS = new_XPOS;
                    YPOS = new_YPOS;
                    RECOMPUTE_VIEWMAT = false;
                }
                // glfwGetTime is called only once, the first time this function is called
                static double lastTime = glfwGetTime();
                // current frame time
                double currentTime = glfwGetTime();
                // compute time difference between current and last frame
                float deltaTime = float(currentTime - lastTime);
		// update last frame
                lastTime = currentTime;
                // check for pressed keys
                key_press_callback(deltaTime);
                // Use shader
                glUseProgram(program_id);
                // Uniform : Model, View, Projection
                glUniformMatrix4fv(      model_id, 1, GL_FALSE, &Model[0][0]);
                glUniformMatrix4fv(       view_id, 1, GL_FALSE, &View[0][0]);
                glUniformMatrix4fv( projection_id, 1, GL_FALSE, &Projection[0][0]);
                // 0 : Vertex positions
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
                glVertexAttribPointer(0,3,GL_DOUBLE,GL_FALSE,0,(void*)0);
                // Elements = Triangles
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
                int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
                // Set color
                // draw faces
                glUniform3f( color_id, FACE_COLOR.r, FACE_COLOR.g, FACE_COLOR.b );
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
                // draw edges
                glUniform3f( color_id, EDGE_COLOR.r, EDGE_COLOR.g, EDGE_COLOR.b );
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
                // Disable attribute arrays
                glDisableVertexAttribArray(0);
                // Swap buffers
                glfwSwapBuffers(window);
                glfwPollEvents();

            } // Check if the ESC key was pressed or the window was closed
            while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );
            // Cleanup VBO and GPU program
            glDeleteBuffers(1, &vbo_vertices);
            glDeleteBuffers(1, &vbo_normals);
            glDeleteBuffers(1, &ibo_elements);
            glDeleteProgram(program_id);
            glDeleteVertexArrays(1, &vertex_array_id);
            // Close OpenGL window and terminate GLFW
            glfwTerminate();
            return 0;
        }
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                ROTATE = !ROTATE;
                if(ROTATE) // save last cursor position
                    glfwGetCursorPos(window, &XPOS, &YPOS);
            }
        }
        static void mouse_scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
        {
            CAMRADIUS -= 0.05 * yoffset * MODELRADIUS;
            RECOMPUTE_VIEWMAT = true;
        }
        void key_press_callback( float deltaTime )
        {            
            if (glfwGetKey( window, GLFW_KEY_PAGE_UP ) == GLFW_PRESS ) {
                CAMRADIUS -= 0.05 * MODELRADIUS;
                RECOMPUTE_VIEWMAT = true;
            }
                
            if (glfwGetKey( window, GLFW_KEY_PAGE_DOWN ) == GLFW_PRESS ) {
                CAMRADIUS += 0.05 * MODELRADIUS;
                RECOMPUTE_VIEWMAT = true;
            }
        }
        static void window_resize_callback( GLFWwindow* window, int width, int height )
        {
            VIEWPORT_W = width;
            VIEWPORT_H = height;
            RECOMPUTE_VIEWMAT = true;
        }
        static void option_toggle_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_W && action==GLFW_PRESS )
                SHOW_WIREFRAME = !SHOW_WIREFRAME;
        }
        ~SimpleViewer() {}
};


#endif