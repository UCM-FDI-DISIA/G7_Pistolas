Menu = {
    MainCamera = {
        Camera = {
            background = "0.6862 0.8784 1",
        },
        AudioListener = {},
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 0 0"
        },
    },
    MainMenuManager={
        MainMenuManager={}
    },

    TitleText = {
        UIText = {
            anchor = "0.5 0.5",
            position = "0 -250",
            pivot = "0.5 0.5",
            size = "0 150",
            text = "Shapes & Guns",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0.3294 0.7372 0.8352",
            colorBot = "0.3294 0.7372 0.8352",
            sortingLayer = 1
        }
    },
    TitleTextShadow = {
        UIText = {
            anchor = "0.5 0.5",
            position = "0 -240",
            pivot = "0.5 0.5",
            size = "0 150",
            text = "Shapes & Guns",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0 0 0",
            colorBot = "0 0 0",
        }
    },

    PlayText = {
        UIText = {
            anchor = "0.5 0.5",
            position = "0 -150",
            pivot = "0.5 0.5",
            size = "0 25",
            text = "Press any button to start round",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0 0 0",
            colorBot = "0 0 0"
        }
    },

    -- CHARACTERS --
    Player_01 = {
        Transform = {
            position = "-2 -.5 -10",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "CubemanMesh.mesh",
            material = "CustomMaterial"
        }
    },
    PlayerPlatform_01 = {
        Transform = {
            position = "-2 -2.3 -10",
            size = "2.5 2.5 2.5",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },
    Player_02 = {
        Transform = {
            position = "2 -.9 -12",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "ConemanMesh.mesh",
            material = "RedCharacter"
        }
    },
    PlayerPlatform_02 = {
        Transform = {
            position = "2 -2.7 -12",
            size = "2.5 2.5 2.5",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },
    Player_03 = {
        Transform = {
            position = "-8 0 -15",
            size = "1 1 1",
            rotation = "0 35 0"
        },
        MeshRenderer = {
            mesh = "SpheremanMesh.mesh",
            material = "GreenCharacter"
        }
    },
    PlayerPlatform_03 = {
        Transform = {
            position = "-8 -1.8f -15",
            size = "2.5 2.5 2.5",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },
    Player_04 = {
        Transform = {
            position = "8 -.5 -15",
            size = "1 1 1",
            rotation = "0 -35 0"
        },
        MeshRenderer = {
            mesh = "CylindermanMesh.mesh",
            material = "PurpleCharacter"
        }
    },
    PlayerPlatform_04 = {   
        Transform = {
            position = "8 -2.3f -15",
            size = "2.5 2.5 2.5",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },

    backPlane = {
        Transform = {
            position = "0 0 -20",
            size = "15 15 15",
            rotation = "90 0 0"
        },
        MeshRenderer = {
            mesh = "BackPlane.mesh",
            material = "BackPlane"
        }
    },

      -- SOUND --
    Emitter = {
        EventEmitter = {
            EventID = "event:/lobby",
            Play = ""
        }  
    }
}