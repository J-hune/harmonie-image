
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

def recolor_image(image, template_angles, rotation):
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    h, s, v = cv2.split(hsv)
    rotated_template = [(angle + rotation) % 180 for angle in template_angles]
    def map_hue(hue_val):
        return min(rotated_template, key=lambda t: min(abs(t - hue_val), 180 - abs(t - hue_val)))
    vectorized_map = np.vectorize(map_hue)
    new_h = vectorized_map(h)
    recolored_hsv = cv2.merge([new_h.astype(np.uint8), s, v])
    recolored_bgr = cv2.cvtColor(recolored_hsv, cv2.COLOR_HSV2BGR)
    return recolored_bgr

st.title("Harmonic Color Palette Generator")

uploaded_file = st.file_uploader("Upload an image", type=["jpg", "jpeg", "png"])
if uploaded_file is not None:
    image = Image.open(uploaded_file).convert("RGB")
    image_np = np.array(image)
    image_bgr = cv2.cvtColor(image_np, cv2.COLOR_RGB2BGR)
    st.image(image, caption="Original Image", use_column_width=True)

    template_name = st.selectbox("Select Harmonic Template", list(templates.keys()))
    st.markdown("### Interactive Color Wheel")
    template_json = json.dumps(templates[template_name])

    components.html(f'''
    <div style="text-align:center">
    <canvas id="wheel" width="300" height="300" style="cursor:grab;"></canvas>
    <p>Rotation: <span id="angleVal">0</span>°</p>
    </div>
    <script>
    const canvas = document.getElementById('wheel');
    const ctx = canvas.getContext('2d');
    const centerX = canvas.width / 2;
    const centerY = canvas.height / 2;
    const radius = 120;
    let isDragging = false;
    let rotation = parseInt(localStorage.getItem('rotation_angle') || '0');
    const templateAngles = {template_json};

    function hsvToRgb(h, s, v) {{
        let r, g, b;
        let i = Math.floor(h * 6);
        let f = h * 6 - i;
        let p = v * (1 - s);
        let q = v * (1 - f * s);
        let t = v * (1 - (1 - f) * s);
        switch (i % 6) {{
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }}
        return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
    }}

    function drawWheel() {{
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        for (let i = 0; i < 360; i++) {{
            const start = (i - 1) * Math.PI / 180;
            const end = i * Math.PI / 180;
            const hue = i / 360;
            const rgb = hsvToRgb(hue, 1, 1);
            ctx.beginPath();
            ctx.moveTo(centerX, centerY);
            ctx.arc(centerX, centerY, radius, start, end);
            ctx.closePath();
            ctx.fillStyle = `rgb(${{rgb[0]}}, ${{rgb[1]}}, ${{rgb[2]}})`;
            ctx.fill();
        }}
        for (let a of templateAngles) {{
            const angle = (a + rotation) % 360;
            const rad = angle * Math.PI / 180;
            ctx.beginPath();
            ctx.moveTo(centerX, centerY);
            ctx.lineTo(centerX + radius * Math.cos(rad), centerY + radius * Math.sin(rad));
            ctx.strokeStyle = 'black';
            ctx.lineWidth = 2;
            ctx.stroke();
        }}
        document.getElementById('angleVal').innerText = rotation;
        localStorage.setItem('rotation_angle', rotation);
    }}

    function getAngle(x, y) {{
        return Math.atan2(y - centerY, x - centerX) * 180 / Math.PI;
    }}
    drawWheel();
    canvas.addEventListener('mousedown', e => {{ isDragging = true; canvas.style.cursor = 'grabbing'; }});
    canvas.addEventListener('mouseup', e => {{ isDragging = false; canvas.style.cursor = 'grab'; }});
    canvas.addEventListener('mousemove', e => {{
        if (isDragging) {{
            const rect = canvas.getBoundingClientRect();
            const x = e.clientX - rect.left;
            const y = e.clientY - rect.top;
            rotation = Math.round((getAngle(x, y) + 360) % 360);
            drawWheel();
        }}
    }});
    </script>
    ''', height=360)

    rotation_js = st_js.st_javascript("parseInt(localStorage.getItem('rotation_angle') || '0')", key="rotation_sync")
    if 'rotation' not in st.session_state or st.session_state['rotation'] != rotation_js:
        st.session_state['rotation'] = rotation_js if rotation_js is not None else 0
    rotation = st.number_input('Rotation angle', value=st.session_state['rotation'], min_value=0, max_value=359)

    if st.button("Apply Template"):
        recolored = recolor_image(image_bgr, templates[template_name], rotation)
        recolored_rgb = cv2.cvtColor(recolored, cv2.COLOR_BGR2RGB)
        st.image(recolored_rgb, caption=f"Recolored Image ({template_name}, {rotation}°)", use_column_width=True)

        recolored_pil = Image.fromarray(recolored_rgb)
        buf = io.BytesIO()
        recolored_pil.save(buf, format="PNG")
        byte_im = buf.getvalue()
        st.download_button("Download Recolored Image", data=byte_im, file_name="recolored.png", mime="image/png")

        # Hue mapping visualization
        fig, ax = plt.subplots(1, 3, figsize=(15, 4))
        original_hsv = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2HSV)
        recolored_hsv = cv2.cvtColor(recolored, cv2.COLOR_BGR2HSV)
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
        colors = [plt.cm.hsv(h / 180.0) for h in range(180)]
        ax_polar.bar(angles, hist, width=np.deg2rad(1), bottom=0, color=colors, edgecolor="none")
        ax_polar.set_xticks([])
        ax_polar.set_yticks([])
        ax_polar.set_title("Polar Hue Histogram", va="bottom")
        st.pyplot(polar_fig)
