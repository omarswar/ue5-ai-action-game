# UE5 Action Game – Enemy AI & Power-up System

A third-person action prototype built in **Unreal Engine 5.5** with gameplay written in **C++**.
The focus is on gameplay programming: an enemy AI that sees, chases and attacks the player,
a family of power-up pickups, player health/combat, a HUD and a save system.

> Only the C++ source and config are in this repo. The `Content/` folder (≈8 GB of
> Marketplace art: Paragon characters, ModSci and Utopian City environments) is
> excluded because those assets can't be redistributed.

## Features

### Enemy AI (`EnemyAIController`, `EnemyAICharacter`)
- **AI Perception (sight)** – 1000 uu sight radius, 90° peripheral vision, memory of 5 s
- **Behavior Tree + Blackboard** driven, started from the controlled pawn's tree asset
- Custom BT tasks written in C++:
  - `BTTask_GetNextPatrolPoint` – cycles through patrol points on the NavMesh
  - `BTTask_AttackTarget` – damages the player, respecting invulnerability

### Power-ups (`APickupBase` + subclasses)
| Pickup | Effect |
|---|---|
| `HealthPickup` | Restores health up to max |
| `FrenzyPickup` | 1.5× attack damage for 30 s |
| `InvulnerabilityPickup` | Temporarily blocks all incoming damage |
| `RotatingSpeedPickup` | Multiplies max walk speed |
| `FloatingScalePickup` | Grows the player by 20% |

Pickups use overlap events, rotate/float in the world and destroy themselves on use.

### Player (`MyCharacter1`)
- Health / max health, damage and healing exposed to Blueprints
- Melee attack with an attack multiplier (used by Frenzy)
- First-person / third-person camera toggle
- Animation variables (speed, direction, in-air) for the Anim Blueprint

### UI & Persistence
- `HealthBarWidget` – UMG health bar bound to the player
- `PauseMenuWidget` – pause / resume / save / load
- `MySaveGame` – saves player location, health and camera mode via `USaveGame`

## Tech
Unreal Engine 5.5 · C++ · AIModule (Behavior Trees, Blackboard, AI Perception) · UMG · NavMesh

## Project structure
```
Source/AIActionGame/
├── EnemyAIController.*        AI perception + behavior tree runner
├── EnemyAICharacter.*         Enemy pawn
├── BTTask_AttackTarget.*      Custom BT task
├── BTTask_GetNextPatrolPoint.*
├── APickupBase.*              Base pickup actor
├── *Pickup.*                  Power-up types
├── MyCharacter1.*             Player character
├── HealthBarWidget.*  PauseMenuWidget.*
└── MySaveGame.*
```

## Running it
1. Clone and right-click `AIActionGame.uproject` → **Generate Visual Studio project files**
2. Build in Visual Studio 2022 (Development Editor, Win64)
3. Add your own level, NavMesh bounds, a Behavior Tree/Blackboard for the enemy and meshes for the pickups

---
Made by **Omar Sherif Swar**.

## Tested
Compiles cleanly with Unreal Engine 5.5 (`AIActionGameEditor`, Win64 Development).
