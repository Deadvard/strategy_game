#version 430

in vertex_data
{
	vec3 world_pos;
	vec2 tex_coord;
	vec3 world_normal;
}fs_in;

out vec4 fragment_color;

uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D roughness;
uniform sampler2D metallic;

const int num_lights = 1;

uniform vec3 dir_light_dir;
uniform vec3 dir_light_color;
uniform float dir_light_intensity;

uniform vec3 camPos;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normal, fs_in.tex_coord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.world_pos);
    vec3 Q2  = dFdy(fs_in.world_pos);
    vec2 st1 = dFdx(fs_in.tex_coord);
    vec2 st2 = dFdy(fs_in.tex_coord);

    vec3 N   = normalize(fs_in.world_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
void main()
{		
	vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - fs_in.world_pos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)

	vec3 albedo_texture = texture2D(albedo,fs_in.tex_coord).xyz;
	float metallic_texture = texture2D(metallic,fs_in.tex_coord).r;
	float roughness_texture = texture2D(roughness,fs_in.tex_coord).r;
	float ao = 1.0;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo_texture, metallic_texture);

    // reflectance equation
    vec3 Lo = vec3(0.0);

	{ //Dir light
		vec3 V = normalize(camPos.xyz - fs_in.world_pos);
		vec3 L = normalize(-dir_light_dir);
		vec3 H = normalize(V + L);     
		              
		vec3 F0 = vec3(0.04); 
		F0 = mix(F0, albedo_texture, metallic_texture);
		         
		vec3 radiance = dir_light_color * dir_light_intensity;        
		
		float NDF = DistributionGGX(N, H, roughness_texture);   
		float G   = GeometrySmith(N, V, L, roughness_texture);      
		vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);      
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic_texture;	  
		
		vec3 nominator    = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
		vec3 specular     = nominator / denominator;
		    
		float NdotL = max(dot(N, L), 0.0);                
		Lo += (kD * albedo_texture / PI + specular) * radiance * NdotL;
    }
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo_texture * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    fragment_color = vec4(color, 1.0);
}
