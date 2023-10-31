cmake_minimum_required(VERSION 3.1.0)

# Find the necessary QT modules
set(QT_MODULES
    Core
    Widgets
    # Gui
    # Network
    # Add any other required modules here
)

exampleMaster(SFML_EditorWidget QSFML_PROFILING ON ON "${QT_MODULES}" ${INSTALL_BIN_PATH})