#version 330 core

in vec3 Position_worldspace;

flat in vec3 Normal_cameraspace_flat;
smooth in vec3 Normal_cameraspace_smooth;

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform vec4 LightPosition_worldspace;
uniform int FlatShading;
uniform int LightOn;
uniform vec3 ConstColor;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
    if( LightOn==0 ) {
        color = ConstColor;
        return;
    }
    vec3 Normal_cameraspace;
    if( FlatShading==1 )
        Normal_cameraspace = Normal_cameraspace_flat;
    else
        Normal_cameraspace = Normal_cameraspace_smooth;
    // Light emission properties
    // You probably want to put them as uniforms
    vec3 LightColor = vec3(1.0,1.0,1.0);
    float LightPower = 1.0f;
    float shininess  =50.0f;
    // Material properties
    vec3 MaterialDiffuseColor  = ConstColor;
    vec3 MaterialAmbientColor  = 0.66 * vec3(1.0,1.0,1.0) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = 0.33 * vec3(1.0,1.0,1.0);
    // Distance to the light
    float distance;
    if( LightPosition_worldspace.w != 0.0 )
        distance = length( LightPosition_worldspace.xyz - Position_worldspace );
    else
        distance = 1.0;
    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( LightDirection_cameraspace );
    // Cosine of the angle between the normal and the light direction, 
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp( dot( n,l ), 0,1 );
    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );
    color = 
        // Ambient : simulates indirect lighting
        MaterialAmbientColor +
        // Diffuse : "color" of the object
        MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
        // Specular : reflective highlight, like a mirror
        MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,shininess) / (distance*distance);
}
