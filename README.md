## System Overview
The Mustang Mobility system is an accessible navigation platform designed for Western University's campus. It helps users find optimal paths between buildings while considering accessibility requirements such as ramp access and building hours. The system features:
* Interactive 3D campus map
* Real-time accessibility filtering
* Multi-stop route planning
* Building information and entrance details
* Visual path display with distance and duration estimates

## Dependencies
### Backend
* C++17 or higher
* CMake 3.10 or higher
* Standard Template Library (STL)

### Frontend
* Modern web browser with JavaScript enabled
* Mapbox GL JS v2.15.0
* PapaParse v5.3.2 (CSV parsing)
* Custom fonts:
  - Open Sauce
  - Sugo Display
  - These fonts are available on frontend/fonts/

## Compilation Instructions

1. Clone or download this repository

2. Start the python server:
```bash
cd backend/src
python3 simple_server.py
```

3. Access the frontend:
   - Open a web browser
   - Navigate to `http://localhost:8000/frontend/src/pathfinding.html`