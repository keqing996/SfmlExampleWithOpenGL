
#include <glad/gl.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include "ModelData.hpp"
#include "Texture.h"
#include "Shader.h"
#include "VertexData.h"
#include "Material.h"
#include "Camera.h"
#include "Object.h"

int main()
{
    /* Init window */
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    const float W_H_ratio = static_cast<float>(WIDTH) / HEIGHT;
    const float FOV = 50.0f;
    
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML OpenGL", sf::Style::Default, contextSettings);
    window.setActive();

    /* Glad load all OpenGL functions */
    ::gladLoaderLoadGL();

    /* OpenGL setting */
    ::glEnable(GL_DEPTH_TEST);
    ::glViewport(0, 0, WIDTH, HEIGHT);
    ::glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    /* Prepare vertex */
    VertexData planeVertexData(sizeof(float) * gPlaneVertices.size(), gPlaneVertices.data());
    VertexData cubeVertexData(sizeof(float) * gCubeVertices.size(), gCubeVertices.data());

    /* Load texture */
    Texture cubeTexture("./Resource/box.jpg", 0);
    Texture planeTexture("./Resource/floor.jpg", 1);

    /* Shader */
    Shader generalShader("./Glsl/general.vert", "Glsl/general.frag");
    Shader lightShader("./Glsl/light.vert", "Glsl/light.frag");

    /* Material */
    Material planeMaterial{ 0.9f, 0.1f };
    Material cubeMaterial{ 0.8f, 0.5f };

    /* Camera */
    Camera camera(glm::vec3(-5.0f, 4.0f, -5.0f), glm::radians(-30.0f), glm::radians(45.0f), glm::vec3(0, 1.0f, 0));

    /* Light shading objects */
    std::vector<Object> lightShadingOpaqueObjects;
    std::array<glm::vec3, 5> gCubePositions =
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(3.0f,  0.0f,  2.0f),
        glm::vec3(2.5f,  0.0f,  -3.7f),
        glm::vec3(-2.5f,  0.0f,  2.4f),
        glm::vec3(-3.0f,  0.0f,  -1.5f),
    };
    
    // Add plane
    Object plane(&planeVertexData);
    plane.SetPosition(0.0f, -0.5f, 0.0f);
    plane.SetRenderPassPara<RenderPass::ForwardPass>(&generalShader, &planeTexture, &planeMaterial);
    lightShadingOpaqueObjects.push_back(plane);

    // Add cubes
    for (auto i = 0; i < gCubePositions.size(); i++)
    {
        Object cubeObj(&cubeVertexData);
        cubeObj.SetPosition(gCubePositions[i]);
        cubeObj.SetRenderPassPara<RenderPass::ForwardPass>(&generalShader, &cubeTexture, &cubeMaterial);
        lightShadingOpaqueObjects.push_back(cubeObj);
    }

    /* Unlit shading objects */
    glm::vec3 gSingleLightPosition = glm::vec3(-0.3f, 0.2f, -2.5f);
    std::vector<Object> unlitShadingOpaqueObjects;
    Object cubeObj(&cubeVertexData);
    cubeObj.SetPosition(gSingleLightPosition);
    cubeObj.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    cubeObj.SetRenderPassPara<RenderPass::ForwardPass>(&lightShader);
    unlitShadingOpaqueObjects.push_back(cubeObj);
    
    while (window.isOpen())
    {
        /* Window event loop */
        sf::Event event;
        bool shouldBreakLoop = false;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                shouldBreakLoop = true;
            }
        }

        if (shouldBreakLoop)
            break;

        /* Clear the screen */
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Prepare VP */
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(FOV), W_H_ratio, 0.1f, 100.0f);

        /* Draw light shading objects */
        for (auto& renderObject : lightShadingOpaqueObjects)
        {
            auto& renderPara = renderObject.GetRenderPassPara<RenderPass::ForwardPass>();
            if (renderPara.pShader == nullptr)
                continue;

            // 1. Bind vertex attribute
            ::glBindVertexArray(renderObject.GetVertexData()->vao);

            // 2. Bind vertex array
            ::glBindBuffer(GL_ARRAY_BUFFER, renderObject.GetVertexData()->vbo);

            // 3. Bind shader program
            ::glUseProgram(renderPara.pShader->GetHandle());

            // 4. Upload uniforms
            renderPara.pShader->SetUniformMat4("model", renderObject.GetModelMatrix());
            renderPara.pShader->SetUniformMat4("view", view);
            renderPara.pShader->SetUniformMat4("projection", projection);
            renderPara.pShader->SetUniformInt("tex", renderPara.pMainTexture->GetSlot());
            
            renderPara.pShader->SetUniformVec3("light.color", glm::vec3{ 1.0f, 1.0f, 1.0f });
            renderPara.pShader->SetUniformVec3("light.position", gSingleLightPosition);
            renderPara.pShader->SetUniformFloat("material.roughness", renderPara.pMaterial->roughness);
            renderPara.pShader->SetUniformFloat("material.matellic", renderPara.pMaterial->metallic);

            // 5. Draw call
            ::glDrawArrays(GL_TRIANGLES, 0, renderObject.GetVertexData()->triangleNum);
        }

        /* Draw unlit shading objects */
        for (auto& renderObject : unlitShadingOpaqueObjects)
        {
            auto& renderPara = renderObject.GetRenderPassPara<RenderPass::ForwardPass>();
            if (renderPara.pShader == nullptr)
                continue;

            // 1. Bind vertex attribute
            ::glBindVertexArray(renderObject.GetVertexData()->vao);

            // 2. Bind vertex array
            ::glBindBuffer(GL_ARRAY_BUFFER, renderObject.GetVertexData()->vbo);

            // 3. Bind shader program
            ::glUseProgram(renderPara.pShader->GetHandle());

            // 4. Upload uniforms
            renderPara.pShader->SetUniformMat4("model", renderObject.GetModelMatrix());
            renderPara.pShader->SetUniformMat4("view", view);
            renderPara.pShader->SetUniformMat4("projection", projection);

            // 5. Draw call
            ::glDrawArrays(GL_TRIANGLES, 0, renderObject.GetVertexData()->triangleNum);
        }
        
        /* Swap back buffer */
        window.display();
    }
}