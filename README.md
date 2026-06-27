# A* Pathfinding Visualizer

A real-time visualization of the A* pathfinding algorithm built with C++ and SFML. Watch the algorithm explore a grid, avoid walls, and find the shortest path between two points.

---

## What it looks like

| Color | Meaning |
|-------|---------|
| 🔴 Red | Start point |
| 🟡 Gold | End point |
| ⬛ Dark | Wall (obstacle) |
| 🟢 Green | Cells explored by the algorithm |
| 🔵 Blue | Shortest path found |

---

## How to Use

| Action | Control |
|--------|---------|
| Place start point | Hold `S` + left click |
| Place end point | Hold `E` + left click |
| Draw walls | Left click / drag |
| Run the algorithm | Press `A` |
| Reset everything | Press `R` |

---

## Recommended Flow

1. Click a cell while holding `S` to set the start (turns red)
2. Click another cell while holding `E` to set the end (turns gold)
3. Draw some walls by clicking around
4. Press `A` and watch it find the path

---

## How it Works

A* is a best-first search algorithm that finds the shortest path between two points. It uses a heuristic (Manhattan distance) to prioritize which cells to explore next, making it faster than blind search algorithms like BFS.

At each step it picks the cell with the lowest `f = g + h` score, where:
- `g` = actual cost from start to this cell
- `h` = estimated cost from this cell to the end

The algorithm runs one step per frame so you can see exactly how it thinks.
