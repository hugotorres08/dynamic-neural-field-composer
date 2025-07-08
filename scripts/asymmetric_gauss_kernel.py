import numpy as np
import matplotlib.pyplot as plt

# Parameters
A = 5.0  # Amplitude
sigma = 2.0  # Width
eta = 1.4  # Asymmetry factor
x = np.linspace(-10, 10, 1000)  # Range

# Compute Gaussian
G = A * np.exp(-x**2 / (2 * sigma**2))

# Compute Gaussian Derivative
G_derivative = -x / sigma**2 * A * np.exp(-x**2 / (2 * sigma**2))

# Combine to create asymmetric kernel
K = G + eta * G_derivative

# Plot
plt.figure(figsize=(8, 5))
plt.plot(x, G, label='Symmetric Gaussian', linestyle='--', color='blue')
plt.plot(x, G_derivative, label='Gaussian Derivative', linestyle=':', color='green')
plt.plot(x, K, label='Asymmetric Gaussian Kernel', color='red')
plt.axhline(0, color='black', linewidth=0.5, linestyle='--')
plt.legend()
plt.title('Asymmetric Gaussian Kernel')
plt.xlabel('x')
plt.ylabel('Kernel Value')
plt.grid(True)
plt.show()
