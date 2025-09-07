import pandas as pd
import matplotlib.pyplot as plt
from itertools import product
from scipy.interpolate import make_interp_spline
import numpy as np

# Function to plot smooth line graphs
def plot_duration_vs_ops(df, num_threads, autonuma):
    df_filtered = df[(df['num_threads'] == num_threads) & (df['duration'] <= 800)]
    plt.figure(figsize=(6, 6))

    linestyles = {
        ('numa', 'numa'): 'solid',
        ('numa', 'regular'): 'dashed',
        ('regular', 'numa'): 'solid',
        ('regular', 'regular'): 'dashed',
        ('reverse', 'numa'): 'solid',
        ('reverse', 'regular'): 'dashed'
    }

    for th, ds in configs:
        label = f"{th} th. / {ds} ds."
        subset = df_filtered[
            (df_filtered['th_config'] == th) &
            (df_filtered['DS_config'] == ds)
        ].sort_values(by='duration')

        if th == 'numa':
            base_color = 'blue'
        elif th == 'regular':
            base_color = 'red'
        else:
            base_color = 'gray'

        linestyle = linestyles.get((th, ds), 'solid')

        if not subset.empty and len(subset) > 3:
            x = subset['duration'].values
            y = subset['Total Ops'].values / 1e9  # Scale to billions

            x_new = np.linspace(x.min(), x.max(), 300)
            spline = make_interp_spline(x, y, k=3)
            y_smooth = spline(x_new)

            plt.plot(x_new, y_smooth, label=label, color=base_color, linestyle=linestyle, linewidth=2)
        elif not subset.empty:
            plt.plot(subset['duration'], subset['Total Ops'] / 1e9,
                     label=label, marker='o', color=base_color,
                     linestyle=linestyle, linewidth=2)

    # Styling
    plt.xlabel("Duration (s)", fontsize=14)
    plt.ylabel("Total Operations (Billions)", fontsize=14)
    plt.tick_params(axis='both', labelsize=12)
    plt.legend(loc="lower right",fontsize=14)
    plt.grid(True)
    plt.tight_layout()

    # Save plots
    suffix = "_AN" if autonuma == 1 else ""
    path_prefix = "./Throughput/AutoNuma" if autonuma == 1 else "./Throughput/NoAutoNuma"

    png_path = f"{path_prefix}/TotalOps_vs_Duration_{num_threads}threads{suffix}2.png"
    pdf_path = f"{path_prefix}/TotalOps_vs_Duration_{num_threads}threads{suffix}2.pdf"

    plt.savefig(png_path, dpi=300)
    plt.savefig(pdf_path, dpi=300)
    plt.show()

# Main block
if __name__ == "__main__":
    with open("/proc/sys/kernel/numa_balancing") as f:
        autonuma = int(f.read().strip())
    print("Plotting with autonuma =", autonuma)

    csv_file = "../BST_Transactions_AN_2.csv" if autonuma == 1 else "../BST_Transactions2.csv"
    df = pd.read_csv(csv_file)
    print("Input file loaded:", csv_file)

    df.columns = [col.strip() for col in df.columns]
    df.rename(columns={'thread_config': 'th_config', 'TotalOps': 'Total Ops'}, inplace=True)

    df['th_config'] = df['th_config'].astype(str).str.strip().str.lower()
    df['DS_config'] = df['DS_config'].astype(str).str.strip().str.lower()

    th_configs = ['numa', 'regular', 'reverse']
    ds_configs = ['numa', 'regular']
    configs = list(product(th_configs, ds_configs))

    plot_duration_vs_ops(df, 40, autonuma)
    plot_duration_vs_ops(df, 80, autonuma)