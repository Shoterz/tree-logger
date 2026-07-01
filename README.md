# Tree Logger

Tree Logger is a small C++ desktop logging app built with wxWidgets. It is designed for developers who want a quick way to capture notes while working, categorize them by type, add timestamps, and save the log as a text file.

## Features

- Categorized log entries for DEBUG, IMPLEMENTATION, SOLUTION, ERROR, FATAL, OBJECTIVE, LESSON, CODE, and TRACE notes.
- Short labels and color styling for each log category.
- Timestamp capture with `Ctrl+T`.
- Fast entry submission with `Shift+Enter`.
- Text file workflow with New, Open, Save, and Save As menu actions.
- Theme and font size menu controls.
- Keyboard focus shortcuts for the input field and category selector.

## Screenshot

Screenshots can be added later in the `assets/` folder.

```md
![Tree Logger screenshot](assets/screenshot.png)
```

## Tech Stack

- C++
- wxWidgets
- wxStyledTextCtrl
- Code::Blocks project file

## Project Files

- `App.cpp` / `App.h` starts the wxWidgets application.
- `MainFrame.cpp` / `MainFrame.h` contains the main logger window and menu actions.
- `SearchDialog.cpp` / `SearchDialog.h` contains the search dialog.
- `GUIDialog.cpp` / `GUIDialog.h` contains wxFormBuilder-generated dialog code.
- `Logger_V4.cbp` is the Code::Blocks project file.

## Build Notes

This project was created as a Code::Blocks wxWidgets project. To build it locally, install wxWidgets development packages and open `Logger_V4.cbp` in Code::Blocks.

On Linux, the project expects wxWidgets libraries similar to:

- `wx_gtk3u_core`
- `wx_gtk3u_stc`
- `wx_baseu`

The generated `bin/` and `obj/` folders are ignored by Git.

## Future Images

When you are ready to add an image:

1. Put the image in `assets/`, for example `assets/screenshot.png`.
2. Uncomment or add the screenshot markdown in this README:

```md
![Tree Logger screenshot](assets/screenshot.png)
```
