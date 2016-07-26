#version 330 core

out vec4 frag_color;

in VS_OUT {
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  vec4 frag_pos_light_space;
} fs_in;

uniform sampler2D shadow_map;
uniform sampler2D ao_map;

uniform sampler2D diffuse_map;

uniform vec2 screen_res;

uniform vec3 light_pos;
uniform vec3 view_pos;

bool AO = true;

float ShadowCalculation(vec4 fragPosLightSpace)
{
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadow_map, projCoords.xy).r; 
  float currentDepth = projCoords.z;

  vec3 normal = normalize(fs_in.normal);
  vec3 lightDir = normalize(light_pos - fs_in.frag_pos);
  float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

  //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
    {
      float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
      shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
    }    
  }
  shadow /= 9.0;

  if(projCoords.z > 1.0)
    shadow = 0.0;

  return shadow;
}

vec3 gamma(vec3 v)
{
  return pow(v, vec3(2.2));
}

void main()
{           
  vec3 color = texture(diffuse_map, fs_in.tex_coords).rgb;
  vec3 normal = normalize(fs_in.normal);
  vec3 lightColor = vec3(0.3);


  vec3 lightDir = normalize(light_pos - fs_in.frag_pos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 viewDirUnNorm = view_pos - fs_in.frag_pos;
  vec3 viewDir = normalize(viewDirUnNorm);
  //SSDO trick
  /*float to_light_dist = length(viewDirUnNorm);
    vec3 to_light_norm = viewDirUnNorm / to_light_dist;
    float light_occlusion = 1.0 - clamp(dot(vec4(-viewDirUnNorm, 1.0), vec4(texture(ao_map, gl_FragCoord.xy / vec2(1280, 720)).r)), 0.0, 1.0);
   */
  vec3 ambient = (AO ? 0.5 * texture(ao_map, gl_FragCoord.xy / screen_res).r : 0.3) * color;
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);  
  spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = spec * lightColor;

  float shadow = ShadowCalculation(fs_in.frag_pos_light_space);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

  //Depth map
  /*vec3 projCoords = fs_in.frag_pos_light_space.xyz / fs_in.frag_pos_light_space.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadow_map, projCoords.xy).r; */
  frag_color = vec4(lighting, 1.0f);
}
