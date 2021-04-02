#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "scene.h"
#include "ray_marched_object.h"
#include "operations/operation.h"

#include <string>

// Forward declarations
class Light;
class Material;

#include "json/json_fwd.h"

class Raytracer
{
    Scene scene;
    unsigned width, height;

    public:

        bool readScene(std::string const &ifname);
        void renderToFile(std::string const &ofname);

    private:

        bool parseObjectNode(nlohmann::json const &node);

        Light parseLightNode(nlohmann::json const &node) const;
        Material parseMaterialNode(nlohmann::json const &node) const;
        void parseRayMarchedObjectNode(nlohmann::json const &node, RayMarchedObject *obj) const;
        Operation *parseOperationNode(nlohmann::json const &node) const;
};

#endif
