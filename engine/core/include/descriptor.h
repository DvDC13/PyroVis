#pragma once
 
#include "device.h"
 
#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <array>

namespace Pyro {
 
class DescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(Device &device) : device_{device} {}
 
    Builder& addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);

    std::unique_ptr<DescriptorSetLayout> build() const;
 
   private:
    Device& device_;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };
 
  DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;
 
  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> getBindings() const { return bindings; }
 
 private:
  Device& device_;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
 
  friend class LveDescriptorWriter;
};
 
class DescriptorPool {
 public:
  class Builder {
   public:
    Builder(Device& device) : device_{device} {}
 
    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<DescriptorPool> build() const;
 
   private:
    Device& device_;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };
 
  DescriptorPool(
      Device& device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~DescriptorPool();
  DescriptorPool(const DescriptorPool &) = delete;
  DescriptorPool &operator=(const DescriptorPool &) = delete;
 
  bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
 
  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
 
  void resetPool();
  
  Device& getDevice() { return device_; }
  VkDescriptorPool getDescriptorPool() const { return descriptorPool; }
 
 private:
  Device &device_;
  VkDescriptorPool descriptorPool;
 
  friend class LveDescriptorWriter;
};
 
class DescriptorWriter {
 public:
  DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);
 
  DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
 
  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);
 
 private:
  DescriptorSetLayout &setLayout;
  DescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};
 
}

