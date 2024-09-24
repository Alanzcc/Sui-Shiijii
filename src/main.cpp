#include <glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transformations.h"
#include "Shader.h"
#include "Camera.h"
#include "ObjRenderer.h"

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float playerPosZ = 0.0f; // Player's Z position
bool playerMovedDuringRed = false;
bool gameOver = false;
bool gameWon = false;
bool isRedPhase = false;
float gameTime = 0.0f;
const float redTime = 10.0f;
const float finishLineZ = -20.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.frag");
    Shader lightCubeShader("/home/seth/Progetti/Sui-Shiijii/shaders/light_cube.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/light_cube.frag");
    Shader backpackShader("/home/seth/Progetti/Sui-Shiijii/shaders/model_loading.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/model_loading.frag");
    Shader moaiShader("/home/seth/Progetti/Sui-Shiijii/shaders/model_loading.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/model_loading.frag");
    Shader floorShader("/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.frag");
    Shader wallShader("/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.vert",
        "/home/seth/Progetti/Sui-Shiijii/shaders/light_casters.frag");


    // load models
    ObjRenderer backpack(
        "/home/seth/Progetti/Sui-Shiijii/objects/backpack/backpack.obj",
        "/home/seth/Progetti/Sui-Shiijii/objects/backpack/diffuse.jpg",
        glm::vec3(0.5, 0.7, 0.3),
        30);
    backpackShader.use();
    backpackShader.setInt("material.diffuse", 0);
    ObjRenderer moai(
        "/home/seth/Progetti/Sui-Shiijii/objects/FabConvert.com_base mesh sculpt 2.obj",
        "/home/seth/Progetti/Sui-Shiijii/textures/Untitled.jpg",
        glm::vec3(0.5, 0.7, 0.3),
        30);
    moaiShader.use();
    moaiShader.setInt("material.diffuse", 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // Vertex data for a wall plane
    // Positions            // Normals        // Texture Coords
    float wall[] = {
// Combined vertex data for all four walls (positions, normals, texture coordinates)
    // Front wall (4 vertices)
    -1.0f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left
     1.0f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // Bottom-right
     1.0f, 2.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // Top-right
    -1.0f, 2.0f,  1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // Top-left

    // Left wall (4 vertices)
    -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
    -1.0f, 0.0f,  1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
    -1.0f, 2.0f,  1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // Top-right
    -1.0f, 2.0f, -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // Top-left

    // Right wall (4 vertices)
     1.0f, 0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
     1.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
     1.0f, 2.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // Top-right
     1.0f, 2.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // Top-left

    // Back wall (4 vertices)
    -1.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // Bottom-left
     1.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
     1.0f, 2.0f, -1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  // Top-right
    -1.0f, 2.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f   // Top-left
};

// Combined index data for all four walls (6 indices per wall, 2 triangles per wall)
unsigned int wallIndices[] = {
    // Front wall
    0, 1, 2, 0, 2, 3,

    // Left wall
    4, 5, 6, 4, 6, 7,

    // Right wall
    8, 9, 10, 8, 10, 11,

    // Back wall
    12, 13, 14, 12, 14, 15
};

    // Vertex data for a floor plane
    float tiles[] = {
        -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
         1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
         1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top-right
        -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top-left
    };
    // Indices for the plane (two triangles)
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };
    glm::vec3 wallPositions[] = {
        glm::vec3( 1.0f,  2.0f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };


    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( -1.0f,  -0.7f,  0.0f),
        glm::vec3( 1.0f,  -0.6f, -15.0f),
        glm::vec3(-1.0f, 0.3f, 0.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -1.4f, -3.5f),
        glm::vec3(-1.7f,  1.0f, -7.5f),
        glm::vec3( 2.3f, -1.0f, -4.5f),
        glm::vec3( 1.5f,  1.0f, -2.5f),
        glm::vec3( 1.5f,  1.2f, -1.5f),
        glm::vec3(-2.3f,  0.0f, -1.5f)
    };
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 1.0f,  2.0f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-3.0f,  2.0f, -7.0f),
        glm::vec3( 3.0f,  5.0f, -3.0f)
    };

    //----------------------------------------------------------------------------------------------------------
    // Containers and light cubes setup
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //-----------------------------------------------------------------------------------------------------------

    // Floor setup
    unsigned int floorVBO, floorVAO, floorEBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);

    // Bind the floor VAO
    glBindVertexArray(floorVAO);

    // Bind and set floor VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tiles), tiles, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attributes for position, normal, and texture coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);


    // Floor setup
    unsigned int wallVBO, wallVAO, wallEBO;
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &wallVBO);
    glGenBuffers(1, &wallEBO);

    // Bind the wall VAO
    glBindVertexArray(wallVAO);

    // Bind and set wall VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    // Set vertex attributes for position, normal, and texture coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int wallTex = ObjRenderer::loadTexture("/home/seth/Progetti/Sui-Shiijii/textures/seamless-wood-planks-3-354038619.jpg");
    unsigned int floorTex = ObjRenderer::loadTexture("/home/seth/Progetti/Sui-Shiijii/textures/tileable_concrete_tiles_texture-1500331845.jpg");
    unsigned int diffuseMap = ObjRenderer::loadTexture("/home/seth/Progetti/Sui-Shiijii/textures/container2.png");
    unsigned int specularMap = ObjRenderer::loadTexture("/home/seth/Progetti/Sui-Shiijii/textures/container2_specular.png");

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);





    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




    float redTimeDuration = 3.0f;
    bool isRed = false;
    glfwSetTime(4);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        gameTime += deltaTime;


        // input
        // -----
        processInput(window);

        // render
        // ------
        if (fmod(gameTime, redTime * 2.0f) < redTime) {
            isRedPhase = false;
            glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Blue sky
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            isRedPhase = true;
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red sky
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // Check game over state
        if (playerMovedDuringRed) {
            gameOver = true;
            std::cout << "You lost!" << std::endl;
            break;
        }

        if (playerPosZ <= finishLineZ) {
            gameWon = true;
            std::cout << "You won!" << std::endl;
            break;
        }
        // Render the floor
        floorShader.use();

        // Set transformation matrices for the floor
        glm::mat4 model = glm::mat4(1.0f);  // Identity matrix for the floor
        model = glm::scale(model, glm::vec3(11.0f, 1.0f, 10.0f)); // Scale the floor
        model = glm::translate(model, glm::vec3(0.0f, -2.0, -0.5f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        floorShader.setMat4("model", model);
        floorShader.setMat4("view", view);
        floorShader.setMat4("projection", projection);



                // be sure to activate shader when setting uniforms/drawing objects
        floorShader.use();
        floorShader.setVec3("light.position", camera.Position);
        floorShader.setVec3("light.direction", camera.Front);
        floorShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        floorShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        floorShader.setVec3("viewPos", camera.Position);
        // light properties
        floorShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.
        floorShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        floorShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("light.constant", 1.0f);
        floorShader.setFloat("light.linear", 0.09f);
        floorShader.setFloat("light.quadratic", 0.032f);

        // material properties
        floorShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        floorShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        floorShader.setVec3("dirLight.ambient",  0.1f, 0.24f, 0.14f);
        floorShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
        floorShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);
        // point light 1
        floorShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        floorShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        floorShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        floorShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("pointLights[0].constant", 1.0f);
        floorShader.setFloat("pointLights[0].linear", 0.09f);
        floorShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        floorShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        floorShader.setVec3("pointLights[1].ambient", 1.05f, 1.05f, 1.05f);
        floorShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        floorShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("pointLights[1].constant", 1.0f);
        floorShader.setFloat("pointLights[1].linear", 0.09f);
        floorShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        floorShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        floorShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        floorShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        floorShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("pointLights[2].constant", 1.0f);
        floorShader.setFloat("pointLights[2].linear", 0.09f);
        floorShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        floorShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        floorShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        floorShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        floorShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("pointLights[3].constant", 1.0f);
        floorShader.setFloat("pointLights[3].linear", 0.09f);
        floorShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        floorShader.setVec3("spotLight.position", camera.Position);
        floorShader.setVec3("spotLight.direction", camera.Front);
        floorShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        floorShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        floorShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        floorShader.setFloat("spotLight.constant", 1.0f);
        floorShader.setFloat("spotLight.linear", 0.09f);
        floorShader.setFloat("spotLight.quadratic", 0.032f);
        floorShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        floorShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // Bind floor texture if necessary
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTex);  // Assuming you want to apply a texture to the floor

        // Draw the floor
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);







        // Render the walls
        wallShader.use();

        // Set transformation matrices for the wall
        model = glm::mat4(1.0f);  // Identity matrix for the wall
        model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f)); // Scale the wall

        model = glm::translate(model, glm::vec3(0.0f,  -1.0, -0.5f));
        wallShader.setMat4("model", model);
        wallShader.setMat4("view", view);
        wallShader.setMat4("projection", projection);

        // Bind wall texture if necessary
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTex);  // Assuming you want to apply a texture to the wall

        // Draw the wall


                // be sure to activate shader when setting uniforms/drawing objects
        wallShader.use();
        wallShader.setVec3("light.position", camera.Position);
        wallShader.setVec3("light.direction", camera.Front);
        wallShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        wallShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        wallShader.setVec3("viewPos", camera.Position);
        // light properties
        wallShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.
        wallShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        wallShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("light.constant", 1.0f);
        wallShader.setFloat("light.linear", 0.09f);
        wallShader.setFloat("light.quadratic", 0.032f);

        // material properties
        wallShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        wallShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        wallShader.setVec3("dirLight.ambient",  0.1f, 0.24f, 0.14f);
        wallShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
        wallShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);
        // point light 1
        wallShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        wallShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        wallShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        wallShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("pointLights[0].constant", 1.0f);
        wallShader.setFloat("pointLights[0].linear", 0.09f);
        wallShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        wallShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        wallShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        wallShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        wallShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("pointLights[1].constant", 1.0f);
        wallShader.setFloat("pointLights[1].linear", 0.09f);
        wallShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        wallShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        wallShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        wallShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        wallShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("pointLights[2].constant", 1.0f);
        wallShader.setFloat("pointLights[2].linear", 0.09f);
        wallShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        wallShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        wallShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        wallShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        wallShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("pointLights[3].constant", 1.0f);
        wallShader.setFloat("pointLights[3].linear", 0.09f);
        wallShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        wallShader.setVec3("spotLight.position", camera.Position);
        wallShader.setVec3("spotLight.direction", camera.Front);
        wallShader.setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
        wallShader.setVec3("spotLight.diffuse", 2.0f, 2.0f, 2.0f);
        wallShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        wallShader.setFloat("spotLight.constant", 1.0f);
        wallShader.setFloat("spotLight.linear", 0.09f);
        wallShader.setFloat("spotLight.quadratic", 0.032f);
        wallShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        wallShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        glBindVertexArray(wallVAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));  // Front wall

        // Left wall (next 6 indices, starting from index 6)
        wallShader.setMat4("model", model);  // Use same or different transformation
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));  // Left wall

        // Right wall (next 6 indices, starting from index 12)
        wallShader.setMat4("model", model);  // Use same or different transformation
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));  // Right wall

        // Back wall (next 6 indices, starting from index 18)
        wallShader.setMat4("model", model);  // Use same or different transformation
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));  // Back wall

        // Unbind the VAO after rendering
        glBindVertexArray(0);












        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", camera.Position);
        lightingShader.setVec3("light.direction", camera.Front);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setVec3("viewPos", camera.Position);
        // light properties
        lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
        // each environment and lighting type requires some tweaking to get the best out of your environment.
        lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient",  0.1f, 0.24f, 0.14f);
        lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
        lightingShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        backpackShader.setMat4("projection", projection);
        backpackShader.setMat4("view", view);

        moaiShader.setMat4("projection", projection);
        moaiShader.setMat4("view", view);

        // world transformation
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);

        glm::mat4 backpackModel = glm::mat4(1.0f);
        backpackModel = glm::translate(backpackModel, glm::vec3(1.0f, 0.0f, -15.0f));
        backpackModel = glm::scale(backpackModel, glm::vec3(0.1f, 0.1f, 0.1f));
        backpackShader.setMat4("model", backpackModel);
        backpack.render(backpackShader);

        glm::mat4 moaiModel = glm::mat4(1.0f);
        moaiModel = applyRotationX(moaiModel, 90);
        if (isRedPhase)
            moaiModel = applyRotationY(moaiModel,  180);
        moaiModel = glm::scale(moaiModel, glm::vec3(0.5f, 0.4f, 0.5f));
        moaiModel = glm::translate(moaiModel, glm::vec3(0.0f, 10.0f, -2.0f));



        moaiShader.setMat4("model", moaiModel);
        moai.render(moaiShader);

        // render containers
        glBindVertexArray(cubeVAO);
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

         // also draw the lamp object(s)
         lightCubeShader.use();
         lightCubeShader.setMat4("projection", projection);
         lightCubeShader.setMat4("view", view);

         // we now draw as many light bulbs as we have point lights.
         glBindVertexArray(lightCubeVAO);
         for (unsigned int i = 0; i < 4; i++)
         {
             model = glm::mat4(1.0f);
             model = glm::translate(model, pointLightPositions[i]);
             model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
             lightCubeShader.setMat4("model", model);
             glDrawArrays(GL_TRIANGLES, 0, 36);
         }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (isRedPhase) {
            playerMovedDuringRed = true; // Player moved during the red phase
        }
        playerPosZ -= 0.05f; // Move forward in Z direction
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
