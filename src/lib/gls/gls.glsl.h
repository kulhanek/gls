/* Generated file, do not edit! */
#ifndef GLS_GLSL_STR
#define GLS_GLSL_STR \
"/*\n" \
" * This file is part of libgls, a library for stereoscopic OpenGL rendering.\n" \
" * It was adapted from video_output_render.fs.glsl from the Bino 3D video\n" \
" * player on 2012-10-02. The copyright holders (listed below) explicitly agreed\n" \
" * with relicensing under the MIT/X11 license.\n" \
" *\n" \
" * Copyright (C) 2010, 2011, 2012\n" \
" * Martin Lambers <marlam@marlam.de>\n" \
" * Frederic Devernay <Frederic.Devernay@inrialpes.fr>\n" \
" *\n" \
" * Permission is hereby granted, free of charge, to any person obtaining a\n" \
" * copy of this software and associated documentation files (the \"Software\"),\n" \
" * to deal in the Software without restriction, including without limitation\n" \
" * the rights to use, copy, modify, merge, publish, distribute, sublicense,\n" \
" * and/or sell copies of the Software, and to permit persons to whom the\n" \
" * Software is furnished to do so, subject to the following conditions:\n" \
" *\n" \
" * The above copyright notice and this permission notice shall be included\n" \
" * in all copies or substantial portions of the Software.\n" \
" *\n" \
" * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n" \
" * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n" \
" * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL\n" \
" * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN\n" \
" * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n" \
" * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n" \
" */\n" \
"#version 110\n" \
"// mode_onechannel\n" \
"// mode_red_cyan_monochrome\n" \
"// mode_red_cyan_half_color\n" \
"// mode_red_cyan_full_color\n" \
"// mode_red_cyan_dubois\n" \
"// mode_green_magenta_monochrome\n" \
"// mode_green_magenta_half_color\n" \
"// mode_green_magenta_full_color\n" \
"// mode_green_magenta_dubois\n" \
"// mode_amber_blue_monochrome\n" \
"// mode_amber_blue_half_color\n" \
"// mode_amber_blue_full_color\n" \
"// mode_amber_blue_dubois\n" \
"// mode_red_green_monochrome\n" \
"// mode_red_blue_monochrome\n" \
"// mode_even_odd_rows\n" \
"// mode_even_odd_columns\n" \
"// mode_checkerboard\n" \
"#define $mode\n" \
"// ghostbust_enabled\n" \
"// ghostbust_disabled\n" \
"#define $ghostbust\n" \
"uniform sampler2D rgb_l;\n" \
"uniform sampler2D rgb_r;\n" \
"uniform float parallax_adjust;\n" \
"#if defined(ghostbust_enabled) && (defined(mode_onechannel) || defined(mode_even_odd_rows) || defined(mode_even_odd_columns) || defined(mode_checkerboard))\n" \
"uniform vec3 crosstalk;\n" \
"#endif\n" \
"#if defined(mode_onechannel)\n" \
"uniform float channel;  // 0.0 for left, 1.0 for right\n" \
"#endif\n" \
"#if defined(mode_even_odd_rows) || defined(mode_even_odd_columns) || defined(mode_checkerboard)\n" \
"uniform sampler2D mask_tex;\n" \
"uniform float step_x;\n" \
"uniform float step_y;\n" \
"#endif\n" \
"#if defined(mode_red_cyan_monochrome) || defined(mode_red_cyan_half_color) || defined(mode_green_magenta_monochrome) || defined(mode_green_magenta_half_color) || defined(mode_amber_blue_monochrome) || defined(mode_amber_blue_half_color) || defined(mode_red_green_monochrome) || defined(mode_red_blue_monochrome)\n" \
"float rgb_to_lum(vec3 rgb)\n" \
"{\n" \
"    // Values taken from http://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/\n" \
"    // Section 6, \"Reading Back Luminance Pixels\"\n" \
"    return dot(rgb, vec3(0.299, 0.587, 0.114));\n" \
"}\n" \
"#endif\n" \
"#if defined(mode_onechannel) || defined(mode_even_odd_rows) || defined(mode_even_odd_columns) || defined(mode_checkerboard)\n" \
"#  if defined(ghostbust_enabled)\n" \
"vec3 ghostbust(vec3 original, vec3 other)\n" \
"{\n" \
"    return original + crosstalk - (other + original) * crosstalk;\n" \
"}\n" \
"#  else\n" \
"#    define ghostbust(original, other) original\n" \
"#  endif\n" \
"#endif\n" \
"vec3 tex_l(vec2 texcoord)\n" \
"{\n" \
"    return texture2D(rgb_l, texcoord + vec2(parallax_adjust, 0.0)).rgb;\n" \
"}\n" \
"vec3 tex_r(vec2 texcoord)\n" \
"{\n" \
"    return texture2D(rgb_r, texcoord - vec2(parallax_adjust, 0.0)).rgb;\n" \
"}\n" \
"void main()\n" \
"{\n" \
"    vec3 l, r;\n" \
"    vec3 result;\n" \
"#if defined(mode_onechannel)\n" \
"    l = tex_l(gl_TexCoord[0].xy);\n" \
"    r = tex_r(gl_TexCoord[1].xy);\n" \
"    result = ghostbust(mix(l, r, channel), mix(r, l, channel));\n" \
"#elif defined(mode_even_odd_rows) || defined(mode_even_odd_columns) || defined(mode_checkerboard)\n" \
"    /* This implementation of the masked modes works around many different problems and therefore may seem strange.\n" \
"     * Why not use stencil buffers?\n" \
"     *  - Because we want to filter, to account for masked out features\n" \
"     *  - Because stencil did not work with some drivers when switching fullscreen on/off\n" \
"     * Why not use polygon stipple?\n" \
"     *  - Because we want to filter, to account for masked out features\n" \
"     *  - Because polygon stippling may not be hardware accelerated and is currently broken with many free drivers\n" \
"     * Why use a mask texture? Why not use the mod() function to check for even/odd pixels?\n" \
"     *  - Because mod() is broken with many drivers, and I found no reliable way to work around it. Some\n" \
"     *    drivers seem to use extremely low precision arithmetic in the shaders; too low for reliable pixel\n" \
"     *    position computations.\n" \
"     */\n" \
"    float m = texture2D(mask_tex, gl_TexCoord[2].xy).x;\n" \
"# if defined(mode_even_odd_rows)\n" \
"    vec3 rgb0_l = tex_l(gl_TexCoord[0].xy - vec2(0.0, step_y));\n" \
"    vec3 rgb1_l = tex_l(gl_TexCoord[0].xy);\n" \
"    vec3 rgb2_l = tex_l(gl_TexCoord[0].xy + vec2(0.0, step_y));\n" \
"    vec3 rgbc_l = (rgb0_l + 2.0 * rgb1_l + rgb2_l) / 4.0;\n" \
"    vec3 rgb0_r = tex_r(gl_TexCoord[1].xy - vec2(0.0, step_y));\n" \
"    vec3 rgb1_r = tex_r(gl_TexCoord[1].xy);\n" \
"    vec3 rgb2_r = tex_r(gl_TexCoord[1].xy + vec2(0.0, step_y));\n" \
"    vec3 rgbc_r = (rgb0_r + 2.0 * rgb1_r + rgb2_r) / 4.0;\n" \
"# elif defined(mode_even_odd_columns)\n" \
"    vec3 rgb0_l = tex_l(gl_TexCoord[0].xy - vec2(step_x, 0.0));\n" \
"    vec3 rgb1_l = tex_l(gl_TexCoord[0].xy);\n" \
"    vec3 rgb2_l = tex_l(gl_TexCoord[0].xy + vec2(step_x, 0.0));\n" \
"    vec3 rgbc_l = (rgb0_l + 2.0 * rgb1_l + rgb2_l) / 4.0;\n" \
"    vec3 rgb0_r = tex_r(gl_TexCoord[1].xy - vec2(step_x, 0.0));\n" \
"    vec3 rgb1_r = tex_r(gl_TexCoord[1].xy);\n" \
"    vec3 rgb2_r = tex_r(gl_TexCoord[1].xy + vec2(step_x, 0.0));\n" \
"    vec3 rgbc_r = (rgb0_r + 2.0 * rgb1_r + rgb2_r) / 4.0;\n" \
"# elif defined(mode_checkerboard)\n" \
"    vec3 rgb0_l = tex_l(gl_TexCoord[0].xy - vec2(0.0, step_y));\n" \
"    vec3 rgb1_l = tex_l(gl_TexCoord[0].xy - vec2(step_x, 0.0));\n" \
"    vec3 rgb2_l = tex_l(gl_TexCoord[0].xy);\n" \
"    vec3 rgb3_l = tex_l(gl_TexCoord[0].xy + vec2(step_x, 0.0));\n" \
"    vec3 rgb4_l = tex_l(gl_TexCoord[0].xy + vec2(0.0, step_y));\n" \
"    vec3 rgbc_l = (rgb0_l + rgb1_l + 4.0 * rgb2_l + rgb3_l + rgb4_l) / 8.0;\n" \
"    vec3 rgb0_r = tex_r(gl_TexCoord[1].xy - vec2(0.0, step_y));\n" \
"    vec3 rgb1_r = tex_r(gl_TexCoord[1].xy - vec2(step_x, 0.0));\n" \
"    vec3 rgb2_r = tex_r(gl_TexCoord[1].xy);\n" \
"    vec3 rgb3_r = tex_r(gl_TexCoord[1].xy + vec2(step_x, 0.0));\n" \
"    vec3 rgb4_r = tex_r(gl_TexCoord[1].xy + vec2(0.0, step_y));\n" \
"    vec3 rgbc_r = (rgb0_r + rgb1_r + 4.0 * rgb2_r + rgb3_r + rgb4_r) / 8.0;\n" \
"# endif\n" \
"    result = ghostbust(mix(rgbc_r, rgbc_l, m), mix(rgbc_l, rgbc_r, m));\n" \
"#elif defined(mode_red_cyan_dubois) || defined(mode_green_magenta_dubois) || defined(mode_amber_blue_dubois)\n" \
"    // The Dubois anaglyph method is generally the highest quality anaglyph method.\n" \
"    // Authors page: http://www.site.uottawa.ca/~edubois/anaglyph/\n" \
"    // This method depends on the characteristics of the display device and the anaglyph glasses.\n" \
"    // According to the author, the matrices below are intended to be applied to linear RGB values,\n" \
"    // and are designed for CRT displays.\n" \
"    l = tex_l(gl_TexCoord[0].xy);\n" \
"    r = tex_r(gl_TexCoord[1].xy);\n" \
"# if defined(mode_red_cyan_dubois)\n" \
"    // Source of this matrix: http://www.site.uottawa.ca/~edubois/anaglyph/LeastSquaresHowToPhotoshop.pdf\n" \
"    mat3 m0 = mat3(\n" \
"             0.437, -0.062, -0.048,\n" \
"             0.449, -0.062, -0.050,\n" \
"             0.164, -0.024, -0.017);\n" \
"    mat3 m1 = mat3(\n" \
"            -0.011,  0.377, -0.026,\n" \
"            -0.032,  0.761, -0.093,\n" \
"            -0.007,  0.009,  1.234);\n" \
"# elif defined(mode_green_magenta_dubois)\n" \
"    // Source of this matrix: http://www.flickr.com/photos/e_dubois/5132528166/\n" \
"    mat3 m0 = mat3(\n" \
"            -0.062,  0.284, -0.015,\n" \
"            -0.158,  0.668, -0.027,\n" \
"            -0.039,  0.143,  0.021);\n" \
"    mat3 m1 = mat3(\n" \
"             0.529, -0.016,  0.009,\n" \
"             0.705, -0.015,  0.075,\n" \
"             0.024, -0.065,  0.937);\n" \
"# elif defined(mode_amber_blue_dubois)\n" \
"    // Source of this matrix: http://www.flickr.com/photos/e_dubois/5230654930/\n" \
"    mat3 m0 = mat3(\n" \
"             1.062, -0.026, -0.038,\n" \
"            -0.205,  0.908, -0.173,\n" \
"             0.299,  0.068,  0.022);\n" \
"    mat3 m1 = mat3(\n" \
"            -0.016,  0.006,  0.094,\n" \
"            -0.123,  0.062,  0.185,\n" \
"            -0.017, -0.017,  0.911);\n" \
"# endif\n" \
"    result = m0 * l + m1 * r;\n" \
"#else // lower quality anaglyph methods\n" \
"    l = tex_l(gl_TexCoord[0].xy);\n" \
"    r = tex_r(gl_TexCoord[1].xy);\n" \
"# if defined(mode_red_cyan_monochrome)\n" \
"    result = vec3(rgb_to_lum(l), rgb_to_lum(r), rgb_to_lum(r));\n" \
"# elif defined(mode_red_cyan_half_color)\n" \
"    result = vec3(rgb_to_lum(l), r.g, r.b);\n" \
"# elif defined(mode_red_cyan_full_color)\n" \
"    result = vec3(l.r, r.g, r.b);\n" \
"# elif defined(mode_green_magenta_monochrome)\n" \
"    result = vec3(rgb_to_lum(r), rgb_to_lum(l), rgb_to_lum(r));\n" \
"# elif defined(mode_green_magenta_half_color)\n" \
"    result = vec3(r.r, rgb_to_lum(l), r.b);\n" \
"# elif defined(mode_green_magenta_full_color)\n" \
"    result = vec3(r.r, l.g, r.b);\n" \
"# elif defined(mode_amber_blue_monochrome)\n" \
"    result = vec3(rgb_to_lum(l), rgb_to_lum(l), rgb_to_lum(r));\n" \
"# elif defined(mode_amber_blue_half_color)\n" \
"    result = vec3(rgb_to_lum(l), rgb_to_lum(l), r.b);\n" \
"# elif defined(mode_amber_blue_full_color)\n" \
"    result = vec3(l.r, l.g, r.b);\n" \
"# elif defined(mode_red_green_monochrome)\n" \
"    result = vec3(rgb_to_lum(l), rgb_to_lum(r), 0.0);\n" \
"# elif defined(mode_red_blue_monochrome)\n" \
"    result = vec3(rgb_to_lum(l), 0.0, rgb_to_lum(r));\n" \
"# endif\n" \
"#endif\n" \
"    gl_FragColor = vec4(result, 1.0);\n" \
"}\n" \
""
#endif
