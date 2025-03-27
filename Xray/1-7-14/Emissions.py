import matplotlib.pyplot as plt
import sys
import os



Transm_dir = os.path.abspath("../../Transmission/Transmission")
sys.path.append(Transm_dir)
import Transmissions
def read_data(file_path):
    """Reads the data from a given file."""
    wavelengths = []
    fluorescence = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        start_reading = False

        for line in lines:
            line = line.strip()

            # Start reading data after the "Data Points" header
            if line.startswith("Data Points"):
                start_reading = True
                continue

            if start_reading:
                parts = line.split()
                if len(parts) == 2:
                    try:
                        wavelength = float(parts[0])
                        intensity = float(parts[1])
                        wavelengths.append(wavelength)
                        fluorescence.append(intensity)
                    except ValueError:
                        continue  # Ignore lines that can't be converted to float
    return wavelengths, fluorescence

def process_fluorescence(wavelengths1, fluorescence1, wavelengths2, fluorescence2, wavelengths3, fluorescence3, calib):
    """Process the fluorescence data based on given rules and normalize it."""
    adjusted_fluorescence = []
    for i in range(len(wavelengths1)):
        if wavelengths1[i] <= 700:
            adjusted_value = (fluorescence1[i] - fluorescence2[i]) / calib[i]
        else:
            adjusted_value = (fluorescence3[i] - fluorescence2[i]) / calib[i]
        adjusted_fluorescence.append(adjusted_value)
    
    # Normalize the fluorescence values
    max_value = max(adjusted_fluorescence)
    normalized_fluorescence = [val*100/ max_value for val in adjusted_fluorescence]
    
    return normalized_fluorescence

def plot_data(ax, wavelengths, normalized_fluorescence, label, ylabel):
    """Plots the fluorescence data and annotates the peak."""
    ax.plot(wavelengths, normalized_fluorescence, label=label, color='b')
    #ax.plot(lam, Tper_L[i-1], label='Longitudinal(16cm)')
    #ax.plot(lam, Tper_T[i-1], label='Transverse(2.2cm)')

    # Find the peak value and its position
    peak_value = max(normalized_fluorescence)
    peak_wavelength = wavelengths[normalized_fluorescence.index(peak_value)]

    # Annotate peak with a larger font size
    ax.annotate(f"Peak: {peak_value:.2f} at {peak_wavelength} nm", 
                xy=(peak_wavelength, peak_value), 
                xytext=(peak_wavelength+20, peak_value+0.1),
                arrowprops=dict(facecolor='black', arrowstyle="->"),
                fontsize=14)  # Increased font size for peak annotation

    ax.grid(True)
    ax.legend()
    ax.set_ylabel(ylabel)
    
    # Add label at the top left for each plot
    ax.text(0.01, 0.98, f'{label} 22x22x160. mm$^3$', transform=ax.transAxes, fontsize=12, verticalalignment='top', horizontalalignment='left', color='black')

lam = Transmissions.lam
Tper_L= Transmissions.Tper_L
Tper_T = Transmissions.Tper_T

# File paths for the three sets of files (corresponding to BGO 1, 7, and 14)
file_paths = {
    1: {'file1': 'b-bgo-1_xel_012925_1.TXT', 'file2': 'b-bgo-1_xel_012925_1_bg.TXT', 'file3': 'b-bgo-1_xel_012925_1_hp.TXT'},
    7: {'file1': 'b-bgo-7_xel_020425_1.TXT', 'file2': 'b-bgo-7_xel_020425_1_bg.TXT', 'file3': 'b-bgo-7_xel_020425_1_hp.TXT'},
    14: {'file1': 'b-bgo-14_xel_020425_1.TXT', 'file2': 'b-bgo-14_xel_020425_1_bg.TXT', 'file3': 'b-bgo-14_xel_020425_1_hp.TXT'}
}

# Example calibration data
calib = [1.0 for _ in range(300)]  # Replace with actual calibration values

# Read and process the data for each set (BGO 1, 7, 14)
data = {}
for key in file_paths:
    wavelengths1, fluorescence1 = read_data(file_paths[key]['file1'])
    wavelengths2, fluorescence2 = read_data(file_paths[key]['file2'])
    wavelengths3, fluorescence3 = read_data(file_paths[key]['file3'])
    
    normalized_fluorescence = process_fluorescence(wavelengths1, fluorescence1, wavelengths2, fluorescence2, wavelengths3, fluorescence3, calib)
    data[key] = {
        'wavelengths': wavelengths1,
        'fluorescence': normalized_fluorescence,
        'label': f'BGO {key}'
    }

# Create a single figure with three connected subplots sharing the x-axis
# fig, axes = plt.subplots(3, 1, figsize=(10, 12), sharex=True, gridspec_kw={'hspace': 0})

# # Plot data for each set (BGO 1, 7, 14)
# ylabels = ['XEL', 'XEL', 'XEL']
# for ax, key, ylabel in zip(axes, [1, 7, 14], ylabels):
#     plot_data(ax, data[key]['wavelengths'], data[key]['fluorescence'], data[key]['label'], ylabel)

# # Set x-axis label only for the last subplot
# axes[-1].set_xlabel('Wavelength (nm)')

# plt.tight_layout()
# plt.savefig("XEL_1-7-14" + ".pdf", format="pdf")
# plt.show()
print(lam)