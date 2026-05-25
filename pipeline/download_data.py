import numpy as np
import serial
import os
import pandas as pd
from datetime import datetime
from tkinter import Tk, filedialog, ttk, messagebox, StringVar, Label, Button
from serial.tools import list_ports
from scipy.io import wavfile

import matplotlib

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt


def receive_and_save_data(ser, bin_filename, packet_size=4096, max_packets=2048 * 64):
    """Riceve pacchetti via seriale e li salva in binario."""
    with open(bin_filename, "wb") as f:
        for packet_count in range(max_packets):
            data = ser.read(packet_size)
            if not data:
                continue
            if data == b"T":  # terminatore
                print("Received Terminator Character.")
                break
            f.write(data)
            print(f"📥 Received packet {packet_count+1}")
    ser.close()
    print("📴 Serial COM Port Closed.")


def process_audio_bin_file(bin_filename, csv_filename=None, wav_filename=None, npz_filename=None, label_str=""):
    print("🧠 Processing audio and spectral binary file...")

    # Updated 4096-byte packet structure
    packet_dtype = np.dtype(
        [
            ("hh", np.uint8),  # 1 byte
            ("mm", np.uint8),  # 1 byte
            ("ss", np.uint8),  # 1 byte
            ("sss", "<u2"),  # 2 bytes
            ("f1", "<u2"),  # 2 bytes (Spectrometer Data Begins)
            ("f2", "<u2"),  # 2 bytes
            ("f3", "<u2"),  # 2 bytes
            ("f4", "<u2"),  # 2 bytes
            ("f5", "<u2"),  # 2 bytes
            ("f6", "<u2"),  # 2 bytes
            ("f7", "<u2"),  # 2 bytes
            ("f8", "<u2"),  # 2 bytes
            ("clear", "<u2"),  # 2 bytes
            ("nir", "<u2"),  # 2 bytes
            ("pad", np.uint8, 3),  # 3 bytes padding
            ("audio", "<i2", 2034),  # 4068 bytes (Audio Data)
        ]
    )

    try:
        # Read the entire file natively based on our custom structure
        packets = np.fromfile(bin_filename, dtype=packet_dtype)
    except Exception as e:
        print(f"⚠️ Errore nella lettura del file binario: {e}")
        return

    if packets.size == 0:
        print("⚠️ No data found!")
        return

    # Extract all the 2034-sample audio chunks and flatten them
    audio_samples = packets["audio"].flatten()

    print(f"📊 Campioni audio totali estratti: {len(audio_samples)}")
    print(
        f"🕒 Primo pacchetto registrato alle: {packets['hh'][0]:02d}:{packets['mm'][0]:02d}:{packets['ss'][0]:02d}.{packets['sss'][0]:03d}"
    )

    # --- 🔊 GENERAZIONE DEL FILE WAV ---
    if wav_filename:
        SAMPLE_RATE = 16000  # Adjust if your I2S/MDF runs at a different rate
        wavfile.write(wav_filename, SAMPLE_RATE, audio_samples)
        print(f"🎵 Audio file WAV generated: {wav_filename}")

    # --- 🌈 ESTRAZIONE DATI SPETTROMETRO ---
    # Create timestamps for each packet
    timestamps = [
        f"{h:02d}:{m:02d}:{s:02d}.{ms:03d}"
        for h, m, s, ms in zip(
            packets["hh"], packets["mm"], packets["ss"], packets["sss"]
        )
    ]

    spectral_df = pd.DataFrame(
        {
            "timestamp": timestamps,
            "F1": packets["f1"],
            "F2": packets["f2"],
            "F3": packets["f3"],
            "F4": packets["f4"],
            "F5": packets["f5"],
            "F6": packets["f6"],
            "F7": packets["f7"],
            "F8": packets["f8"],
            "CLEAR": packets["clear"],
            "NIR": packets["nir"],
        }
    )

    if csv_filename:
        spectral_df.to_csv(csv_filename, index=False)
        print(f"📄 Spectral data CSV generated: {csv_filename}")

    if npz_filename:
        # Impila le colonne dello spettrometro. Risultato: array 2D (N_pacchetti, 10)
        spectro_matrix = np.column_stack((
            packets["f1"], packets["f2"], packets["f3"], packets["f4"],
            packets["f5"], packets["f6"], packets["f7"], packets["f8"],
            packets["clear"], packets["nir"]
        ))
        
        # packets["audio"] è già un array 2D (N_pacchetti, 2034)
        audio_matrix = packets["audio"]
        
        # Crea un array di etichette lungo N_pacchetti
        labels_array = np.full(packets.size, label_str)
        
        # Salva tutto compresso
        np.savez_compressed(
            npz_filename, 
            spectro=spectro_matrix, 
            audio=audio_matrix, 
            labels=labels_array
        )
        print(f"📦 Dataset NPZ generato: {npz_filename} (Shape Spectro: {spectro_matrix.shape}, Shape Audio: {audio_matrix.shape})")

    # --- GRAFICO DEI DATI ---
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(15, 10))

    # Top Plot: Audio Waveform
    ax1.plot(audio_samples, color="blue", linewidth=0.5)
    ax1.set_title("Audio Waveform Extracted from NAND")
    ax1.set_xlabel("Sample Index")
    ax1.set_ylabel("Amplitude (int16)")
    ax1.grid(True)

    # Bottom Plot: Spectrometer Channels
    channels = ["F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8"]
    # Approximate visible spectrum colors for the plot lines
    colors = [
        "#8A2BE2",
        "#0000FF",
        "#00BFFF",
        "#00FF00",
        "#ADFF2F",
        "#FFFF00",
        "#FFA500",
        "#FF0000",
    ]

    for ch, color in zip(channels, colors):
        ax2.plot(spectral_df[ch], label=ch, color=color, linewidth=1.5)

    ax2.set_title("AS7341 Optical Channels over Time (10Hz Sampling)")
    ax2.set_xlabel("Packet Index")
    ax2.set_ylabel("Raw ADC Value")
    ax2.grid(True)
    ax2.legend(loc="upper right", ncol=8)

    plt.tight_layout()
    print("📉 Mostro i grafici...")
    plt.show()

    return audio_samples, spectral_df


def gui_select_com_and_folder():
    """Apre una piccola GUI per selezionare COM, cartella e Label."""
    root = Tk()
    root.title("Data Logger - Configuration")
    root.geometry("400x450")
    root.resizable(False, False)

    Label(root, text="🔌 Select the COM Port:", font=("Segoe UI", 10)).pack(pady=5)

    com_var = StringVar()
    ports = [p.device for p in list_ports.comports()]

    if not ports:
        ports = ["No COM Port found"]
    com_box = ttk.Combobox(
        root, textvariable=com_var, values=ports, state="readonly", width=30
    )
    com_box.pack(pady=5)
    com_box.current(0)

    Label(root, text="🏷️ Select Target Label:", font=("Segoe UI", 10)).pack(pady=5)
    label_var = StringVar()
    dummy_labels = ["LABEL0", "LABEL1", "LABEL2", "LABEL3"]
    label_box = ttk.Combobox(
        root, textvariable=label_var, values=dummy_labels, state="readonly", width=30
    )
    label_box.pack(pady=5)
    label_box.current(0)

    def browse_folder():
        folder = filedialog.askdirectory(title="📂 Select the folder")
        if folder:
            folder_var.set(folder)

    folder_var = StringVar()
    Label(root, text="📁 Folder:", font=("Segoe UI", 10)).pack(pady=5)
    Button(root, text="Select folder...", command=browse_folder).pack()
    Label(root, textvariable=folder_var, fg="blue", wraplength=350).pack(pady=5)

    def confirm():
        if not folder_var.get() or "Nessuna" in com_var.get() or not label_var.get():
            messagebox.showerror("Error", "Select a valid COM Port, a folder, and a Label.")
            return
        root.destroy()

    Button(root, text="✅ Confirm", command=confirm, bg="#4CAF50", fg="white").pack(
        pady=10
    )
    root.mainloop()

    return com_var.get(), folder_var.get(), label_var.get()


# ==============================
# Main
# ==============================


def main():
    com_port, save_path, selected_label = gui_select_com_and_folder()
    if not com_port or not save_path:
        print("❌ Application Stopped.")
        return

    base_filename = datetime.now().strftime("SensorData_%Y%m%d_%H%M%S")
    bin_filename = os.path.join(save_path, f"{base_filename}.bin")
    csv_filename = os.path.join(save_path, f"{base_filename}_spectral.csv")
    wav_filename = os.path.join(save_path, f"{base_filename}_audio.wav")
    npz_filename = os.path.join(save_path, f"{base_filename}_dataset.npz")

    BAUD_RATE = 250000

    try:
        ser = serial.Serial(com_port, BAUD_RATE, timeout=10)
        print(f"🔌 Connected to {com_port}")
        receive_and_save_data(ser, bin_filename)
    except serial.SerialException as e:
        print(f"⚠️ Serial Error: {e}")
        return

    process_audio_bin_file(bin_filename, csv_filename, wav_filename, npz_filename, selected_label)


# ==============================
# Entry point
# ==============================

if __name__ == "__main__":
    main()