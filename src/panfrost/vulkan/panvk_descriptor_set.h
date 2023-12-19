/*
 * Copyright © 2021 Collabora Ltd.
 * SPDX-License-Identifier: MIT
 */

#ifndef PANVK_DESCRIPTOR_SET_H
#define PANVK_DESCRIPTOR_SET_H

#include <stdint.h>

#include "vulkan/runtime/vk_object.h"

struct panvk_descriptor_pool;
struct panvk_descriptor_set_layout;
struct panvk_priv_bo;

struct panvk_buffer_desc {
   struct panvk_buffer *buffer;
   VkDeviceSize offset;
   VkDeviceSize size;
};

struct panvk_descriptor_set {
   struct vk_object_base base;
   struct panvk_descriptor_pool *pool;
   const struct panvk_descriptor_set_layout *layout;
   struct panvk_buffer_desc *dyn_ssbos;
   void *ubos;
   struct panvk_buffer_desc *dyn_ubos;
   void *samplers;
   void *textures;
   void *img_attrib_bufs;
   uint32_t *img_fmts;

   struct panvk_priv_bo *desc_bo;
};

VK_DEFINE_NONDISP_HANDLE_CASTS(panvk_descriptor_set, base, VkDescriptorSet,
                               VK_OBJECT_TYPE_DESCRIPTOR_SET)

#endif
