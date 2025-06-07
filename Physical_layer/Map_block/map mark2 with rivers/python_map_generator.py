import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LightSource
from matplotlib import cm
from scipy.ndimage import gaussian_filter
import noise
import os
import random

# Create output folder
os.makedirs("Sim_logs", exist_ok=True)

# Terrain size and parameters
grid_size = 5000
scale_mountain = 300.0
scale_hill = 100.0
scale_detail = 30.0
octaves = 6
persistence = 0.5
lacunarity = 2.0
seed = random.randint(0, 100)

# River and lake probability
river_probability = 0.6
lake_probability = 0.5
lake_count = random.randint(3, 7)

def generate_noise_layer(scale, amplitude=1.0, smoothing=0):
    layer = np.zeros((grid_size, grid_size))
    for y in range(grid_size):
        for x in range(grid_size):
            layer[y][x] = noise.pnoise2(x / scale,
                                        y / scale,
                                        octaves=octaves,
                                        persistence=persistence,
                                        lacunarity=lacunarity,
                                        repeatx=1024,
                                        repeaty=1024,
                                        base=seed)
    if smoothing > 0:
        layer = gaussian_filter(layer, sigma=smoothing)
    return layer * amplitude

# Generate terrain
mountains = generate_noise_layer(scale=scale_mountain, amplitude=200, smoothing=3)
hills     = generate_noise_layer(scale=scale_hill, amplitude=70, smoothing=2)
details   = generate_noise_layer(scale=scale_detail, amplitude=30, smoothing=0.5)
terrain   = mountains + hills + details
terrain  -= terrain.min()

# Add lakes (random low basins filled with water)
if random.random() < lake_probability:
    for _ in range(lake_count):
        lx = random.randint(200, grid_size - 200)
        ly = random.randint(200, grid_size - 200)
        radius = random.randint(100, 300)
        lake_level = np.percentile(terrain, random.randint(5, 20))  # Fill shallow basins
        for y in range(ly - radius, ly + radius):
            for x in range(lx - radius, lx + radius):
                if 0 <= x < grid_size and 0 <= y < grid_size:
                    dx, dy = x - lx, y - ly
                    dist = np.sqrt(dx * dx + dy * dy)
                    if dist < radius:
                        terrain[y, x] = min(terrain[y, x], lake_level)

# Add rivers (paths that carve downhill from peaks)
if random.random() < river_probability:
    for _ in range(random.randint(5, 10)):
        # Start from high point
        y, x = np.unravel_index(np.argmax(terrain + np.random.rand(*terrain.shape) * 20), terrain.shape)
        path_length = random.randint(1000, 3000)
        for _ in range(path_length):
            current_height = terrain[y, x]
            terrain[y, x] = current_height * 0.95  # Slightly lower to carve river
            # Move to steepest neighbor
            neighbors = [(i, j) for i in range(y-1, y+2) for j in range(x-1, x+2)
                         if 0 <= i < grid_size and 0 <= j < grid_size and (i != y or j != x)]
            if not neighbors:
                break
            y, x = min(neighbors, key=lambda p: terrain[p[0], p[1]])
            if terrain[y, x] > current_height:
                break  # stop if going uphill

# Normalize again after modification
terrain -= terrain.min()

# Save heightmap
np.savetxt("Sim_logs/heightmap.txt", terrain, fmt="%d")

# Create photorealistic hillshaded image
ls = LightSource(azdeg=315, altdeg=45)
rgb = ls.shade(terrain, cmap=cm.gist_earth, vert_exag=1.0, blend_mode='overlay')

# Save the final image
plt.figure(figsize=(20, 20))
plt.imshow(rgb, interpolation='bilinear')
plt.title("Terrain with Hills, Mountains, Rivers, Lakes", fontsize=20)
plt.axis('off')
plt.savefig("photorealistic_heightmap.png", dpi=300, bbox_inches='tight')
plt.show()

