#python3 fadingGraphScript.py

import re
import matplotlib.pyplot as plt

# Путь к лог-файлу
log_file_path = "../Sim_logs/FadingModelLogFile.log"
target_id = 1  # Укажи нужный Drone ID

with open(log_file_path, "r") as file:
    log_data = file.read()

lines = log_data.strip().split('\n')
filtered_data = []
current_id = None

for line in lines:
    id_match = re.search(r'ID\s*=\s*(\d+)', line)
    if id_match:
        current_id = int(id_match.group(1))
    loss_match = re.search(r'loss value:\s*([0-9.eE+-]+), distance:\s*([0-9.]+)', line)
    if loss_match and current_id == target_id:
        loss = float(loss_match.group(1))
        distance = float(loss_match.group(2))
        if loss <= 1.0:
            filtered_data.append((distance, loss))

distances = [d for d, l in filtered_data]
losses = [l for d, l in filtered_data]

plt.figure(figsize=(8, 5))
plt.plot(distances, losses, marker='o', linestyle='-', label=f"Drone ID = {target_id}")
plt.xlabel("Distance (m)")
plt.ylabel("Loss value")
plt.title("Loss vs Distance")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
