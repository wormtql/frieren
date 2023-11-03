#include "RenderingContext.h"

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

namespace frieren_core {
    WGPUAdapter RenderingContext::request_adapter(const WGPURequestAdapterOptions& options) {
        struct UserData {
            WGPUAdapter adapter = nullptr;
            bool requestEnded = false;
        };

        UserData userData;

        auto onAdapterRequestEnded = [] (WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData) {
            UserData& userData = *reinterpret_cast<UserData*>(pUserData);
            if (status == WGPURequestAdapterStatus_Success) {
                userData.adapter = adapter;
            } else {
                std::cout << "Could not get WebGPU adapter: " << message << std::endl;
            }
            userData.requestEnded = true;
        };

        wgpuInstanceRequestAdapter(instance, &options, onAdapterRequestEnded, (void*) &userData);

        assert(userData.requestEnded);

        return userData.adapter;
    }

    void RenderingContext::inspect_adapter() {
        std::vector<WGPUFeatureName> features;

        size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

        features.resize(featureCount);

        wgpuAdapterEnumerateFeatures(adapter, features.data());

        std::cout << "Adapter features:" << std::endl;
        for (auto f : features) {
            std::cout << " - " << f << std::endl;
        }

        WGPUSupportedLimits limits = {};
        limits.nextInChain = nullptr;
        bool success = wgpuAdapterGetLimits(adapter, &limits);
        if (success) {
            std::cout << "Adapter limits:" << std::endl;
            std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
            std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
            std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
            std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
            std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
            std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
            std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
            std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
            std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
            std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
            std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
            std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
            std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
            std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
            std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
            std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
            std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
            std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
            std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
            std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
            std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
            std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
            std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
            std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
        }

        WGPUAdapterProperties properties = {};
        properties.nextInChain = nullptr;
        wgpuAdapterGetProperties(adapter, &properties);
        std::cout << "Adapter properties:" << std::endl;
        std::cout << " - vendorID: " << properties.vendorID << std::endl;
        std::cout << " - deviceID: " << properties.deviceID << std::endl;
        std::cout << " - name: " << properties.name << std::endl;
        if (properties.driverDescription) {
            std::cout << " - driverDescription: " << properties.driverDescription << std::endl;
        }
        std::cout << " - adapterType: " << properties.adapterType << std::endl;
        std::cout << " - backendType: " << properties.backendType << std::endl;
    }

    WGPUDevice RenderingContext::request_device(const WGPUDeviceDescriptor& descriptor) {
        struct UserData {
            WGPUDevice device = nullptr;
            bool requestEnded = false;
        };
        UserData userData;

        auto onDeviceRequestEnded = [] (WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* pUserData) {
            UserData& userData = *reinterpret_cast<UserData*>(pUserData);
            if (status == WGPURequestDeviceStatus_Success) {
                userData.device = device;
            } else {
                std::cout << "Could not get WebGPU device: " << message << std::endl;
            }
            userData.requestEnded = true;
        };

        wgpuAdapterRequestDevice(adapter, &descriptor, onDeviceRequestEnded, (void*) &userData);

        assert(userData.requestEnded);

        return userData.device;
    }

    void RenderingContext::inspect_device() {
        std::vector<WGPUFeatureName> features;
        size_t featureCount = wgpuDeviceEnumerateFeatures(device, nullptr);
        features.resize(featureCount);
        wgpuDeviceEnumerateFeatures(device, features.data());

        std::cout << "Device features:" << std::endl;
        for (auto f : features) {
            std::cout << " - " << f << std::endl;
        }

        WGPUSupportedLimits limits = {};
        limits.nextInChain = nullptr;
        bool success = wgpuDeviceGetLimits(device, &limits);
        if (success) {
            std::cout << "Device limits:" << std::endl;
            std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
            std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
            std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
            std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
            std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
            std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
            std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
            std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
            std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
            std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
            std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
            std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
            std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
            std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
            std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
            std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
            std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
            std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
            std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
            std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
            std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
            std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
            std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
            std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
        }
    }

    void RenderingContext::setup_material_manager() {
        this->material_manager = make_shared<MaterialManager>(
            shader_manager,
            texture_manager,
            sampler_manager
        );
        // material_manager->add_search_path("")
    }

    void RenderingContext::setup_sampler_manager() {
        this->sampler_manager = make_shared<SamplerManager>();
    }

    void RenderingContext::setup_texture_manager() {
        this->texture_manager = make_shared<TextureManager>();
    }

    void RenderingContext::setup_shader_manager() {
        this->shader_manager = make_shared<ShaderManager>();
    }

    void RenderingContext::setup_mesh_manager() {
        this->mesh_manager = make_shared<MeshManager>();
        mesh_manager->init_builtin_mesh(device, queue);
    }

    RenderingContext::RenderingContext() {
        WGPUInstanceDescriptor instance_desc;
        instance_desc.nextInChain = nullptr;

        this->instance = wgpuCreateInstance(&instance_desc);
        if (!instance) {
            std::cerr << "Could not initialize WebGPU!" << std::endl;
            assert(false);
        }
        std::cout << "WGPU instance: " << instance << std::endl;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow *window = glfwCreateWindow(640, 480, "Learn WebGPU", nullptr, nullptr);
        if (!window) {
            std::cerr << "Could not open window!" << std::endl;
            glfwTerminate();
            assert(false);
        }

        this->surface = glfwGetWGPUSurface(instance, window);

        WGPURequestAdapterOptions adapterOpts{};
        adapterOpts.nextInChain = nullptr;
        adapterOpts.compatibleSurface = surface;
        this->adapter = request_adapter(adapterOpts);

        WGPUDeviceDescriptor device_desc{};
        device_desc.nextInChain = nullptr;
        device_desc.label = "My Device";
        device_desc.requiredFeaturesCount = 0;
        device_desc.requiredLimits = nullptr;
        device_desc.defaultQueue.nextInChain = nullptr;
        device_desc.defaultQueue.label = "The default queue";
        this->device = request_device(device_desc);

        auto onDeviceError = [](WGPUErrorType type, char const *message, void *) {
            std::cout << "Uncaptured device error: type" << type;
            if (message) std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr);

        inspect_device();

        this->queue = wgpuDeviceGetQueue(device);

#ifdef WEBGPU_BACKEND_DAWN
        // Add a callback to monitor the moment queued work finished
        auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void* /* pUserData */) {
            std::cout << "Queued work finished with status: " << status << std::endl;
        };
        wgpuQueueOnSubmittedWorkDone(queue, 0 /* non standard argument for Dawn */, onQueueWorkDone, nullptr /* pUserData */);
#else
        auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void *) {
            std::cout << "Queued work finished with status: " << status << std::endl;
        };
        wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr);
#endif // WEBGPU_BACKEND_DAWN

        WGPUSwapChainDescriptor swap_chain_desc;
        swap_chain_desc.nextInChain = nullptr;
        swap_chain_desc.width = 640;
        swap_chain_desc.height = 480;
#ifdef WEBGPU_BACKEND_WGPU
        WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(surface, adapter);
#else
        WGPUTextureFormat swapChainFormat = WGPUTextureFormat_BGRA8Unorm;
#endif
        swap_chain_desc.format = swapChainFormat;
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;

        this->swap_chain = wgpuDeviceCreateSwapChain(device, surface, &swap_chain_desc);


        this->setup_sampler_manager();
        this->setup_shader_manager();
        this->setup_texture_manager();
        this->setup_material_manager();
        this->setup_mesh_manager();
    }
}