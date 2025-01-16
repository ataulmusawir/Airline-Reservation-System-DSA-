# GIKI Airline Reservation System

This repository contains the implementation of a **GIKI Airline Reservation System** with various data structures and algorithms to manage and simulate flight reservations, routes, and passengers.

## Data Structures Used

### 1. Linked List
- **FlightType**: A linked list is used to represent each flight. Each `FlightType` node contains details about the flight, including flight number, departure time, arrival time, and linked pointers to the next departure and arrival flights.
- **CityListType**: A linked list is also used to maintain a list of cities. Each node stores the city name and pointers to the first flight departing from and arriving at that city.

### 2. Hash Table
- **Hash Function**: A hash table is employed to efficiently index cities. Each city is mapped to an index based on its name, allowing for quick access to the list of flights departing from or arriving at that city. This helps minimize search time when adding or retrieving flights related to a specific city.

### 3. Queue
- **FindRoute**: A queue is used for breadth-first search to find routes between two cities. It explores all possible flights departing from a city and checks if the destination city can be reached with valid layovers.

### 4. Priority Queue
- **ShowShortestPath**: A priority queue is utilized in Dijkstra’s algorithm to compute the shortest path between two cities based on flight times. This algorithm ensures the fastest travel time by prioritizing the flight with the shortest arrival time.

### 5. Set
- **Visited Cities**: A set is used to track visited cities during the shortest path search to avoid revisiting the same city and ensure the algorithm's efficiency.

## Algorithms Highlighted

### 1. Dijkstra’s Algorithm (for Shortest Path Calculation)
- **Purpose**: Used in the `ShowShortestPath` function to find the shortest path between two cities based on flight times. The priority queue ensures that the city with the least total travel time is processed first, and the algorithm continues to expand the shortest path.

### 2. Breadth-First Search (BFS) (for Finding Routes)
- **Purpose**: The `FindRoute` function uses BFS to explore all available flights from a starting city, ensuring that layovers are respected and multiple routes are found between two cities.

### 3. Hashing
- **Purpose**: The `Hash` function maps city names to a fixed index, ensuring constant-time lookups for city-based flight management. The hash table helps in efficiently storing and accessing the list of flights for each city.

## Key Features

- **Flight Departure and Arrival Management**: Manage and display flights departing from and arriving at any city.
- **Route Calculation**: Find all possible routes between two cities, considering layovers and flight schedules.
- **Shortest Path Finder**: Calculate the shortest travel time between cities using flight data with Dijkstra’s algorithm.
- **Reservation System**: Allows passengers to make reservations for flights, view their schedules, and delete reservations if needed.
