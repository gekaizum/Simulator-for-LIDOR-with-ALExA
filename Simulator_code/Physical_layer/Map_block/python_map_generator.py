import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LightSource
from matplotlib import cm
from scipy.ndimage import gaussian_filter
import noise
import os
from PIL import Image, ImageEnhance

# ✅ Ensure output directory exists
os.makedirs("Sim_logs", exist_ok=True)

# 🗺️ Simulation grid setup
grid_size = 5000  # Size of the terrain: 5000x5000 meters (1 meter per cell)

# 🔧 Terrain generation settings for 3 layers
# Lower scale → more detail, higher scale → larger features

# Mountains: large features with long wavelengths
scale_mountain = 300.0    # Coarse features for mountains
# Hills: medium-scale terrain variation
scale_hill = 100.0        # Mid-size features for hills
# Fine details: small bumps and irregularities
scale_detail = 30.0       # Small-scale terrain detail

# 🌐 Noise parameters
octaves = 6               # Number of detail layers added together (higher = more complexity)
persistence = 0.5         # Controls how quickly amplitudes decrease for higher octaves
lacunarity = 2.0          # Controls frequency increase between octaves
seed = np.random.randint(0, 100)  # Random seed for terrain variety

# 🧠 Function to generate a Perlin noise layer
def generate_noise_layer(scale, amplitude=1.0, smoothing=0):
    """
    Generates a 2D Perlin noise terrain layer.
    - scale: Controls the frequency of noise (feature size)
    - amplitude: Scales the height contribution of this layer
    - smoothing: Gaussian blur applied to soften sharp transitions
    """
    layer = np.zeros((grid_size, grid_size))
    for y in range(grid_size):
        for x in range(grid_size):
            layer[y][x] = noise.pnoise2(x / scale,       # Normalize X coordinate
                                        y / scale,       # Normalize Y coordinate
                                        octaves=octaves,
                                        persistence=persistence,
                                        lacunarity=lacunarity,
                                        repeatx=1024,    # Wraparound values
                                        repeaty=1024,
                                        base=seed)
    if smoothing > 0:
        layer = gaussian_filter(layer, sigma=smoothing)
    return layer * amplitude

# 🌄 Generate base terrain layers
mountains = generate_noise_layer(scale=scale_mountain, amplitude=200, smoothing=3)  # Tall mountains
hills     = generate_noise_layer(scale=scale_hill, amplitude=70, smoothing=2)       # Rolling hills
details   = generate_noise_layer(scale=scale_detail, amplitude=30, smoothing=0.5)   # Surface texture

# 🧬 Combine all terrain features
terrain = mountains + hills + details

# 🧮 Normalize the entire terrain so the lowest point is 0 meters
terrain -= terrain.min()

# 📁 Save terrain height values to file (for simulation)
np.savetxt("../../Sim_logs/heightmap.txt", terrain, fmt="%d")  # Format to 3 decimal places

# ☀️ Simulate lighting and shading for realistic look
ls = LightSource(azdeg=315, altdeg=45)  # Light from northwest, 45° elevation

# 🖼️ Create RGB hillshaded image using earth-like colormap
rgb = ls.shade(terrain, cmap=cm.gist_earth, vert_exag=1.0, blend_mode='overlay')

# Convert numpy image to PIL Image
rgb_uint8 = (rgb * 255).astype(np.uint8)
pil_img = Image.fromarray(rgb_uint8)

# Adjust contrast
enhancer = ImageEnhance.Contrast(pil_img)
pil_img = enhancer.enhance(0.3)  # 1.0 = original, >1 = higher contrast

# Convert back to numpy if needed
rgb = np.array(pil_img) / 255.0

# 💾 Save photorealistic terrain image
plt.figure(figsize=(20, 20))
plt.imshow(rgb, interpolation='bilinear')  # Smooth rendering
#plt.title("Realistic Hill-Mountain Terrain", fontsize=20)
plt.axis('off')  # Hide axis
plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
plt.savefig("../../images/photorealistic_heightmap.png", dpi=300, bbox_inches='tight', pad_inches=0)
#plt.show()
