# ytdown Documentation

This document provides a detailed guide on how to use `ytdown`.

## Introduction

`ytdown` is a terminal-based user interface (TUI) for the popular command-line video downloader, `yt-dlp`. It provides a simple and interactive way to download videos from YouTube without having to remember the complex command-line options of `yt-dlp`.

## Features

*   **Simple and Interactive TUI**: `ytdown` provides a clean and intuitive TUI to guide you through the video download process.
*   **Resolution Selection**: You can choose from a range of resolutions, from 360p up to 8K.
*   **Format Selection**: You can select the desired video format (mp4, webm) or audio format (mp3, wav).
*   **Automatic Command Generation**: `ytdown` automatically generates the appropriate `yt-dlp` command based on your selections.

## Building and Running

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/amar/ytdown.git
    cd ytdown
    ```

2.  **Build the executable**:
    ```bash
    make
    ```
    This will compile the `ytdown` executable and place it in the current directory. The `Makefile` will also automatically clone the `ftxui` library.

3.  **Run the application**:
    ```bash
    ./ytdown
    ```
    This will launch the TUI.

## Usage

1.  **Enter the YouTube URL**:
    The cursor will be in the URL input field. Paste the YouTube URL of the video you want to download.

3.  **Select Resolution**:
    Use the arrow keys to navigate the "Resolution" menu and select your desired resolution.

4.  **Select Format**:
    Use the arrow keys to navigate the "Format" menu and select your desired video or audio format.

5.  **Download**:
    Press the "Download" button to start the download. `ytdown` will then exit and you will see the `yt-dlp` output in your terminal.

6.  **Quit**:
    Press the "Quit" button to exit the application without downloading.

## Command-line Options

`ytdown` does not have any command-line options. It is purely a TUI-based application.

## Configuration

`ytdown` does not have a configuration file. All options are selected within the TUI.
