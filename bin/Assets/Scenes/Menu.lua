Menu = {
    MainCamera = {
        Camera = {
            background = "0.9 0.7 0.5"
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
            size = "50 50",
            text = "Juego de Pistolas",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0.9 0 0.2",
            colorBot = "0.2 0 0.9"
        }
    },
    PlayText = {
        UIText = {
            anchor = "0.5 0.5",
            position = "0 -150",
            pivot = "0.5 0.5",
            size = "20 25",
            text = "Pulsa Espacio o algun boton del mando para jugar",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0.9 0 0.2",
            colorBot = "0.2 0 0.9"
        }
    },
    -- CHARACTERS --
    Player_01 = {
        Transform = {
            position = "-3 0 -15",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "CubemanMesh.mesh",
            material = "CustomMaterial"
        }
    },
    Player_02 = {
        Transform = {
            position = "3 0 -15",
            size = "1 1 1",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "ConemanMesh.mesh",
            material = "RedCharacter"
        }
    },
    Player_03 = {
        Transform = {
            position = "-8 0 -15",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "SpheremanMesh.mesh",
            material = "GreenCharacter"
        }
    },
    Player_04 = {
        Transform = {
            position = "8 0 -15",
            size = "1 1 1",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "CylindermanMesh.mesh",
            material = "PurpleCharacter"
        }
    }
}