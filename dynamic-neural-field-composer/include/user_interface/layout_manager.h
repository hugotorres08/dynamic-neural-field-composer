#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <imgui.h>
#include <string>
#include <unordered_map>
#include <functional>

namespace dnf_composer::user_interface
{

    struct PanelInfo
    {
        ImVec2 position;        // Position relative to the main window (0-1 range)
        ImVec2 size;           // Size relative to the main window (0-1 range)
        ImVec2 minSize;        // Minimum size in pixels
        bool isResizable;
        bool isVisible;
        std::string title;

        explicit PanelInfo(const ImVec2 pos = {0, 0}, const ImVec2 sz = {0.5f, 0.5f},
                           const ImVec2 minSz = {200, 150}, const bool resizable = true)
            : position(pos), size(sz), minSize(minSz), isResizable(resizable), isVisible(true) {}
    };

    class LayoutManager
    {
    private:
        std::unordered_map<std::string, PanelInfo> panels;
        ImVec2 mainWindowSize;
        ImVec2 mainWindowPos;
        bool layoutLocked;

    public:
        LayoutManager();

        void setMainWindowBounds(ImVec2 pos, ImVec2 size);
        void registerPanel(const std::string& panelId, const PanelInfo& info);
        void beginPanel(const std::string& panelId);

        static void endPanel();

        bool isPanelVisible(const std::string& panelId) const;
        void setPanelVisibility(const std::string& panelId, bool visible);
        void togglePanelVisibility(const std::string& panelId);

        void setLayoutLocked(bool locked) { layoutLocked = locked; }
        bool isLayoutLocked() const { return layoutLocked; }

        void saveLayout(const std::string& filename) const;
        void loadLayout(const std::string& filename);

        // Get absolute position and size for a panel
        ImVec2 getAbsolutePosition(const std::string& panelId) const;
        ImVec2 getAbsoluteSize(const std::string& panelId) const;

    private:
        void constrainPanel(PanelInfo& panel) const;
        ImVec2 relativeToAbsolute(ImVec2 relative, bool isSize = false) const;
        ImVec2 absoluteToRelative(ImVec2 absolute, bool isSize = false) const;
    };

}