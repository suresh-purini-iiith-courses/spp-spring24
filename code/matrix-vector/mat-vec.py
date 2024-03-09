import numpy as np
import time

# Define matrix size
matrix_size = 1000

# Generate random matrix and vector
matrix = np.random.rand(matrix_size, matrix_size)
vector = np.random.rand(matrix_size)

# Perform matrix-vector multiplication
start_time = time.time()
result = np.dot(matrix, vector)
end_time = time.time()

# Calculate elapsed time
elapsed_time = end_time - start_time

# Calculate total floating point operations
flops = 2 * matrix_size * matrix_size

# Calculate GFLOPS
gflops = flops / (elapsed_time * 1e9)

print("Time taken:", elapsed_time, "seconds")
print("GFLOPS:", gflops)

