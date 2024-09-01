-- properties
Name = "gameoflife"
Props = {
    std = "c23",
    version = "0.1",
    type = "bin",
    compiler = "gcc",
}

-- external dependenciess
Dependencies = {
    -- { "https://github.com/Surtur-Team/surtests", 0.1 }
}

Libraries = {
    "raylib",
    "GL",
    "m",
    "pthread",
    "dl",
    "rt",
    "X11",
}
