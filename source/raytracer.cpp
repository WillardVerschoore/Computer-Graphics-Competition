#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/quad.h"
#include "shapes/sphere.h"
#include "shapes/ray_marched_sphere.h"
#include "shapes/sierpinski_tetrahedron.h"
#include "shapes/mandelbulb.h"
#include "shapes/menger_sponge.h"
#include "shapes/torus.h"
#include "shapes/octahedron.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

// =============================================================================
// -- Include all your operations here ---------------------------------------------
// =============================================================================

#include "operations/rotate.h"
#include "operations/scale.h"
#include "operations/translate.h"

// =============================================================================
// -- End of operation includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        if (node.count("rotation"))
        {
            // Create sphere with rotation
            Vector rotation(node["rotation"]);
            double angle = node["angle"];
            obj = ObjectPtr(new Sphere(pos, radius, rotation, angle));
        }
        else
        {
            obj = ObjectPtr(new Sphere(pos, radius));
        }
    }
    else if (node["type"] == "quad")
    {
        Point v0(node["v0"]);
        Point v1(node["v1"]);
        Point v2(node["v2"]);
        Point v3(node["v3"]);
        obj = ObjectPtr(new Quad(v0, v1, v2, v3));
    }
    else if (node["type"] == "ray_marched_sphere")
    {
        obj = ObjectPtr(new RayMarchedSphere());
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else if (node["type"] == "sierpinski_tetrahedron")
    {
        size_t iterations = node["iterations"];
        obj = ObjectPtr(new SierpinskiTetrahedron(iterations));
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else if (node["type"] == "mandelbulb")
    {
        size_t iterations = node["iterations"];
        obj = ObjectPtr(new Mandelbulb(iterations));
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else if (node["type"] == "menger_sponge")
    {
        size_t iterations = node["iterations"];
        obj = ObjectPtr(new MengerSponge(iterations));
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else if (node["type"] == "torus")
    {
        double height = node["height"];
        double width = node["width"];
        obj = ObjectPtr(new Torus( height, width));
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else if (node["type"] == "octahedron")
    {
        obj = ObjectPtr(new Octahedron());
        RayMarchedObject *rayMarchedObj = dynamic_cast<RayMarchedObject*>(obj.get());
        parseRayMarchedObjectNode(node, rayMarchedObj);
    }
    else
    {
        cerr << "Unknown object type: " << node["type"] << ".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"]);
    scene.addObject(obj);
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];

    if (node.count("nt"))
    {
        Color color(node["color"]);
        double nt = node["nt"];
        return Material(color, ka, kd, ks, n, nt);
    }

    if (node.count("color"))
    {
        Color color(node["color"]);
        return Material(color, ka, kd, ks, n);
    }

    if (node.count("texture"))
    {
        string imagePath = node["texture"];
        Image im(imagePath);
        return Material(im, ka, kd, ks, n);
    }

    // No color or texture specified
    return Material(Color(1, 0, 1), ka, kd, ks, n);
}

void Raytracer::parseRayMarchedObjectNode(nlohmann::json const &node, RayMarchedObject *obj) const
{
    obj->maxSteps = (node["maxSteps"]);
    obj->distanceThreshold = (node["distanceThreshold"]);
    for (auto const &operationNode : node["Operations"])
        obj->operations.push_back(parseOperationNode(operationNode));
}

Operation *Raytracer::parseOperationNode(nlohmann::json const &node) const
{
     if (node["type"] == "rotate")
    {
        Vector rotation(node["rotation"]);
        return new Rotate(rotation);
    }
    if (node["type"] == "scale")
    {
        double scale = node["scale"];
        return new Scale(scale);
    }
    else if (node["type"] == "translate")
    {
        Vector translation(node["translation"]);
        return new Translate(translation);
    }

    // No operation specified, return identity operation.
    return new Operation();
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);

    if (jsonscene.count("MaxRecursionDepth"))
    {
        int depth = jsonscene["MaxRecursionDepth"];
        scene.setRecursionDepth(depth);
    }

    if (jsonscene.count("SuperSamplingFactor"))
    {
        int factor = jsonscene["SuperSamplingFactor"];
        scene.setSuperSample(factor);
    }

    if (jsonscene.count("SuperSamplingFactor"))
    {
        int factor = jsonscene["SuperSamplingFactor"];
        scene.setSuperSample(factor);
    }

    if (jsonscene.count("Shadows"))
    {
        bool shadows = jsonscene["Shadows"];
        scene.setRenderShadows(shadows);
    }

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
