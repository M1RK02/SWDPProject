import json
import re
from pathlib import Path

# Paths
metadata_path = Path("/Users/mirko/Developer/SWDPProject/pipeline/output/tinyml_context_model/model_metadata.json")
features_c_path = Path("/Users/mirko/Developer/SWDPProject/firmware/inference/Core/Src/features.c")
main_c_path = Path("/Users/mirko/Developer/SWDPProject/firmware/inference/Core/Src/main.c")

if not metadata_path.exists():
    print(f"Error: metadata file not found at {metadata_path}. Run the notebook first.")
    exit(1)

with open(metadata_path, "r", encoding="utf-8") as f:
    meta = json.load(f)

feature_cols = meta["feature_columns"]
num_features = len(feature_cols)

means_dict = meta["normalization"]["mean"]
stds_dict = meta["normalization"]["std"]

# 1. Generate means and stds C arrays
means_c_lines = []
stds_c_lines = []

for idx, col in enumerate(feature_cols):
    mean_val = means_dict[col]
    std_val = stds_dict[col]
    
    # format float value
    mean_str = f"    {mean_val:.9g}f"
    std_str = f"    {std_val:.9g}f"
    
    if idx < num_features - 1:
        mean_str += ","
        std_str += ","
        
    means_c_lines.append(mean_str + f" // [{idx}] {col}")
    stds_c_lines.append(std_str + f" // [{idx}] {col}")

means_block = "\n".join(means_c_lines)
stds_block = "\n".join(stds_c_lines)

# Read features.c
if not features_c_path.exists():
    print(f"Error: features.c not found at {features_c_path}")
    exit(1)

with open(features_c_path, "r", encoding="utf-8") as f:
    features_c_content = f.read()

# Replace means array in features.c
# We search for: static const float model_normalization_means[SIZE] = { ... };
means_pattern = r"(static const float model_normalization_means\[\d+\] = \{)[^\}]*(\};)"
features_c_content = re.sub(
    means_pattern,
    f"\\1\n{means_block}\n\\2",
    features_c_content,
    flags=re.DOTALL
)

# Replace stds array in features.c
stds_pattern = r"(static const float model_normalization_stds\[\d+\] = \{)[^\}]*(\};)"
features_c_content = re.sub(
    stds_pattern,
    f"\\1\n{stds_block}\n\\2",
    features_c_content,
    flags=re.DOTALL
)

# Update array sizes in features.c declarations to the new count using regex
features_c_content = re.sub(
    r"(static const float model_normalization_means)\[\d+\]",
    f"\\1[{num_features}]",
    features_c_content
)
features_c_content = re.sub(
    r"(static const float model_normalization_stds)\[\d+\]",
    f"\\1[{num_features}]",
    features_c_content
)

# Update loop size dynamically in Apply_Normalization
features_c_content = re.sub(
    r"(for \(int i = 0; i < )\d+(; i\+\+\) \{)",
    f"\\g<1>{num_features}\\g<2>",
    features_c_content
)

with open(features_c_path, "w", encoding="utf-8") as f:
    f.write(features_c_content)

print(f"Successfully updated {features_c_path} with {num_features} normalization parameters.")

# 2. Update main.c quantization scale & zero-point
scale = meta["tflite"]["input_quantization"]["scale"]
zero_point = meta["tflite"]["input_quantization"]["zero_point"]

if not main_c_path.exists():
    print(f"Error: main.c not found at {main_c_path}")
    exit(1)

with open(main_c_path, "r", encoding="utf-8") as f:
    main_c_content = f.read()

# Replace scale and zero point comments & calculation line
# e.g., // Using scale 0.057999223470687866f and zero-point -83
comment_pattern = r"\/\/ Using scale [0-9\.\-ef]+ and zero-point [\d\-]+"
main_c_content = re.sub(
    comment_pattern,
    f"// Using scale {scale:.16g}f and zero-point {zero_point}",
    main_c_content
)

# e.g., float q_val = roundf(nn_input_features_float[i] / 0.057999223470687866f) - 83.0f;
calc_pattern = r"(float q_val = roundf\(nn_input_features_float\[i\] / )[0-9\.\-ef]+(f\) - )[0-9\.\-ef]+(f;)"
main_c_content = re.sub(
    calc_pattern,
    f"\\g<1>{scale:.16g}\\g<2>{float(-zero_point):.1f}\\g<3>",
    main_c_content
)

with open(main_c_path, "w", encoding="utf-8") as f:
    f.write(main_c_content)

print(f"Successfully updated {main_c_path} with scale={scale:.16g} and zero_point={zero_point}.")
