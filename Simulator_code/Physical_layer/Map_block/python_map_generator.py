import numpy as np
from scipy.ndimage import gaussian_filter
import matplotlib.pyplot as plt

# Map size
grid_size = 5000  # 5000x5000 meters
resolution = 1    # Resolution of 1x1 meter

# Generate terrain heights
np.random.seed(42)  # For reproducibility
terrain = np.random.rand(grid_size, grid_size) * 300  # Heights from 0 to 100 meters
terrain = gaussian_filter(terrain, sigma=50)  # Smoothing for a natural appearance

# Convert heights to integers in centimeters
terrain_cm = np.round(terrain * 100).astype(int)  # Convert meters to centimeters

# Save to file
np.savetxt("heightmap_cm.txt", terrain_cm, fmt="%d")  # Save as integers

# Visualize the heightmap in high resolution
plt.figure(figsize=(20, 20))  # Adjust the figure size for high resolution
plt.imshow(terrain_cm, cmap="terrain")
plt.colorbar(label="Height (cm)")
plt.title("Heightmap (in centimeters)", fontsize=20)
plt.axis('off')  # Optional: turn off axes for a cleaner look

# Save the image in high resolution
plt.savefig("heightmap_high_res_cm.png", dpi=300, bbox_inches='tight')  # Save with high DPI
plt.show()
