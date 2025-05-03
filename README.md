# 🎆 Fireworks Simulation – C++ / OpenGL

This small project is a 3D simulation of fireworks based on a particle physics system using the **Cyclone** physics library, **FLTK** for the interface, and **OpenGL** for visual rendering.

---

## 🚀 Compilation

This project uses **CMake** and (optionally) **vcpkg** for dependency management.

---

### 📦 Requirements

- CMake ≥ 3.15  
- A C++ compiler (Visual Studio, MinGW, Clang…)  
- FLTK and OpenGL (managed via `vcpkg` if needed)  
- GLUT (basic OpenGL utility library)

---

## ⚙️ Running the Application

### Steps:

1. **Start**  
   Click the **"Start"** button to begin the simulation. This starts the update loop (`update()`).

2. **Launch**  
   Then click the **"Launch"** button to trigger a series of fireworks.  
   Each primary firework (type 0) will explode and generate multiple child particles.
