# Restaurant Management System — Data Structures & Algorithms Project

A C++ simulation of a restaurant's order management system, built for the **CMPG104 (Data Structures & Algorithms)** course at the Faculty of Engineering, Cairo University — Spring 2026.

The system simulates the full lifecycle of restaurant orders (dine-in, takeaway, and delivery) across discrete timesteps, managing chefs, tables, and delivery scooters as shared resources, and produces detailed statistics about the simulation.

## Overview

The project models a restaurant that handles three order types:
- **Dine-in (OD)** — served at a table, split into Grilled (ODG) and Normal (ODN)
- **Takeaway (OT)** — picked up on-site by the customer
- **Delivery (OV)** — delivered by scooter, split into Cold (OVC), Grilled (OVG), and Normal (OVN)

Each order moves through a defined lifecycle: **Pending → Cooking → Ready → In-Service → Finished**, with resources (chefs, tables, scooters) assigned according to type-specific priority rules at every timestep.

## Key Features

- Custom-built **Stack**, **Queue**, and **Priority Queue** data structures — no STL containers used for core order/resource lists
- Priority-based order assignment logic (chef specialization, table best-fit, scooter distance-based assignment)
- Order cancellation handling with proper resource release
- File-driven simulation: reads structured input files and produces a statistics-rich output file
- Interactive and silent simulation modes
- **Bonus: COMBO Orders** — orders requiring multiple chefs and multiple scooters, with the highest assignment priority in the system

## Architecture

- **`Action`** — abstract base class (`RequestAction`, `CancelAction`) representing input file events
- **`Order`**, **`Chef`**, **`Scooter`**, **`Table`** — core entity classes
- **`Restaurant`** — the central class managing all lists, running the per-timestep simulation loop, and producing the output file
- **`UI`** — handles console input/output and formatted printing of all lists

All objects are managed by pointer across lists (never duplicated) so that moving an order between stages just repoints it, following a strict share-don't-copy design.

## My Contributions

This was a 3-person team project. My primary responsibility was the **core simulation loop and order-completion logic**:

- Implemented the main simulation loop in the `Restaurant` class, coordinating order/chef/scooter/table state transitions at every timestep
- Implemented scooter lifecycle management (available → maintenance → back-to-restaurant transitions)
- Implemented order completion logic for both delivery (OV) and dine-in (OD) orders, including resource release (scooter/table) and statistics calculation (Ti, Tc, Tw, Tserv)
- Implemented the **COMBO Orders** bonus feature — orders requiring up to 4 chefs and 2+ scooters, with top assignment priority

My teammates contributed to the remaining core areas, including the custom data structure implementations, the order-to-chef/table/scooter assignment stages, and input/output file handling.

## Tech Stack

- **Language:** C++
- **Data Structures:** Custom Stack, Queue, and Priority Queue (no STL containers)

## How to Build & Run

**Windows (recommended):**

Open `RM Project.sln` in Visual Studio, build the solution (Ctrl+Shift+B), and run.

**Linux / macOS:**

```bash
cd "RM Project"
g++ -std=c++17 -o restaurant main.cpp Restaurant/Restaurant.cpp UI/Ui.cpp Action/CancelAction.cpp Action/RequestAction.cpp
./restaurant
```

> Note: the project was developed on Windows, whose filesystem is not case-sensitive. One header is referenced with different letter casing than its actual filename, which is harmless on Windows/Visual Studio but will cause a "file not found" error on case-sensitive filesystems (Linux/macOS). If you hit this, rename the offending `#include` (or the file) so the casing matches.

On startup, the program asks you to choose a mode:
- **Interactive mode** — prints the full system state at every timestep and waits for input to proceed
- **Silent mode** — runs the full simulation and only produces the output file

## Input / Output Format

The program reads a structured input file describing chefs, scooters, tables, and a chronological list of order requests/cancellations, and produces an output file listing every finished order (sorted by finish time) along with system-wide statistics (completion rate, cancellation rate, average wait times, scooter/chef utilization).

## Course Context

This project was completed as part of CMPG104 (Data Structures & Algorithms), Spring 2026, Faculty of Engineering, Cairo University. It was submitted in three graded phases: data structure design, data structure implementation & testing, and full simulation logic.
