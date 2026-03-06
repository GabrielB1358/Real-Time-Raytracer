# Real-Time Raytracer

A real-time ray-tracing application showing a basic scene with simple shapes and a visible light source.

# Overview
- Written in C++, Uses OpenGL for all rendering
  - Specifically uses OpenGL's Compute Shader functionality to run all ray tracing calculations on the GPU
 
- Showcases:
  - Reflections
  - Soft Shadows
  - Global illumination
 
- Includes a simple UI
  - Displays FPS
  - Buttons to swap scenes
  - Toggles for the different lighting effects
  - Sliders for increasing / decreasing the sample sizes on the relevant effects
  - Toggle for making the light source move passively (to better display the effects)
  - Colour space for changing the colour of the light source
    
- User has the choice of 2 scenes which they can switch between while the application is running
  - Scene 1 better shows off the reflections & soft shadows
  - Scene 2 better shows off the global illumination
