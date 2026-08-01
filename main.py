# main.py for the CART UNO Q (plant_car app, Linux side)
 
import time
import cv2
import numpy as np
import requests
import os
 
# ----------------- settings (tune these numbers only) -----------------
MODEL_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "plant_classifier.onnx")
CLASS_NAMES = ["Background", "Dead", "Healthy", "Wilted"]
STATION_URL = "http://172.20.10.6:5000/plant"
# ----------------------------------------------------------------------
 
# --- reporting to the station (Stage 4) ---
last_status = None
 
def report_plant(status, conf_percent):
    """Send the classification to the station board over Wi-Fi."""
    global last_status
    
    # We report every time a photo is manually taken, so we removed the REPORT_EVERY timer
    try:
        requests.get(
            STATION_URL,
            params={"status": status, "conf": conf_percent},
            timeout=2,
        )
        print(f"  -> reported to station: {status} ({conf_percent}%)")
        last_status = status
    except requests.exceptions.RequestException:
        print("  -> station not reachable, skipping report")
 
# --- finding the camera ---
def open_camera():
    """Try each video device until one actually hands over a picture."""
    for i in range(10):
        cam = cv2.VideoCapture(i)
        ok, _ = cam.read()
        if ok:
            print(f"Camera found at index {i}")
            return cam
        cam.release()
    return None
 
# --- the classifier ---
def preprocess(frame):
    """Turn a camera frame into the exact format ResNet18 expects."""
    blob = cv2.dnn.blobFromImage(
        frame,
        scalefactor=1.0 / 255.0,
        size=(224, 224),
        mean=(0, 0, 0),
        swapRB=True,
        crop=False,
    )
    mean = np.array([0.485, 0.456, 0.406]).reshape(1, 3, 1, 1)
    std = np.array([0.229, 0.224, 0.225]).reshape(1, 3, 1, 1)
    return (blob - mean) / std
 
def softmax(scores):
    """Turn raw model scores into probabilities that add up to 1."""
    e = np.exp(scores - np.max(scores))
    return e / e.sum()
 
def main():
    print("Loading model...")
    net = cv2.dnn.readNetFromONNX(MODEL_PATH)
 
    print("Opening camera...")
    cam = open_camera()
    if cam is None:
        print("ERROR: no camera found. Check the USB connection")
        print("and that /dev/video* exists.")
        return
 
    print("\n=================================================")
    print("Ready! System is now in Manual Photo Mode.")
    print("Drive the robot to a plant using your joystick.")
    print("=================================================")
 
    while True:
        # Wait for the user to trigger the camera via SSH/Console
        user_input = input("\n> Press ENTER to take a photo and evaluate (or type 'q' to quit): ")
        
        if user_input.lower() == 'q':
            break
            
        print("Snapping fresh photo...")
        
        # 1. Flush the buffer to fix the lag
        # OpenCV buffers old frames. We rapidly read 5 frames and throw them away
        # to ensure the final frame we evaluate is exactly what the camera sees right now.
        for _ in range(5):
            cam.read()
            
        # 2. Take the actual photo
        ok, frame = cam.read()
        if not ok:
            print("Camera read failed, please try again.")
            continue
 
        # 3. Evaluate
        net.setInput(preprocess(frame).astype(np.float32))
        scores = net.forward().flatten()
        probs = softmax(scores)
 
        best = int(np.argmax(probs))
        label = CLASS_NAMES[best]
        conf_percent = round(float(probs[best]) * 100)
 
        stamp = time.strftime("%H:%M:%S")
        print(f"[{stamp}] Plant status: {label} ({conf_percent}%)")
 
        # 4. Report
        report_plant(label, conf_percent)
 
    cam.release()
    print("Camera safely closed.")
 
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nStopped.")