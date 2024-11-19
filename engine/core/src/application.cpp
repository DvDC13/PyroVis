#include "application.h"

namespace Pyro
{

    Application::Application()
    {
        Logger logger;
        logger.log(Logger::LogLevel::INFO, "Application started");

        descriptorPool_ = DescriptorPool::Builder(device_)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT * 2)
            .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
            .build();
        loadGameObjects();
    }

    Application::~Application() {}

    void Application::run()
    {
        std::vector<std::unique_ptr<Buffer>> uniformBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers[i] = std::make_unique<Buffer>(device_, sizeof(Ubo), 1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uniformBuffers[i]->map();
        }

        auto descriptorSetLayout = DescriptorSetLayout::Builder(device_)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        std::vector<VkDescriptorSet> descriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            auto bufferInfo = uniformBuffers[i]->descriptorInfo();
            DescriptorWriter(*descriptorSetLayout, *descriptorPool_)
            .writeBuffer(0, &bufferInfo)
            .build(descriptorSets[i]);
        }

        RendererSystem rendererSystem(device_, renderer_.getSwapChainRenderPass(), descriptorSetLayout->getDescriptorSetLayout());

        Camera camera{};
        camera.lookAtDirection(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 1.0f));

        auto viewerObject = GameObject::createObject();
        Keyboard keyboard{};

        // IMGUI
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(window_.getWindow(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device_.instance();
        init_info.PhysicalDevice = device_.physicalDevice();
        init_info.Device = device_.device();
        init_info.Queue = device_.graphicsQueue();
        init_info.RenderPass = renderer_.getSwapChainRenderPass();
        init_info.DescriptorPool = descriptorPool_->getDescriptorPool();
        init_info.ImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.MinImageCount = SwapChain::MAX_FRAMES_IN_FLIGHT;
        ImGui_ImplVulkan_Init(&init_info);

        VkCommandBuffer commandBuffer = device_.beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture();
        device_.endSingleTimeCommands(commandBuffer);
        vkDeviceWaitIdle(device_.device());
        ImGui_ImplVulkan_DestroyFontsTexture();

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window_.isClosed())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            if (frameTime > 1.0f / 30.0f) {
                frameTime = 1.0f / 30.0f;
            }

            keyboard.moveInPlaneXZ(window_.getWindow(), frameTime, viewerObject);

            camera.lookAtInYXZ(viewerObject.transform_.translation, viewerObject.transform_.rotation);

            float aspect = renderer_.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(50.0f, aspect, 0.1f, 10.0f);
            
            int frameIndex = renderer_.getCurrentFrameIndex();

            Ubo ubo{};
            ubo.projectionViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
            uniformBuffers[frameIndex]->writeToBuffer(&ubo);
            uniformBuffers[frameIndex]->flush();

            if (auto commandBuffer = renderer_.beginFrame()) {

                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    descriptorSets[frameIndex]
                };

                renderer_.beginSwapChainRenderPass(commandBuffer);
                rendererSystem.renderGameObjects(frameInfo, gameObjects_);
                renderer_.endSwapChainRenderPass(commandBuffer);
                renderer_.endFrame();
            }
        }

        vkDeviceWaitIdle(device_.device());

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::loadGameObjects()
    {
        auto cupMesh = Mesh::createMeshFromFile(device_, "../assets/models/obj/Cup.obj");
        auto cup = GameObject::createObject();
        cup.mesh_ = cupMesh;
        cup.transform_.translation = glm::vec3(0.0f, 0.0f, 2.5f);
        cup.transform_.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        cup.transform_.rotation.x = glm::pi<float>();

        gameObjects_.push_back(std::move(cup));
    }
}