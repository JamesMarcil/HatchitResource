#pragma once

#include <ht_platform.h>
#include <ht_noncopy.h>
#include <ht_refcounted.h>
#include <ht_singleton.h>
#include <ht_file.h>
#include <map>
#include <type_traits>
#include <ht_path_singleton.h>
#include <ht_jsonhelper.h>
#include <fstream>

namespace Hatchit
{
    namespace Resource
    {
        template<typename ResourceType>
        class HT_API FileResource : public Core::RefCounted<ResourceType>
        {
        public:
            virtual ~FileResource() { };

            template<typename... Args>
            static Core::Handle<const ResourceType> GetHandle(const std::string& ID, Args&&... args)
            {
                Core::Handle<ResourceType> handle = RefCounted::GetHandle(ID, std::forward<Args>(args)...);
                if (handle.IsValid() /*&& handle->VInitFromFile(ID)*/)
                    return handle.StaticCastHandle<const ResourceType>();
                else
                    return Core::Handle<const ResourceType>();
            }

            static Core::Handle<const ResourceType> GetHandleFromFileName(const std::string& fileName);

        protected:
            FileResource(std::string ID);
            virtual bool VInitFromFile(const std::string& file) = 0;
        };
    }
}

/*
    Here's how the classes are going to be structured for resources:
    Resource<T> is the base class for all resources that have to be loaded / generated
    T is going to be the actual implementation of the data.
    Then we create a TPtr alias that is equivalent to Resource<T>::Handle
*/

#include <ht_resource.inl>