#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    return pair<ObjectPtr, Hit>(obj, min_hit);
}

Color Scene::trace(Ray const &ray, unsigned depth)
{
    pair<ObjectPtr, Hit> mainhit = castRay(ray);
    ObjectPtr obj = mainhit.first;
    Hit min_hit = mainhit.second;

    // No hit? Return background color.
    if (!obj)
        return sampleBackground(ray);

    Material const &material = obj->material;
    Point hit = ray.at(min_hit.t);
    Vector V = -ray.D;

    // Pre-condition: For closed objects, N points outwards.
    Vector N = min_hit.N;

    // The shading normal always points in the direction of the view,
    // as required by the Phong illumination model.
    Vector shadingN = N;
    if (N.dot(V) >= 0.0)
        shadingN = N;
    else
        shadingN = -N;

    Color matColor = material.color;

    // Add ambient once, regardless of the number of lights.
    Color color = material.ka * matColor;

    // Add diffuse and specular components.
    for (auto const &light : lights)
    {
        Vector L = (light->position - hit).normalized();

        if (renderShadows)
        {
            // Cast a ray from the hit to the light source.
            Ray shadowRay(hit + epsilon * shadingN, L); // Move a bit along the normal to prevent shadow acne.
            pair<ObjectPtr, Hit> shadowHit = castRay(shadowRay);

            // Check whether the shadow ray intersected an object.
            if (shadowHit.first)
            {
                // We only skip this light's contribution if the intersected object was between the hit and the light.
                double distanceToLight = (light->position - hit).length();
                if (shadowHit.second.t < distanceToLight) 
                    continue; // Skip this light's contribution.
            }
        }

        // Add diffuse.
        double dotNormal = shadingN.dot(L);
        double diffuse = std::max(dotNormal, 0.0);
        color += diffuse * material.kd * light->color * matColor;

        // Add specular.
        if(dotNormal > 0)
        {
            Vector reflectDir = reflect(-L, shadingN); // Note: reflect(..) is not given in the framework.
            double specAngle = std::max(reflectDir.dot(V), 0.0);
            double specular = std::pow(specAngle, material.n);

            color += specular * material.ks * light->color;
        }
    }

    if (depth > 0 and material.isTransparent)
    {
        // Trace a ray in the reflected direction.
        Vector reflectDir = reflect(-V, shadingN);
        Ray reflectRay(hit + epsilon * shadingN, reflectDir);
        Color reflectColor = trace(reflectRay, depth - 1);

        // Determine incident and transimitant refraction indices as well as cos(phi).
        double ni, nt, dotNormal;
        if (N.dot(-V) >= 0.0)
        {
            ni = material.nt;
            nt = 1.0;
            dotNormal = N.dot(-V);
        }
        else
        {
            ni = 1.0;
            nt = material.nt;
            dotNormal = -N.dot(-V);
        }

        // Determine the refraction direction.
        Vector refractDir = refract(-V, shadingN, ni / nt);

        // In case of total internal reflection we just return the specular component.
        if (refractDir.x == 0 and refractDir.y == 0 and refractDir.z == 0)
            return color + reflectColor;

        // Trace a ray in the refracted direction.
        Ray refractRay(hit - epsilon * shadingN, refractDir);
        Color refractColor = trace(refractRay, depth - 1);

        // Schlick’s approximation.
        double kr0 = ((ni - nt) / (ni + nt)) * ((ni - nt) / (ni + nt));
        double kr = kr0 + (1 - kr0) * pow((1 - dotNormal), 5);
        double kt = 1 - kr;

        // Return combination of reflected and refracted color.
        color += kr * reflectColor + kt * refractColor;
    }
    else if (depth > 0 and material.ks > 0.0)
    {
        // Trace a ray in the reflected direction.
        Vector reflectDir = reflect(-V, shadingN);
        Ray reflectRay(hit + epsilon * shadingN, reflectDir);
        Color reflectColor = trace(reflectRay, depth - 1);

        // Multiply the resulting color by the specular component and add it to the output color.
        color += material.ks * reflectColor; 
    }

    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    aspectRatio = static_cast<double>(w) / static_cast<double>(h);

    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            // Convert the pixels to camera space positions.
            double pixelX = (2.0 * (static_cast<double>(x) + 0.5) / static_cast<double>(w) - 1.0) * aspectRatio * tan(fieldOfView / 2.0);
            double pixelY = (1.0 - 2.0 * (static_cast<double>(y) + 0.5) / static_cast<double>(h)) * tan(fieldOfView / 2.0);

            // Create and rotate the pixel location.
            Point pixel(pixelX, pixelY, -1.0); // The camera looks along the negative z-axis, hence the -1.0.
            pixel.rotate(rotation);
            
            Ray ray(eye, pixel.normalized());
            Color col = trace(ray, recursionDepth);
            col.clamp();
            img(x, y) = col;
        }
    }
}

Color Scene::sampleBackground(Ray const &ray) const
{
    // If this ray came directly from the camera, we return a gradient background color.
    if (ray.O == eye)
    {
        // Determine the central view ray.
        Vector central(0.0, 0.0, -1.0);
        central.rotate(rotation);

        // Project the rays onto the xz-plane and the yz-plane.
        Vector centralX = Vector(central.x, 0.0, central.z).normalized();
        Vector centralY = Vector(0.0, central.y, central.z).normalized();
        Vector rayX = Vector(ray.D.x, 0.0, ray.D.z).normalized();
        Vector rayY = Vector(0.0, ray.D.y, ray.D.z).normalized();

        // Calculate the angles between the projected rays and use this for the gradient factor.
        double factorX = 1.0 - acos(centralX.dot(rayX)) / (fieldOfView * aspectRatio);
        double factorY = 1.0 - acos(centralY.dot(rayY)) / fieldOfView;
        double factor = (factorX + factorY) / 2.0; // Average the x and y factors.

        return backgroundColor * factor;
    }
    // Otherwise, we just return the background color.
    else
        return backgroundColor;
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene()
:
    objects(),
    lights(),
    eye(),
    rotation(),
    fieldOfView(90.0),
    renderShadows(false),
    recursionDepth(0),
    supersamplingFactor(1),
    backgroundColor()
{}

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setRotation(Triple const &rotation)
{
    this->rotation = rotation;
}

void Scene::setFieldOfView(double fieldOfView)
{
    this->fieldOfView = fieldOfView;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}

void Scene::setRenderShadows(bool shadows)
{
    renderShadows = shadows;
}

void Scene::setRecursionDepth(unsigned depth)
{
    recursionDepth = depth;
}

void Scene::setSuperSample(unsigned factor)
{
    supersamplingFactor = factor;
}

void Scene::setBackgroundColor(Triple const &color)
{
    backgroundColor = color;
}
