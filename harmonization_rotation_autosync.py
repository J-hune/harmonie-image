
import streamlit as st
import cv2
import numpy as np
from PIL import Image
import io
import matplotlib.pyplot as plt
import streamlit.components.v1 as components
import streamlit_javascript as st_js
import colorsys
import json
import subprocess

# To run this script, use the following command in your terminal:
# streamlit run harmonization_rotation_autosync.py 

templates = {
    "i": [0, 18],
    "V": [0, 93.6],
    "L": [0, 18, 60, 120],
    "I": [0, 18, 180, 198],
    "T": [0, 180],
    "Y": [0, 93.6, 217.8, 235.8],
    "X": [0, 93.6, 180, 273.6]
}

def recolor_image(image, template_letter, rotation):
    if template_letter == "ND":
        template_letter = "ND"
    if rotation is None:
        rotation = "ND"
    image.save("./temp.png")

    process_result = subprocess.run(['./main', './temp.png', str(template_letter), str(rotation)], capture_output=True, text=True)
    #st.write(process_result.stdout)

    recolored = Image.open("./output.png")

    return recolored

st.title("Cohen-Or et al: Harmonic Color Palette Generator")

uploaded_file = st.file_uploader("Upload an image", type=["jpg", "jpeg", "png"])
if uploaded_file is not None:
    image = Image.open(uploaded_file).convert("RGB")
    image_np = np.array(image)
    image_bgr = cv2.cvtColor(image_np, cv2.COLOR_RGB2BGR)
    st.image(image, caption="Original Image", use_container_width=True)

    st.markdown("### Parameters")

    mode = st.selectbox("Process mode", ["Best template & angle", "Given template, best angle", "Given template, given angle"])
    template_name = st.selectbox("Harmonic Template", list(templates.keys()))
    template_json = json.dumps(templates[template_name])

    rotation = st.number_input('Rotation angle', min_value=0, max_value=359)

    if st.button("Apply Template"):

        if 'rotation' not in st.session_state or st.session_state['rotation'] != rotation:
            st.session_state['rotation'] = rotation if rotation is not None else 0
        
        if mode == "Given template, best angle": rotation = "ND"
        if mode == "Best template & angle": rotation = template_name = "ND"

        
        recolored = recolor_image(image, template_name, rotation)
        recolored_rgb = np.array(image)
        st.image(recolored, caption=f"Recolored Image ({template_name}, {rotation}°)", use_container_width=True)

        recolored_pil = Image.fromarray(recolored_rgb)
        buf = io.BytesIO()
        recolored_pil.save(buf, format="PNG")
        byte_im = buf.getvalue()
        st.download_button("Download Recolored Image", data=byte_im, file_name="recolored.png", mime="image/png")

        # Hue mapping visualization
        fig, ax = plt.subplots(1, 3, figsize=(15, 4))
        original_hsv = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2HSV)
        recolored_hsv = cv2.cvtColor(recolored_rgb, cv2.COLOR_RGB2HSV)
        ax[0].hist(original_hsv[:, :, 0].flatten(), bins=180, range=(0, 180), color="orange")
        ax[0].set_title("Original Hue Histogram")
        ax[1].hist(recolored_hsv[:, :, 0].flatten(), bins=180, range=(0, 180), color="green")
        ax[1].set_title("Recolored Hue Histogram")
        for a in ax[:2]:
            a.set_xlim(0, 180)
            a.set_xlabel("Hue")
            a.set_ylabel("Pixels")
        # Overlay
        orig_hues = original_hsv[:, :, 0].flatten()
        recol_hues = recolored_hsv[:, :, 0].flatten()
        sampled = np.random.choice(len(orig_hues), size=min(2000, len(orig_hues)), replace=False)
        ax[2].scatter(orig_hues[sampled], recol_hues[sampled], s=1, alpha=0.3)
        ax[2].set_title("Hue Shift Mapping")
        ax[2].set_xlabel("Original Hue")
        ax[2].set_ylabel("Recolored Hue")
        ax[2].set_xlim(0, 180)
        ax[2].set_ylim(0, 180)
        st.pyplot(fig)

        # Polar hue histogram
        polar_fig = plt.figure(figsize=(4, 4))
        ax_polar = polar_fig.add_subplot(111, polar=True)
        hist, _ = np.histogram(original_hsv[:, :, 0].flatten(), bins=180, range=(0, 180))
        angles = np.deg2rad(np.arange(180))
        colors = [plt.cm.hsv((h - 90) / 180.0) for h in range(180)]
        ax_polar.bar(angles, hist, width=np.deg2rad(1), bottom=0, color=colors, edgecolor="none")
        ax_polar.set_xticks([])
        ax_polar.set_yticks([])
        ax_polar.set_title("Polar Hue Histogram", va="bottom")
        st.pyplot(polar_fig)
