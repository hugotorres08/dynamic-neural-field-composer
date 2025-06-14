#include "user_interface/layout_manager.h"

namespace dnf_composer::user_interface
{

    LayoutManager::LayoutManager()
        : mainWindowSize(1920, 1080), mainWindowPos(0, 0), layoutLocked(false)
    {

        // Set up the default layout
        registerPanel("simulation_control", PanelInfo({0.0f, 0.0f}, {0.25f, 0.3f}, {250, 200}));
        registerPanel("element_window", PanelInfo({0.0f, 0.3f}, {0.25f, 0.4f}, {250, 300}));
        registerPanel("field_metrics", PanelInfo({0.0f, 0.7f}, {0.25f, 0.3f}, {250, 200}));

        registerPanel("node_graph", PanelInfo({0.25f, 0.0f}, {0.5f, 0.6f}, {400, 400}));
        registerPanel("plot_control", PanelInfo({0.25f, 0.6f}, {0.5f, 0.4f}, {400, 250}));

        registerPanel("plots_window", PanelInfo({0.75f, 0.0f}, {0.25f, 1.0f}, {300, 400}));
    }

    void LayoutManager::setMainWindowBounds(const ImVec2 pos, const ImVec2 size)
    {
        mainWindowPos = pos;
        mainWindowSize = size;
    }

    void LayoutManager::registerPanel(const std::string& panelId, const PanelInfo& info)
    {
        panels[panelId] = info;
        panels[panelId].title = panelId; // Default title
    }

    void LayoutManager::beginPanel(const std::string& panelId)
    {
        const auto it = panels.find(panelId);
        if (it == panels.end() || !it->second.isVisible)
            return;

        PanelInfo& panel = it->second;

        // Calculate absolute position and size
        const ImVec2 absPos = getAbsolutePosition(panelId);
        const ImVec2 absSize = getAbsoluteSize(panelId);

        // Set window flags
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;
        if (layoutLocked)
            flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

        // Set the next window position and size
        ImGui::SetNextWindowPos(absPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(absSize, ImGuiCond_Always);

        // Set size constraints
        ImGui::SetNextWindowSizeConstraints(panel.minSize,
            ImVec2(mainWindowSize.x - absPos.x + mainWindowPos.x,
                   mainWindowSize.y - absPos.y + mainWindowPos.y));

        // Begin the window
        ImGui::Begin(panel.title.c_str(), &panel.isVisible, flags);

        // Update panel info if the window was moved/resized by the user (and layout not locked)
        if (!layoutLocked) {
            const ImVec2 currentPos = ImGui::GetWindowPos();
            const ImVec2 currentSize = ImGui::GetWindowSize();

            panel.position = absoluteToRelative(ImVec2(currentPos.x - mainWindowPos.x,
                                                      currentPos.y - mainWindowPos.y));
            panel.size = absoluteToRelative(currentSize, true);

            constrainPanel(panel);
        }
    }

    void LayoutManager::endPanel()
    {
        ImGui::End();
    }

    bool LayoutManager::isPanelVisible(const std::string& panelId) const
    {
        const auto it = panels.find(panelId);
        return it != panels.end() && it->second.isVisible;
    }

    void LayoutManager::setPanelVisibility(const std::string& panelId, const bool visible)
    {
        const auto it = panels.find(panelId);
        if (it != panels.end()) {
            it->second.isVisible = visible;
        }
    }

    void LayoutManager::togglePanelVisibility(const std::string& panelId) {
        const auto it = panels.find(panelId);
        if (it != panels.end()) {
            it->second.isVisible = !it->second.isVisible;
        }
    }

    ImVec2 LayoutManager::getAbsolutePosition(const std::string& panelId) const {
        auto it = panels.find(panelId);
        if (it == panels.end()) return {0, 0};

        return relativeToAbsolute(it->second.position);
    }

    ImVec2 LayoutManager::getAbsoluteSize(const std::string& panelId) const {
        auto it = panels.find(panelId);
        if (it == panels.end()) return {200, 200};

        return relativeToAbsolute(it->second.size, true);
    }

    void LayoutManager::constrainPanel(PanelInfo& panel) const {
        // Ensure panel stays within bounds
        panel.position.x = std::max(0.0f, std::min(panel.position.x, 1.0f - panel.size.x));
        panel.position.y = std::max(0.0f, std::min(panel.position.y, 1.0f - panel.size.y));

        // Ensure minimum size
        ImVec2 minRelativeSize = absoluteToRelative(panel.minSize, true);
        panel.size.x = std::max(panel.size.x, minRelativeSize.x);
        panel.size.y = std::max(panel.size.y, minRelativeSize.y);

        // Ensure panel doesn't exceed window bounds
        panel.size.x = std::min(panel.size.x, 1.0f - panel.position.x);
        panel.size.y = std::min(panel.size.y, 1.0f - panel.position.y);
    }

    ImVec2 LayoutManager::relativeToAbsolute(ImVec2 relative, bool isSize) const {
        if (isSize) {
            return ImVec2(relative.x * mainWindowSize.x, relative.y * mainWindowSize.y);
        } else {
            return ImVec2(mainWindowPos.x + relative.x * mainWindowSize.x,
                         mainWindowPos.y + relative.y * mainWindowSize.y);
        }
    }

    ImVec2 LayoutManager::absoluteToRelative(ImVec2 absolute, bool isSize) const {
        if (isSize) {
            return ImVec2(absolute.x / mainWindowSize.x, absolute.y / mainWindowSize.y);
        } else {
            return ImVec2(absolute.x / mainWindowSize.x, absolute.y / mainWindowSize.y);
        }
    }

    void LayoutManager::saveLayout(const std::string& filename) const {
        nlohmann::json j;

        for (const auto& [id, panel] : panels) {
            j[id] = {
                {"position", {panel.position.x, panel.position.y}},
                {"size", {panel.size.x, panel.size.y}},
                {"visible", panel.isVisible}
            };
        }

        std::ofstream file(filename);
        file << j.dump(4);
    }

    void LayoutManager::loadLayout(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        nlohmann::json j;
        file >> j;

        for (auto& [id, panel] : panels) {
            if (j.contains(id)) {
                auto& panelData = j[id];
                if (panelData.contains("position")) {
                    panel.position = ImVec2(panelData["position"][0], panelData["position"][1]);
                }
                if (panelData.contains("size")) {
                    panel.size = ImVec2(panelData["size"][0], panelData["size"][1]);
                }
                if (panelData.contains("visible")) {
                    panel.isVisible = panelData["visible"];
                }
                constrainPanel(panel);
            }
        }
    }

}