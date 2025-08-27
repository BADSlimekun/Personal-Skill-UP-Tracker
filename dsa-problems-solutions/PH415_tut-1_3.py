import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

N = 100
TRIALS_PER_P = 100
p_values = np.round(np.arange(0.40, 0.70 + 1e-9, 0.02), 2)

def has_top_bottom_wrap(medium: np.ndarray) -> bool:
    n = medium.shape[0]
    from collections import deque
    q = deque()
    dist = np.full(medium.shape, -1, dtype=np.int32)
    for c in range(n):
        if medium[0, c]:
            q.append((0, c))
            dist[0, c] = 0
    if not q:
        return False
    deltas = [(-1,0),(1,0),(0,-1),(0,1)]
    while q:
        r, c = q.popleft()
        if r == n - 1:
            return True
        for dr, dc in deltas:
            nr = r + dr
            nc = c + dc
            if nr < 0 or nr >= n or nc < 0 or nc >= n:
                continue
            if not medium[nr, nc]:
                continue
            if dist[nr, nc] != -1:
                continue
            dist[nr, nc] = dist[r, c] + 1
            q.append((nr, nc))
    return False

BASE_SEED = 77
ss = np.random.SeedSequence(BASE_SEED)
rngs = [np.random.default_rng(s) for s in ss.spawn(len(p_values))]

rows = []
for idx, p in enumerate(p_values):
    rng = rngs[idx]
    success = 0
    for t in range(TRIALS_PER_P):
        medium = rng.random((N, N)) < p
        if has_top_bottom_wrap(medium):
            success += 1
    wrap_prob = success / TRIALS_PER_P
    rows.append({"p": float(p), "wrapping_probability": wrap_prob, "successes": success, "trials": TRIALS_PER_P})

df = pd.DataFrame(rows)

fig = plt.figure(figsize=(7.5, 5.0), dpi=140)
ax = plt.gca()
ax.plot(df["p"].values, df["wrapping_probability"].values, marker="o")
ax.set_xlabel("Site open probability p")
ax.set_ylabel("Wrapping probability (top-to-bottom)")
ax.set_title("Percolation wrapping probability vs p (100×100 grid, 200 trials each)")
ax.grid(True, which="both", linestyle="--", linewidth=0.5)
fig.tight_layout()
plt.show()