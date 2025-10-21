#include <iostream>
#include "engine.h"

// engine name is gearbit

int main(void)
{
    Window window = Window_Init(1280, 720, 60.0f, "Engine");
    if (Window_Generate(window) == -1)
    {
        Window_Delete();
        return -1;
    }

    // Create shader
    Shader shader;
    Shader_Init(shader, "shaders/vertex.glsl", "shaders/fragment.glsl");

    // Enable depth testing
    Window_EnableDepthTesting();

    // Create a triangle
    Mesh triangle;
    Mesh_SetTriangle(triangle);
    Mesh_Generate(triangle);

    // Create a rectangle
    Mesh rectangle;
    Mesh_SetRectangle(rectangle);
    Mesh_Generate(rectangle);

    // Create a circle
    Mesh circle;
    Mesh_SetCircle(circle, 1.0f, 48);
    Mesh_Generate(circle);

    // Create a cube
    Mesh cube;
    Mesh_SetCube(cube);
    Mesh_Generate(cube);

    // Create a sphere
    Mesh sphere;
    Mesh_SetSphere(sphere, 1.0f, 24, 48);
    Mesh_Generate(sphere);

    // Create the camera
    Camera camera;
    Camera_Init(camera, 5.0f, 90.0f, {0.0f, 0.0f, 0.0f});

    // Render loop
    while (Window_ShouldClose(window))
    {
        Time_Update();

        // Update the camera
        Camera_Update(window, camera, Time_Delta());

        Window_Clear(Colour::White);

        // Drawing the rectangle
        Transform_PushMatrix();

            // Perform transformations here
            Transform_Translate({-5.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(45.0f)*Time_Total(), {0,1,0});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform4f(shader, "uColor", Colour::SteelBlue);
            Mesh_Draw(rectangle);
            Shader_SetUniform4f(shader, "uColor", Colour::DarkGray);
            Mesh_DrawWireFrame(rectangle);
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the triangle
        Transform_PushMatrix();

            Transform_Translate({5.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(30.0f)*Time_Total(), {1,1,0});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform4f(shader, "uColor", Colour::DeepPink);
            Mesh_Draw(triangle);
            Shader_SetUniform4f(shader, "uColor", Colour::DarkGray);
            Mesh_DrawWireFrame(triangle);

            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the circle
        Transform_PushMatrix();

            Transform_Translate({2.0f,0.0f,7.0f});
            Transform_Rotate(Math_DegToRad(50.0f)*Time_Total(), {0,1,1});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform4f(shader, "uColor", Colour::Crimson);
            Mesh_Draw(circle);
            Shader_SetUniform4f(shader, "uColor", Colour::DarkGray);
            Mesh_DrawWireFrame(circle);

            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the cube
        Transform_PushMatrix();

            Transform_Translate({0.0f,0.0f,-10.0f});
            Transform_Rotate(Math_DegToRad(25.0f)*Time_Total(), {1,1,1});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform4f(shader, "uColor", Colour::Turquoise);
            Mesh_Draw(cube);
            Shader_SetUniform4f(shader, "uColor", Colour::Black);
            Mesh_DrawWireFrame(cube);

            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the sphere
        Transform_PushMatrix();

            Transform_Translate({0.0f,0.0f,10.0f});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform4f(shader, "uColor", Colour::Gold);
            Mesh_Draw(sphere);
            Shader_SetUniform4f(shader, "uColor", Colour::Bronze);
            Mesh_DrawWireFrame(sphere);

            Shader_Disable();

        Transform_PopMatrix();


        Window_PollEvents();
        Window_SwapBuffers(window);
    }

    // Here we delete any meshes, shaders, and the window
    Shader_Delete(shader);

    Mesh_Delete(triangle);
    Mesh_Delete(rectangle);
    Mesh_Delete(cube);
    Mesh_Delete(sphere);
    Mesh_Delete(circle);

    Window_Delete();

    return 0;
}