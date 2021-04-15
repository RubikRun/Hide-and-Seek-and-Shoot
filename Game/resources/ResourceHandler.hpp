#pragma once

#include <memory>
#include <string>
#include <map>
#include <stdexcept>

namespace HideAndSeekAndShoot
{

namespace Resources
{

/**
 * A template class for handling SFML resources, such as textures, sound effects and fonts.
 * The user of the class can load resources from files,
 * and then retrieve them through the class and use them.
 * 
 * @param[in] ResourceIdType
 *  The type of the resource IDs that will be used to specify resources
 *  when loading or retrieving resources
 * @param[in] ResourceType
 *  The type of the resources themselves
 */
template <class ResourceIdType, class ResourceType>
class ResourceHandler
{
  
  public:

    /**
     * Loads a resource for an ID from a file.
     * 
     * @param[in] id
     *  Id of the resource that we want to load
     * @param[in] filename
     *  Name of the file where the resource is located
     */
    void Load(
        ResourceIdType id,
        std::string const& filename
    );

    /**
     * Returns a reference to the resource with the requested Id.
     * Note that the resource at that Id should be loaded first.
     * 
     * @param[in] id
     *  Id of the resource that we want to retrieve
     * 
     * @return reference to the requested resource
     */
    //ResourceType& Get(ResourceIdType id);
    ResourceType const& Get(ResourceIdType id) const;

  private:

    /* Map of the resources.
       Maps each resource Id with the corresponding resource object. */
    std::map<
      ResourceIdType,
      std::unique_ptr<ResourceType>
    > _resourceMap;
};

// RIDT = ResourceIdType, RT = ResourceType
template <class RIDT, class RT>
void ResourceHandler<RIDT, RT>::Load(
    RIDT id,
    std::string const& filename)
{
    std::unique_ptr<RT> resource = std::make_unique<RT>();
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("Error: Cannot load resource from file " + filename);
    }
    
    _resourceMap.insert(std::make_pair(id, std::move(resource)));
}

/*
// RIDT = ResourceIdType, RT = ResourceType
template <class RIDT, class RT>
RT& ResourceHandler<RIDT, RT>::Get(RIDT id)
{
    return const_cast<RT&>(const_cast<ResourceHandler<RIDT, RT> const*>(this)->get());
}
*/

// RIDT = ResourceIdType, RT = ResourceType
template <class RIDT, class RT>
RT const& ResourceHandler<RIDT, RT>::Get(RIDT id) const
{
    auto found = _resourceMap.find(id);
    if (found == _resourceMap.end())
    {
        throw std::runtime_error("Error: Requested resource is not loaded in the resource handler.");
    }
    return *(found->second);
}

} // namespace Resources

} // namespace HideAndSeekAndShoot