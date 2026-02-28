
---

### README.md Content

```markdown
# P2P Mesh Chat 🚀

A decentralized, Peer-to-Peer (P2P) chat application built with **C++** and **Qt6**. This project implements a serverless mesh network where users can discover each other automatically on a local network and communicate through private or global channels.



## 🌟 Features
* **Automatic Peer Discovery:** Uses UDP Broadcasting (Port 45454) to find other users on the network without a central server.
* **Private Messaging:** Establishes direct TCP connections (Port 12345) for secure, one-to-one communication.
* **Global Group Chat:** Seamlessly broadcast messages to every peer in the mesh network using UDP.
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

```

## 🚀 Getting Started

### 1. Clone & Build

```bash
git clone [https://github.com/rajakerkar/Peer-Chat.git](https://github.com/rajakerkar/Peer-Chat.git)
cd Peer-Chat
mkdir build && cd build
cmake ..
make

```

### 2. Configure Firewall

To allow P2P traffic, ensure your firewall permits the following ports:

```bash
sudo ufw allow 45454/udp
sudo ufw allow 12345/tcp

```

### 3. Run

```bash
./P2PChat

```

## 🧠 How it Works

1. **Discovery:** Every 3 seconds, the app sends a UDP packet containing your username to the broadcast address `255.255.255.255`.
2. **Tracking:** Peers maintain a `lastSeen` map. If a heartbeat isn't received from an IP for 10 seconds, the peer is marked as offline.
3. **Chatting:** * **Private:** Selecting a user initiates a `QTcpSocket` connection to their specific IP.
* **Global:** Selecting `GLOBAL_NET` wraps the message in a UDP packet with a `GRP:` prefix for network-wide distribution.



## 🤝 Contributing

Feel free to fork this project, report issues, or submit pull requests to improve the mesh logic or UI!

```

---

### Commands to Push the Code
Once you have created the `README.md` file in your project folder, run these commands to push everything to GitHub:

```bash
# Navigate to project root
cd ~/P2PChat

# Initialize and add files
git init
git add src/ include/ CMakeLists.txt README.md
# We avoid adding the 'build' folder to keep the repo clean
echo "build/" >> .gitignore
git add .gitignore

# Commit and Push
git commit -m "Initial commit: P2P Mesh Chat with Hacker Theme and Presence Logic"
git branch -M main
git remote add origin https://github.com/rajakerkar/Peer-Chat.git
git push -u origin main

```

