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
#include <cstdlib>
#include <string>
#include <vector>

using namespace ftxui;

int main() {
    auto screen = ScreenInteractive::TerminalOutput();

    std::string url;
    int res_selected  = 3; // default 1080p
    int fmt_selected  = 0; // default mp4

    const std::vector<std::string> resolutions = {
        "4320p (8K)", "2160p (4K)", "1440p (2K)",
        "1080p (FHD)", "720p (HD)", "480p", "360p"
    };
    const std::vector<int> res_values = { 8320, 2160, 1440, 1080, 720, 480, 360 };


    struct FmtOption { std::string label; std::string ytdlp_fmt; bool audio_only; };
    const std::vector<FmtOption> formats = {
        { "mp4  (video)", "mp4",  false },
        { "webm (video)", "webm", false },
        { "mp3  (audio)", "mp3",  true  },
        { "wav  (audio)", "wav",  true  },
    };

    bool download_started = false;
    bool exit_app         = false;

    auto url_input    = Input(&url, "https://www.youtube.com/watch?v=...");
    auto res_menu     = Menu(&resolutions, &res_selected);

    std::vector<std::string> fmt_labels;
    for (auto& f : formats) fmt_labels.push_back(f.label);
    auto fmt_menu = Menu(&fmt_labels, &fmt_selected);

    auto download_btn = Button("  ⬇  Download  ", [&] {
        if (url.empty()) return;
        download_started = true;
        screen.ExitLoopClosure()();
    });
    auto quit_btn = Button("  ✕  Quit  ", [&] {
        exit_app = true;
        screen.ExitLoopClosure()();
    });

    auto layout = Container::Vertical({
        url_input,
        Container::Horizontal({ res_menu, fmt_menu }),
        Container::Horizontal({ download_btn, quit_btn }),
    });

    auto renderer = Renderer(layout, [&] {
        auto& sel_fmt = formats[fmt_selected];
        std::string hint = sel_fmt.audio_only
            ? "audio: " + sel_fmt.label
            : resolutions[res_selected] + " · " + sel_fmt.label;

        return vbox({
            text("  yt-dlp TUI  ") | bold | color(Color::Yellow) | center,
            separator(),

            text(" YouTube URL") | color(Color::Cyan),
            hbox({ text(" ❯ "), url_input->Render() }) | border,
            separator(),

            hbox({
                vbox({
                    text(" Resolution") | color(Color::Cyan),
                    res_menu->Render() | border | flex,
                }) | flex,
                vbox({
                    text(" Format") | color(Color::Cyan),
                    fmt_menu->Render() | border,
                }),
            }),
            separator(),

            hbox({
                download_btn->Render() | color(Color::Green),
                text("  "),
                quit_btn->Render() | color(Color::Red),
            }) | center,
            separator(),

            text(" Selected: " + hint) | dim | center,
        }) | border;
    });

    screen.Loop(renderer);

    if (exit_app || !download_started) return 0;

    // Build yt-dlp command
    auto& sel_fmt = formats[fmt_selected];
    std::string cmd;

    if (sel_fmt.audio_only) {
        // Extract best audio, convert to target format
        cmd = "yt-dlp -x --audio-format " + sel_fmt.ytdlp_fmt
            + " \"" + url + "\"";
    } else {
        int h = res_values[res_selected];
        std::string f = "bv*[height<=" + std::to_string(h) + "]+ba/b[height<=" + std::to_string(h) + "]";
        cmd = "yt-dlp -f \"" + f + "\" --merge-output-format " + sel_fmt.ytdlp_fmt
            + " \"" + url + "\"";
    }

    std::puts("\n\033[1;33m[ytdlp-tui]\033[0m Running:");
    std::puts(("  " + cmd).c_str());
    std::puts("");

    return std::system(cmd.c_str());
}