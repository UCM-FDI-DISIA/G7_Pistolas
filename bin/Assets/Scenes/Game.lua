Game = {
    -- MainCamera = {
    --     Camera = {},
    --     AudioListener = {}
    -- },
    Object = {
        AudioSource = {
            Volume = 1,
            PlayOnAwake = "Assets/Sounds/prueba.wav"
        }
    },
    Emitter = {
        EventEmitter = {
            EventID = "event:/Music/Level 01",
            Play = "";
        }
    },
    cube = {
        Transform = {
            position = "0 0 -150",
            size = "20 20 20",
            rotation = "15 -40 0"
        },
        MeshRenderer = {
            mesh = "ConemanMesh.mesh",
            material = "CustomMaterial"
        },
        PlayerController = {}
    },
    MainCamera = {
        Camera = {
            background = "0.5 0.7 0.9"
        },
        AudioListener = {}
    },
    platform = {
        Transform = {
            position = "0 -38 -150",
            size = "35 35 35",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "Platform_01.mesh",
            material = "Platform"
        },
        ParticleSystem = {
            Particle = "GunShot",
            PlayOnStart = ""
        }
    },
    -- cube = {
    --     Transform = {
    --         position = "0 0 -150",
    --         size = "20 20 20",
    --         rotation = "15 -40 0"
    --     },
    --     MeshRenderer = {
    --         mesh = "SpheremanMesh.mesh",
    --         material = "GreenCharacter"
    --     }
    -- },
    -- cube = {
    --     Transform = {
    --         position = "0 0 -150",
    --         size = "20 20 20",
    --         rotation = "15 -40 0"
    --     },
    --     MeshRenderer = {
    --         mesh = "CylindermanMesh.mesh",
    --         material = "PurpleCharacter"
    --     },
    --     ParticleSystem = {
    --         Particle = "Smoke",
    --         PlayOnStart = ""
    --     }
    -- },
    -- revolver = {
    --     Transform = {
    --         position = "-50 0 -150",
    --         size = "15 15 15",
    --         rotation = "115 -130 0"
    --     },
    --     MeshRenderer = {
    --         mesh = "Revolver.mesh",
    --         material = "Revolver"
    --     }
    -- },
    -- platform = {
    --     Transform = {
    --         position = "0 -38 -150",
    --         size = "35 35 35",
    --         rotation = "0 -40 0"
    --     },
    --     MeshRenderer = {
    --         mesh = "Platform_01.mesh",
    --         material = "Platform"
    --     }
    -- },
}