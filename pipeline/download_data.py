import numpy as np
import serial
import os
import pandas as pd
from datetime import datetime
from tkinter import Tk, filedialog, ttk, messagebox, StringVar, Label, Button, Entry
from serial.tools import list_ports
from scipy.io import wavfile

import matplotlib

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt


def receive_and_save_data(ser, bin_filename, packet_size=4096, max_packets=2048 * 64):
    """Receives packets via serial and saves them into a binary file."""
    with open(bin_filename, "wb") as f:
        for packet_count in range(max_packets):
            data = ser.read(packet_size)
            if not data:
                continue
            if data == b"T":  # terminator character
                print("Received Terminator Character.")
                break
            f.write(data)
            print(f"📥 Received packet {packet_count}")
    ser.close()
    print("📴 Serial COM Port Closed.")


def process_audio_bin_file(bin_filename, csv_filename=None, wav_filename=None, npz_filename=None, label_id=None):
    print("🧠 Processing audio and spectral binary file...")

    # --- 📐 REVISED 4096-BYTE PACKET LAYOUT ---
    # Header: 1 + 2 + 1 + 20 + 4 = 28 bytes
    # Audio:  2034 samples * 2 bytes = 4068 bytes
    # Total:  28 + 4068 = 4096 bytes
    packet_dtype = np.dtype(
        [
            ("start_char", "S1"),       # 1 byte ('S')
            ("packet_index", "<u2"),    # 2 bytes (uint16_t)
            ("spectral_flag", "u1"),    # 1 byte (Flag: New vs Old spectral data)
            ("f1", "<u2"),              # 20 bytes total for spectral channels
            ("f2", "<u2"),
            ("f3", "<u2"),
            ("f4", "<u2"),
            ("f5", "<u2"),
            ("f6", "<u2"),
            ("f7", "<u2"),
            ("f8", "<u2"),
            ("clear", "<u2"),
            ("nir", "<u2"),   
            ("flickering_type", "<u4"), # 4 bytes (32-bit enum, values 0 to 3)
            ("audio", "<i2", 2034),     # 4068 bytes (Audio Data -> 2034 int16 samples)
        ]
    )

    try:
        # Read the binary file natively based on our custom layout
        packets = np.fromfile(bin_filename, dtype=packet_dtype)
    except Exception as e:
        print(f"⚠️ Error reading binary file: {e}")
        return

    if packets.size == 0:
        print("⚠️ No data found!")
        return

    # --- 🔍 PACKET INTEGRITY CHECK ---
    valid_starts = packets["start_char"] == b"S"
    if not np.all(valid_starts):
        bad_indices = np.where(~valid_starts)[0]
        print(f"❌ MISALIGNED PACKETS DETECTED! {len(bad_indices)} packets did not start with 'S'.")

    indices = packets["packet_index"].astype(int)
    if len(indices) > 1:
        # Simple step check: every packet must be exactly 1 index higher than the last
        index_diffs = np.diff(indices)
        missing_packet_intervals = np.where(index_diffs != 1)[0]

        if len(missing_packet_intervals) > 0:
            print("❌ DATA CORRUPTION / DROP DETECTED!")
            for idx in missing_packet_intervals:
                lost_after = indices[idx]
                found_next = indices[idx + 1]
                print(f"⚠️ Gap found: Packet index jumped from {lost_after} to {found_next} (At file packet position {idx})")
            print(f"⚠️ Total gaps detected: {len(missing_packet_intervals)}")
        else:
            print("✅ DATA INTEGRITY VERIFIED: Packet indices are perfectly sequential (+1). No drops detected.")

    # Extract all the 2034-sample audio chunks and flatten them into a single 1D vector
    audio_samples = packets["audio"].flatten()

    print(f"📊 Total audio samples extracted: {len(audio_samples)}")
    print(f"🕒 Validated packet sweep: from {packets['packet_index'][0]} to {packets['packet_index'][-1]}")

    # --- 🔊 WAV FILE GENERATION ---
    if wav_filename:
        SAMPLE_RATE = 16000  
        wavfile.write(wav_filename, SAMPLE_RATE, audio_samples)
        print(f"🎵 Audio file WAV generated: {wav_filename}")

    # --- 🌈 SPECTROMETER & TELEMETRY EXTRACTION ---
    spectral_df = pd.DataFrame(
        {
            "F1": packets["f1"], "F2": packets["f2"], "F3": packets["f3"],
            "F4": packets["f4"], "F5": packets["f5"], "F6": packets["f6"],
            "F7": packets["f7"], "F8": packets["f8"], 
            "CLEAR": packets["clear"], "NIR": packets["nir"],
            "SPECTRAL_FLAG": packets["spectral_flag"],
            "FLICKERING_TYPE": packets["flickering_type"],
        }
    )

    if csv_filename:
        spectral_df.to_csv(csv_filename, index=False)
        print(f"📄 Spectral and telemetry CSV generated: {csv_filename}")

    if npz_filename:
        total_packets = packets.size
        num_windows = total_packets // 3
        
        if num_windows > 0:
            # Reconstruct spectrometer raw columns to 2D
            spectro_matrix_raw = np.column_stack((
                packets["f1"], packets["f2"], packets["f3"], packets["f4"],
                packets["f5"], packets["f6"], packets["f7"], packets["f8"],
                packets["clear"], packets["nir"]
            ))
            
            # Pre-allocate using strict target datatypes (<u2, <i2, <u4)
            merged_spectro = np.zeros((num_windows, 10), dtype="<u2")
            merged_audio = np.zeros((num_windows, 2034 * 3), dtype="<i2")
            merged_flicker = np.zeros(num_windows, dtype="<u4")
            
            for i in range(num_windows):
                start_idx = i * 3
                end_idx = start_idx + 3
                
                # 1. Audio Concatenation
                merged_audio[i] = packets["audio"][start_idx:end_idx].flatten()
                
                # 2. Window flags slicing
                window_spectro = spectro_matrix_raw[start_idx:end_idx]
                window_flicker = packets["flickering_type"][start_idx:end_idx]
                window_flags = packets["spectral_flag"][start_idx:end_idx]
                
                valid_indices = np.where(window_flags > 0)[0]
                
                if len(valid_indices) >= 1:
                    mean_vals = window_spectro[valid_indices].mean(axis=0)
                    merged_spectro[i] = np.round(mean_vals).astype("<u2")
                    merged_flicker[i] = window_flicker[valid_indices[-1]].astype("<u4")
                else:
                    merged_spectro[i] = window_spectro[-1].astype("<u2")
                    merged_flicker[i] = window_flicker[-1].astype("<u4")
            
            # Prepare saving dict with optional label array
            save_dict = {
                "spectro": merged_spectro, 
                "audio": merged_audio,
                "flicker": merged_flicker
            }
            if label_id is not None:
                save_dict["label"] = np.full((num_windows,), label_id, dtype="i1")

            np.savez_compressed(npz_filename, **save_dict)
            print(f"📦 Pipeline NPZ Dataset generated (3-Merged): {npz_filename} (Shape Spectro: {merged_spectro.shape}, Shape Audio: {merged_audio.shape}, Shape Flicker: {merged_flicker.shape}, Shape Label: {save_dict['label'].shape if label_id is not None else 'N/A'})")
            if label_id is not None:
                print(f"🏷️  Saved environment label ID array filled with: {label_id}")
        else:
            print("⚠️ Not enough packets to generate a single 3-packet merged NPZ window.")


    # --- DATA PLOTTING ---
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(15, 10))

    # Top Plot: Audio Waveform
    ax1.plot(audio_samples, color="blue", linewidth=0.5)
    ax1.set_title("Audio Waveform Extracted from NAND")
    ax1.set_xlabel("Sample Index")
    ax1.set_ylabel("Amplitude (int16)")
    ax1.grid(True)

    # Bottom Plot: Spectrometer Channels (Plots smoothly using dataframe row tracking)
    channels_to_plot = ["F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "CLEAR", "NIR"]
    colors = [
        "#8A2BE2", "#0000FF", "#00BFFF", "#00FF00", "#ADFF2F", 
        "#FFFF00", "#FFA500", "#FF0000", "#7F7F7F", "#000000"
    ]

    for ch, color in zip(channels_to_plot, colors):
        ax2.plot(spectral_df[ch], label=ch, color=color, linewidth=1.5)

    ax2.set_title("AS7341 Optical Channels over Time (Flickering Enums Extracted)")
    ax2.set_xlabel("Row/Packet Matrix Index")
    ax2.set_ylabel("Raw ADC Value")
    ax2.grid(True)
    ax2.legend(loc="upper right", ncol=10)

    plt.tight_layout()
    print("📉 Rendering plots...")
    plt.show()

    return audio_samples, spectral_df


def gui_select_com_and_folder():
    """Opens a GUI to select COM port, environment label, target folder, and custom file name."""
    root = Tk()
    root.title("Data Logger - Configuration")
    root.geometry("400x530")
    root.resizable(False, False)

    # COM Port Selection
    Label(root, text="🔌 Select the COM Port:", font=("Segoe UI", 10)).pack(pady=5)
    com_var = StringVar()
    ports = [p.device for p in list_ports.comports()]
    if not ports:
        ports = ["No COM Port found"]
    com_box = ttk.Combobox(root, textvariable=com_var, values=ports, state="readonly", width=30)
    com_box.pack(pady=5)
    com_box.current(0)

    # Environment Label Selection
    Label(root, text="🏷️ Select environment label:", font=("Segoe UI", 10)).pack(pady=5)
    label_var = StringVar()
    labels = ["UNDERGROUND", "OUTDOOR", "INDOOR_QUIET", "INDOOR_NORMAL", "COVERED_DARK"]
    label_box = ttk.Combobox(root, textvariable=label_var, values=labels, state="readonly", width=30)
    label_box.pack(pady=5)
    label_box.current(0)

    # Custom File Name Input
    Label(root, text="📝 Enter File Name (without extension):", font=("Segoe UI", 10)).pack(pady=5)
    filename_var = StringVar()
    # Prefill with timestamp default to avoid empty field submissions
    default_name = datetime.now().strftime("SensorData_%Y%m%d_%H%M%S")
    filename_var.set(default_name)
    filename_entry = Entry(root, textvariable=filename_var, width=33)
    filename_entry.pack(pady=5)

    # Folder Selection
    def browse_folder():
        folder = filedialog.askdirectory(title="📂 Select Target Folder")
        if folder:
            folder_var.set(folder)

    folder_var = StringVar()
    Label(root, text="📁 Select Output Folder:", font=("Segoe UI", 10)).pack(pady=5)
    Button(root, text="Select folder...", command=browse_folder).pack()
    Label(root, textvariable=folder_var, fg="blue", wraplength=350).pack(pady=5)
    final_label_id = [0]

    def confirm():
        if not folder_var.get() or "No" in com_var.get() or not filename_var.get().strip():
            messagebox.showerror("Error", "Please fill in all fields (COM, Label, File Name, Folder).")
            return
        try:
            final_label_id[0] = labels.index(label_var.get())
        except ValueError:
            final_label_id[0] = 0

        root.destroy()

    Button(root, text="✅ Confirm & Download", command=confirm, bg="#4CAF50", fg="white", font=("Segoe UI", 10, "bold")).pack(pady=20)
    root.mainloop()

    # Sanitize spaces out of the filename string
    clean_filename = filename_var.get().strip().replace(" ", "_")

    return com_var.get(), folder_var.get(), clean_filename, final_label_id[0]


# ==============================
# Main
# ==============================

def main():
    com_port, save_path, file_name, label_id = gui_select_com_and_folder()
    if not com_port or not save_path or not file_name:
        print("❌ Application Stopped.")
        return

    # Build output paths using the user's input filename
    bin_filename = os.path.join(save_path, f"{file_name}.bin")
    csv_filename = os.path.join(save_path, f"{file_name}_spectral.csv")
    wav_filename = os.path.join(save_path, f"{file_name}_audio.wav")
    npz_filename = os.path.join(save_path, f"{file_name}_dataset.npz")

    BAUD_RATE = 250000

    try:
        ser = serial.Serial(com_port, BAUD_RATE, timeout=10)
        print(f"🔌 Connected to {com_port}")
        receive_and_save_data(ser, bin_filename)
    except serial.SerialException as e:
        print(f"⚠️ Serial Error: {e}")
        return

    process_audio_bin_file(bin_filename, csv_filename, wav_filename, npz_filename, label_id=label_id)


# ==============================
# Entry point
# ==============================

if __name__ == "__main__":
    main()