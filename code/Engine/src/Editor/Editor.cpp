//
// Created by Radek on 26.03.2024.
//

#include "imgui.h"
#include "Editor/Gizmos.h"
#include "Editor/Editor.h"
#include "Scene/SceneManager.h"
#include "Editor/Panels/FileDialog.h"
#include "Editor/YamlWriter.h"


using namespace EditorLayer;

Editor* Editor::Instance = nullptr;

//!Shared pointers for a rescue
//!FALSE ALARM (FOR NOW)

Editor::Editor() {
    if(Editor::Instance == nullptr)
    {
        Editor::Instance = this;
    }
    else
    {
        delete this;
    }

    keyMapReader.parseFile(keyMap);
}

void Editor::draw() {
    hierarchy->draw(editorCamera);
    drawMainMenu();
}


void Editor::setCamera(Camera * newCamera) {
    editorCamera = newCamera;
}

void Editor::chooseEntityWithMouse() {
    auto[mx, my] = ImGui::GetMousePos();
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0)
    {
        int pixelData;

        glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        //LOG_INFO(to_string(pixelData));
//        if(pixelData != -1)
//        {
//
//        }
    }
}

void Editor::init(Camera* camera) {
    setCamera(camera);
}

void Editor::end() {
    delete hierarchy;
    delete editorCamera;
}

void Editor::drawMainMenu() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open"))
            {
                std::string path = FileDialog::openFile("YAML Files (*.yaml;*.yml)\0*.yaml;*.yml\0All Files (*.*)\0*.*\0");

                if(!path.empty())
                {
                    SceneManagement::SceneManager::Instance->unloadScene(SceneManagement::SceneManager::Instance->getLoadedScenes()[0]);
                    SceneManagement::SceneManager::Instance->loadScene(path);
                }
            }

            if(ImGui::MenuItem("Save", "CTRL+S"))
            {
                std::string path = FileDialog::saveFile("YAML Files (*.yaml;*.yml)\0*.yaml;*.yml\0All Files (*.*)\0*.*\0");

                if(!path.empty())
                {
                    YamlWriter::WriteTo(SceneManagement::SceneManager::Instance->getLoadedScenes()[0], path);
                }
            }

            ImGui::EndMenu();
        }
//        if (ImGui::BeginMenu("Edit"))
//        {
//            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
//            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
//            ImGui::Separator();
//            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
//            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
//            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
//            ImGui::EndMenu();
//        }
        ImGui::EndMainMenuBar();
    }
}
