/*
 * ytdlp_tui.cpp
 * C++ TUI for yt-dlp downloads using FTXUI
 *
 * Build:
 *   # Install ftxui (AUR: ftxui or build from source)
 *   # With CMake (recommended):
 *
 *   mkdir build && cd build
 *   cmake .. && make
 *
 * Or single-file compile (assuming ftxui installed system-wide):
 *   g++ -std=c++20 ytdlp_tui.cpp -lftxui-component -lftxui-dom -lftxui-screen -o ytdlp_tui
 */

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <array>
#include <cstdlib>
#include <string>
#include <vector>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    // --- State ---
    std::string url;
    int res_selected = 1; // default 1080p

    const std::vector<std::string> resolutions = {
        "4320p (8K)", "2160p (4K)", "1440p (2K)",
        "1080p (FHD)", "720p (HD)", "480p", "360p"
    };
    const std::vector<int> res_values = {8320, 2160, 1440, 1080, 720, 480, 360};

    bool download_started = false;
    bool exit_app = false;

    // --- Components ---
    auto url_input = Input(&url, "https://www.youtube.com/watch?v=...");

    auto res_menu = Menu(&resolutions, &res_selected);

    auto download_btn = Button("  ⬇  Download  ", [&] {
        if (url.empty()) return;
        screen.ExitLoopClosure()();
        download_started = true;
    });

    auto quit_btn = Button("  ✕  Quit  ", [&] {
        exit_app = true;
        screen.ExitLoopClosure()();
    });

    auto layout = Container::Vertical({
        url_input,
        res_menu,
        Container::Horizontal({download_btn, quit_btn}),
    });

    auto renderer = Renderer(layout, [&] {
        return vbox({
            // Header
            text("  yt-dlp TUI  ") | bold | color(Color::Yellow) | center,
            separator(),

            // URL section
            text(" YouTube URL") | color(Color::Cyan),
            hbox({text(" ❯ "), url_input->Render()}) | border,

            separator(),

            // Resolution section
            text(" Resolution") | color(Color::Cyan),
            res_menu->Render() | border | flex,

            separator(),

            // Buttons
            hbox({
                download_btn->Render() | color(Color::Green),
                text("  "),
                quit_btn->Render() | color(Color::Red),
            }) | center,

            separator(),

            // Footer hint
            text(" Selected: " + resolutions[res_selected]) | dim | center,
        }) | border;
    });

    screen.Loop(renderer);

    if (exit_app || !download_started) return 0;

    // Build and execute yt-dlp command
    int height = res_values[res_selected];
    std::string fmt = "bv*[height<=" + std::to_string(height) + "]+ba/b[height<=" + std::to_string(height) + "]";
    std::string cmd = "yt-dlp -f \"" + fmt + "\" --merge-output-format mp4 \"" + url + "\"";

    std::puts("\n\033[1;33m[ytdlp-tui]\033[0m Running:");
    std::puts(("  " + cmd).c_str());
    std::puts("");

    return std::system(cmd.c_str());
}