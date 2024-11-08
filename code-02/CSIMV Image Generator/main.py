#!/usr/bin/env python3

from PIL import Image
import numpy as np
import warnings

# Disable 'scalar divide by 0' and other runtime warnings
warnings.filterwarnings("ignore", category=RuntimeWarning)

# Extracting pixels from image
# ============================

# Open the images and convert to array
image1 = Image.open("img1.jpeg")
image2 = Image.open("img2.jpeg")
image_arr = np.array(image1)
np.append(image_arr, image2)

pixels = []
pairs = []

# Loop over each pixel in the image
for y in range(image_arr.shape[0]):	# X axis
	for x in range(image_arr.shape[1]):	# Y axis
		pixel = image_arr[y, x]
		pixels.append(pixel)

# Convert the list of pixels to a NumPy array
pixels = np.array(pixels)

for i in range(0, len(pixels) - 1, 2):
	pair = np.array([pixels[i], pixels[i + 1]], dtype=np.uint8)	# make sure datatypes are handled consistently
	pairs.append(pair)

# Learning pixels
# ===============
#			    a⋅b
# sim(a, b) = -------
#			  |a|*|b|

# a⋅b = Dot product of vector a and b
# |a| = Euclidean norm of vector a
# |b| = Euclidean norm of vector b

#	  [ sim(ab) ]
#	  [    1    ]
# v = [    1    ]
#	  [   ...   ]
#	  [    1    ]

pixels = []

for index, pair in enumerate(pairs):
	a = pair[0]
	b = pair[1]
	if index < len(pairs) - 1:
		c = pairs[index+1][0]
		d = pairs[index+1][1]
	else:
		c = pairs[-1][0]
		d = pairs[-1][1]

	# Normalize:
	norm_a = np.sqrt(np.sum(a**2))
	norm_b = np.sqrt(np.sum(a**2))
	norm_c = np.sqrt(np.sum(c**2))
	norm_d = np.sqrt(np.sum(d**2))

	# Find cosine similarities:
	sim_ab = (np.dot(a, b))/(norm_a*norm_b)
	sim_cd = (np.dot(c, d))/(norm_c*norm_d)

	# Linear vector representation:
	vector_ab = np.ones(2)
	vector_ab[0] = sim_ab
	vector_cd = np.ones(2)
	vector_cd[0] = sim_cd

	# M = ((x₁ + x₂) / 2, (y₁ + y₂) / 2)
	midpoint = [(vector_ab[0]**2 + vector_ab[1]**2) / 2, (vector_cd[0]**2 + vector_cd[1]**2) / 2]
	pixels.append(midpoint)

# Calculate vector properties
# Magnitude: ||v|| = √(v1² + v2² + ... + vn²)
# Direction: θ = arctan(y/x)
for p in pixels:
	magnitude = np.sqrt(p[0]**2 + p[1]**2)
	direction = np.arctan(p[1] / p[0])
	p.append(magnitude)
	p.append(255)

# Clip values (to 255)
for pixel in pixels:
	for i in range(4):
		pixel[i] = 255 if pixel[i] > 255 else pixel[i]

# Testing
# =======
# a = pairs[0][0]
# b = pairs[0][1]
# norm_a = np.sqrt(np.sum(a**2))
# norm_b = np.sqrt(np.sum(a**2))

# # Repeat to finish final coordinate pair (for later calculation)
# c = pairs[1][0]
# d = pairs[1][1]
# norm_c = np.sqrt(np.sum(c**2))
# norm_d = np.sqrt(np.sum(d**2))

# sim_ab = (np.dot(a, b))/(norm_a*norm_b)
# sim_cd = (np.dot(c, d))/(norm_c*norm_d)
# print(f"Cosine similarities for A and B: {sim_ab}")
# print(f"Cosine similarities for C and D: {sim_cd}")

#	  [ sim(ab) ]
#	  [    1    ]
# v = [    1    ]
#	  [   ...   ]
#	  [    1    ]

# vector_ab = np.ones(2)
# vector_ab[0] = sim_ab
# vector_cd = np.ones(2)
# vector_cd[0] = sim_cd
# print(f"Linear vector A, B: {vector_ab}")
# print(f"Linear vector C, D: {vector_cd}")

# midpoint = [(vector_ab[0]**2 + vector_ab[1]**2) / 2, (vector_cd[0]**2 + vector_cd[1]**2) / 2]
# print(midpoint)

# Statistics Test
# ===============
# The geometric mean gives us an insight on the basic simple values of each pixel
# The range gives us an idea of the spread of data; larger is better

# CALCULATE GEOMETRIC MEAN
r, g, b, a = [], [], [], []

# Sort values
for index, pixel in enumerate(pixels):
	r.append(pixel[0])
	g.append(pixel[1])
	b.append(pixel[2])
	a.append(pixel[3])
	if index == 1000:
		break

# Calculate
average_r = sum(r)**(1 / len(r))
average_g = sum(g)**(1 / len(g))
average_b = sum(b)**(1 / len(b))
average_a = sum(a)**(1 / len(a))

# CALCULATE RANGE
range_r = max(r) - min(r)
range_g = max(g) - min(g)
range_b = max(b) - min(b)
range_a = max(a) - min(a)

# PRINT FINAL RESULTS
print(f"Geometric means:\n================\n{average_r}\n{average_g}\n{average_b}\n{average_a}")
print(f"\nRanges\n======\n{range_r}\n{range_g}\n{range_b}\n{range_a}")

# Image reconstruction
# ====================

# Convert to NumPy array and reshape
pixels = np.array(pixels, dtype=np.uint8)
pixels = pixels.reshape((137, 184, 4))

# Create image from array
image = Image.fromarray(pixels, mode='RGBA')

# Save image
image.save('Outputs/output.png')

# Display image
image.show()





