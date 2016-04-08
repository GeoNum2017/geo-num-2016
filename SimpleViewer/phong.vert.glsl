#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

out vec3 Position_worldspace;

flat out vec3 Normal_cameraspace_flat;
smooth out vec3 Normal_cameraspace_smooth;

out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

uniform vec4 LightPosition_worldspace;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
    
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = Projection * View * Model * vec4(vertexPosition_modelspace,1);

    // Position of the vertex, in worldspace : M * position
    Position_worldspace = (Model * vec4(vertexPosition_modelspace,1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( View * Model * vec4(vertexPosition_modelspace,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    if( LightPosition_worldspace.w == 0.0 )
        LightDirection_cameraspace = ( View * LightPosition_worldspace).xyz;
    else
        LightDirection_cameraspace = ( View * LightPosition_worldspace).xyz + EyeDirection_cameraspace;

    // Normal of the the vertex, in camera space
    vec3 Normal_cameraspace = ( View * Model * vec4(vertexNormal_modelspace,0)).xyz;
    
    Normal_cameraspace_flat = Normal_cameraspace;
    Normal_cameraspace_smooth = Normal_cameraspace;
}

