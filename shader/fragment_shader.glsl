#version 330 core

in vec4 out_pos;
in vec4 input_color;
out vec4 color;

uniform vec4 rect_info;
uniform vec4 bg_color;

float length(vec2 vector){
   return sqrt(vector.x * vector.x + vector.y * vector.y);
}

void main(){
   //color = vec4(input_color.r, input_color.g, input_color.b, 1.0);
   //color = u_Color;
   
   vec4 rect_normal = vec4((rect_info.xy + vec2(1.0)) * 0.5, rect_info.zw * 0.5);

   vec3 center = vec3(rect_normal.xy + 0.5 * rect_normal.zw,
                      0.25 * (rect_normal.z * step(rect_normal.z, rect_normal.w)
                      + rect_normal.w * step(rect_normal.w, rect_normal.z))
                     + 0.25
                      * abs(rect_normal.w * step(rect_normal.w, rect_normal.z) - rect_normal.z * step(rect_normal.z, rect_normal.w)));

   vec4 new_pos = vec4((out_pos.rg + vec2(1.0)) * 0.5, 0.0, 1.0);

   float dist = step(0.0, center.z - length(new_pos.xy - center.xy));

   color = vec4(input_color.rgb, 1.) * dist + bg_color * (1 - dist);
}
