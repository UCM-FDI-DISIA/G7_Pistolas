Scene = {
    
    -- CAMERA --
    MainCamera = {
        Camera = {
            background = "0.6862 0.8784 1",
        },
        AudioListener = {},
        Transform = {
            position = "0 50 45",
            size = "1 1 1",
            rotation = "-50 0 0"
        },
    },

    -- UI --
    Cross_01 = {
        UIImage = {
            anchor = "0.5 0.5",
            position = "-80 96",
            pivot = "0.5 0.5",
            size = "70 70",
            image = "CrossMaterial",
            sortingLayer = 1
        }
    },
    Cross_02 = {
        UIImage = {
            anchor = "0.5 0.5",
            position = "0 96",
            pivot = "0.5 0.5",
            size = "70 70",
            image = "CrossMaterial",
            sortingLayer = 1
        }
    },
    Cross_03 = {
        UIImage = {
            anchor = "0.5 0.5",
            position = "80 96",
            pivot = "0.5 0.5",
            size = "70 70",
            image = "CrossMaterial",
            sortingLayer = 1
        }
    },
    ScoreBackground = {
        UIImage = {
            anchor = "0.5 0.5",
            position = "0 100",
            pivot = "0.5 0.5",
            size = "375 150",
            image = "UIMaterial",
            sortingLayer = 0
        }
    },
    UICountdown = {
        UIText = {
            anchor = ".5 .5",
            position = "0 -40",
            pivot = ".5 .5",
            size = "0 120",
            text = "3",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0.9 0 0.2",
            colorBot = "0.2 0 0.9"
        }
    },
    UICountdownShadow = {
        UIText = {
            anchor = ".5 .5",
            position = "0 -35",
            pivot = ".5 .5",
            size = "0 120",
            text = "3",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0 0 0",
            colorBot = "0 0 0"
        }
    },
    UIPlayerWin = {
        UIText = {
            anchor = ".5 .5",
            position = "0 -100",
            pivot = ".5 .5",
            size = "0 100",
            text = "BLUE WINS",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0.3294 0.7372 0.8352",
            colorBot = "0.3294 0.7372 0.8352",
            sortingLayer = 1
        }
    },
    UIPlayerWinShade = {
        UIText = {
            anchor = ".5 .5",
            position = "0 -95",
            pivot = ".5 .5",
            size = "0 100",
            text = "BLUE WINS",
            font = "Heavitas",
            alignment = "c",
            colorTop = "0 0 0",
            colorBot = "0 0 0",
        }
    },

    -- SOUNDS --
    
    Emitter = {
        EventEmitter = {
            EventID = "event:/ambience2",
            Play = ""
        },
        -- MeshRenderer = {
        --     mesh = "CubemanMesh.mesh",
        --     material = "RedCharacter"
        -- }
    },
    EmitterDeath = {
        EventEmitter = {
            EventID = "event:/PlayerDeath",
           
        }
       
    },
    EmitterCD = {
        EventEmitter = {
            EventID = "event:/CountDown",
           
        }
    },
    EmitterWin = {
        EventEmitter = {
            EventID = "event:/Win",
           
        }
    },
    

        -- CHARACTERS --
    Player_1 = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "CubemanMesh.mesh",
            material = "CustomMaterial"
        },
        PlayerController = {
        },
        ParticleSystem = {
            Particle = "GunShot",
            PlayOnStart = ""
        },
        ParticleSystem = {
            Particle = "Smoke",
            PlayOnStart = ""
        },
        RigidBody = {
            mass = "1",
            group = "player",
            ignoreGroup = "player",
            size = "2 2 2"
        },
        EventEmitter = {
            EventID = "event:/Jump",
            
        }
    },
    Player_2 = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "ConemanMesh.mesh",
            material = "RedCharacter"
        },
        PlayerController = {
        },
        ParticleSystem = {
            Particle = "GunShot",
            PlayOnStart = ""
        },
        ParticleSystem = {
            Particle = "Smoke",
            PlayOnStart = ""
        },
        RigidBody = {
            mass = "1",
            group = "player",
            ignoreGroup = "player",
            size = "2 2 2"
        },
        EventEmitter = {
            EventID = "event:/Jump",
            
        }
    },
    Player_3 = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "SpheremanMesh.mesh",
            material = "GreenCharacter"
        },
        PlayerController = {
        },
        ParticleSystem = {
            Particle = "GunShot",
            PlayOnStart = ""
        },
        ParticleSystem = {
            Particle = "Smoke",
            PlayOnStart = ""
        },
        RigidBody = {
            mass = "1",
            group = "player",
            ignoreGroup = "player",
            size = "2 2 2"
        },
        EventEmitter = {
            EventID = "event:/Jump",
            
        }
    },
    Player_4 = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 -40 0"
        },
        MeshRenderer = {
            mesh = "CylindermanMesh.mesh",
            material = "PurpleCharacter"
        },
        PlayerController = {
        },
        ParticleSystem = {
            Particle = "GunShot",
            PlayOnStart = ""
        },
        ParticleSystem = {
            Particle = "Smoke",
            PlayOnStart = ""
        },
        RigidBody = {
            mass = "1",
            group = "player",
            ignoreGroup = "player",
            size = "2 2 2"
        },
        EventEmitter = {
            EventID = "event:/Jump",
            
        }
    },
    -- CHARACTERS SPAWNPOINTS
    CharacterSpawnpoint_1 = {
        Transform = {
            position = "8 2 8",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    CharacterSpawnpoint_2 = {
        Transform = {
            position = "-8 2 8",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    CharacterSpawnpoint_3 = {
        Transform = {
            position = "8 2 -8",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    CharacterSpawnpoint_4 = {
        Transform = {
            position = "-8 2 -8",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },

    GunSpawnpoint_1 = {
        Transform = {
            position = "0 3 0",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_2 = {
        Transform = {
            position = "-18 7 5",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_3 = {
        Transform = {
            position = "18 7 5",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_4 = {
        Transform = {
            position = "-15 7 20",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_5 = {
        Transform = {
            position = "15 7 20",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_6 = {
        Transform = {
            position = "15 10 -15",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_7 = {
        Transform = {
            position = "-15 10 -15",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    GunSpawnpoint_8 = {
        Transform = {
            position = "0 10 -15",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- }
    },
    -- GUNS SPAWN POINTS

    -- GAME LOGIC --
    GameplayManager = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        GameplayManager = {
        },
        Spawner = {
        }
    },
    LocalMultiplayerManager = {
        Transform = {
            position = "0 0 0",
            size = "1 1 1",
            rotation = "0 0 0"
        },
        LocalMultiplayerManager = {
        }
    },

    -- GUNS SPAWN POINTS
    -- revolver = {
    --     Transform = {
    --         position = "-10 10 0",
    --         size = "1 1 1",
    --         rotation = "115 -130 0"
    --     },
    --     MeshRenderer = {
    --         mesh = "Revolver.mesh",
    --         material = "Revolver"
    --     },
    --     Weapon = {
    --     }
    -- },

    -- PLATFORMS --
    platform_01 = {
        Transform = {
            position = "0 0 0",
            size = "13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_01.mesh",
            material = "Platform"
        }
    },
    platform_02_left = {
        Transform = {
            position = "-16 7.8 -16",
            size = "13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },
    platform_02_right = {
        Transform = {
            position = "16 7.8 -16",
            size = "13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_02.mesh",
            material = "Platform"
        }
    },
    platform_03_left = {
        Transform = {
            position = "-16 4 4",
            size = "13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_03.mesh",
            material = "Platform"
        }
    },
    platform_03_right = {
        Transform = {
            position = "16 4 4",
            size = "-13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_03.mesh",
            material = "Platform"
        }
    },
    platform_04 = {
        Transform = {
            position = "0 6 -18",
            size = "-13 13 13",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "Platform_04.mesh",
            material = "Platform"
        }
    },
    backPlane = {
        Transform = {
            position = "0 -14 -50",
            size = "100 100 100",
            rotation = "0 0 0"
        },
        MeshRenderer = {
            mesh = "BackPlane.mesh",
            material = "BackPlane"
        }
    },


    -- COLLISION --
    platform_01_collision = {
        Transform = {
            position = "0 -4 0",
            size = "25 5 25",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = { mass = 0, size = "25 5 25"  },
    },
    platform_02_left_collision = {
        Transform = {
            position = "-16 -1.8 -16",
            size = "8 10 8",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "8 10 8"
        }
    },
    platform_02_right_collision = {
        Transform = {
            position = "16 -1.8 -16",
            size = "8 10 8",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "8 10 8"
        }
    },
    platform_03_left_collision = {
        Transform = {
            position = "-19 0 8",
            size = "5 5 16",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "5 5 16"
        }
    },
    platform_03_right_collision = {
        Transform = {
            position = "19 0 8",
            size = "5 5 16",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "5 5 16"
        }
    },
    platform_03_left_2_collision = {
        Transform = {
            position = "-11 0 19",
            size = "3 5 5",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "3 5 5"
        }
    },
    platform_03_right_2_collision = {
        Transform = {
            position = "11 0 19",
            size = "3 5 5",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "3 5 5"
        }
    },
    platform_04_collision = {
        Transform = {
            position = "0 7.2 -16",
            size = "8 1 5",
            rotation = "0 0 0"
        },
        -- MeshRenderer = {
        --     mesh = "DebugCube.mesh",
        --     material = "DebugCube"
        -- },
        RigidBody = {
            mass = 0,
            size = "8 1 5"
        }
    }
}