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
#include "../libigl/include/igl/per_vertex_normals.h"
bool ROTATE = false, RECOMPUTE_VIEWMAT = false;
double XPOS, YPOS;
double CAMRADIUS = 5.0;
glm::vec3 CAMLOOKAT = glm::vec3(0.0,0.0,0.0);
glm::vec4 LIGHTPOS = glm::vec4(10.0f,-10.0f,10.0f,0.0f); // 0.0 at the end means directional light
double MODELRADIUS = 1;
unsigned int VIEWPORT_W = 1280;
unsigned int VIEWPORT_H = 800;
bool SHOW_WIREFRAME = true;
bool SHOW_FACES = true;
bool FLAT_SHADING = false;
bool LIGHT_ON = true;
bool QUADS = false;
class SimpleViewer
{        
    public: 
        GLFWwindow* window;
        // vertex array id
        GLuint vao;
        // buffers
        GLuint vbo_vertices;
        GLuint vbo_normals;
        GLuint ibo_elements;
        // shader program id
        GLuint program_id;
        // uniform shader variables
        GLuint model_id, view_id, projection_id, color_id, lightpos_id, lighton_id, flatshade_id;
        // model, view, projection matrices
        glm::mat4 Model, View, Projection;
        glm::vec3 LightPosition;
        // mesh topology = faces
        MatiX3 F;
        // mesh geometry
        MatdX3 V, N;
        glm::vec3 FACE_COLOR;
        glm::vec3 EDGE_COLOR;
        glm::vec3 CONTROLNET_COLOR;
        double az, el; // spherical angles for camera
        void set_facecolor(int r, int g, int b){FACE_COLOR=glm::vec3( (float)r/255.0,(float)g/255.0,(float)b/255.0);}
        void set_edgecolor(int r, int g, int b){EDGE_COLOR=glm::vec3( (float)r/255.0,(float)g/255.0,(float)b/255.0);}
        void set_cnetcolor(int r, int g, int b){CONTROLNET_COLOR=glm::vec3( (float)r/255.0,(float)g/255.0,(float)b/255.0);}
        
        void clear_data()
        {
            V.setZero(0,3);
            N.setZero(0,3);
            F.setZero(0,3);
        }
        void set_mesh(
            const MatdX3& newV,
            const MatiX3& newF )
        {
            clear_data();
            QUADS = false;
            V = newV;
            F = newF;
        }
        void generate_test_cube() 
        {
            clear_data();
            QUADS = true;
            //V.clear();
            V.resize(8,3);
            V <<
            // front
            -1.0, -1.0, +1.0,
            +1.0, -1.0, +1.0,
            +1.0, +1.0, +1.0,
            -1.0, +1.0, +1.0,
            // back
            -1.0, -1.0, -1.0,
            +1.0, -1.0, -1.0,
            +1.0, +1.0, -1.0,
            -1.0, +1.0, -1.0;
            
            //N.clear();
            N.resize(8,3);
            N <<
            // front
            -1.0, -1.0, +1.0,
            +1.0, -1.0, +1.0,
            +1.0, +1.0, +1.0,
            -1.0, +1.0, +1.0,
            // back
            -1.0, -1.0, -1.0,
            +1.0, -1.0, -1.0,
            +1.0, +1.0, -1.0,
            -1.0, +1.0, -1.0;
            
            //F.clear();
            F.resize(12,3);
            F <<
            // front
            0, 1, 2,
            2, 3, 0,
            // top
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // bottom     
            4, 0, 3,
            3, 7, 4,
            // left
            4, 5, 1,
            1, 0, 4,
            // right
            3, 2, 6,
            6, 7, 3;
        }
        void add_patch(
            const MatdXX &X,
            const MatdXX &Y,
            const MatdXX &Z,
            bool wireframe = false,
            bool closedx = false,
            bool closedy = false )
        {
            QUADS = true;
            unsigned int 
                vx = X.cols(),
                vy = X.rows(),
                fx = vx,
                fy = vy;
                
            if( !closedx ) fx--;
            if( !closedy ) fy--;
            
            // check if matrices have the correct size
            assert( vy==Y.rows() && vy==Z.rows() &&  vx==Y.cols() && vx==Z.cols() );
            
            // V : generate geometry
            unsigned int numV = vy * vx;
            MatdX3 newV;
            newV.resize(numV,3);
            newV.fill(0);
            MatdXX co;
            co = X; co.resize(numV,1); newV.col(0) = co;                        
            co = Y; co.resize(numV,1); newV.col(1) = co;
            co = Z; co.resize(numV,1); newV.col(2) = co;
            
            // F : generate topology
            unsigned int numF = 2 * fy * fx;
            MatiX3 newF;
            newF.resize(numF,3);
            unsigned int fcount = 0;
            for( unsigned y=0; y < fy; y++ ) {
                for( unsigned x=0; x < fx; x++ ) {
                    short A = ((y+0)%vy)*vy + x;
                    short B = ((y+0)%vy)*vy + (x+1)%vx;
                    short C = ((y+1)%vy)*vy + x;
                    short D = ((y+1)%vy)*vy + (x+1)%vx;
                    newF.row(fcount++) = Veci3(A,B,D);
                    newF.row(fcount++) = Veci3(D,C,A);
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
        void add_wireframe(
            const MatdXX &X,
            const MatdXX &Y,
            const MatdXX &Z)
        {
            add_patch(X,Y,Z,true);
        }
        void add_surface(
            const MatdXX &X,
            const MatdXX &Y,
            const MatdXX &Z,
            bool closedU=false,
            bool closedV=false)
        {
            add_patch(X,Y,Z,false,closedU,closedV);
        }
        void fill_buffers( void )
        {            
            if(V.isZero(0) || F.isZero(0)) {std::cout << "WARNING: vertices or faces are empty." << std::endl;return;}  
            
            std::cout << "setting positions\n";
            glGenBuffers(1, &vbo_vertices);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
            glBufferData(GL_ARRAY_BUFFER, sizeof(V(0,0))*V.size(), &V(0,0), GL_STATIC_DRAW); 
            
            std::cout << "computing normals\n";
            igl::per_vertex_normals(V,F,N);
            std::cout << "setting normals\n";
            glGenBuffers(1, &vbo_normals);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
            glBufferData(GL_ARRAY_BUFFER, sizeof(N(0,0))*N.size(), &N(0,0), GL_STATIC_DRAW);
            
            std::cout << "setting faces\n";
            glGenBuffers(1, &ibo_elements);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(F(0,0))*F.size(), &F(0,0), GL_STATIC_DRAW);
            
            double AABBx = V.col(0).maxCoeff() - V.col(0).minCoeff();
            double AABBy = V.col(1).maxCoeff() - V.col(1).minCoeff();
            double AABBz = V.col(2).maxCoeff() - V.col(2).minCoeff();
            MODELRADIUS = std::max(AABBx, std::max(AABBy,AABBz) );
            CAMRADIUS   = 1.5 * MODELRADIUS; 
            RECOMPUTE_VIEWMAT = true;
            
            std::cout << std::endl
                << "******************************" << std::endl
                << "****  simple viewer help  ****" << std::endl
                << "******************************" << std::endl
                << "    face toggle  : [F]" << std::endl
                << "    edge toggle  : [E]" << std::endl
                << "    light on/off : [L]" << std::endl
                << "    smooth/flat  : [S]" << std::endl
                << "    zoom         : (mouse scroll) or [pageup]/[pagedown]" << std::endl
                << "    rotate       : (mouse click & drag)" << std::endl
                << std::endl;
        }
        
        void compile_shaders( void ) {
            // Logging
            GLint Result = GL_FALSE;
            int InfoLogLength;
            
            std::ifstream ifstr1("../../SimpleViewer/phong.vert.glsl");
            std::stringstream buffer1;
            buffer1 << ifstr1.rdbuf();
            std::string mesh_vertex_shader_string = buffer1.str();
            
            std::ifstream ifstr2("../../SimpleViewer/phong.frag.glsl");
            std::stringstream buffer2;
            buffer2 << ifstr2.rdbuf();
            std::string mesh_fragment_shader_string = buffer2.str();
            
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
            set_cnetcolor(255,0,0); // gray
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
            window = glfwCreateWindow( VIEWPORT_W, VIEWPORT_H, "GeoNumTP", NULL, NULL);
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
            glClearDepth(1.0f);
            // Accept fragment if it closer to the camera than the former one
            glDepthFunc(GL_LESS);
            // polygon offset
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f,1.0f);
            // Vertex Array
            glGenVertexArrays(1,&vao);
            glBindVertexArray(vao);
            // compile shaders
            compile_shaders();
            // Get a handle for MVP matrices(uniform)
            model_id = glGetUniformLocation(program_id, "Model");
            view_id = glGetUniformLocation(program_id, "View");
            projection_id = glGetUniformLocation(program_id, "Projection");
            color_id = glGetUniformLocation(program_id, "ConstColor");
            lightpos_id = glGetUniformLocation(program_id, "LightPosition_worldspace");
            lighton_id = glGetUniformLocation(program_id, "LightOn");
            flatshade_id = glGetUniformLocation(program_id, "FlatShading");
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
            Projection = glm::perspective(45.0f, (GLfloat) VIEWPORT_W / (GLfloat) VIEWPORT_H, 0.001f, 10000.0f);
            glViewport(0, 0,VIEWPORT_W, VIEWPORT_H);
            // Model matrix = identity
            Model = glm::mat4(1.0f);
        }
        int show() {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            fill_buffers();
            int linecount = 0;
            
            /** SETTING EDGES **/
            MatiX2 E;
            if( QUADS ){
                E.resize(2*F.rows(),2);
                E << 
                    F.col(0),F.col(1),
                    F.col(1),F.col(2);
            }else{
                E.resize(3*F.rows(),2);
                E << 
                    F.col(0),F.col(1),
                    F.col(1),F.col(2),
                    F.col(2),F.col(0);
            }
            
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
                
                // Light position
                glUniform4f( lightpos_id, LIGHTPOS.x, LIGHTPOS.y, LIGHTPOS.z, LIGHTPOS.w );
                glUniform1i( lighton_id, (int) LIGHT_ON );
                glUniform1i( flatshade_id, (int) FLAT_SHADING );
                
                // 0 : Vertex positions
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
                glVertexAttribPointer(0,3,GL_DOUBLE,GL_FALSE,0,(void*)0);
                
                // 1 : Vertex normals
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
                glVertexAttribPointer(1,3,GL_DOUBLE,GL_FALSE,0,(void*)0);
                
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
                if(SHOW_FACES) {
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(F(0,0))*F.size(), &F(0,0), GL_STATIC_DRAW);
                    glUniform3f( color_id, FACE_COLOR.r, FACE_COLOR.g, FACE_COLOR.b );
                    glDrawElements(GL_TRIANGLES, F.rows()*F.cols(), GL_UNSIGNED_SHORT, 0);
                }
                if(SHOW_WIREFRAME) {
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(E(0,0))*E.size(), &E(0,0), GL_STATIC_DRAW);
                    glUniform3f( color_id, EDGE_COLOR.r, EDGE_COLOR.g, EDGE_COLOR.b );
                    glDrawElements(GL_LINES, E.rows()*E.cols(), GL_UNSIGNED_SHORT, 0);
                }
                
                // Disable attribute arrays
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
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
            glDeleteVertexArrays(1, &vao);
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
            else if (key == GLFW_KEY_F && action==GLFW_PRESS )
                SHOW_FACES = !SHOW_FACES;
            else if (key == GLFW_KEY_S && action==GLFW_PRESS )
                FLAT_SHADING = !FLAT_SHADING;
            else if (key == GLFW_KEY_L && action==GLFW_PRESS )
                LIGHT_ON = !LIGHT_ON;
        }
        ~SimpleViewer() {}
};


#endif
