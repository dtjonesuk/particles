//
// Created by David on 29/06/2025.
//

#ifndef PARTICLES_SCENE_H
#define PARTICLES_SCENE_H

#include <vector>
#include <map>

#include "Entity.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

namespace framework {

    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene() {
            _root = std::make_shared<SceneEntity>(nullptr, nullptr);

            InitializeShaders();
            InitializeMaterials();
            InitializeLights();
            InitializeMatrices();
        }

        [[nodiscard]] Camera *GetCamera() const { return _camera.get(); }

        void SetCamera(Camera *camera) { _camera.reset(camera); }

        void MakePerspectiveCamera(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 viewport) {
            _camera = std::make_unique<PerspectiveCamera>(location, lookAt, up, viewport);
        }

        void MakeOrthographicCamera(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 viewport) {
            _camera = std::make_unique<OrthographicCamera>(location, lookAt, up, viewport);
        }

        std::shared_ptr<Material> GetMaterial(std::string_view name) {
            auto it = std::find_if(_materials.begin(), _materials.end(),
                                   [&name](auto &material) { return material->name == name; });
            if (it != _materials.end())
                return *it;
            throw std::runtime_error("Material not found!");
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> AddEntity(Args... args) {
            auto e = std::make_shared<T>(args...);
            _root->GetChildren().push_back(e);
            return e;
        }

        void DrawScene() {
            // Enable z-buffer
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glViewport(0, 0, _camera->GetViewport().x, _camera->GetViewport().y);

            ClearScreen();

            // for each entity...
            for (auto &entity: _root->GetChildren()) {

                const auto &geometry = entity->GetGeometry();

                const auto &material = geometry->GetMaterial();

                // Set shaders and uniform values
                auto &shader = _shaders.at(material->type);
                shader.Use();

                Light &light = _lights.front();
                light.Bind();

                material->Bind(shader);

                // Model transform
                MatrixUniform matrices;
                matrices.model = entity->GetLocalTransform().GetMatrix();

                // View transform
                matrices.view = _camera->GetViewMatrix();

                // Projection transform
                matrices.projection = _camera->GetProjectionMatrix();

                // Normal transform
                matrices.normal = glm::transpose(glm::inverse(matrices.model));

                if (material->type == MaterialType::Unlit) {
                    shader.SetUniform("u_MVP", matrices.projection * matrices.view * matrices.model);
                } else {
                    // Eye position
                    shader.SetUniform("u_ViewPos", _camera->GetPosition());
                }

                matrices_ubo.Bind();
                matrices_ubo.BindToBindingPoint(MATRICES_BINDING_POINT);
                matrices_ubo.BufferSubData(0, sizeof(MatrixUniform), &matrices);
                matrices_ubo.Unbind();

                geometry->Render();

                material->Unbind();
                light.Unbind();
                shader.Unuse();
            }
        }

    protected:
        std::vector<std::shared_ptr<Material>> _materials;
        std::map<MaterialType, ShaderProgram> _shaders;
        glm::vec3 clearColour{0.1f, 0.1f, 0.1f};

        std::shared_ptr<SceneEntity> _root;

        std::vector<Light> _lights;

        std::unique_ptr<Camera> _camera;

        Buffer<GL_UNIFORM_BUFFER> matrices_ubo;

        void ClearScreen() {
            // Clear screen
            glClearColor(clearColour.r, clearColour.g, clearColour.b, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void InitializeShaders() {
            // Compile shaders and create shader program
            _shaders.emplace(MaterialType::Unlit, ShaderProgram("assets/shaders/unlit.vert.glsl",
                                                                "assets/shaders/unlit.frag.glsl"));
            _shaders.emplace(MaterialType::Phong, ShaderProgram("assets/shaders/blinn.vert.glsl",
                                                                "assets/shaders/blinn.frag.glsl"));

            auto &solid_shader = _shaders[MaterialType::Phong];
            solid_shader.UniformBlockBinding("Material", MATERIAL_BINDING_POINT);
            solid_shader.UniformBlockBinding("Light", LIGHT_BINDING_POINT);
            solid_shader.UniformBlockBinding("Matrices", MATRICES_BINDING_POINT);
        }

        void InitializeMatrices() {
            matrices_ubo.Generate();
            matrices_ubo.Bind();
            GLuint size = _shaders[MaterialType::Phong].GetUniformBlockSize("Matrices");
            matrices_ubo.BufferData(size, nullptr, GL_DYNAMIC_DRAW);
            matrices_ubo.BindToBindingPoint(MATRICES_BINDING_POINT);
            matrices_ubo.Unbind();
        }

        void InitializeLights() {
            auto light = Light({0.0, 5.0, 2.0});
            light.Create(_shaders[MaterialType::Phong]);
            _lights.push_back(std::move(light));
        }
        void InitializeMaterials() {
            // Unlit materials
            _materials.push_back(std::make_shared<UnlitMaterial>("white", glm::vec3{1.0, 1.0, 1.0f}));

            // Standard materials
            _materials.push_back(std::make_shared<SolidMaterial>("emerald",
                                                                 glm::vec3{0.0215, 0.1745, 0.0215},
                                                                 glm::vec3{0.07568, 0.61424, 0.07568},
                                                                 glm::vec3{0.633, 0.727811, 0.633},
                                                                 0.6f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("jade",
                                                                 glm::vec3{0.135, 0.2225, 0.1575},
                                                                 glm::vec3{0.54, 0.89, 0.63},
                                                                 glm::vec3{0.316228, 0.316228, 0.316228},
                                                                 0.1f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("obsidian",
                                                                 glm::vec3{0.05375, 0.05, 0.06625},
                                                                 glm::vec3{0.18275, 0.17, 0.22525},
                                                                 glm::vec3{0.332741, 0.328634, 0.346435},
                                                                 0.3f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("pearl",
                                                                 glm::vec3{0.25, 0.20725, 0.20725},
                                                                 glm::vec3{1, 0.829, 0.829},
                                                                 glm::vec3{0.96648, 0.296648, 0.296648},
                                                                 0.088f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("ruby",
                                                                 glm::vec3{0.1745, 0.01175, 0.01175},
                                                                 glm::vec3{0.61424, 0.04136, 0.04136},
                                                                 glm::vec3{0.727811, 0.626959, 0.626959},
                                                                 0.6f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("turquoise",
                                                                 glm::vec3{0.1, 0.18725, 0.1745},
                                                                 glm::vec3{0.396, 0.74151, 0.69102},
                                                                 glm::vec3{0.297254, 0.30829, 0.306678},
                                                                 0.1f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("brass",
                                                                 glm::vec3{0.329412, 0.223529, 0.027451},
                                                                 glm::vec3{0.780392, 0.568627, 0.113725 * 128},
                                                                 glm::vec3{0.992157, 0.941176, 0.807843},
                                                                 0.21794872f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("bronze",
                                                                 glm::vec3{0.2125, 0.1275, 0.054},
                                                                 glm::vec3{0.714, 0.4284, 0.18144},
                                                                 glm::vec3{0.393548, 0.271906, 0.166721},
                                                                 0.2f * 128));
            _materials.push_back(std::make_shared<SolidMaterial>("chrome",
                                                                 glm::vec3{0.25, 0.25, 0.25},
                                                                 glm::vec3{0.4, 0.4, 0.4},
                                                                 glm::vec3{0.774597, 0.774597, 0.774597},
                                                                 0.6f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("copper",
                                                                 glm::vec3{0.19125, 0.0735, 0.0225},
                                                                 glm::vec3{0.7038, 0.27048, 0.0828},
                                                                 glm::vec3{0.256777, 0.137622, 0.086014},
                                                                 0.1f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("gold",
                                                                 glm::vec3{0.24725, 0.1995, 0.0745},
                                                                 glm::vec3{0.75164, 0.60648, 0.22648},
                                                                 glm::vec3{0.628281, 0.555802, 0.366065},
                                                                 0.4f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("silver",
                                                                 glm::vec3{0.19225, 0.19225, 0.19225},
                                                                 glm::vec3{0.50754, 0.50754, 0.50754},
                                                                 glm::vec3{0.508273, 0.508273, 0.508273 * 128},
                                                                 0.4f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("black_plastic",
                                                                 glm::vec3{0.0, 0.0, 0.0},
                                                                 glm::vec3{0.01, 0.01, 0.01},
                                                                 glm::vec3{0.50, 0.50, 0.50}, 
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("cyan_plastic",
                                                                 glm::vec3{0.0, 0.1, 0.06},
                                                                 glm::vec3{0.0, 0.50980392, 0.50980392},
                                                                 glm::vec3{0.50196078, 0.50196078, 0.50196078 * 128},
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("green_plastic",
                                                                 glm::vec3{0.0, 0.0, 0.0},
                                                                 glm::vec3{0.1, 0.35, 0.1},
                                                                 glm::vec3{0.45, 0.55, 0.45},
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("red_plastic",
                                                                 glm::vec3{0.0, 0.0, 0.0},
                                                                 glm::vec3{0.5, 0.0, 0.0},
                                                                 glm::vec3{0.7, 0.6, 0.6}, 
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("white_plastic",
                                                                 glm::vec3{0.0, 0.0, 0.0},
                                                                 glm::vec3{0.55, 0.55, 0.55},
                                                                 glm::vec3{0.70, 0.70, 0.70}, 
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("yellow_plastic",
                                                                 glm::vec3{0.0, 0.0, 0.0},
                                                                 glm::vec3{0.5, 0.5, 0.0},
                                                                 glm::vec3{0.60, 0.60, 0.50},
                                                                 .25f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("black_rubber",
                                                                 glm::vec3{0.02, 0.02, 0.02},
                                                                 glm::vec3{0.01, 0.01, 0.01},
                                                                 glm::vec3{0.4, 0.4, 0.4},
                                                                 .078125f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("cyan_rubber",
                                                                 glm::vec3{0.0, 0.05, 0.05},
                                                                 glm::vec3{0.4, 0.5, 0.5},
                                                                 glm::vec3{0.04, 0.7, 0.7}, 
                                                                 .078125f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("green_rubber",
                                                                 glm::vec3{0.0, 0.05, 0.0},
                                                                 glm::vec3{0.4, 0.5, 0.4},
                                                                 glm::vec3{0.04, 0.7, 0.04}, 
                                                                 .078125f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("red_rubber",
                                                                 glm::vec3{0.05, 0.0, 0.0},
                                                                 glm::vec3{0.5, 0.4, 0.4},
                                                                 glm::vec3{0.7, 0.04, 0.04}, 
                                                                 .078125f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("white_rubber",
                                                                 glm::vec3{0.05, 0.05, 0.05},
                                                                 glm::vec3{0.5, 0.5, 0.5},
                                                                 glm::vec3{0.7, 0.7, 0.7},
                                                                 .078125f * 128));

            _materials.push_back(std::make_shared<SolidMaterial>("yellow_rubber",
                                                                 glm::vec3{0.05, 0.05, 0.0},
                                                                 glm::vec3{0.5, 0.5, 0.4},
                                                                 glm::vec3{0.7, 0.7, 0.04},
                                                                 .078125f * 128));
            
            for (auto &material : _materials) {
                material->Create(_shaders.at(material->type));
            }
        }
    };
}

#endif //PARTICLES_SCENE_H
