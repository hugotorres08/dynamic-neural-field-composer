

#include "user_interface/logger_window.h"

namespace dnf_composer
{
	namespace user_interface
	{
        Logger LoggerWindow::logger;

		LoggerWindow::LoggerWindow()
		{
			logger.autoScroll = true;
			clean();
		}

		void LoggerWindow::render()
		{
            draw();
		}

		void LoggerWindow::clean()
		{
			logger.buffer.clear();
			logger.lineOffsets.clear();
            logger.lineOffsets.push_back(0);
		}

		void LoggerWindow::draw()
		{
            if (!ImGui::Begin("logger"))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Auto-scroll", &logger.autoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            const bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            const bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            logger.filter.Draw("Filter", -100.0f);

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (clear)
                    clean();
                if (copy)
                    ImGui::LogToClipboard();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                const char* buf = logger.buffer.begin();
                const char* buf_end = logger.buffer.end();


                //// Determine the color based on the log level
                ImVec4 text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default: White
                if (strstr(logger.buffer.c_str(), "[INFO]"))
                    text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
                else if (strstr(logger.buffer.c_str(), "[WARNING]"))
                    text_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
                else if (strstr(logger.buffer.c_str(), "[ERROR]"))
                    text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red

                // Render the text with the determined color
                ImGui::TextColored(text_color, "%s", logger.buffer.c_str());

                //if (logger.filter.IsActive())
                //{
                //    // In this example we don't use the clipper when Filter is enabled.
                //    // This is because we don't have random access to the result of our filter.
                //    // A real application processing logs with ten of thousands of entries may want to store the result of
                //    // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                //    for (int line_no = 0; line_no < logger.lineOffsets.Size; line_no++)
                //    {
                //        const char* line_start = buf + logger.lineOffsets[line_no];
                //        const char* line_end = (line_no + 1 < logger.lineOffsets.Size) ? (buf + logger.lineOffsets[line_no + 1] - 1) : buf_end;
                //        if (logger.filter.PassFilter(line_start, line_end))
                //            ImGui::TextUnformatted(line_start, line_end);
                //    }
                //}
                //else
                //{
                //    // The simplest and easy way to display the entire buffer:
                //    //   ImGui::TextUnformatted(buf_begin, buf_end);
                //    // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                //    // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                //    // within the visible area.
                //    // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                //    // on your side is recommended. Using ImGuiListClipper requires
                //    // - A) random access into your data
                //    // - B) items all being the  same height,
                //    // both of which we can handle since we have an array pointing to the beginning of each line of text.
                //    // When using the filter (in the block of code above) we don't have random access into the data to display
                //    // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                //    // it possible (and would be recommended if you want to search through tens of thousands of entries).
                //    ImGuiListClipper clipper;
                //    clipper.Begin(logger.lineOffsets.Size);
                //    while (clipper.Step())
                //    {
                //        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                //        {
                //            const char* line_start = buf + logger.lineOffsets[line_no];
                //            const char* line_end = (line_no + 1 < logger.lineOffsets.Size) ? (buf + logger.lineOffsets[line_no + 1] - 1) : buf_end;
                //            //ImGui::TextUnformatted(line_start, line_end);

                //            //// Determine the color based on the log level
                //            ImVec4 text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default: White
                //            if (strstr(line_start, "[INFO]"))
                //                text_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
                //            else if (strstr(line_start, "[WARNING]"))
                //                text_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
                //            else if (strstr(line_start, "[ERROR]"))
                //                text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red

                //            // Render the text with the determined color
                //            ImGui::TextColored(text_color, "%s", logger.buffer.c_str());
                //        }
                //    }
                //    clipper.End();
                //}
                ImGui::PopStyleVar();

                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (logger.autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::EndChild();
            ImGui::End();
		}

        void LoggerWindow::addLog(LogLevel level, const std::string& message, ...)
		{
            int old_size = logger.buffer.size();
            va_list args;
            va_start(args, message.c_str());

            // Append log level prefix based on the provided LogLevel
            switch (level)
			{
            case LogLevel::_INFO:
                logger.buffer.appendf("[INFO] ");
                break;
            case LogLevel::_WARNING:
                logger.buffer.appendf("[WARNING] ");
                break;
           case LogLevel::_ERROR:
                logger.buffer.appendf("[ERROR] ");
                break;
                // Add more cases as needed
            }

            // Append the log message
            logger.buffer.appendfv(message.c_str(), args);
            logger.buffer.append("\n");
            va_end(args);

            // Update lineOffsets
            for (const int new_size = logger.buffer.size(); old_size < new_size; old_size++)
                if (logger.buffer[old_size] == '\n')
                    logger.lineOffsets.push_back(old_size + 1);
        }
	}
}
