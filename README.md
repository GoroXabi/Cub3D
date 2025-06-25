# Cub3D 🧊

Cub3D is a C-based clone of the classic DOOM game. It uses raycasting to simulate a 3D environment and features customizable textures and maps defined in `.cub` configuration files.

---

## 🛠️ How to Run

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd Cub3D
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the game with a `.cub` file:
   ```bash
   ./cub3d <path-to-map.cub>
   ```

---

## 📁 Repository Contents

- `maps/`: Sample `.cub` files (some are not playable).
- `textures/`: Sample `.png` wall textures.
- `includes/structures.h`: Contains definitions for screen resolution (`WIDTH`, `HEIGHT`).

---

## 🧩 The `.cub` File Format

`.cub` files are divided into two parts:
1. **Texture & color information**
2. **Map layout**

### 1. Texture and Color Info

- Each texture line format:
  ```
  <ORIENTATION> <path-to-texture.png>
  ```
  - Orientations: `NO`, `SO`, `EA`, `WE`
  - Any `.png` file is accepted — it will be dynamically rescaled at runtime.

- Floor and ceiling colors:
  ```
  F R,G,B   # Floor color
  C R,G,B   # Ceiling color
  ```
  - `R`, `G`, `B` values must be between 0–255.

> ⚠️ All four wall textures, as well as floor and ceiling colors, are **required**. If any are missing or declared after the map, the file is **invalid**.

### 2. Map Layout

The map is defined using plain text characters:
- `'0'` or `' '`: floor
- `'1'`: wall
- `'N'`, `'S'`, `'E'`, `'W'`: player start position and orientation

> ⚠️ A map is considered playable **only** if the playable area is fully enclosed by walls.

> 🌊 A **flood-fill algorithm** checks whether the player's starting area is properly closed.

---

## 🎮 Controls

- **W**: Move forward  
- **A**: Move left  
- **S**: Move backward  
- **D**: Move right  
- **Left Shift**: Sprint  
- **Arrow Up / Down**: Increase / decrease field of view (FOV)  
- **ESC** or close window: Exit game

---

## 🚀 Highlights

### 🔦 Raycasting Engine

- Uses an **optimized raycasting algorithm** to avoid unnecessary recalculations, improving performance on large maps.
- Calculates wall distances and renders vertical strips for a smooth 3D effect.

### 🖼️ Texture Rendering

- Textures are **drawn directly on projection**, without prior memory rescaling.
- Efficiently handles large `.png` textures with no significant performance drop.
- Supports **full and partial transparency**, layering textures from back to front when necessary.

---

## ⚠️ Notes

- `.cub` files must include all textures and color definitions **before** the map section.
- If any required texture or color is missing, or if the map is not enclosed, the file is **invalid** and the game will not run.
- Some example maps in the `maps/` folder are for testing purposes only and may not be playable.

---

## 📷 Screenshot

![](https://github.com/GoroXabi/Cub3D/blob/main/screenshots/Screenshot1.png?raw=true)
![](https://github.com/GoroXabi/Cub3D/blob/main/screenshots/Screenshot2.png?raw=true)

---

## 📄 License

This project is for educational purposes and is not affiliated with id Software or DOOM.

---

😇 Carmack please dont sue us.
