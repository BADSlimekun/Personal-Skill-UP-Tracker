import numpy as np
import random
import tkinter as tk

GRID_SIZE = 100
TIME_PER_CELL_S = 10

STAGE_WIDTH = 50
EXIT_WIDTH  = 50
stage_cols = (GRID_SIZE - STAGE_WIDTH)//2, (GRID_SIZE + STAGE_WIDTH)//2
exit_cols  = (GRID_SIZE - EXIT_WIDTH)//2,  (GRID_SIZE + EXIT_WIDTH)//2

HELIPAD_CENTER = (30, 50)
HELIPAD_H = 20
HELIPAD_W = 20

PITCH_CENTER = (60, 50)
PITCH_H = 6
PITCH_W = 20

P_GREEN = 0.65
P_GREY  = 0.30

CELL_PX = 9
NUMBER_EVERY = 1

RNG_SEED = random.randint(1,99)
# RNG_SEED = 40

rng = np.random.default_rng(RNG_SEED)

def region_masks(n=GRID_SIZE):
    grid = np.zeros((n, n), dtype=bool)
    def place_rect(center, h, w):
        r0 = max(0, int(center[0] - h//2))
        r1 = min(n, int(center[0] + (h - h//2)))
        c0 = max(0, int(center[1] - w//2))
        c1 = min(n, int(center[1] + (w - w//2)))
        grid[r0:r1, c0:c1] = True
    place_rect(HELIPAD_CENTER, HELIPAD_H, HELIPAD_W)
    place_rect(PITCH_CENTER,   PITCH_H,   PITCH_W)
    return grid

def build_medium(n=GRID_SIZE, p_green=P_GREEN, p_grey=P_GREY):
    grey = region_masks(n)
    open_green = rng.random((n, n)) < p_green
    open_grey  = rng.random((n, n)) < p_grey
    medium = np.where(grey, open_grey, open_green)
    return medium, grey

def burning_time_and_path(medium, stage_cols, exit_cols):
    n = medium.shape[0]
    c0, c1 = stage_cols
    d0, d1 = exit_cols

    starts = [(0, c) for c in range(c0, c1) if medium[0, c]]
    if not starts:
        return None, []

    dist = np.full((n, n), -1, dtype=int)
    parent_r = np.full((n, n), -1, dtype=int)
    parent_c = np.full((n, n), -1, dtype=int)

    from collections import deque
    q = deque()
    for r, c in starts:
        dist[r, c] = 0
        q.append((r, c))

    moves = [(-1,0),(1,0),(0,-1),(0,1)]
    exit_cell = None
    while q:
        r, c = q.popleft()
        if r == n-1 and d0 <= c < d1:
            exit_cell = (r, c)
            break
        for dr, dc in moves:
            nr, nc = r + dr, c + dc
            if 0 <= nr < n and 0 <= nc < n and medium[nr, nc] and dist[nr, nc] == -1:
                dist[nr, nc] = dist[r, c] + 1
                parent_r[nr, nc] = r
                parent_c[nr, nc] = c
                q.append((nr, nc))

    if exit_cell is None:
        return None, []

    path = []
    r, c = exit_cell
    while r != -1 and c != -1:
        path.append((r, c))
        pr, pc = parent_r[r, c], parent_c[r, c]
        r, c = pr, pc
    path.reverse()
    return dist[exit_cell], path

def run_trials(num_trials=300):
    times = []
    for _ in range(num_trials):
        medium, _ = build_medium()
        steps, _ = burning_time_and_path(medium, stage_cols, exit_cols)
        if steps is not None:
            times.append(steps * TIME_PER_CELL_S)
    return np.array(times)


def draw_scene_tk(medium, path, stage_cols, exit_cols, cell_px=8, number_every=1):
    n = medium.shape[0]
    width_px = n * cell_px
    height_px = n * cell_px
    root = tk.Tk()
    root.title("Burning Method - Tkinter")
    canvas = tk.Canvas(root, width=width_px, height=height_px, bg="white", highlightthickness=0)
    canvas.pack(fill="both", expand=True)
    color_open = "#c6efc6"
    color_blocked = "#5f6b6b"
    color_path = "#0033cc"
    color_stage_exit = "#004d40"
    for row in range(n):
        for col in range(n):
            x0 = col * cell_px
            y0 = row * cell_px
            x1 = x0 + cell_px
            y1 = y0 + cell_px
            fill = color_open if medium[row, col] else color_blocked
            canvas.create_rectangle(x0, y0, x1, y1, outline="", fill=fill)
    c0, c1 = stage_cols
    d0, d1 = exit_cols
    canvas.create_rectangle(c0*cell_px, 0, (c1)*cell_px, 2, outline="", fill=color_stage_exit)
    canvas.create_rectangle(d0*cell_px, (n-1)*cell_px-2, (d1)*cell_px, (n-1)*cell_px, outline="", fill=color_stage_exit)
    if path:
        coords = []
        for r, c in path:
            x = c * cell_px + cell_px/2
            y = r * cell_px + cell_px/2
            coords.extend([x, y])
        canvas.create_line(*coords, width=max(2, cell_px//4), fill=color_path)
        for idx, (r, c) in enumerate(path):
            if NUMBER_EVERY > 0 and (idx % NUMBER_EVERY == 0):
                x = c * cell_px + cell_px/2
                y = r * cell_px + cell_px/2
                canvas.create_text(x, y, text=str(idx), fill="black", font=("Arial", max(6, cell_px//2)))
    root.mainloop()

def main():
    medium, grey = build_medium()
    steps, path = burning_time_and_path(medium, stage_cols, exit_cols)
    if steps is None:
        print("No path found in this sample.")
    else:
        print(f"Shortest path: {steps} steps (~{steps*TIME_PER_CELL_S} s)")
    draw_scene_tk(medium, path, stage_cols, exit_cols, cell_px=CELL_PX, number_every=NUMBER_EVERY)


    times = run_trials(100)
    if times.size:
        print(f"Success rate: {100.0*times.size/100.0:.1f}%")
        print(f"Average evacuation time (successful only): {times.mean():.1f} s")
    else:
        print("No successful burns in the trials.")

if __name__ == "__main__":
    main()
