# P2P Mesh Chat 🚀

A decentralized, Peer-to-Peer (P2P) chat application built with **C++** and **Qt6**. This project implements a serverless mesh network where users can discover each other automatically on a local network and communicate through private or global channels.

!

## 🌟 Features
* **Automatic Peer Discovery:** Uses UDP Broadcasting (Port 45454) to find other users on the network without a central server.
* **Private Messaging:** Establishes direct TCP connections (Port 12345) for secure, one-to-one communication.
* **Global Group Chat:** Seamlessly broadcast messages to every peer in the mesh network.
* **Real-time Presence:** Automatically detects when users join or leave the network using a "Heartbeat" timeout system.
* **Hacker-Terminal UI:** A custom-themed "Dark Mode" interface with neon green aesthetics for a professional terminal look.

## 🛠 Tech Stack
* **Language:** C++17
* **Framework:** Qt6 (Network, Widgets)
* **Build System:** CMake
* **Networking:** Berkeley Sockets (TCP/UDP)

## 📋 Requirements
Before building, ensure you have the following installed (Tested on Ubuntu/Kali Linux):
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libxkbcommon-dev